package visualjava;

import java.lang.reflect.Constructor;

/** joey Nov 2, 2004 5:21:08 PM */
public class ReflectionHelper {

    public static final Class[] primitiveTypes = {
        Integer.class, Long.class,
        Double.class, Float.class,
        Boolean.class, Character.class,
        Byte.class, Short.class,
        // No it isn't (it doesn't have a TYPE field!): String.class /** Well at least ParameterTarget needs to create a JLabel! */
    };

    public static boolean isPrimitiveType(Class type) {
        for (int i=0;i<primitiveTypes.length;i++) {
            Class aType = primitiveTypes[i];
            if (type.equals(aType))
                return true;
            try {
                Class primitiveAType = (Class)aType.getField("TYPE").get(null);
                if (type.equals(primitiveAType))
                    return true;
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
        return false;
    }

    public static Class getClassTypeFromPrimitiveType(Class type) {
        /*
        if (type.equals(Boolean.TYPE))
        return Boolean.class;
        if (type.equals(Integer.TYPE))
        return Integer.class;
        */

        for (int i=0;i<primitiveTypes.length;i++) {
            Class classType = primitiveTypes[i];
            try {
                Class primitiveType = (Class)classType.getField("TYPE").get(null);
                if (type.equals(primitiveType)) {
                    return classType;
                }
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
        /* || @todo ... */
        System.err.println("ParameterTarget.getClassTypeFromPrimitiveType() needs more work for type: " + type);
        return type;
    }

    public static Object createPrimitiveObjectFromText(String text, Class type) {
        try {

            // Convert from primitive type to class wrapper (so we can get a constructor)
            // (I think return type is irrelevant, since we cannot return true primitives, only their class wrappers.)
            if (type.isPrimitive()) {
                type = getClassTypeFromPrimitiveType(type);
            }

            // Special case: character does not have a String constructor
            if (type.equals(Character.class)) {
                if (text.length()>0) {
                    return new Character(text.charAt(0));
                } else {
                    return null;
                }
            }

            Constructor con = type.getConstructor(new Class[]{String.class});
            Object object = con.newInstance(new Object[]{text});
            return object;

        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

}

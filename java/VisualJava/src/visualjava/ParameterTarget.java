package visualjava;

import visualjava.objecteditingcomponents.BooleanChooser;

import javax.swing.*;
import java.awt.dnd.DropTarget;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

/**
 * @todo Consider how user might be able to pass null instead of a non-null Object.
 */

/** joey Nov 2, 2004 12:36:40 AM */
public class ParameterTarget extends JPanel {

    Class type;

    JComponent component;

    public ParameterTarget(Class _type) {
        type = _type;
        if (type.equals(Boolean.class) || type.equals(Boolean.TYPE)) {
            component = new BooleanChooser();
        } else if (type.isPrimitive()
            || type.equals(String.class)
            || type.equals(Double.class)
            /* || @todo ... */
        ) {
            // System.out.println("type = " + type);
            component = new JTextField("" + VisualJavaStatics.getSimpleClassName(type));
        } else {
            // component = new JButton("" + VisualJava.getSimpleClassName(type));
            component = new JLabel("" + VisualJavaStatics.getSimpleClassName(type));
        }
        add(component);
        // this.setDropTarget();
    }

    public Object getObject() {
        if (component instanceof HasObject) {
            return ((HasObject)component).getObject();
        }
        if (component instanceof JTextField) {
            String text = ((JTextField)component).getText();
            try {
                if (type.isPrimitive()) {
                    type = getClassTypeFromPrimitiveType(type);
                }
                Constructor con = type.getConstructor(new Class[]{String.class});
                Object object = con.newInstance(new Object[]{text});
                if (object != null) {
                    return object;
                }
            } catch (NoSuchMethodException e) {
                e.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
            System.err.println("Could not instantiate a " + type.getName() + " from \"" + text + "\".");
        // } else if (component instanceof ObjectTarget) {
            // return ((ObjectTarget)component).getObject();
        }
        /* || @todo ... */
        return null;
    }

    private Class getClassTypeFromPrimitiveType(Class type) {
        if (type.equals(Boolean.TYPE))
            return Boolean.class;
        if (type.equals(Integer.TYPE))
            return Integer.class;
        /* || @todo ... */
        System.err.println("ParameterTarget.getClassTypeFromPrimitiveType() needs more work for type: " + type);
        return type;
    }

}

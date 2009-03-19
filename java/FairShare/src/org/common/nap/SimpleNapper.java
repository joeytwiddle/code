package org.common.nap;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.common.lib.StreamReadingUtils;
import org.fairshare.Logger;
import org.neuralyte.util.reflection.ReflectionHelper;

/*
 * TODO
// Currently this implementation knows a few vital types.
// The list of types appears in writeNap() and readNap().
// A better model we be to have a list of TypeHandlers
// which each has a write() and read() implementation.
 * Actually they have detect() before read() or something like that.
 */

public class SimpleNapper extends StreamReadingUtils implements Napper {

    public static String neatClass(Object obj) {
        // TODO: Doesn't work on e.g. Integer.class - shows "java.lang.Class" O_o
        return obj.getClass().getName();
    }

    public static void write(OutputStream out, String str) {
        try {
            out.write(str.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public Object fromString(String str) {
        
        // TODO!!!
        
        return null;
    }
    
    

    public String toString(Object obj) {
        // OLD: StringBuffer out = new StringBuffer();
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        writeNap(obj, "", out);
        return out.toString();
    }

    public void writeNap(Object obj, OutputStream out) {
        writeNap(obj,"",out);
    }
    
    public void writeNap(Object obj, String indent, OutputStream out) {

        if (obj == null) {
            write(out,"null");
            return;
        }
        
        // write(out,neatClass(obj) + " ");

        // Is this a type we handle in a special way?
        if (ReflectionHelper.isPrimitiveHolder(obj.getClass())) {
            write(out,""+obj);
            return;
        }
        
        if (obj instanceof String) {
            write(out,'\"' + lazyEncode(""+obj) + '\"');
            return;
        }
        
        String subIndent = indent + "\t";

        if (obj.getClass().isArray()) {
            //// This first attempt actually created a List containing the Array, causing stack overflow below!
            //// It's a different implementation of asList().
            // List<Object> list = Arrays.asList(obj);
            List<Object> list = Arrays.asList((Object[])obj);
            // obj = list;
            //// obj is now a java.util.Arrays$ArrayList
            //// This writes ok, but we cannot reload a java.util.Arrays$ArrayList
            obj = new Vector();
            ((Vector<Object>)obj).addAll(list);
        }
        
        if (obj instanceof Collection) {
            Collection col = (Collection)obj;
            write(out,neatClass(col)+" {\n");
            int i = 0;
            for (Object item : col) {
                // append(out,indent+"["+i+"] = "+toString(item));
                write(out,subIndent+"["+i+"] = ");
                writeNap(item,subIndent+"\t",out);
                write(out,"\n");
                i++;
            }
            write(out,indent+"}");
            return;
        }
        
        if (obj instanceof Map) {
            write(out,neatClass(obj) + " {\n");
            Map map = (Map)obj;
            Collection keySet = map.keySet();
            for (Object key : keySet) {
                write(out,subIndent);
                Object val = map.get(key);
                writeNap(key,subIndent,out);
                write(out," -> ");
                writeNap(val,subIndent,out);
                write(out,"\n");
            }
            write(out,indent+"}\n"); // TODO: We don't really need this '\n' but maps are big so it can be useful to make things more readable.
            return;
        }
        
        if (obj instanceof File) {
            File file = (File)obj;
            // write(out,"file:" + file.getPath());
            write(out,neatClass(obj) + " \"" + lazyEncode(file.getPath()) + "\"");
            return;
        }
        
        writeGeneric(obj, indent, out);
    }

    public Object readNap(InputStream in) throws Exception {

        // PushbackReader reader = new PushbackReader(new InputStreamReader(in));
        
        String token = readToken(in);
        
        //// First we check for primitives:
        // Boolean?
        if (token.equals("true") || token.equals("false")) {
            return new Boolean(token);
        }
        // String?
        if (token.startsWith("\"")) {
            if (token.endsWith("\"")) {
                return lazyDecode(token.substring(1,token.length()-1));
            } else {
                // This should not happen now that we are encoding the " "s in strings.
                return lazyDecode(token.substring(1) + " " + readTo(in,"\""));
            }
        }
        // Number?
        try {
            Double d = new Double(token);
            return d;
        } catch (NumberFormatException e) {
        }
        
        // Dodgy fix to make Map parsing possible! 
        if (token.equals("}")) {
            return "INLINE_}"; // Will be picked up in a moment.
        }
        
        if (token.equals("null")) {
            return null;
        }
        
        // The token is not a primitive - therefore it should be a class name.
        String className = token;
        Class c;
        try {
            c = Class.forName(className);
        } catch (Exception e) {
            Logger.error("Failed to load class \""+className+"\"");
            Logger.error(e);
            String rest = readTo(in,"djksjfdslkjsdfwoirpwepro;xk;kx");
            Logger.error("Rest = " + rest);
            return null;
        }
        
        // Primitives never actually report their type.
        // This makes the data smaller but the code nastier.  :P
        /*
        if (ReflectionHelper.isPrimitiveType(c)) {
            // TODO readPrimitive
            return null;
        }
        */
        
        /*
        // This will never get called - we never output String "xyz" - just "xyz".
        if (c.equals(String.class)) {
            // TODO BUG Will fail if String contains any \"s :P
            // (Suggest HTML en/decoding)
            readTo(in, "\"");
            String str = readTo(in,"\"");
            return decode(str);
        }
        */
        
        if (File.class.isAssignableFrom(c)) {
            readTo(in,"\"");
            String path = readTo(in,"\"");
            path = lazyDecode(path);
            return new File(path);
        }

        if (Collection.class.isAssignableFrom(c)) {
            Collection<Object> col = (Collection<Object>)c.newInstance();
            // Logger.log("Reading collection into type "+neatClass(c));
            readExpect(in, "{");
            int i = 0;
            while (true) {
                token = readToken(in);
                if (token.equals("}"))
                    break;
                if (!token.equals("["+i+"]")) {
                    Logger.error("Array broken: expected \"["+i+"]\" but got \""+token+"\"!");
                }
                readExpect(in, "=");
                Object value = readNap(in);
                col.add(value);
                i++;
            }
            return col;
        }
        
        if (Map.class.isAssignableFrom(c)) {
            Map<Object, Object> map = (Map<Object, Object>)c.newInstance();
            readExpect(in, "{");
            while (true) {
                Object key = readNap(in);
                if (key.equals("INLINE_}"))
                    break;
                readExpect(in, "->");
                Object value = readNap(in);
                map.put(key,value);
            }
            return map;
        }
        
        if (c.equals(File.class)) {
            String fileName = readTo(in,"\"");
            return new File(fileName);
        }
      
        return readGeneric(c,in);
        
    }
    
    public void writeGeneric(Object obj, String indent, OutputStream out) {

        if (Thread.currentThread().getStackTrace().length > 64) {
            throw new Error("Stack is too big!");
        }
        
        write(out, neatClass(obj) + " {\n");
        final String subIndent = indent + "\t";
        
        Field[] fields = obj.getClass().getFields();
        if (fields.length == 0) {
            Logger.warn("Doing " + fields.length + " fields for " + obj);
        }
        for (Field f : fields) {
            Logger.debug("Doing " + f);
            try {
                // We might not need to show the type if the instance type ==
                // the field's declared type,
                // but we do need to display it, if the instance is a subclass
                // (one of many possible).
                Object val = f.get(obj);
                // String valStr = toString(val);
                write(out,subIndent + f.getName() + " = ");
                writeNap(val, subIndent, out);
                write(out,"\n");
            } catch (Throwable e) {
                Logger.error("Problem writing field "+f);
                Logger.error(e);
            }
        }
        write(out,indent + "}");
        
    }

    public Object readGeneric(Class c, InputStream in) throws Exception {
        Object obj = c.newInstance();
        // TODO: Read {} block and everything inside :P
        readExpect(in,"{");
        while (true) {
            // readWhitespace(in);
            // String fieldName = readTo(in," = ").trim();
            String token = readToken(in);
            if (token.equals("}")) {
                break;
            }
            
            String fieldName = token;
            readExpect(in,"= ");
            Field f = c.getField(fieldName);
            Object value = readNap(in);
            
//            if (c.equals(Long.class) || c.equals(Integer.class)
//                    || c.equals(Byte.class) || c.equals(Short.class)) {
//            value = new Long(""+value);
//            }
                
            // Logger.info("Setting "+fieldName+" to "+value);
            
            if (
                    f.getType().isArray()
                    && value instanceof Collection
            ) {
                // Object got = f.getType().newInstance(); // FAILS
                // Object got = Array.newInstance(f.getType(), 0); // Gives us array of arrays.
                Object got = Array.newInstance(f.getType().getComponentType(), 0);
                value = ((Collection)value).toArray( (Object[])got ); 
            }

            try {
                f.set(obj, value);
                continue;
            } catch (Exception e) {
            }

            // if (f.getType().isPrimitive())
            // if (f.getType().isLocalClass())

            // We could not set this value directly.
            // Maybe it is a primitive type.
            // e.g. we have been given a Double, but are trying to set an int.
            try {
                Class classType = f.getType();
                if (classType.isPrimitive()) {
                    classType = getClassTypeFromPrimitiveType(f.getType());
                    // Logger.log("Got "+neatClass(classType)+" from "+neatClass(f.getType()));
                    // Logger.log("Got "+(classType)+" from "+(f.getType()));
                }
                // Class[] parameterTypes = { value.getClass() };
                Class[] parameterTypes = { String.class };
                // f.getType().get;
                // ((PrimitiveType)f.getType()).getKind().
                Constructor cons = classType.getConstructor(parameterTypes);
                
                // If we are trying to convert to an integer, then we cannot have any "."s in the String, or the constructor will fail.
                if (
                        classType.equals(Integer.class) || classType.equals(Long.class)
                        || classType.equals(Byte.class) || classType.equals(Short.class)
                ) {
                    /*
                    if (value.toString().contains(".")) {
                        value = value.toString().replaceFirst("\\..*", "");
                    }
                    */
                    value = ((Number)value).longValue();
                }
                
                Object[] args = { value.toString() };
                value = cons.newInstance(args);
                f.set(obj, value);
                continue;
            } catch (Exception e) {
                Logger.warn(e);
            }

            try {
                value = new Long((long)(double)new Double(""+value));
                f.set(obj, value);
                continue;
            } catch (Exception e2) {
                Logger.error("Got exception trying to set "+f+" to "+value);
                Logger.error(e2);
            }
            
            Logger.error("Failed to set "+f+" to "+value);
            
        }
        return obj;
    }

    public final static String encode(String str) {
        return URLEncoder.encode(str);
    }
    
    public final static String decode(String str) {
        return URLDecoder.decode(str);
    }
    
    // For easier reading, we only encode the chars we really need to.
    // TODO: These are not 100% reversible!!!
    public final static String lazyEncode(String str) {
        str = str.replaceAll("&","&amp;");
        str = str.replaceAll("\"","&quot;");
        return str;
    }
    public final static String lazyDecode(String str) {
        str = str.replaceAll("&quot;","\"");
        str = str.replaceAll("&amp;","&");
        return str;
    }
    
    public static final Class[] primitiveTypes = {
        Integer.class, Long.class,
        Double.class, Float.class,
        Boolean.class, Character.class,
        Byte.class, Short.class,
    };
    
    public static Class getClassTypeFromPrimitiveType(Class type) {
        for (int i=0;i<primitiveTypes.length;i++) {
            Class classType = primitiveTypes[i];
            try {
                Class primitiveType = (Class)classType.getField("TYPE").get(null);
                if (type.equals(primitiveType)) {
                    return classType;
                }
            } catch (Exception e) {
                org.neuralyte.Logger.error(e);
            }
        }
        Logger.error("I do not know how to convert "+neatClass(type)+" to non-primitive.");
        return type;
    }
    
}

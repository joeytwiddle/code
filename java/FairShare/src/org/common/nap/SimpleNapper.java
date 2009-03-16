package org.common.nap;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.util.Collection;
import java.util.Map;

import org.fairshare.Logger;
import org.neuralyte.util.reflection.ReflectionHelper;

/*
 * TODO
// Currently this implementation knows a few vital types.
// The list of types appears in writeNap() and readNap().
// A better model we be to have a list of NapTypes
// which each has a write() and read() implementation.
 */

public class SimpleNapper implements Napper {

    public static String neatClass(Object obj) {
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

        // write(out,neatClass(obj) + " ");

        // Is this a type we handle in a special way?
        if (ReflectionHelper.isPrimitiveType(obj.getClass())) {
            write(out,""+obj);
            return;
        }
        
        if (obj instanceof String) {
            write(out,'\"' + ""+obj + '\"');
            return;
        }
        
        String subIndent = indent + "\t";

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
            write(out,indent+"}\n");
            return;
        }
        
        if (obj instanceof File) {
            File file = (File)obj;
            write(out,"file:" + file.getPath());
            return;
        }
        
        genericToString(obj, indent, out);
    }

    public Object readNap(InputStream in) throws Exception {

        String className = readTo(in," ");
        Class c = Class.forName(className);
        
        if (ReflectionHelper.isPrimitiveType(c)) {
            // TODO readPrimitive
            return null;
        }
        
        if (c.equals(String.class)) {
            // TODO BUG Will fail if String contains any \"s :P
            // (Suggest HTML en/decoding)
            readTo(in, "\"");
            String str = readTo(in,"\"");
            return str;
        }

        // TODO
        /*
        if (obj instanceof Collection) {
            Collection col = (Collection)obj;
            write(out,neatClass(col)+":\n");
            int i = 0;
            for (Object item : col) {
                // append(out,indent+"["+i+"] = "+toString(item));
                write(out,subIndent+"["+i+"] = ");
                writeNap(item,subIndent+"\t",out);
                write(out,"\n");
                i++;
            }
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
                write(out," = ");
                writeNap(val,subIndent,out);
                write(out,"\n");
            }
            write(out,indent+"}\n");
            return;
        }
        */
        
        if (c.equals(File.class)) {
            String fileName = readTo(in,"\"");
            return new File(fileName);
        }
      
        return readGeneric(c,in);
        
    }

    private Object readGeneric(Class c, InputStream in) throws Exception {
        Object obj= c.newInstance();
        // TODO: Read {} block and everything inside :P
        
        return obj;
    }

    public void genericToString(Object obj, String indent, OutputStream out) {
        
        write(out,indent + neatClass(obj) + " {\n");
        
        Field[] fields = obj.getClass().getFields();
        final String subIndent = indent + "\t";
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
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
        write(out,indent + "}\n");
        
    }

    public String readTo(InputStream in, String hit) throws IOException {
        StringBuffer got = new StringBuffer();
        StringBuffer check = new StringBuffer();
        while (true) {
            int i = in.read();
            if (i == -1)
                break;
            check.append((char)i);
            if (check.length() > hit.length()) {
                int c = check.charAt(0);
                check.deleteCharAt(0);
                got.append(c);
            }
            if (check.equals(hit))
                break;
        }
        return got.toString();
    }
    
}

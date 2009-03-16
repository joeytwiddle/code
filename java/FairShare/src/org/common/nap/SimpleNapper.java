package org.common.nap;

import java.io.File;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.util.Collection;
import java.util.Map;

import org.fairshare.Logger;
import org.fairshare.data.Database;
import org.neuralyte.util.reflection.ReflectionHelper;

public class SimpleNapper implements Napper {

    public Object fromString(String str) {
        return null;
    }

    public String toString(Object obj) {
        StringBuffer out = new StringBuffer();
        toString(obj, "", out);
        return out.toString();
    }

    public void toString(Object obj, String indent, StringBuffer out) {

        // Is this a type we handle in a special way?
        if (ReflectionHelper.isPrimitiveType(obj.getClass())) {
            out.append(indent + obj);
            return;
        }
        
        if (obj instanceof String) {
            out.append('\"' + ""+obj + '\"');
            return;
        }
        
        final String subIndent = indent = "\t";

        if (obj instanceof Collection) {
            Collection col = (Collection)obj;
            int i = 0;
            for (Object item : col) {
                // out.append(indent+"["+i+"] = "+toString(item));
                out.append(subIndent+"["+i+"] = ");
                toString(item,subIndent+"\t",out);
                out.append("\n");
                i++;
            }
            return;
        }
        
        if (obj instanceof Map) {
            out.append(indent + "[Map] " + obj.getClass() + " {\n");
            Map map = (Map)obj;
            Collection keySet = map.keySet();
            for (Object key : keySet) {
                Object val = map.get(key);
                
                out.append(subIndent+"\t"+toString(key)+" = "+toString(val)+"\n");
            }
            out.append(indent+"} #(end map)\n");
            return;
        }
        
        if (obj instanceof File) {
            File file = (File)obj;
            out.append(file.getPath());
            return;
        }
        
        genericToString(obj, indent, out);
    }

    public void genericToString(Object obj, String indent, StringBuffer out) {
        
        out.append(indent + obj.getClass().getName() + " {\n");
        
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
                out.append(subIndent + f.getName() + " = ");
                toString(val, subIndent, out);
                out.append("\n");
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
        out.append(indent + "}\n");
        
    }

    public void sendToStream(Object obj, OutputStream out) {
        // TODO
    }

}

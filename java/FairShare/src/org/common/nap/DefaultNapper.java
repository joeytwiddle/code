package org.common.nap;

import java.lang.reflect.Field;
import java.util.Collection;

import org.neuralyte.Logger;
import org.neuralyte.util.reflection.ReflectionHelper;

public class DefaultNapper implements Napper {

    public Object fromString(String str) {
        return null;
    }

    public String toString(Object obj) {
        return toString(obj, "");
    }

    public String toString(Object obj, String indent) {

        // Is this a type we handle in a special way?
        if (ReflectionHelper.isPrimitiveType(obj.getClass())) {
            return indent + obj;
        }
        
        if (obj instanceof Collection) {
            StringBuffer out = new StringBuffer();
            Collection col = (Collection)obj;
            int i = 0;
            for (Object item : col) {
                out.append(indent+"["+i+"] = "+toString(item,indent+"\t"));
                i++;
            }
        }
        
        return genericToString(obj, indent).toString();
    }

    private StringBuffer genericToString(Object obj, String indent) {
        StringBuffer out = new StringBuffer();
        out.append(indent + obj.getClass().getName() + " {\n");
        String subIndent = indent + "\t";
        Logger.log("Doing " + obj.getClass().getFields().length
                + " fields for " + obj);
        for (Field f : obj.getClass().getFields()) {
            Logger.debug("Doing " + f);
            try {
                // We might not need to show the type if the instance type ==
                // the field's declared type,
                // but we do need to display it, if the instance is a subclass
                // (one of many possible).
                Object val = f.get(obj);
                String valStr = toString(val, subIndent);
                out.append(subIndent + f.getName() + " = " + valStr + "\n");
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
        out.append(indent + "}\n");
        return out;
    }

}

package visualjava;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.jar.JarInputStream;

/** joey Nov 2, 2004 3:15:05 PM */
public class VisualJavaStatics {
	
    public static String listParams(Class[] parameterTypes) {
        String s = "";
        for (int i=0;i<parameterTypes.length;i++) {
            s += getSimpleClassName(parameterTypes[i]);
            if (i < parameterTypes.length - 1) {
                s += ", ";
            }
        }
        return s;
    }

    public static String getSimpleClassName(Class c) {
        if (c.isArray()) {
            return getSimpleClassName(c.getComponentType()) + "[]";
        } else {
            String full = c.getName();
            int i = full.lastIndexOf('.');
            if (i>=0) {
                full = full.substring(i+1);
            }
            return full;
        }
    }

    public static String niceString(Object o) {
	    final int maxLength = 50;
	    String s = ""+o;
	    if (s.length() > maxLength) {
	        return '"' + s.substring(0,maxLength - 3) + '"' + "...";
	    } else {
	        return '"' + s + '"';
	    }
	}

    /*
	public static String getShortClassName(Object _obj) {
		return JavaStackUtils.getShortClassName(_obj);
	}
	*/
	
    public static String getShortClassName(Object obj) {
    	if (obj == null)
    		return null;
    	try {
    		Class theClass = obj.getClass();
    		String name = theClass.getName();
    		int i = name.lastIndexOf(".");
    		if (i>=0) {
    			name = name.substring(i+1);
    		}
    		return name;
    	} catch (Exception e3) { }
    	return ""+obj;
	}

}

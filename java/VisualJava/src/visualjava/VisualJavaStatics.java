package visualjava;

import java.io.*;

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

    static String getSimpleClassName(Class c) {
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

    static BufferedReader getListOfAllClasses() {
        try {
            // final Process process = Runtime.getRuntime().exec("jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
            final Process process = Runtime.getRuntime().exec("/home/joey/j/jsh memo jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
            final BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            final PipedOutputStream out = new PipedOutputStream();
            final PipedInputStream in = new PipedInputStream(out);
            final BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
            new Thread() {
                public void run() {
                    try {
                        while (true) {
                            String line = reader.readLine();
                            if (line == null)
                                break;
                            // Strip .class extension
                            if (line.endsWith(".class")) {
                                line = line.substring(0,line.length() - ".class".length());
                            }
                            // Convert path '/'s into package '.'s
                            int i;
                            while ((i = line.indexOf("/")) >= 0) {
                                line = line.substring(0,i) + "." + line.substring(i+1);
                            }
                            while ((i = line.indexOf("\\")) >= 0) {
                                line = line.substring(0,i) + "." + line.substring(i+1);
                            }
                            // Inner classes
                            i = line.indexOf("$");
                            if (i >= 0) {
                                String after = line.substring(i+1);
                                try {
                                    Integer.parseInt(after);
                                    // Is an anonymous class: skip
                                    continue;
                                } catch (NumberFormatException e) {
                                    // Is not anonymous =)
                                    line = line.substring(0,i) + "." + line.substring(i+1);
                                }
                            }
                            writer.write(line + "\n");
                        }
                    } catch (IOException e) {
                        e.printStackTrace(System.err);
                    }
                }
            }.start();
            return new BufferedReader(new InputStreamReader(in));
        } catch (IOException e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    public static String getLastInPath(String fullPackage) {
        int i = fullPackage.lastIndexOf(".");
        if (i >= 0) {
            return fullPackage.substring(i+1);
        } else {
            return fullPackage;
        }
    }

    public static String getPackageFromClass(String className) {
        int i = className.lastIndexOf(".");
        return className.substring(0,i);
    }
}

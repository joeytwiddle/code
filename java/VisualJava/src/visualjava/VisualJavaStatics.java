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
        PipedOutputStream out = new PipedOutputStream();
        PipedInputStream in = null;
        try {
            in = new PipedInputStream(out);
        } catch (IOException e) {
            e.printStackTrace(System.err); // If we can't create the PipedOutputStream (never happens)
        }
        final PrintWriter writer = new PrintWriter(new OutputStreamWriter(out));

        new Thread() {
            public void run() {
                String classPath = System.getProperty("java.class.path");
                System.out.println(classPath);
                int j;
                while (( j = classPath.indexOf(":")) >= 0) {
                    String jarOrDir = classPath.substring(0,j);
                    classPath = classPath.substring(j+1);
                    String command;
                    File jarOrDirFile = new File(jarOrDir);
                    if (jarOrDirFile.isFile() && jarOrDirFile.getName().endsWith(".jar")) {
                        if (jarOrDirFile.getName().equals("rt.jar")) {
                            System.out.println("Extracting class list from jar: " + jarOrDirFile);
                            printClassesInJarTo(jarOrDirFile, writer);
                        } else {
                            System.out.println("Skipping non rt.jar: " + jarOrDirFile);
                        }
                    } else {
                        System.out.println("Do not yet know how to parse class list from directory or non-jar: " + jarOrDirFile);
                    }
                }
            }
        }.start();

        return new BufferedReader(new InputStreamReader(in));
    }

    private static void printClassesInJarTo(File jarFile, PrintWriter writer) {
        try {
            // final Process process = Runtime.getRuntime().exec("jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
            // final Process process = Runtime.getRuntime().exec("/home/joey/j/jsh memo jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
            // final Process process = Runtime.getRuntime().exec("sh -c 'jar tf `locate rt.jar | grep \"/rt.jar$\" | head -1`'");
            String[] cmdArray = { "jar", "tf", jarFile.getCanonicalPath() };
            final Process process = Runtime.getRuntime().exec(cmdArray);
            final BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
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
                    continue;
                    /** @todo It appears inner classes never have constructors
                     *  We may need to access their fields at some point,
                     *  but for the moment I have disabled them.
                    **/
                    /*
                    String after = line.substring(i+1);
                    try {
                        Integer.parseInt(after);
                        // Is an anonymous class: skip
                        continue;
                    } catch (NumberFormatException e) {
                        // Is not anonymous =)
                        line = line.substring(0,i) + "." + line.substring(i+1);
                    }
                    */
                }
                writer.println(line);
            }
        } catch (IOException e) {
            e.printStackTrace(System.err);
        }
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

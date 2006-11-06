package visualjava;

import java.io.*;

import org.neuralyte.common.Assertion;

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

    public static BufferedReader getListOfAllClasses() {
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
                Thread.currentThread().setPriority(1);
                String classPath = System.getProperty("java.class.path");
                if (classPath.indexOf(File.separatorChar + "rt.jar") == -1) { // a bit dodgy
                    classPath += ":" + System.getProperty("java.home") + "/lib/rt.jar";
                }
                System.out.println("Will parse classpath: " + classPath);
                int j;
                while (classPath.length() > 0) {
                    j = classPath.indexOf(":");
                    String jarOrDir;
                    if (j >= 0) {
                        jarOrDir = classPath.substring(0,j);
                        classPath = classPath.substring(j+1);
                    } else {
                        jarOrDir = classPath;
                        classPath = "";
                    }
                    if (VisualJavaGUIStatics.statusBar != null) {
                        VisualJavaGUIStatics.statusBar.setStatus("Parsing class names from "  + jarOrDir + " (" + classPath.split(":").length + " remaining on classpath) ...");
                    }
                    String command;
                    File jarOrDirFile = new File(jarOrDir);
                    if (jarOrDirFile.isFile() && jarOrDirFile.getName().endsWith(".jar")) {
                        // if (jarOrDirFile.getName().equals("rt.jar")) {
                            System.out.println("Extracting class list from jar: " + jarOrDirFile);
                            printClassesInJarTo(jarOrDirFile, writer);
                        // } else {
                            // System.out.println("Skipping non rt.jar: " + jarOrDirFile);
                        // }
                    } else {
                        System.out.println("Extracting class list from directory: " + jarOrDirFile);
                        printClassesInDirTo(jarOrDirFile,jarOrDirFile, writer);
                        // System.out.println("Do not yet know how to parse class list from directory or non-jar: " + jarOrDirFile);
                    }
                }
                writer.flush();
                writer.close();
                System.out.println("Class list sent.");
            }
        }.start();

        return new BufferedReader(new InputStreamReader(in));
    }

    private static void printClassesInDirTo(File topDir, File dir, PrintWriter writer) {
        try {
            Thread.yield();
            if (dir.isDirectory()) {
                File[] list = dir.listFiles();
                for (int i=0;i<list.length;i++) {
                    File file = list[i];
                    if (file.getName().startsWith(".")) {
                        System.out.println("Skipping " + file + " because it starts with a '.'.");
                        continue;
                    }
                    if (file.isDirectory()) {
                        if (file.getName().equals("CVS") && new File(file,"Repository").isFile()) {
                            System.out.println("Skipping " + file + " because it looks like a CVS directory.");
                            continue;
                        }
                        printClassesInDirTo(topDir, file, writer);
                    } else if (file.isFile() && file.getName().endsWith(".class")) {
                        //// Can get munged by symlinks!
                        // String relative = file.getCanonicalPath();
                        // relative = relative.substring( topDir.getCanonicalPath().length() + 1);
                        String relative = file.getAbsolutePath();
                        relative = relative.substring( topDir.getAbsolutePath().length() + 1);
                        if (!relative.endsWith(".class")) {
                            System.out.println("Managed to munge \"" + file.getAbsolutePath() + "\" into \"" + relative + "\" when topDir was \"" + topDir.getAbsolutePath() + "\".");
                        }
                        relative = getClassFromPath(relative);
                        if (relative == null)
                            continue;
                        writer.println( relative );
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
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
                Thread.yield();
                String line = reader.readLine();
                if (line == null)
                    break;
                // Strip .class extension
                if (!line.endsWith(".class")) {
                    continue;
                }
                line = getClassFromPath(line);
                if (line == null)
                    continue;
                if (line.indexOf("WebResponse")>=0)
                    System.out.println("> "+line);
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

    static String getClassFromPath(String line) {
        // Assertion.assertThat(line.endsWith(".class"));
        line = line.substring(0,line.length() - ".class".length());
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
            return null;
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
        return line;
    }

    public static String getPackageFromClass(String className) {
        int i = className.lastIndexOf(".");
        if (i>=0) {
            return className.substring(0,i);
        } else {
            return "";
        }
    }

}

package visualjava;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class ClassMenuBuilder {

	public static void printClassListFromResource(final PrintWriter writer, String jarOrDir) {
	    File jarOrDirFile = new File(jarOrDir);
	    if (jarOrDirFile.isFile() && jarOrDirFile.getName().endsWith(".jar")) {
	        // if (jarOrDirFile.getName().equals("rt.jar")) {
	    	org.neuralyte.Logger.log("Extracting class list from jar: " + jarOrDirFile);
	    	ClassMenuBuilder.printClassesInJarTo(jarOrDirFile, writer);
	        // } else {
	        // org.neuralyte.Logger.log("Skipping non rt.jar: " + jarOrDirFile);
	        // }
	    } else {
	    	org.neuralyte.Logger.log("Extracting class list from directory: " + jarOrDirFile);
	    	ClassMenuBuilder.printClassesInDirTo(jarOrDirFile,jarOrDirFile, writer);
	        // org.neuralyte.Logger.log("Do not yet know how to parse class list from directory or non-jar: " + jarOrDirFile);
	    }
	}

	public static void printClassesInDirTo(File topDir, File dir, PrintWriter writer) {
	    try {
	        Thread.yield();
	        if (dir.isDirectory()) {
	            File[] list = dir.listFiles();
	            for (int i=0;i<list.length;i++) {
	                File file = list[i];
	                if (file.getName().startsWith(".")) {
	                	org.neuralyte.Logger.log("Skipping " + file + " because it starts with a '.'.");
	                    continue;
	                }
	                if (file.isDirectory()) {
	                    if (file.getName().equals("CVS") && new File(file,"Repository").isFile()) {
	                    	org.neuralyte.Logger.log("Skipping " + file + " because it looks like a CVS directory.");
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
	                    	org.neuralyte.Logger.log("Managed to munge \"" + file.getAbsolutePath() + "\" into \"" + relative + "\" when topDir was \"" + topDir.getAbsolutePath() + "\".");
	                    }
	                    relative = ClassMenuBuilder.getClassFromPath(relative);
	                    if (relative == null)
	                        continue;
	                    writer.println( relative );
	                }
	            }
	        }
	    } catch (Exception e) {
	        org.neuralyte.Logger.error(e);
	    }
	}

	public static void printClassesInJarTo(File jarFile, PrintWriter writer) {
	    try {
	    	
	    	/*
	    	//// Method 1 - Call an external command to retrieve the list of classes in the jar.
	    	//// This method can potentially work under Unix and Windows.
	        // final Process process = Runtime.getRuntime().exec("jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
	        // final Process process = Runtime.getRuntime().exec("/home/joey/j/jsh memo jar tf /usr/lib/j2se/1.4/jre/lib/rt.jar");
	        // final Process process = Runtime.getRuntime().exec("sh -c 'jar tf `locate rt.jar | grep \"/rt.jar$\" | head -1`'");
	        
	        // String[] cmdArray = { "jar", "tf", jarFile.getCanonicalPath() };
	        String[] cmdArray = { "sh", "-c", "unzip -v "+jarFile.getCanonicalPath()+" | fromcol 9" };
	        
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
	                org.neuralyte.Logger.log("> "+line);
	            writer.println(line);
	        }
	        */
	        
	    	/*
	    	//// Method 2 - Use a JarInputStream (means streaming the whole jar!).
	        // JarInputStream jis = new JarInputStream(new BufferedInputStream(new FileInputStream(jarFile)));
	        JarInputStream jis = new JarInputStream(new FileInputStream(jarFile));
	        while (true) {
	        	JarEntry jarEntry = jis.getNextJarEntry();
	        	if (jarEntry == null)
	        		break;
	        		*/
	
	    	//// Method 3 - Use a jarfile.
	    	JarFile realJarFile = new JarFile(jarFile);
	    	Enumeration<JarEntry> entries = realJarFile.entries();
	    	while (entries.hasMoreElements()) {
	    		JarEntry jarEntry = entries.nextElement();
	
	    		if (jarEntry.getName().endsWith(".class")) {
	        		// writer.println(getClassFromPath(jarEntry.getName()));
	        		String path = jarEntry.getName();
	        		String className = ClassMenuBuilder.getClassFromPath(path); 
	        		// org.neuralyte.Logger.log("VJS adding: "+path+" -> "+className);
	        		if (className == null) {
	            		// Still breaks on inner classes (with $s)
	            		// org.neuralyte.Logger.log("VJS problem: "+path+" -> "+className);
	        		} else {
	        			writer.println(className);
	        		}
	        	}
	        }
	        
	    } catch (IOException e) {
	        org.neuralyte.Logger.error(e);
	    }
	}

	public static String getPackageFromClass(String className) {
	    int i = className.lastIndexOf(".");
	    if (i>=0) {
	        return className.substring(0,i);
	    } else {
	        return "";
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

	public static String getClassFromPath(String line) {
	    // Assertion.assertThat(line.endsWith(".class"));
	    line = line.substring(0,line.length() - ".class".length());
	    // Convert path '/'s into package '.'s
	    int i;
	    /*
	    while ((i = line.indexOf("/")) >= 0) {
	        line = line.substring(0,i) + "." + line.substring(i+1);
	    }
	    while ((i = line.indexOf("\\")) >= 0) {
	        line = line.substring(0,i) + "." + line.substring(i+1);
	    }
	    */
	    line = line.replaceAll("[/\\\\]",".");
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

}

// $Id: UncaughtExceptionLogger.java,v 1.1 2009/05/15 18:30:57 joey Exp $
/*
 * UncaughtExceptionLogger.java
 * Copyright (C) 2005 David Clausen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later 
 * version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library; if not, write to the 
 * Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */
package net.dclausen.util;

import java.util.logging.*;

/**
 * <p>
 * A <code>ThreadGroup</code> which logs its child <code>Threads'</code>
 * uncaught exceptions using the java logging API. This overrides the default 
 * <code>ThreadGroup</code> behavior of printing the stack trace to 
 * <code>System.err</code>.  This is particularly handy when dealing with 
 * threads started by libraries which are outside of your control.  For example,
 * when you have a Swing app and a misbehaving component is crashing the AWT 
 * event thread.
 * </p>
 * <p>
 * The recommended way to use this class is to construct an instance
 * very early in your application's bootstrap process -- ideally before
 * most classes are initialized -- and then continue your bootstrapping
 * on a new child thread.  The reason for doing this early on is because
 * some classes start threads in their static initializers, and you probably
 * want those threads to be owned by the <code>UncaughtExceptionLogger</code>.
 * Even the <code>java.util.logging.*</code> classes can start threads, which 
 * is why the constructors for this class take <code>String</code> arguments
 * rather than <code>Logger</code> and <code>Level</code> instances.  Those
 * names are resolved on a child thread.
 * </p>
 * <p>
 * UPDATE: As of Java 1.5, there is a better way of solving this problem.
 * Take a look at <code>ThreadGroup.setUncaughtExceptionHandler</code>
 * </p>
 * <p>
 * Here is a simple example of how to bootstrap your app:
 * </p>
 * 
 * <pre>
 * 
 *  ----------
 *      
 *  // MyAppLauncher.java
 * 
 *  // Keep it simple. Don't reference any unnecessary classes.
 * 
 *  public class MyAppLauncher {
 * 
 *     // Start a new thread which will have its (and all of its children's)
 *     // exceptions logged.  This thread will call MyApp.main(args)...
 *     public static void main(String[] args) {
 *         UncaughtExceptionLogger.invokeMain("", "WARNING", "MyApp", args);
 *     }
 * 
 *  }
 *  
 *  ----------     
 *       
 *  // MyApp.java
 * 
 *  import java.util.logging.*;
 *  import javax.swing.*;
 *  import oracle.jdbc.*;
 * 
 *  public class MyApp extends JFrame {
 * 
 *     public static void main(String[] args) {
 *         // Start the real app here. Initiaize classes, create threads, 
 *         // go crazy...
 *         MyApp app = new MyApp();
 *         ...
 *     }
 * 
 *  }
 *      
 *  ----------     
 * 
 * </pre>
 * 
 * @see java.util.logging.Logger
 * @see java.lang.Thread#run
 * @see java.lang.ThreadGroup#uncaughtException
 */
public class UncaughtExceptionLogger extends ThreadGroup {

    private final String loggerName, levelName;
    private Logger logger;
    private Level level;

    /**
     * Construct a new UncaughtExceptionLogger.
     * 
     * @param loggerName
     *            the name of the logger to use; defaults to "" if null
     * @param levelName
     *            the name of the logging level to use; defaults to "WARNING" if
     *            null
     */
    public UncaughtExceptionLogger(String loggerName, String levelName) {
        this(loggerName, levelName, null);
    }

    /**
     * Construct a new UncaughtExceptionLogger, and run <code>runnable</code>
     * on a child thread.
     * 
     * @param loggerName
     *            the name of the logger to use; defaults to "" if null
     * @param levelName
     *            the name of the logging level to use; defaults to "WARNING" if
     *            null
     * @param runnable
     *            an optional <code>Runnable</code> to run on a new child
     *            thread
     */
    private UncaughtExceptionLogger(String loggerName, String levelName,
            final Runnable runnable) {
        super("UncaughtExceptionLogger");
        if (loggerName == null) {
            loggerName = "";
        }
        if (levelName == null) {
            levelName = "WARNING";
        }
        this.loggerName = loggerName;
        this.levelName = levelName;
        // initialize the Logger on a child thread, and then run "runnable"
        new Thread(this, Thread.currentThread().getName() + "-logged") {
            public void run() {
                System.err.println("Starting: " + this);
                UncaughtExceptionLogger.this.init();
                if (runnable != null) {
                    runnable.run();
                }
            }
        }.start();
    }

    /**
     * Initialize the java.util.logging classes. This needs to be done on a
     * spawned thread because these classes could initialize the AWT event
     * thread, and we want that to happen on a thread which is owned by this
     * ThreadGroup (so the AWT thread will a member of this group).
     */
    private void init() {
        if (logger != null) {
            throw new IllegalStateException();
        }
        try {
            logger = Logger.getLogger(loggerName);
            level = Level.parse(levelName);
        } catch (Throwable e) {
            logger = Logger.getLogger("");
            level = Level.parse("WARNING");
            logger.log(level, "Error constructing logger or level", e);
        }
    }

    /**
     * Log the <code>Throwable</code> using the java logging API.  This 
     * method is called by the JVM when a child thread's <code>run()</code>
     * throws an <code>Exception</code> or <code>Error</code>.
     * 
     * @see Thread#run
     */
    public void uncaughtException(Thread t, Throwable e) {
        logger.log(level, "Thread[\"" + t.getName() + "\"] threw an exception", 
                e);
    }

    /**
     * Construct a new UncaughtExceptionLogger, and invoke 
     * <code>mainClass.main(args)</code> on a child thread.  
     * 
     * @param loggerName
     *            the name of the logger to use
     * @param levelName
     *            the name of the logging level to use
     * @param mainClass
     * 	          the name of a class which contains a static main method
     * @param args
     *            the arguments to pass to mainClass.main
     */
    public static void invokeMain(String loggerName, String levelName,
            String mainClass, String[] args) {
        new UncaughtExceptionLogger(loggerName, levelName,
                new ReflectiveInvoker(mainClass, "main",
                        new Class[] { new String[0].getClass() }, 
                        new Object[] { args }));        
    }
    
}
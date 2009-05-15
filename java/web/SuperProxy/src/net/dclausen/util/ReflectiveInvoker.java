// $Id: ReflectiveInvoker.java,v 1.1 2009/05/15 18:30:57 joey Exp $
/*
 * MainInvoker.java
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

import java.lang.reflect.*;

/**
 * <code>Runnable</code> which reflectively invokes a given static method.
 * The class is not initialized until the <code>run()</code> method is called.
 * This is used by <code>UncaughtExceptionLogger</code>.
 * 
 * @see UncaughtExceptionLogger
 */
class ReflectiveInvoker implements Runnable {

    private final String className;
    private final String methodName;
    private final Class[] argTypes;
    private final Object[] args;

    ReflectiveInvoker(String className, String methodName,
            Class[] argTypes, Object[] args) {
        this.className = className;
        this.methodName = methodName;
        this.argTypes = argTypes;
        this.args = args;
    }

    /**
     * Initialize the class named by <code>className</code> and call the static
     * method defined by <code>methodName</code> and <code>argTypes</code>,
     * passing <code>args</code> as the arguments.
     */
    public void run() {
        try {
            Class cls = Class.forName(className);
            Method method = cls.getMethod(methodName, argTypes);
            try {
                method.invoke(null, args);
            } catch (InvocationTargetException e) {
                throw e.getCause();
            }
        } catch (Throwable e) {
            if (e instanceof RuntimeException) {
                throw (RuntimeException) e;
            } else if (e instanceof Error) {
                throw (Error) e;
            } else {
                throw new RuntimeException(e);
            }
        }
    }
}
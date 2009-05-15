// $Id: SignalInterceptor.java,v 1.1 2009/05/15 18:30:57 joey Exp $
/*
 * SignalInterceptor.java
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

/**
 * <p>
 * A simple wrapper around the undocumented and unsupported
 * <code>sun.misc.Signal*</code> classes which are present in most JVM
 * implementations. It can be used to trap OS signals such as SIGINT, SIGTERM,
 * and SIGHUP on these JVMs.
 * </p>
 * 
 * <p>
 * <code>SignalInterceptor</code> provides a level of indirection
 * between your code and <code>sun.misc.SignalHandler</code> which can
 * be useful for the following reasons:
 * </p>
 * 
 * <ol>
 * <li>It gracefully handles the situation when the
 * <code>sun.misc.Signal*</code> classes are not available, throwing a checked
 * exception out of the <code>register</code> method, rather than awkward
 * <code>LinkageErrors</code> durring class initialization.</li>
 * <li>It has a simple technique for chaining signal handlers.</li>
 * <li>It is documented and has gone through some testing.</li>
 * </ol>
 * 
 * <p>
 * Some possible uses of this class are:
 * </p>
 * <ul>
 * <li>Change the behavior of SIGHUP so that instead of exiting the JVM, your
 * application reloads its configuration file.</li>
 * <li>Log the fatal signals SIGHUP, SIGINT, and SIGTERM before passing them 
 * onto the JVM's native handlers, so that you have a record of when and why 
 * your application exited.</li>
 * <li>Implement your own version of ShutdownHooks which run prior to the
 * standard ones. This can be useful because many other packages have their own
 * ShutdownHooks which close resources which you might like to use. For example
 * the <code>java.util.logging</code> package closes all of its files in its
 * ShutdownHook, which means your ShutdownHook might not be able to log
 * anything. Note that you would still need to deal with
 * <code>System.exit()</code>, but there is a different hack available for
 * that <code>;)</code>.</li>
 * </ul>
 *  
 */
public abstract class SignalInterceptor {

    protected SignalInterceptor() {
    }

    /**
     * <p>
     * Register for the given signal. Note that the signal name should 
     * <b>not</b> begin with <b>SIG</b>. For example, if you are interested in
     * <b>SIGTERM</b>, you should call <code>register("TERM")</code>.
     * </p>
     * 
     * <p>
     * If the registration fails for any reason, a
     * <code>SignalInterceptorException</code> will be thrown. This is usually
     * caused by one of the following conditions:
     * </p>
     * <ul>
     * <li>The <code>sun.misc.Signal*</code> classes are not available (e.g.
     * you are not using Sun's JVM).</li>
     * <li><code>signame</code> is not a valid trappable signal name on this
     * OS (e.g. <b>KILL</b> can't be trapped, <b>HUP</b> does not exist on
     * Windows)</li>
     * <li>The JVM refuses to let you trap <code>signame</code> because it is
     * already being used for some other important purpose (e.g. <b>QUIT</b>
     * and/or <b>BREAK</b> cause the JVM to print diagnostic output).</li>
     * </ul>
     */
    protected void register(String signame)
            throws SignalInterceptorException {
        try {
            new SignalInterceptorHelper(signame, this);
        } catch (Throwable e) {
            throw new SignalInterceptorException(signame, e);
        }
    }

    /**
     * A wrapper around <code>register(String)</code> which never throws an
     * exception. Instead, it returns <code>true</code> if registration
     * succeeded, and <code>false</code> if it failed.
     */
    protected boolean registerQuietly(String signame) {
        try {
            register(signame);
        } catch (Throwable e) {
            return false;
        }
        return true;
    }

    /**
     * Handle the given signal (which you had previously registered for). If 
     * this method return false, or throws an exception, subsequent handlers 
     * in the chain will <b>not</b> be called.
     */
    protected abstract boolean handle(String signame);

}
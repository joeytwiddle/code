// $Id: ExitLogger.java,v 1.1 2009/05/15 18:30:57 joey Exp $
/*
 * ExitLogger.java
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

import java.security.Permission;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * A simple {@link java.lang.SecurityManager} which logs calls to {@link #checkExit}.
 * When an instance of <code>ExitLogger</code> is installed as the System's default
 * <code>SecurityManager</code>, it acts as a trap for {@link System#exit} and
 * {@link Runtime#exit}, because those two functions both call
 * <code>System.getSecurityManager().checkExit()</code> before terminating the JVM.
 * <p>
 * All permission checks (including <code>checkExit</code>) are forwarded to the
 * System's previous <code>SecurityManager</code>, if one was set.
 * <p>
 * Use the static {@link #install} method to install a new instance of this class
 * as the System's <code>SecurityManager</code>.
 *
 * @see System#exit(int)
 * @see Runtime#exit(int)
 * @see SecurityManager
 * @see System#setSecurityManager(SecurityManager)
 * @see Logger
 */
public class ExitLogger extends SecurityManager {

    /**
     * Install an instance of ExitLogger as the System's <code>SecurityManager</code>.
     * The existing <code>SecurityManager</code>, if one exists, will be used as a
     * delegate for all permission checks.
     *
     * @param logger the Logger to use when logging calls to System.exit() and
     *               Runtime.exit()
     * @param level the level to use
     * @see System#setSecurityManager
     * @throws SecurityException if the current SecurityManager doesn't want to be
     *                           replaced
     */
    public static void install(Logger logger, Level level) {
        new ExitLogger(logger, level);
    }

    private final Logger logger;
    private final Level level;
    private final SecurityManager parent;

    private ExitLogger(Logger logger, Level level) {
        if ((logger == null) || (level == null)) {
            throw new NullPointerException();
        }
        this.logger = logger;
        this.level = level;
        parent = System.getSecurityManager(); // race condition
        System.setSecurityManager(this);
    }

    /**
     * Delegate the check to the parent SecurityManager, if there is one.
     */
    public void checkPermission(Permission perm) {
        if (parent != null) {
            parent.checkPermission(perm);
        }
    }

    /**
     * Use stack introspection to identify the calling class/method/line, and
     * log it. This method is called by <code>Runtime.exit(int)</code> before
     * actually stopping the JVM.
     */
    public void checkExit(int status) {
        Exception e = new Exception();
        StackTraceElement[] trace = e.getStackTrace();
        String method = "<unknown>";
        if ((trace != null) && (trace.length > 2)) {
            int idx = 2;
            // System.exit() calls Runtime.exit(), so if that is how
            // we got here, we need to go up one more stack frame
            // to get into user-level code
            if ((trace.length > 3)
		&& "java.lang.System".equals(trace[2].getClassName())
		&& "exit".equals(trace[2].getMethodName())) {
                idx++;
            }
            method = trace[idx].toString();
        }
        logger.log(level, "exit({0}) called at {1}", new Object[] {
	    new Integer(status), method });
        super.checkExit(status);
    }

}

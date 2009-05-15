// $Id: SignalLogger.java,v 1.1 2009/05/15 18:30:57 joey Exp $
/*
 * SignalLogger.java
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
 * A <code>SignalInterceptor</code> which logs incoming signals using the java
 * logging API, then invokes the next handler in the chain.
 */
public class SignalLogger extends SignalInterceptor {

    private final Logger logger;
    private final Level level;

    /**
     * Construct a <code>SignalLogger</code> which will log signals to the
     * root <code>Logger</code> at the <code>INFO</code> level.
     */
    public SignalLogger() {
        this(null, null);
    }

    /**
     * Construct a <code>SignalLogger</code> which will log signals to the
     * given <code>Logger</code> at the given <code>Level</code>.
     * 
     * @param logger the logger to use; if null, the root logger will be used
     * @param level the level to use; if null the INFO level will be used
     */
    public SignalLogger(Logger logger, Level level) {
        if (logger == null) {
            logger = Logger.getLogger("");
        }
        if (level == null) {
            level = Level.INFO;
        }
        this.logger = logger;
        this.level = level;
    }

    /**
     * Log the signal and return true, so that the default handler will be invoked.
     */
    protected boolean handle(String signame) {
        logger.log(level, "Received signal: SIG{0}", signame);
        return true;
    }

}
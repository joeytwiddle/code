// $Id: SignalInterceptorHelper.java,v 1.1 2009/05/15 18:30:57 joey Exp $
/*
 * SignalInterceptorHelper.java
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

import sun.misc.Signal;
import sun.misc.SignalHandler;

/**
 * <p>
 * Private helper class for <code>SignalInterceptor</code>.
 * </p>
 * 
 * <p>
 * This class exists separately from <code>SignalInterceptor</code> to permit
 * graceful handling of LinkageErrors when the <code>sun.misc.Signal*</code>
 * classes don't exist.
 * </p>
 */
final class SignalInterceptorHelper implements SignalHandler {

    private final SignalHandler oldHandler;

    private final SignalInterceptor interceptor;

    SignalInterceptorHelper(String signame, SignalInterceptor interceptor) {
        this.interceptor = interceptor;
        Signal signal = new Signal(signame);
        oldHandler = Signal.handle(signal, this);
    }

    public void handle(Signal sig) {
        if (interceptor.handle(sig.getName()) && (oldHandler != null)) {
            oldHandler.handle(sig);
        }
    }

}
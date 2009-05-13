package org.common.nap;

/**
 * An object which can have text appended to it.
 * 
 * The underlying model could be, for example, a Stream, or a StringBuffer.
 * 
 * @author joey
 *
 */

public abstract class Appender {

    public abstract void append(String str);
    
    /**
     * AKA a "break".
     */
    public void nl() {
        append("\n");
    }
    
}

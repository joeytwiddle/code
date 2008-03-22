/*
 * Created on Nov 1, 2006
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package org.neuralyte.common;


/**
 * @author joey
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public abstract class LoudThread /* extends java.lang.Thread */ {

    String type = null;
    
    public LoudThread(String _type) {
        type = _type;
    }
    
    public synchronized void start() {
        new Thread() {
            public void run() {
            	org.neuralyte.Logger.error("STARTING THREAD "+this+" "+type);
                this.run();
                org.neuralyte.Logger.error("END THREAD "+this+" "+type);
            }
        }.start();
    }
    
    public abstract void run();
    
}

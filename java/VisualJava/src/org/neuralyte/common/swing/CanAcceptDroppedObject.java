package org.neuralyte.common.swing;

/** joey Nov 3, 2004 4:38:47 PM */
public interface CanAcceptDroppedObject {
    public boolean isAcceptable(Object o);
    /** @todo I think this shouldn't return a boolean,
     *  but it should throw an Exception if a problem occurs,
     *  which should not happen provided isAcceptable(o).
    **/
    public boolean acceptObject(Object o);
}

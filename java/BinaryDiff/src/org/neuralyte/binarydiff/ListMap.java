package org.neuralyte.binarydiff;

import java.util.Map;
import java.util.List;
import java.util.Vector;
import java.util.HashMap;

/** joey Jun 15, 2006 6:48:06 PM */
public class ListMap extends HashMap {
    public synchronized List getListFor(Object key) {
        List list = (List)get(key);
        if (list == null) {
            list = new Vector();
            put(key,list);
        }
        return list;
    }

}

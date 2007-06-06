package org.neuralyte.fastbuffers;

import java.util.HashMap;

/** joey Jun 15, 2006 6:48:06 PM */
public class IntListMap extends HashMap {

    public synchronized IntList getListFor(Object key) {
        IntList list = (IntList)get(key);
        if (list == null) {
            list = new IntList();
            put(key,list);
        }
        return list;
    }

}

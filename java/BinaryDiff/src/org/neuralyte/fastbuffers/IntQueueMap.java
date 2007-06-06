package org.neuralyte.fastbuffers;

import java.util.HashMap;

/** joey Jun 15, 2006 6:48:06 PM */
public class IntQueueMap extends HashMap {

    public synchronized IntQueue getQueueFor(int sum) {
        Object key = new Integer(sum);
        IntQueue intQueue = (IntQueue)super.get(key);
        if (intQueue == null) {
            intQueue = new IntQueue();
            put(key,intQueue);
        }
        return intQueue;
    }

}

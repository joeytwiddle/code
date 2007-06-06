package org.neuralyte.fastbuffers;



/** joey Jun 30, 2006 10:57:55 PM */

/** This is like a IntList except that items can be efficiently removed
 *  from the front of the list.
 */

public class IntQueue extends IntList {

    int start = 0;

    public int get(int index) {
        return super.get(start + index);
    }

    public void removeFirst() {
        start++;
    }

    public int size() {
        return length - start;
    }

    public void expandStore() {
        synchronized (this) {
            // @todo We could save and retrieve older small lists in/from a pool, rather than reclaiming old ones and creating new ones.
            int newMaxSize = maxSize * 2;
            int[] newList = new int[newMaxSize];
            for (int i=start;i<length;i++) {
                newList[i-start] = list[i];
            }
            list = newList;
            maxSize = newMaxSize;
            length = length - start;
            start = 0;
        }
    }

}

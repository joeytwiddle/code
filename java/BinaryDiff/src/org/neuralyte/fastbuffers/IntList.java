package org.neuralyte.fastbuffers;

/** joey Jun 30, 2006 12:52:45 AM */
public class IntList {

    int maxSize;
    int[] list;

    int length = 0;

    public IntList() {
        this(2);
    }

    public IntList(int initialMaxSize) {
        maxSize = initialMaxSize;
        list = new int[maxSize];
    }

    public int size() {
        return length;
    }

    public void add(int val) {
        synchronized (this) {
            if (length >= maxSize) {
                expandStore();
            }
            list[length] = val;
            length++;
        }
    }

    public void expandStore() {
        synchronized (this) {
            // @todo We could save and retrieve older small lists in/from a pool, rather than reclaiming old ones and creating new ones.
            int newMaxSize = maxSize * 2;
            int[] newList = new int[newMaxSize];
            for (int i=0;i<length;i++) {
                newList[i] = list[i];
            }
            list = newList;
            maxSize = newMaxSize;
        }
    }

    public int get(int index) {
        if (index<0 || index>length-1) {
            throw new ArrayIndexOutOfBoundsException();
        }
        return list[index];
    }

    /** Removes all entries from startIndex (inclusive) to endIndex (exclusive). **/
    public void removeEntries(int startIndex, int endIndex) {
        if (endIndex == length) {
            length = startIndex;
        } else {
            //// Incorrect; overflows if numToRemove > length - endIndex:
            // for (int i=startIndex;i<endIndex;i++) {
                // list[i] = list[endIndex+i-startIndex];
            // }
            int numToRemove = endIndex - startIndex;
            for (int i=0;i<numToRemove && endIndex+i<length;i++) {
                list[startIndex+i] = list[endIndex+i];
            }
            length = length - (endIndex-startIndex);
        }
    }

}

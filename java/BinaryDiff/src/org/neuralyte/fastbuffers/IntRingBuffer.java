package org.neuralyte.fastbuffers;

/** joey Jun 30, 2006 10:54:31 PM */
public class IntRingBuffer {

    public int max;
    public int position;
    public int[] buffer;

    public IntRingBuffer(int max) {
        this.max = max;
        this.position = 0;
        this.buffer = new int[max];
    }

    public void addInt(int i) {
        buffer[position%max] = i;
        position++;
    }

    public int get(int oldPos) {
        // @todo Optional check; remove for efficiency
//        if (oldPos>=position || oldPos<position-max) {
//            throw new ArrayIndexOutOfBoundsException(oldPos);
//        }
        return buffer[oldPos%max];
    }

}

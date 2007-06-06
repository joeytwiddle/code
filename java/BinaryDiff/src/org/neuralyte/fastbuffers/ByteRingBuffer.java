package org.neuralyte.fastbuffers;

/** joey Jun 30, 2006 10:54:31 PM */
public class ByteRingBuffer {

    public int max;
    public int position;
    public byte[] buffer;

    public ByteRingBuffer(int max) {
        this.max = max;
        this.position = 0;
        this.buffer = new byte[max];
    }

    public void addByte(byte b) {
        buffer[position%max] = b;
        position++;
    }

    public byte get(int oldPos) {
        // @todo Optional check; remove for efficiency
//        if (oldPos>=position || oldPos<position-max) {
//            throw new ArrayIndexOutOfBoundsException(oldPos);
//        }
        return buffer[oldPos%max];
    }

}

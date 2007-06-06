package org.neuralyte.binarydiff;

import org.neuralyte.fastbuffers.ByteRingBuffer;

import java.io.*;

/** joey Jun 29, 2006 12:31:45 AM */
public class Outputter {

    BinaryDiffOptions options;

    OutputStream out;

//    final private int STATE_WRITING_BYTES = 1; // pretty much implied by bytesToWrite.length()>0
//    final private int STATE_STARTING_NEW_BLOCK = 2;
//    int state;

    // StringBuffer bytesToWrite = new StringBuffer();
    ByteRingBuffer bytesToWrite = new ByteRingBuffer(255);
    // Replaced StringBuffer with a ByteRingBuffer, because this doesn't alter the contents!
    // I should have used a FastByteList really, but I haven't implemented one yet.  :P

    int bytesWritten = 0;

    Outputter(BinaryDiffOptions options, File outputFile) throws FileNotFoundException {
        this(options,new FileOutputStream(outputFile));
    }

    Outputter(BinaryDiffOptions options, OutputStream out) throws FileNotFoundException {
        this.out = out;
        out = new BufferedOutputStream(out);
        this.options = options;
    }

    public void output_line(String line) throws IOException {
        if (!options.printNoHeaders) {
            write_byte_array( (line+"\n").getBytes() );
            bytesWritten += line.length() + 1;
            System.err.println("output_line: "+line);
        }
    }

    public void output_rawByte(byte b) throws IOException {
//        state = STATE_WRITING_BYTES;
        // bytesToWrite.append((char)b);
        bytesToWrite.addByte(b);
        bytesWritten++;
        if (bytesToWrite.position >= options.MAX_BEFORE_FLUSH) {
            flushBytesBuffer();
        }
    }

    public void output_copyEarlierBlock(int earlierPos, short length) throws IOException {
        flushRawByteBufferIfNeeded();
        // System.out.println("Copying block length "+length);
        write_one_byte((byte)0); // Indicates we are about to write copy information
        write_one_int(earlierPos); // If *relative* could possibly be a 3-byte number ^^
        write_one_short(length);
        bytesWritten += 7;
    }

    /*
    public void output_copyEarlierBlock(byte sourcenum, int sourcePos, short length) throws IOException {
        flushRawByteBufferIfNeeded();
        // System.out.println("Copying block length "+length);
        write_one_byte((byte)0); // Indicates we are about to write copy information
        write_one_short((byte)sourcenum);
        write_one_int(sourcePos);
        write_one_short(length);
        bytesWritten += 9;

        // Instead of sourcenum and pos, have the number of bytes back in the stream to start copying from
        // This can have a max known to the builder and the extractor.
        // three bytes can point back 16meg; 4 bytes, 4gig

        // Size of data structures in memory will be:
        // (part_sum_size*total_length) + (total_length-scansize)*(size_of_cksum_entry+list_entry_cost) ( + a_list_for_each_cksum )
        // = total_length * (part_sum_size + size_of_cksum_entry+list_entry_cost)
        // We can divide this all by resolution if say, instead of scanning bytes, we want to scan whole 1k blocks
        // part_sum_size = 2/4bytes preferably
        // size_of_cksum_entry = 4byte back-reference
        // so we expand data 8 times in memory :S

        // Time taken will be harder to calculate
        // It depends on the number of false (and +ve) cksum collisions
        // which depends on the repetition of the input, it's length, and the scanSize

    }
    */

    private void flushRawByteBufferIfNeeded() throws IOException {
        if (bytesToWrite.position > 0) {
            flushBytesBuffer();
        }
    }

    private void flushBytesBuffer() throws IOException {
        if (!options.quiet) {
            // System.out.println("Writing "+bytesToWrite.position+" bytes.");
            // System.out.println("Writing "+bytesToWrite.position+" bytes: "+showBytes(bytesToWrite.toString().getBytes()));
        }
        write_one_byte((byte)bytesToWrite.position);
        // write_byte_array(bytesToWrite.toString().getBytes());
        write_byte_array(bytesToWrite.buffer,0,bytesToWrite.position);
        bytesToWrite.position = 0;
        // bytesToWrite.delete(0,bytesToWrite.length());
    }

    public void flush() throws IOException {
        flushRawByteBufferIfNeeded();
        out.flush();
    }

    public void close() throws IOException {
        out.close();
    }



    // For debugging:

    private String showBytes(byte[] bytes) {
        String show = "";
        for (int i=0;i<8 && i<bytes.length;i++) {
            show += "" + bytes[i] + ", ";
        }
        return show.substring(0,show.length()-2);
    }



    // Utility methods:

    private void write_one_byte(byte i) throws IOException {
        out.write(i);
    }

    private void write_one_short(short i) throws IOException {
        out.write((byte)((i) & 0xFF));
        out.write((byte)((i>>>8) & 0xFF));
    }

    private void write_one_int(int i) throws IOException {
        out.write((byte)((i) & 0xFF));
        out.write((byte)((i>>>8) & 0xFF));
        out.write((byte)((i>>>16) & 0xFF));
        out.write((byte)((i>>>24) & 0xFF));
    }

    private void write_byte_array(byte[] bytes) throws IOException {
        out.write(bytes);
    }

    private void write_byte_array(byte[] bytes, int offset, int length) throws IOException {
        out.write(bytes,offset,length);
    }

}

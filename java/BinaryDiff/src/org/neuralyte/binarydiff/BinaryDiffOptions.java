package org.neuralyte.binarydiff;

import java.io.IOException;

/** joey Jun 30, 2006 1:42:30 AM */
public class BinaryDiffOptions {

    /** @todo Move all options to one class, and have them optionally added as
     * headers at the beginning of the output. **/

    public boolean printDetailedHeaders = true;

    public boolean printNoHeaders = false; // This was for debugging; might be a good idea to remove it now

    public boolean quiet = true;

    // public int scanSize = 64;
    public int scanSize = 8;
    // public static int resolution;
    // public static int numBytesforRawBlockLength;
    // public static int numBytesForCopyBackRef;
    // public static int numBytesForCopyLength;

    // History cleaning used to not work properly on multiple files, but I think now it does.
    public int maxHistory = 1024*1024*5; // Keep history of last 20Meg's checksums, -1 to keep all.

    // int HISTORY_RECHECK = 1024*1024; // clear old history once every 1meg (this is quite slow, but needed now and then to preserve memory)

    //// For the outputter, depends on numBytesForRawBlockLength
    // int MAX_BEFORE_FLUSH = 65536 / 2 - 2; // withing range of short; @todo could be expanded to 65535
    // int MAX_BEFORE_FLUSH = 65535; // withing range of short
    int MAX_BEFORE_FLUSH = 255; // withing range of byte (because we only use 1 byte to specify the number of raw bytes to copy)

    // int MAX_EXPANSION_LENGTH = 256*256*256*256 / 2 - 2; // within range of int
    // int MAX_EXPANSION_LENGTH = 256*256 / 2 - 2; // within range of short
    // int MAX_EXPANSION_LENGTH = 65535; // within range of short (implied by maxReadAhead for new algorithm; but shorts don't really have this max!!)
    // int MAX_EXPANSION_LENGTH = 65536/2-1; // within range of short (implied by maxReadAhead for new algorithm; but shorts don't really have this max!!)
    // int MAX_EXPANSION_LENGTH = 255; // within range of byte

    // maxReadAhead says: you mustn't make a match longer than this, because a number that large won't fit in the output format. (currently 16-bits) 
    public int maxReadAhead = 65535-scanSize; // DONE we can fix this for 65535 and store it as a short, provided we fix it to an int when we get it back!
    // Doesn't seem much point making this an option but DONE it may be too large if scanSize is increased by user

    public void printOptionsToOutput(Outputter output) throws IOException {

        if (printDetailedHeaders) {
            output.output_line("Scan-size: "+scanSize);
            output.output_line("Max-history: "+maxHistory);
            output.output_line("Max-read-ahead: "+maxReadAhead);
            /*
            output.output_line("Max-history: "+maxHistory);
            output.output_line("History-recheck: "+HISTORY_RECHECK);
            output.output_line("Max-before-flush: "+MAX_BEFORE_FLUSH);
            output.output_line("Max-expansion-length: "+MAX_EXPANSION_LENGTH);
            output.output_line("Quiet: "+quiet);
            */
        }

    }

}

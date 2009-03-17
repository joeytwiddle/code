package org.neuralyte.binarydiff;

import org.neuralyte.fastbuffers.ByteRingBuffer;
import org.neuralyte.fastbuffers.IntQueue;
import org.neuralyte.fastbuffers.IntQueueMap;
import org.neuralyte.fastbuffers.IntRingBuffer;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

// TODO: create a queue to hold each sum's window positions, like an expandableRingBuffer with a moving head as well as tail. :-o

// Mmmm trying to oprimize everything here
// Instead of adding a window's position to the psum quque when a match has been found for it,
// it should overwrite the earlier position as a more recent copy.
// This will save later checks with the same sum but different data from comparing against this duplicate twice.
// Still even this won't work on those window sums added when passing (skipping) already copied data.
// Mmmm maybe we can do something special during skipping mode? ^^
// Ah soddit; maybe these undesirable duplicates are irrelevant compared to the over steps we are making towards efficiency... :)

/** joey Jun 30, 2006 5:28:35 PM */
public class RepetitionFinder {

    // Initialisation:
    BinaryDiffOptions options;
    Outputter output;

    // Running:
    ByteRingBuffer byteHistory;
    IntRingBuffer psumHistory;
    IntQueueMap windowSumHistoryMap;
    int position = 0;
    int skipOutput = 0;
    int bytesRemaining = -1; // unknown at start; once end of stream is reached, this is the number of bytes we still need to output...

    void run(BinaryDiffOptions _options, InputStream in, Outputter _output) throws IOException {

        this.options = _options;
        this.output = _output;

        // this.in = new PushbackInputStream(inOriginal, maxReadAhead);
        // this.in = inOriginal;

        byteHistory = new ByteRingBuffer(options.maxHistory+options.maxReadAhead);
        psumHistory = new IntRingBuffer(options.maxHistory+options.maxReadAhead);
        windowSumHistoryMap = new IntQueueMap();

        int psum = 0;

        while (true) {

            // If sumListMap has reached capacity, remove oldest item
            // (In theory we could look at the sum of the window we just lost), and remove the first element from the list
            if (position >= options.maxHistory) {
                removeRedundantWindowEntry();
            }

            if (bytesRemaining == -1) {

                psum = readAndPropogateByte(in, psum);

            }

            // To begin with we just fill up the buffers
            // Only after a certain amount of data has been read,
            // then we start generating sums for windows, and look for repetitions.

            if (position >= options.scanSize + options.maxReadAhead) {

                // This byte has created a new window

                int windowStart = position - options.scanSize - options.maxReadAhead;

                if (bytesRemaining>-1 && bytesRemaining<options.scanSize+1) { // Never question the +1 !! ;P
                    // At the end of the file, there is no window to scan; just output this byte
                    if (skipOutput > 0) {
                        // We have already written (copied) this data to output
                        skipOutput--;
                    } else {
                        System.out.println("Sending one of the last bytes (#"+windowStart+") ("+byteHistory.get(windowStart)+")");
                        sendOutputRawByte(byteHistory.get(windowStart));
                    }
                } else {

                    writeMatchOrRaw(windowStart);

                }

                if ((windowStart % 4096) == 0 /* || bytesRemaining>=0 */ /* && !options.quiet */) {
                    // System.out.println("Pos "+windowStart+"  Out "+output.bytesWritten+"  BytesTodo "+bytesRemaining+"  Skip "+skipOutput);
                }

            }

            position++;

            if (bytesRemaining>0) {
                bytesRemaining--;
                if (bytesRemaining == 0) {
                    break;
                }
            }

        }

        output.flush();

    }

    private void removeRedundantWindowEntry() {
        int oldwindowStart = position - options.maxHistory;
        int oldwindowEnd = position - options.maxHistory + options.scanSize;
        int oldwindowSum = psumHistory.get(oldwindowEnd) - psumHistory.get(oldwindowStart);
        IntQueue queue = windowSumHistoryMap.getQueueFor(oldwindowSum);
        if ( queue.get(0) != oldwindowStart ) {
            System.err.println("Failure in history clearing logic: "+queue.get(0)+" != "+oldwindowStart);
            System.err.println("Oldwin="+oldwindowStart+" Sum="+oldwindowSum);
        } else {
//                    System.err.println("Clearing oldPos="+oldwindowStart+" from queue Sum="+oldwindowSum);
            queue.removeFirst();
            // To save memory, but it doesn't seem to work, and it will probably slow down the process a bit:
            // Disabled, since it wasn't having the desired effect.
            if (queue.size() == 0 /* && queue.maxSize >= 64 */ ) {
                windowSumHistoryMap.remove(new Integer(oldwindowSum));
            }
            // For some reason even if I do this, the memory usage keeps expanding =/
        }
    }

    private int readAndPropogateByte(InputStream in, int psum) throws IOException {
        int v = in.read();
        if ( v == -1 ) {
            // OK so we can't read any more in
            // But we still have maxReadAhead+scanSize bytes to output
            // the first maxReadAhead of which have windows which may be matched
            // the last scanSize of which are raw data.
            bytesRemaining = options.maxReadAhead + options.scanSize; // Well this is the max it could be, assuming all buffers are full when this last byte is read
            // But if we haven't got far enough to fill the buffers...
            // if (position < options.maxReadAhead + options.scanSize) {
                // bytesRemaining = options.maxReadAhead + options.scanSize - position;
                // // Argh maybe the algorithm is inequipped to deal with such small data!
            // }
            // break;
        } else {

            byte b = (byte)v;

            // Add byte to history
            byteHistory.addByte(b);

            // Add byte's partial sum to psHistory
            psum += sumFunction(b);
            psumHistory.addInt(psum);

        }
        return psum;
    }

    private void writeMatchOrRaw(int windowStart) throws IOException {

        int windowEnd = position - options.maxReadAhead;

        int startSum = psumHistory.get(windowStart);
        int endSum = /* psum; */ psumHistory.get(windowEnd); // Was but is no longer, the last psum added

        int windowSum = endSum - startSum; // window for last scanSize bytes of data

        IntQueue possibleMatches = windowSumHistoryMap.getQueueFor(windowSum);

        // System.out.println("Pos="+position+" Sum="+windowSum+" PosMatches="+possibleMatches.size());

        if (skipOutput > 0) {

            // We have already written (copied) this data to output
            skipOutput--;

        } else {

            // We want to write this byte (and later ones) to output
            boolean matchFound = false;
            int maxLength = ( bytesRemaining > -1 ? bytesRemaining : options.maxReadAhead+options.scanSize );
            skipOutput = seekEarlierMatchForCopy(possibleMatches, byteHistory, windowStart, maxLength);
            if (skipOutput>0) {
                matchFound = true;
//                            System.out.println("BytesToEnd="+bytesRemaining);
                skipOutput--; // Since 1 byte is dealt with during this iteration
            }
            if (!matchFound) {
                sendOutputRawByte(byteHistory.get(windowStart));
            }

        }

        // Add window's sum to sumListMap
        possibleMatches.add(windowStart);
    }

    private int seekEarlierMatchForCopy(IntQueue possibleMatches, ByteRingBuffer byteHistory, int windowStart, int maxLength) throws IOException {
        // Check to see if this new window can be a copy of an earlier window
        // If so, expand it, send it to output, and set skipping
        // Otherwise queue the first byte of the window for output, and move on...
        // for (int i=0;i<possibleMatches.size();i++) {
        for (int i=possibleMatches.size()-1;i>=0;i--) { // I made it so it will find the later (most recent) match first
            int possibleStartPosition = possibleMatches.get(i);
            int matchedLength = expandMatch(byteHistory,possibleStartPosition,windowStart,maxLength); // Done I hope: if max is less if end-of-stream has been found
            // Ah now this is interesting
            // We may have found a block which matches current block length scanSize
            // But maybe there are other blocks which might expand to a longer match than this one! :-o
            // Maybe it's an option for the user: scanSize = minimum match; fullSearch = find longest expanding match
            // OTOH maybe it's find to take the first we find; if a longer expansion (>scanSize) was possible, these later blocks should find matches too. :)
            // But for ultimate efficiency, maybe our goal here should be to find the longest matching block >= scanSize.  Later contenders might be rejected quickly using a binary search on the partialSums ... :S
            if (matchedLength >= options.scanSize) {
                // skipOutput = matchedLength;
                sendOutputCopy(possibleStartPosition,matchedLength);
                // System.out.println("Pos="+(position-scanSize-maxReadAhead)+" MATCH! "+i+"/"+possibleMatches.size()+" Copy: "+possibleStartPosition+" ("+matchedLength+")");
                return matchedLength;
            }
        }
        return 0;
    }

    private int expandMatch(ByteRingBuffer byteHistory, int startA, int startB, int max) {
        int length = 0;
        while (true) {
            byte testByte;
            if (length>=max) {
                break;
            }
                /*
            if (length>=scanSize) {
                break;
                /*
                int r = in.read();
                if (r == -1) {
                    break;
                } else {
                    testByte = (byte)r;
                }
            } else {
                testByte = byteHistory.get(startA+length);
            }
            */
            if (byteHistory.get(startA+length) == byteHistory.get(startB+length)) {
                length++;
            } else {
                break;
            }
        }
        return length;
    }

    public int sumFunction(byte b) {
        return ((int)b*(int)b);
    }

    public void sendOutputCopy(int earlierPos, int copyLength) throws IOException {
        output.output_copyEarlierBlock(earlierPos,(short)copyLength);
        if (!options.quiet) {
            System.out.println("Pos="+(position-options.scanSize-options.maxReadAhead)+" Copying "+copyLength+" bytes from "+earlierPos);
            System.out.println("Pos="+(position-options.scanSize-options.maxReadAhead+copyLength));
        }
    }

    public void sendOutputRawByte(byte b) throws IOException {
//        System.out.println("Raw: "+b);
        output.output_rawByte(b);
    }

    /***************************************************************************
     * @todo Add this small heuristic for improving compression: group files
     *       with similar names close together, even if they are in different
     *       folders. This will required generating the list of files first,
     *       sorting it, and then outputting and creating the input streams. (Do
     *       we have to create a vector of inputStreams, can't we just generate
     *       them from our new list of files/filenames? We might not need
     *       LazyFileInputStream any more...)
     **************************************************************************/
    
    public  void addInputStreamOrStreamsForFile(File file, Vector inputStreams, Outputter output_) throws IOException {
        if (file.isFile()) {
            // Note: if we ever find we are opening too many inputstreams right at the start,
            // consider implementing a LazyFileInputStream which doesn't actually create its underlying
            // FileInputStream until it receives its first read() request.
            // Well it turns out that this was indeed needed!  I got a FileNotFoundException (too many open files)
            // InputStream is = new FileInputStream( file );
            InputStream is = new LazyFileInputStream( file );
            // is = new BufferedInputStream(is);
            inputStreams.add( is );
            String filePath = "" + file;
            if (filePath.startsWith("/")) {
                filePath=filePath.substring(1);
            }
            output_.output_line("File: "+file.length()+" "+filePath);
            ETA.addTodo(file.length());
        } else if (file.isDirectory()) {
            File[] subFiles = file.listFiles();
            if (subFiles==null) {
                System.err.println("Got null for subFiles of "+file);
            } else {
                for (int i=0;i<subFiles.length;i++) {
                    String handle = new File(file.getCanonicalFile(),subFiles[i].getName()).toString();
                    String followed = subFiles[i].getCanonicalFile().toString();
                    // System.out.println("handle = "+handle);
                    // System.out.println("follow = "+followed);
                    if (!handle.equals(followed) && subFiles[i].isDirectory()) {
                        System.err.println("I fear "+handle+" is a symlink AND a directory - SKIPPING!");
                    } else {
                        addInputStreamOrStreamsForFile(subFiles[i],inputStreams,output_);
                    }
                }
            }
        } else {
            System.err.println("WARNING: I don't know what to do with non-file and non-directory "+file+" (may be a broken symlink)");
        }
    }
    
}


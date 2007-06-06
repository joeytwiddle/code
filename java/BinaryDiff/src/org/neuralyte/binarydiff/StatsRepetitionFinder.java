package org.neuralyte.binarydiff;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

/** joey Jul 18, 2006 10:30:33 AM */
public class StatsRepetitionFinder extends RepetitionFinder {

    /**
     * CONSIDER: Limitation: If we only ever copy blocks of size <n>eg. 4096,
     * then ... Possible improvement: ...
     */

    // Changed all these from int to long, since ratio was showing as -ve.

    long unmatchedBytes    = 0;

    long matchedBytes      = 0;

    long unmatchedSize     = 0;

    long matchedSize       = 0;

    long longestCopyLength = 0;

    long longestDistance   = 0;

    // Another problem: what i actually wanted to record was the distance from
    // earlierPos,
    // which is a statistic the RepetitionFinder could give us.

    public void sendOutputCopy(int earlierPos, int copyLength)
            throws IOException {
        super.sendOutputCopy(earlierPos, copyLength);
        long currentPosition = (matchedBytes + unmatchedBytes);
        matchedBytes += copyLength;
        matchedSize += 7;
        if (copyLength > longestCopyLength) {
            longestCopyLength = copyLength;
        }
        int distance = (int)(currentPosition - earlierPos);
        if (distance > longestDistance) {
            longestDistance = distance;
        }
        update();
    }

    public void sendOutputRawByte(byte b) throws IOException {
        // Problem: we really want to be working with the outputter
        // because the Outputter knows how many real bytes it costs when
        // outputting raw data
        // (overhead due to initial tag)
        super.sendOutputRawByte(b);
        unmatchedBytes++;
        unmatchedSize++;
        update();
    }

    String resultString() {
        return "Original: " + perc(matchedBytes, matchedBytes + unmatchedBytes)
                + " matched, "
                + perc(unmatchedBytes, matchedBytes + unmatchedBytes)
                + " unmatched  "
                + "Output: "
                + perc(matchedSize, matchedSize + unmatchedSize) + " matched, "
                + perc(unmatchedSize, matchedSize + unmatchedSize)
                + " unmatched.  "
                + "Longest copy: " + longestCopyLength
                + " bytes.  Longest distance: " + longestDistance + " bytes.  "
                + "Ratio: "+100*(unmatchedBytes+matchedBytes)/(unmatchedSize+matchedSize)
                + "unmatchedBytes+matchedBytes="+(unmatchedBytes+matchedBytes)+","
                + "unmatchedSize+matchedSize="+(unmatchedSize+matchedSize)+")";
    }

    String perc(long a, int b) {
        return perc(a, (long)b);
    }

    String perc(int a, long b) {
        return perc((long)a, b);
    }

    String perc(int a, int b) {
        return perc((long)a, (long)b);
    }

    String perc(long top, long bottom) {
        return (100 * top / bottom) + "%";
    }

    long   lastUpdate  = 0;

    String currentFile = "?";

    long   totalBytes  = 0;

    public void update() {
        if (unmatchedBytes + matchedBytes - lastUpdate > 40960) {
            totalBytes = ETA.total;
            lastUpdate = unmatchedBytes + matchedBytes;
            double proportionDone = ((double) unmatchedBytes + (double) matchedBytes)
                    / (double) totalBytes;
            System.err.print("\r" + perc(unmatchedBytes + matchedBytes, totalBytes)
                    + " of total, ratio "
                    + perc(unmatchedSize + matchedSize, unmatchedBytes
                            + matchedBytes) + " on " + currentFile + " :: "
                    + ETA.getETA(proportionDone) + "  ");
        }
    }

    public void addInputStreamOrStreamsForFile(File file, Vector inputStreams,
            Outputter output_) throws IOException {
        super.addInputStreamOrStreamsForFile(file, inputStreams, output_);
        if (file.isFile()) {
            totalBytes += file.length();
        }
    }

}

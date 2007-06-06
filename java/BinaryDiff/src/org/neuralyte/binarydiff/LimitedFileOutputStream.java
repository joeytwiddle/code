package org.neuralyte.binarydiff;

import java.io.*;

/**
 * Writes a the given number of bytes to the given File, then closes the File.
 * 
 * When this happens isFinished() will become true.
 * 
 * The File is not opened until the first bytes are written to it. 
 * 
 *  joey Jul 4, 2006 2:04:23 AM */

public class LimitedFileOutputStream extends Object {

    int length;
    File file;

    OutputStream out;

    int count;

    boolean finished;

    public LimitedFileOutputStream(int length, File file) {
        this.length = length;
        this.file = file;
        out = null;
        count = 0;
        finished = false;
    }

    public void write(int b) throws IOException {
        // We delay creation of the actual FileOutputStream until the first write.  :)
        if (out == null) {
            file.getParentFile().mkdirs();
            out = new FileOutputStream(file);
            out = new BufferedOutputStream(out);
        }
        out.write(b);
        count++;
        if (count == length) {
            out.flush();
            out.close();
            finished = true;
        }
    }

    public boolean isFinished() {
        return finished;
    }

}

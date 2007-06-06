package org.neuralyte.binarydiff;

import java.io.OutputStream;
import java.io.IOException;
import java.io.File;
import java.util.Vector;

/** joey Jul 4, 2006 1:55:53 AM */
public class MultiFileOutputStream extends OutputStream {

    Vector filesTodo;

    LimitedFileOutputStream current;

    public MultiFileOutputStream() {
        filesTodo = new Vector();
        current = null;
    }

    public void addOutputFile(int length, String fileName) {
        filesTodo.add( new LimitedFileOutputStream(length,new File(fileName)) );
    }

    public void write(int b) throws IOException {
        if (current == null) {
            if (filesTodo.size()>0) {
                current = (LimitedFileOutputStream)filesTodo.remove(0); // Without the check this was throwing NPE, possibly from cast?
            } else {
                current = null;
            }
        }
        current.write(b);
        if (current.isFinished()) {
            current = null;
        }
    }

}

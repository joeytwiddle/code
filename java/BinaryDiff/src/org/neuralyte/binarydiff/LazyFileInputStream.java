package org.neuralyte.binarydiff;

import java.io.*;

/**
 * Acts like a FileInputStream, but does not open the underlying FileInputStream
 * until data is actually read.
 * <P/>
 * (Basically allows you to create a lot of these virtual FileInputStreams,
 * without exceeding max-open-files limit.)
 * <P/>
 * joey Jan 22, 2007 2:12:50 PM */

public class LazyFileInputStream extends InputStream {

    File file;

    FileInputStream fis;

    public LazyFileInputStream(File _file) {
       file = _file;
       fis = null;
    }

    public LazyFileInputStream(String fileName) {
        this(new File(fileName));
    }

    private void checkFisInstantiated() throws FileNotFoundException {
        if (fis == null) {
            fis = new FileInputStream(file);
        }
    }

    public int read() throws IOException {
        checkFisInstantiated();
        return fis.read();
    }

    public int read(byte b[]) throws IOException {
        checkFisInstantiated();
        return fis.read(b);
    }

    public long skip(long n) throws IOException {
        checkFisInstantiated();
        return fis.skip(n);
    }

    public int read(byte b[], int off, int len) throws IOException {
        checkFisInstantiated();
        return fis.read(b,off,len);
    }

    public void close() throws IOException {
        fis.close();
    }

}

package org.neuralyte.binarydiff;

import java.io.File;

/** joey Jun 15, 2006 6:23:32 PM */
public class DataSource {

    private File file = null;
    private String content;

    final int id; // unique

    public DataSource(File file, String content, int id) {
        this.file = file;
        this.content = content;
        this.id = id;
    }

    public byte getByteAt(int i) {
        return content.getBytes()[i];
    }

    public int getLength() {
        return content.length();
    }

    public String toString() {
        return file+"("+content.length()+")";
    }

}

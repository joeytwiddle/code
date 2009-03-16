package org.common.nap;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

// TODO: Default I/O should not be strings, but streams.

public class Nap {

    static Napper globalNapper = new SimpleNapper();
    
    public static String toString(Object obj) {
        // Could maybe get SessionNapper from currentThread. :P
        return globalNapper.toString(obj);
    }
    
    public static String fromString(String str) {
        throw new Error(Nap.class+".fromString() not defined!");
    }
    
    public static Object fromFile(String fileName) throws Exception {
        return fromStream(new FileInputStream(fileName));
    }
    
    public static Object fromStream(InputStream in) throws Exception {
        return globalNapper.readNap(in);
    }

    public static void sendToStream(Object obj, OutputStream out) throws Exception {
        globalNapper.writeNap(obj,out);
    }
    
}

package org.common.nap;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import org.fairshare.Logger;

// TODO: Default I/O should not be strings, but streams.

public class Nap {

    static Napper globalNapper = new SimpleNapper();
    
    public static String toString(Object obj) {
        // Could maybe get SessionNapper from currentThread. :P
        return globalNapper.toString(obj);
    }

    public static void writeToFile(Object obj, String fileName) throws Exception {
        globalNapper.writeNap(obj,new FileOutputStream(fileName));
        Logger.log("Wrote "+obj+" to "+fileName);
    }
    
    public static String fromString(String str) {
        // TODO
        throw new Error(Nap.class+".fromString() not defined!");
    }
    
    public static Object fromFile(String fileName) throws Exception {
        Logger.log("Reading from "+fileName);
        return fromStream(new FileInputStream(fileName));
    }
    
    public static Object fromFile(File file) throws Exception {
        return fromFile(file.toString());
    }
    
    public static Object fromStream(InputStream in) throws Exception {
        return globalNapper.readNap(in);
    }
    
}

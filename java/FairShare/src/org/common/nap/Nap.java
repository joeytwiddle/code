package org.common.nap;

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
    
}

package org.common.nap;


public class Nap {

    static Napper globalNapper = new SimpleNapper();
    
    public static String toString(Object obj) {
        return globalNapper.toString(obj);
    }
    
}

package org.common.nap;

import java.io.OutputStream;

import org.fairshare.data.Database;


public interface Napper {

    public String toString(Object obj);
    
    public Object fromString(String str);

    public void sendToStream(Object obj, OutputStream out); // throws IOException?
    
}

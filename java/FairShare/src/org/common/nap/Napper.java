package org.common.nap;

import java.io.InputStream;
import java.io.OutputStream;

import org.fairshare.data.Database;


public interface Napper {

    public String toString(Object obj);
    
    public Object fromString(String str);

    public void writeNap(Object obj, OutputStream out) throws Exception;

    public Object readNap(InputStream in) throws Exception;
    
}

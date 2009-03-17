package org.fairshare;

import java.io.File;


public class Config {

    public File[] shareDirs = { new File(".") };
    
    public int port;
    
    public boolean autoGenerateTags = true;
    public boolean autoGenerateRatings = true;
    
}

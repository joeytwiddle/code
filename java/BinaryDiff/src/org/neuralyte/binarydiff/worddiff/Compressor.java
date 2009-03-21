package org.neuralyte.binarydiff.worddiff;

import java.io.FileInputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

import org.neuralyte.Logger;
import org.neuralyte.common.io.StreamUtils;
import org.neuralyte.hashing.SHA1Hash;


public class Compressor {

    int maxInTable = 1024*1024*5; // 5meg history :o
    
    public int minWordLength = 5;
    public int maxWordLength = 255;
    public int stepSize = 4;
    
    public char[] breakChars = ". /\n\r\t:|".toCharArray();
    
    // A "word" is a sequence of chars (String) we will seek duplicates for. 
    
    StringBuffer data;
    
    public static final void main(String[] args) {
        try {
            new Compressor(new FileInputStream(args[0])).start();
        } catch (Exception e) {
            Logger.error(e);
        }
    }
    
    public Compressor(InputStream in) {
        try {
            data = StreamUtils.streamStringBufferFrom(in);
        } catch (Exception e) {
            Logger.error(e);
        }
    }
    
    public void start() {

        // Pass 1 - accumulate the most popular blocks.
        // Map<Integer,List<Long>> hashToLocationList = new HashMap<Integer, List<Long>>();
        Map<Integer,Integer> hashToCountList = new HashMap<Integer,Integer>();

        // length needs to be stored also, it is relevant to reclaim
        
        for (int i=0;i<data.length();i++) {
            for (int len=minWordLength;len<maxWordLength;len+=stepSize) {
                int start = i;
                int end = i+len;
                if (end<data.length()) {

                    String bit = data.substring(start, end);
                    String hash = SHA1Hash.calculateSHA1(bit);
                    int hashNum = hash.hashCode();

                    Integer count = hashToCountList.get(hashNum);
                    if (count == null) {
                        hashToCountList.put(hashNum, 1);
                    } else {
                        count++;
                        hashToCountList.put(hashNum,count);
                    }

                }
            }
            if (i%1000 == 0)
                Logger.log("Done "+i+" size of hashtable = "+hashToCountList.keySet().size());
        }
        
    }
    
}

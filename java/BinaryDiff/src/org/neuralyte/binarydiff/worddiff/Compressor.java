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
    public int maxWordLength = 48;
    public int stepSize = 1;
    
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
        Map<Integer,Integer> hashToCount = new HashMap<Integer,Integer>();

        // length needs to be stored also, it is relevant to reclaim
        Map<Integer,Integer>[] maps = new Map[maxWordLength+1];
        for (int i=0;i<maps.length;i++) {
            maps[i] = new HashMap<Integer,Integer>();
        }
        
        // TODO: SortedList of best 100 results ...
        // For occasional sorting, see Collections.sort().
        // See also java.util.TreeMap and Collections.binarySearch().
        float bestScore = 0;
        String bestString = null;
        
        for (int i=0;i<data.length();i++) {
            for (int len=minWordLength;len<maxWordLength;len+=stepSize) {
                int start = i;
                int end = i+len;
                if (end<data.length()) {

                    String bit = data.substring(start, end);
                    // TODO: check breakChars - skip if not a legal word
                    String hash = SHA1Hash.calculateSHA1(bit);
                    int hashNum = hash.hashCode();

                    Map<Integer,Integer> myHashToCount = maps[len];
                    
                    Integer count = myHashToCount.get(hashNum);
                    if (count == null) {
                        myHashToCount.put(hashNum, 1);
                    } else {
                        count++;
                        myHashToCount.put(hashNum,count);
                        float score = len * count;
                        if (score > bestScore) {
                            bestString = bit;
                            bestScore = score;
                            if (i>1024) {
                                Logger.log("New leader: >"+bit+"<");
                            }
                        }
                    }

                }
            }
            if (i%1000 == 0) {
                // int totalCount = hashToCount.keySet().size();
                int totalCount = 0;
                for (int j=0;j<maps.length;j++)
                    totalCount += maps[j].keySet().size();
                Logger.log("Done "+i+" with "+totalCount+" hashtable records.");
            }
        }

        /*
        Logger.log("Summary:");
        for (int len=minWordLength;len<=maxWordLength;len++) {
            int countThisLength = maps[len].keySet().size();
            if (countThisLength > 0) {
                Logger.log("I saw "+countThisLength+" distinct hashes of length "+len);
            }
            for (Integer hashNum : maps[len].keySet()) {
                int countThisHash = maps[len].get(hashNum);
                if (countThisHash>3) {
                    Logger.log("Saw "+countThisHash+" occurrences of word with length "+len+" and hash "+hashNum);
                }
                float score = len * countThisHash;
                // TODO: sortedByScore.add() ...
            }
        }
        */
        
        // TODO: Pass 2
        
    }
    
}

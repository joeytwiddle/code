package org.fairsharex.io;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.neuralyte.hashing.Hash;

import com.sun.org.apache.xalan.internal.xsltc.runtime.Hashtable;


public class SearchRequest extends Request {

    public String searchString;
    
    public Response process(Context context) {
        SearchResponse response = new SearchResponse();
        String[] searchStringSplit = searchString.split(" ");
        for (String hash : context.fairshare.cache.fileRecordsByHash.keySet()) {
            File file = context.fairshare.cache.fileRecordsByHash.get(hash).file;
            double score = 0.5d;
            int upCount=0, downCount=0;
            for (String searchWord : searchStringSplit) {
                searchWord = searchWord.toLowerCase();
                if (file.getName().toLowerCase().contains(searchWord)) {
                    upCount++;
                } else {
                    downCount++;
                }
            }
            score = (1.0*upCount + 0.0*downCount + 0.5) / (upCount + downCount + 1);
            if (score > 0.5) {
                response.results.put(hash,score);
            }
        }
        return response;
    }

}

package org.fairshare.data;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.common.lib.ListMap;


public class Database {

    /*
     * This is the local database, which retains the user's data.
     * 
     * It might also contain some caches (e.g. the file hash DB)
     * 
     * There is elsewhere a KnowledgeDB of knowledge we cache, share, and gain,
     * then process.
     */
    
    // #define Hash String
    
    // public Map<String,File> localFiles = new HashMap<String,File>();
    // These are not really associated with user, or not publically anyway.
    
    public Map<String,FileRecord> fileRecordsByPath = new HashMap<String,FileRecord>();
    
    public ListMap<String,Object> tagDB = new ListMap<String,Object>();
    // There may be global tags and user tags.
    // No maybe only global tags i.e. words :P
    
    // User ratings
    // KnowledgeDB will have GlobalUserRating!
    public Map<String,Float> userRatings = new HashMap<String,Float>();
    
    // There are different types of rating.
    // Normally ofc 
    
}

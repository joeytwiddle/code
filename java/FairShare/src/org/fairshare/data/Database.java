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
    
    public Map<Hash,File> localFiles = new HashMap<Hash,File>();
    // These are not really associated with user, or not publically anyway.
    
    public ListMap tagDB = new ListMap<String,Object>();
    // There may be global tags and user tags.
    // No maybe only global tags i.e. words :P
    
    // User ratings
    // KnowledgeDB will have GlobalUserRating!
    
}

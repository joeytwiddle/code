package org.fairshare;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;

import org.common.nap.Nap;
import org.fairshare.data.Database;
import org.fairshare.tasks.FileScanner;


/*
 * Sometimes we will just have to guess whether a remote KnownFile is a duplicate of another KnownFile. 
 */



public class FairShare {

    public static Config config = new Config();
    
    public static Database database = new Database();
    
    public static void main(String[] args) {
        
        
        
        File[] dirs = { new File(".") };
        
        
        // TODO: Load config
        try {
            config = (Config)Nap.fromFile("fairshare.cfg");
        } catch (Exception e) {
            Logger.info(""+e);
        }
        
        
        // TODO: Operate cmdline args.
        
        
        // TODO: Start in background, if idling.
        new FileScanner().scanDB(dirs, database);
        
        
        
        
        // Write files before exiting:
        
        System.out.println("Database:");
        System.out.println(Nap.toString(database));
        
        try {
            
            OutputStream cacheOut = new FileOutputStream("cache.new");
            Nap.sendToStream(database.localFiles,cacheOut);
            
            OutputStream dataOut = new FileOutputStream("data.new");
            Nap.sendToStream(database.tagDB,dataOut);
            // Actually the current tagDB is all auto-generated too, so more cache than data :P
            
        } catch (Exception e) {
            Logger.error(e);
        }
        
        
        
        // Collection col = database.tagDB.iterator();
        // Iterator it = ((Collection)database.tagDB).iterator();
        
    }

}

package org.fairshare;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;

import org.common.nap.Nap;
import org.fairshare.data.Cache;
import org.fairshare.data.Database;
import org.fairshare.tasks.FileScanner;


/*
 * Sometimes we will just have to guess whether a remote KnownFile is a duplicate of another KnownFile. 
 */



public class FairShare {

    public Config config = new Config();
    public Database database = new Database();
    public Cache cache = new Cache();

    public static void main(String[] args) {
        FairShare fairshare = new FairShare();
    }

    public FairShare() {
        init();
    }
    
    public void init() {
        
        // TODO: Load config
        try {
            config = (Config)Nap.fromFile("config.nap");
            database = (Database)Nap.fromFile("database.nap");
        } catch (Exception e) {
            // Logger.error(e);
            Logger.warn("Failed to read config file ("+e+").  I hope this is your first run!");
        }
        
        // TODO: Operate cmdline args.
        
        // TODO: Start in background, if idling.
        new FileScanner().scanDB(config.shareDirs, this);
        
        //// Write files before exiting:
        // System.out.println("Database:");
        // System.out.println(Nap.toString(database));
        try {
            Nap.sendToStream(config, new FileOutputStream("config.nap"));
            Nap.sendToStream(database, new FileOutputStream("database.nap"));
            Nap.sendToStream(cache, new FileOutputStream("cache.nap"));
            // Actually the current tagDB is all auto-generated too, so more cache than data :P
        } catch (Exception e) {
            Logger.error(e);
        }
        
//         new FileScanner().scanDB(dirs, database);
        
        // Collection col = database.tagDB.iterator();
        // Iterator it = ((Collection)database.tagDB).iterator();
        
    }
    
}

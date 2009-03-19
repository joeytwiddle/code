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
        
        File configFile = new File("config.nap");
        File databaseFile = new File("database.nap");
        File cacheFile = new File("cache.nap");
        
        // TODO: Load config
        try {
            if (configFile.exists())
                config = (Config)readFromFile(configFile);
            if (databaseFile.exists())
                database = (Database)readFromFile(databaseFile);
            if (cacheFile.exists())
                cache = (Cache)readFromFile(cacheFile);
        } catch (Exception e) {
            Logger.error("Problem reading files at startup:");
            Logger.error(e);
            // Logger.warn("Failed to read config file ("+e+").  I hope this is your first run!");
        }
        
        // TODO: Operate cmdline args.
        
        // TODO: Start in background, if idling.
        new FileScanner().scanDB(config.shareDirs, this);
        
        //// Write files before exiting:
        // System.out.println("Database:");
        // System.out.println(Nap.toString(database));
        try {
            writeToFile(config, configFile+".new");
            writeToFile(database, databaseFile+".new");
            writeToFile(cache, cacheFile+".new");
            
            configFile.renameTo(new File(configFile+".old"));
            new File(configFile+".new").renameTo(configFile);
            databaseFile.renameTo(new File(databaseFile+".old"));
            new File(databaseFile+".new").renameTo(databaseFile);
            cacheFile.renameTo(new File(cacheFile+".old"));
            new File(cacheFile+".new").renameTo(cacheFile);
            // Actually the current tagDB is all auto-generated too, so more cache than data :P
        } catch (Exception e) {
            Logger.error(e);
        }
        
//         new FileScanner().scanDB(dirs, database);
        
        // Collection col = database.tagDB.iterator();
        // Iterator it = ((Collection)database.tagDB).iterator();
        
    }

    private Object readFromFile(File file) throws Exception {
        Logger.log("Reading from "+file);
        return Nap.fromFile(file);
    }
    
    private void writeToFile(Object obj, String fileName) throws Exception {
        Nap.writeToFile(obj, fileName);
        Logger.log("Wrote "+obj+" to "+fileName);
    }
    
}

package org.fairshare;

import java.io.File;
import java.util.Collection;
import java.util.Iterator;

import org.common.nap.Nap;
import org.fairshare.data.Database;
import org.fairshare.tasks.FileScanner;


/*
 * Sometimes we will just have to guess whether a remote KnownFile is a duplicate of another KnownFile. 
 */



public class FairShare {

    static Database database = new Database();
    
    public static void main(String[] args) {
        File[] dirs = { new File(".") };
        new FileScanner().scanDB(dirs, database);
        System.out.println("Database:");
        System.out.println(Nap.toString(database));
        
        
        
        
        
        
        
        
        // Collection col = database.tagDB.iterator();
        // Iterator it = ((Collection)database.tagDB).iterator();
        
    }

}

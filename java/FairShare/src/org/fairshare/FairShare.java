package org.fairshare;

import java.io.File;

import org.common.nap.Nap;
import org.fairshare.data.Database;
import org.fairshare.tasks.FileScanner;


public class FairShare {

    static Database database = new Database();
    
    public static void main(String[] args) {
        File[] dirs = { new File(".") };
        new FileScanner().scanDB(dirs, database);
        System.out.println("Database:");
        System.out.println(Nap.toString(database));
    }

}

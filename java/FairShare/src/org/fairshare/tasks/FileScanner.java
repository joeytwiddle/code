package org.fairshare.tasks;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Date;
import java.util.List;
import java.util.Vector;

import org.common.lib.ProcessResult;
import org.common.lib.Streamer;
import org.fairshare.FairShare;
import org.fairshare.Logger;
import org.fairshare.data.Database;
import org.fairshare.data.FileRecord;
import org.fairshare.data.SHA1Hash;
import org.neuralyte.common.io.StreamUtils;

/**
 * This should really be run as a slow and gentle background thread.
 * TODO: We will need to lock the Hashtables before making each change, in case
 * another Thread is using them!
 * 
 * We might want to delete old records.  (Especially we want to notice if a File
 * has moved!)  But we do not want to delete a record if it has a rating
 * associated with it.  (Actually we CAN delete it, provided the network has it,
 * then we can always lookup what file it was using the hash.)
 * 
 * TODO: Files which change, rather than being re-scanned, could be marked and
 * removed from database, as no use for file-sharing.  :P
 */

public class FileScanner {

    public void scanDB(File[] dirs, FairShare fairshare) {
        List<String> tags = new Vector();
        for (File dir : dirs) {
            scanDir(fairshare,tags,dir);
        }
    }

    private void scanDir(FairShare fairshare, List<String> tags, File parent) {
        Logger.log("Scanning "+parent+" ...");
        for (File file : parent.listFiles()) {
            if (file.isFile()) {
                
                FileRecord record = fairshare.database.fileRecordsByPath.get(file.getPath());
                if (record != null
                     && file.length() == record.size
                     &&  file.lastModified() == record.lastModified
                     // We don't need to check or even store record.file
                     // because the table key holds the file path!
                     && record.file != null
                     && file.getPath().equals(record.file.getPath())
                ) {
                        // Logger.log("We can skip re-hashing "+file);
                    fairshare.cache.fileRecordsByHash.put(record.hash,record);
                    continue;
                }
                
                if (record == null) {
                    record = new FileRecord();
                    Logger.log("Creating new record for "+file);
                } else {
                    Logger.log("Updating existing record for "+record.file);
                }

                String hashStr = hashFile(file);
                if (hashStr == null) {
                    Logger.warn("Got NULL hash so skipping file: "+file);
                } else {
                    
                    // Hash hash = new Hash(hashStr);
                    
                    // fairshare.database.localFiles.put(hashStr, file);

                    // fairshare.database.fileRecordsByPath.put(file.getPath(),record);

                    record.size = file.length();
                    record.lastModified = file.lastModified();
                    record.hash = hashStr;
                    record.file = file;
                    
                    fairshare.database.fileRecordsByPath.put(file.getPath(),record); // Only really needed if record was not already in DB.
                    fairshare.cache.fileRecordsByHash.put(hashStr,record);
                    
                    if (fairshare.config.autoGenerateTags) {
                        for (String tag : tags) {
                            fairshare.database.tagDB.getList(tag).add(hashStr);
                        }
                    }

                    if (fairshare.config.autoGenerateRatings) {
                        long timeNow = new Date().getTime();
                        // long lastRead = file.lastModified();
                        long lastWritten = file.lastModified();
                        long age = timeNow - lastWritten;
                        double rating = 1.0/Math.max(1+Math.log(1+((double)age)/50000d)/400d,1.0);
                        fairshare.database.userRatings.put(hashStr, (float)rating);
                    }
                    
                    // org.fairshare.Logger.log("Added "+file+" (tags="+tags+")");
                    // org.fairshare.Logger.log("  hash="+hashStr+" written="+new Date(file.lastModified()));
                    
                }
            } else if (file.isDirectory() && !isSymlink(file)) {
                if (parent.getName() != ".") {
                    tags.add(parent.getName()); 
                    scanDir(fairshare,tags,file);
                    tags.remove(tags.size()-1);
                } else {
                    scanDir(fairshare,tags,file);
                }
            }
        }
    }

    private String hashFile(File file) {
        try {
            BufferedInputStream in = new BufferedInputStream(new FileInputStream(file));
            String data = StreamUtils.streamStringFrom(in);
//            String hash = SHA1.calculateSHA1_s(data);
            String hash = SHA1Hash.calculateSHA1(data);
            return "sha1:"+hash;
        } catch (Exception e) {
            org.fairshare.Logger.error(e);
            return null;
        }
    }
    
    public static boolean isSymlink(File file) {
        /*
        String[] existTest = { "find", ""+file, "-maxdepth", "0" };
        String[] linkTest = { "find", ""+file, "-maxdepth", "0", "-type", "l" };
        try {
            // Process existsProcess = runCommand(existTest);
            // Process linkProcess = runCommand(linkTest);
            // String[] existsResults = runCommand(existTest);
            // String[] linkResults = runCommand(linkTest);
            // boolean exists = existsRes.exitValue == 0;
            // boolean isLink = linkRes.exitValue == 0;
            ProcessResult existsRes = runCommand(existTest);
            ProcessResult linkRes = runCommand(linkTest);
            boolean exists = existsRes.stdOutStr.length()>0;
            boolean isLink = linkRes.stdOutStr.length()>0;
            return ( exists && isLink );
        } catch (Exception e) {
            Logger.log(e);
            return false; // true?
        }
        */
        return false;
    }

    private static ProcessResult runCommand(String[] command) throws IOException {
        java.lang.Process process = Runtime.getRuntime().exec(command);
        Streamer stdOut = new Streamer(process.getInputStream());
        stdOut.start();
        Streamer stdErr = new Streamer(process.getErrorStream());
        stdErr.start();
        try {
            process.waitFor();
            // stdOut.wait();
            // stdErr.wait();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        // try { Thread.currentThread().sleep(10); } catch (InterruptedException e) { e.printStackTrace(); }
        ProcessResult result = new ProcessResult();
        if (stdOut.result == null || stdErr.result == null) {
            Logger.warn("One of the process streams is empty!");
        }
        result.exitValue = process.exitValue();
        result.stdOutStr = stdOut.result;
        result.stdErrStr = stdErr.result;
        if (result.stdOutStr == null)
            result.stdOutStr = "";
        if (result.stdErrStr == null)
            result.stdErrStr = "";
        return result;
    }

}

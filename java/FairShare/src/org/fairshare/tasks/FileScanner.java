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
import org.fairshare.data.Database;
import org.fairshare.data.SHA1Hash;
import org.fairshare.FairShare;
import org.fairshare.Logger;
import org.neuralyte.common.io.StreamUtils;

import com.sun.org.apache.xalan.internal.xslt.Process;


public class FileScanner {

    public void scanDB(File[] dirs, Database db) {
        List<String> tags = new Vector();
        for (File dir : dirs) {
            scanDir(db,tags,dir);
        }
    }

    private void scanDir(Database db, List<String> tags, File parent) {
        for (File file : parent.listFiles()) {
            if (file.isFile()) {
                String hashStr = hashFile(file);
                if (hashStr != null) {
                    // Hash hash = new Hash(hashStr);
                    db.localFiles.put(hashStr, file);
                    if (FairShare.config.autoGenerateTags) {
                        for (String tag : tags) {
                            db.tagDB.getList(tag).add(hashStr);
                        }
                    }
                    org.fairshare.Logger.log(hashStr+" file:"+file+" (tags="+tags+")");
                    // Logger.log("  Tagged with "+tags);
                    if (FairShare.config.autoGenerateRatings) {
                        long timeNow = new Date().getTime();
                        // long lastRead = file.lastModified();
                        long lastWritten = file.lastModified();
                        long age = timeNow - lastWritten;
                        double rating = 1.0/Math.max(1+Math.log(1+((double)age)/50000d)/400d,1.0);
                        db.userRatings.put(hashStr, (float)rating);
                    }
                }
            } else if (file.isDirectory() && !isSymlink(file)) {
                if (parent.getName() != ".") {
                    tags.add(parent.getName()); 
                    scanDir(db,tags,file);
                    tags.remove(tags.size()-1);
                } else {
                    scanDir(db,tags,file);
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

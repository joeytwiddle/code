package org.fairshare.tasks;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.List;
import java.util.Vector;

import org.common.lib.ProcessResult;
import org.common.lib.Streamer;
import org.fairshare.data.Database;
import org.fairshare.data.SHA1Hash;
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
                    db.localFiles.put(new SHA1Hash(hashStr), file);
                    for (String tag : tags) {
                        db.tagDB.getList(tag).add(hashStr);
                    }
                    Logger.log("Tagged with "+tags);
                }
            } else if (file.isDirectory() && !isSymlink(file)) {
                tags.add(parent.getName()); 
                scanDir(db,tags,file);
                tags.remove(tags.size()-1); 
            }
        }
    }

    private String hashFile(File file) {
        try {
            BufferedInputStream in = new BufferedInputStream(new FileInputStream(file));
            String data = StreamUtils.streamStringFrom(in);
//            String hash = SHA1.calculateSHA1_s(data);
            String hash = SHA1Hash.calculateSHA1(data);
            org.fairshare.Logger.log("sha1:"+hash+" file:"+file);
            return hash;
        } catch (Exception e) {
            org.fairshare.Logger.error(e);
            return null;
        }
    }
    
    public static boolean isSymlink(File file) {
        String[] existTest = { "find", ""+file, "-maxdepth", "0" };
        String[] linkTest = { "find", ""+file, "-type", "l", "-maxdepth", "0" };
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
    }

    private static ProcessResult runCommand(String[] existTest) throws IOException {
        java.lang.Process process = Runtime.getRuntime().exec(existTest);
        Streamer stdOut = new Streamer(process.getInputStream());
        stdOut.start();
        Streamer stdErr = new Streamer(process.getErrorStream());
        stdErr.start();
        try {
            process.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        ProcessResult result = new ProcessResult();
        if (stdOut.result == null || stdErr.result == null) {
            Logger.warn("Process streams are empty! "+stdOut+" "+stdErr);
        }
        result.exitValue = process.exitValue();
        result.stdOutStr = stdOut.result;
        result.stdErrStr = stdErr.result;
        return result;
    }

}

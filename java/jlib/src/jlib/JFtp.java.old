package jlib;

import java.io.*;

import com.fooware.net.*;

import jlib.Files;
import java.lang.Runtime;

public class JFtp {

  private String host,user,pass;
  private FtpClient ftp;
  public boolean error=false;
  public String log="";

  public static String getLynxDump(String host,String dir,String file) {
    String s="lynx -dump "+host+"/"+dir+"/"+file;
    String data=JLib.execute(s);
    return data;
  }

  public JFtp(String h,String u,String p) {
    setup(h,u,p);
  }

  public JFtp() {
    setup("host","user","pass");
  }

  public void setup(String h,String u,String p) {
    host=h;
    user=u;
    pass=p;
  }

  private void checkresponsefor(String action) {
    checkresponsefor(action,false);
  }

  private void checkresponsefor(String action,boolean expectcompletion) {
    FtpResponse resp = ftp.getResponse();
    if ( resp.isPermanentNegativeCompletion() ||
      (expectcompletion && !resp.isPositiveCompletion()) ) {
      error=true;
      log+="!!! ";
    }
    log+=action+": "+resp.getMessage();
  }

  public void open() {
    ftp=new FtpClient();
    try {
      ftp.connect(host);
      checkresponsefor("connection",true);
      ftp.userName(user);
      checkresponsefor("user");
      ftp.password(pass);
      checkresponsefor("password",true);

      //ftp.dataPort();
      //checkresponsefor("dataPort",true);
    } catch (Exception e) {
      error=true;
      log+="On open (host \""+host+"\",user \""+user+"\") got "+e+"\n";
    }
  }

  public void close() {
    try {
      ftp.logout();
      checkresponsefor("logout",true);
    } catch (Exception e) {
      error=true;
      log+="On close got "+e+"\n";
    }
  }

  public String getdir() {
    try {
      ftp.printWorkingDirectory();
      checkresponsefor("getting dir",true);
      // Chop string out of response line
      String s=ftp.getResponse().toString();
      int i=s.indexOf("\"");
      int j=s.indexOf("\"",i+1);
      return s.substring(i+1,j);
    } catch (Exception e) {
      error=true;
      log+="When getting directory got "+e+"\n";
      return "";
    }
  }

  public void changedir(String dir) {
    try {
      System.out.println("Chaging dir to "+dir);
      ftp.changeWorkingDirectory(dir);
      checkresponsefor("changing dir",true);
    } catch (Exception e) {
      error=true;
      log+="When changing directory got "+e+"\n";
    }
  }

  public void readfile(String fname,String dir) {
    String origdir=getdir();
    changedir(dir);
    readfile(fname);
    changedir(origdir);
  }

  public void readfile(String fname) {
//    final int blocksize=2048;
    try {
      ftp.dataPort();
      checkresponsefor("dataPort",true);
      ftp.representationType(ftp.IMAGE_TYPE);
//      ftp.representationType(ftp.ASCII_TYPE,ftp.CARRIAGE_CONTROL_FORMAT);
//      ftp.representationType(ftp.EBCDIC_TYPE,ftp.NON_PRINT_FORMAT);
//      checkresponsefor("repr EBCDIC",true);
      FtpReader ftpin=ftp.retrieve(fname);
      checkresponsefor("retrieve "+fname);
      BufferedWriter br=new BufferedWriter(new FileWriter(fname));
      while (ftpin.ready()) {
        br.write(ftpin.read());
//        br.write(ftpin.readLine()+"\n");
//        char[] block=new char[blocksize];
//        int justread=ftpin.read(block,0,blocksize);
//        br.write(block);
//        for (int i=0;i<justread;i++)
      }
    br.close();
    ftpin.close();
    checkresponsefor("close "+fname,true);
    } catch (Exception e) {
      error=true;
      log+="When reading "+fname+" got "+e+"\n";
    }
  }

  public void writefile(String fname,String dir) {
    String origdir=getdir();
    changedir(dir);
    writefile(fname);
    changedir(origdir);
  }

  public void writefile(String fname) {
    try {
//      ftp.representationType(ftp.ASCII_TYPE,ftp.CARRIAGE_CONTROL_FORMAT);
//      ftp.representationType(ftp.EBCDIC_TYPE,ftp.NON_PRINT_FORMAT);
//      checkresponsefor("repr EBCDIC",true);
      System.out.println("Requesting stream...");
      ftp.dataPort();
      checkresponsefor("dataPort",true);
      FtpWriter ftpout=ftp.store(fname);
      checkresponsefor("store "+fname);
      String s=Files.readStringfromfile(fname);
      System.out.println("Got stream starting write...");
      ftpout.write(s);
      checkresponsefor("write "+fname);
      System.out.println("Written, closing...");
      ftpout.close();
      checkresponsefor("close "+fname,true);
    } catch (Exception e) {
      error=true;
      log+="When writing "+fname+" got "+e+"\n";
    }
  }

  public String getlist() {
    try {
      // Doesn't work
//      FtpReader r=ftp.nameList();
      FtpReader r=ftp.list();
      // It can work using    "."
      // but then file write crashes!
      String s="";
      while (r.ready()) {
        s+=r.readLine()+"\n";
      }
      r.close();
      checkresponsefor("close list",true);
//      ftp.closeTransferSocket();
      log+="getlist(): "+s+"\n";
      return s;
    } catch (Exception e) {
      error=true;
      log+="When reading list got "+e+"\n";
      return "";
    }
  }

}
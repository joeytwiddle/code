import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;
import java.net.Socket;

import jlib.*;

public class DefaultCollect implements Collect, Serializable {

  // Parameters of sync
public  int maxdepth=20; // -1 = oo
public  long maxsize=5000000; // 5Mb // -1 = oo
public  long maxtotal=(long)(7*1000000/1.5*2.0*5.0/4.5); // quarter of a Mb
public  boolean explain=false;
public  boolean changedonly=true;

public  boolean skiphiddendirs=false;
public  boolean skipverdirs=false;
public  boolean skiptrawleddirs=false;
public  boolean skipjavadocdirs=false;
public  boolean skipsymlinkfiles=true;
public  boolean skipsymlinkdirs=false;
public  boolean skiphiddenfile=false;
public  boolean skipsyncfile=false;
public  boolean skipdifffile=false;
public  boolean skipbrokenfile=false;
public  boolean skipbackups=false;
public  boolean skiplatexoutput=false;
public  boolean skipkdethumbnails=false;
public  Map skipExts=new HashMap();

  public DefaultCollect() {
  }

  public void init() {
    echo("Default connect skips symbolic links (except top dir), sync files, hidden directories (start with '.').");
    skipsymlinkdirs=true;
    skipsyncfile=true;
    skipdifffile=true;
    skiphiddendirs=true;
    // part of above skipkdethumbnails=true;
    echo("And it skips tex output 'cos I don't like it.");
    skipTexOutput();
    // skiphiddenfile=true;
    // skipbackups=true;
    // skipArchives();
    // skipImages();
	 skipVimSwapFiles();
  }

  void skipVimSwapFiles() {
    skipExts.put("swp","vim swap file");
  }

  void skipArchives() {
    // Archives
    skipExts.put("tgz","zip file");
    skipExts.put("gz","zip file");
    skipExts.put("zip","zip file");
    skipExts.put("bz2","zip file");
    skipExts.put("tar","zip file");
    skipExts.put("jar","zip file");
  }

  void skipImages() {
    // Images
    skipExts.put("gf","image file");
    skipExts.put("pgm","image file");
    skipExts.put("bmp","image file");
    skipExts.put("pcx","image file");
    skipExts.put("gif","image file");
    skipExts.put("jpg","image file");
    skipExts.put("jpeg","image file");
  }

  void skipTexOutput() {
    // Latex created files
      skipExts.put("dvi","DVI file");
      skipExts.put("aux","Latex output");
      skipExts.put("blg","Latex output");
      skipExts.put("bbl","Latex output");
      skipExts.put("log","Latex output");
      skipExts.put("toc","Latex output");
      skipExts.put("lof","Latex output");
      skipExts.put("lot","Latex output");
      skipExts.put("ps","Latex output");
      skipExts.put("pdf","Latex output");
  }

  long lastsync; // -1 = never : get all files, not just new
  File topDir;

  // Files / dirs to check
  List tocheck=new Vector();

  // Results
  int possible=0;
  List needSync=new Vector(); // NB: not Files, but Strings relative to topDir
  Map leaving=new HashMap(); // (File,String reason)


  public List whatsrelevant(File dir) {
    return whatsnew(null,dir);
  }

  public List whatsnew(Date lastsyncdate,File dir) {

    init();

    echo("Collector "+this.getClass().getName()+" started.");

    lastsync=( lastsyncdate==null ? -1 : lastsyncdate.getTime() );
    topDir=dir;
    tocheck.add(topDir);

    echo("Searching "+topDir+" for new files since "+JLib.niceDate(lastsync)+" ...");

    // File syncfile=new File(JLib.JPATH+"/tmp/lastsync.txt");
    // File syncfile=new File(JLib.JPATH+"/uptodate.txt");
    // if (syncfile.exists())
      // lastsync=syncfile.lastModified();

    int totalsize=0;

    while (tocheck.size()>0) {

/*      Satisfier finddir=new FindDirSatisfier(); {
        public boolean satisfies(Object o) {
          return ((File)o).isDirectory();
        }
      };*/
      File /*f=(File)JVector.getfirst(finddir,tocheck);
      if (f!=null)
        tocheck.remove(f);
      else                   */
        f=(File)tocheck.remove(0);
      // echo(""+f);

      if (f.isDirectory()) {

        // echo(""+tocheck.size()+f);

        String s=checkDir(f);
        if (s==null)
          getFilesinDir(f);
        else {
          if (explain)
            echo("Leaving "+f+" because: "+s);
          leaving.put(f,s);
        }
      }
      if (f.isFile()) {
        String s=keepFile(f);
        if (s==null) {
          needSync.add(trim(f));
          totalsize+=f.length();
        } else {
          if (explain)
            echo("Leaving "+f+" because: "+s);
          leaving.put(f,s);
        }
      }

    }

    // echo(totalsize/1000+"Kb");
    if (totalsize>maxtotal) {
      long initsize=totalsize;
      while (totalsize>maxtotal) {
        String fname=findLargest();
        needSync.remove(fname);
        File f=new File(topDir,fname);
        totalsize-=f.length();
        String s="to trim size "+f.length();
        if (explain)
          echo("Leaving "+f+" because: "+s);
        leaving.put(f,s);
      }
      echo("Trimmed "+(initsize-totalsize)/1000+"Kb off "+initsize/1000);
    }
    echo(totalsize/1000+"Kb");

    /* List mustgo=Files.posixLinks(needSync);
    for (int i=0;i<mustgo.size();i++) {
      File f=(File)mustgo.get(i);
      needSync.remove(f);
      echo("Rejecting link "+f+" at end");
    } */

    // Print a list of the files to be kept
    //StringBuffer keep=new StringBuffer();
    //for (int i=0;i<needSync.size();i++) {
    //  File f=(File)needSync.get(i);
      // echo(""+f);
    //  keep.append(f+"\n");
    //}
    // Files.writeStringtofile(keep.toString(),JLib.JPATH+"/tmp/sync-keeping.txt");
    // echo("$JPATH/tmp/sync-keeping.txt and /sync-summery.txt written.");

    // Print a list of the rejected files and the reasons why
    StringBuffer summary=new StringBuffer("I avoided the following:\n");
    Iterator i=leaving.keySet().iterator();
    while (i.hasNext()) {
      File f=(File)i.next();
      String s=(String)leaving.get(f);
      if (s.length()>0)
        summary.append(s+" : "+f+"\n");
    }
    Files.writeStringtofile(summary.toString(),JLib.JPATH+"/tmp/sync-summary.txt");
    // echo("$JPATH/tmp/sync-summery.txt written.");

    return needSync;

  }

















  public String checkDir(File f) {
    if (maxdepth>-1 && JString.countOf(File.pathSeparatorChar,f.toString())>maxdepth)
      return "directory too deep";
    String name=f.getName();
    if (name.equalsIgnoreCase("CVS"))
      return "The directory is called \"CVS\"!";
    if (name.equalsIgnoreCase("nosync"))
      return "The directory is called nosync!";
    if (new File(f,".nosync").exists())
      return "The directory contains .nosync!";
		// else
			// System.out.println(f+"/.nosync does not exist\n");
    if (skiphiddendirs && name.startsWith(".") && !f.getName().equals("."))
      return "hidden directory";    // not just "." !
    if (skipverdirs && JString.startsWithIgnoreCase(name,"ver"))
      return "an earlier version directory";
    if (skiptrawleddirs && f.toString().indexOf("trawled")>0)
      return "avoiding trawler results";
    if (skipjavadocdirs && f.toString().indexOf("javadoc")>0)
      return "avoiding Java documentation";
    if (skipsymlinkdirs && Files.isPosixLink(f) && !f.equals(topDir))
      return "symbolic link directory";
    if (skipkdethumbnails && ( f.getName().toString().equals(".pics") || f.getName().toString().equals(".xvpics") ) )
      return "KDE thumbnail directory";
    return null;
  }






  public String keepFile(File f) {

    // Copies of files which should be ignored
    String name=f.getName();
    if (skiphiddenfile && name.startsWith(".") && !f.getName().equals("."))
      return "hidden file";    // not just "." which is allowed ;)
    if (skipsyncfile && JString.endsWithIgnoreCase(name,".sync"))
      return "sync file";
    if (skipdifffile && JString.endsWithIgnoreCase(name,".diff"))
      return "diff file";
    if (skipbrokenfile && JString.endsWithIgnoreCase(name,".broken"))
      return "broken version";
    if (skipbackups) {
      if (JString.endsWithIgnoreCase(name,".bak"))
        return "backup file";
      if (JString.endsWithIgnoreCase(name,".b4sr"))
        return "sreplace backup";
    }

    Iterator keys=skipExts.keySet().iterator();
    while (keys.hasNext()) {
      String ext=(String)keys.next();
      if (JString.endsWithIgnoreCase(name,"."+ext))
        return (String)skipExts.get(ext);
    }


boolean nosizecheck=false;
    if (JString.endsWithIgnoreCase(name,".ps"))
nosizecheck=true;//return null;//      return "PostScript file";
    if (JString.endsWithIgnoreCase(name,".eps"))
nosizecheck=true;//return null;//      return "encapsulated PostScript";

    if (maxsize>-1 && f.length()>maxsize && !nosizecheck)
      return "file too large "+f.length()/1000+"k";

    possible++;

    if (lastsync>-1 && f.lastModified()<lastsync && changedonly)
      return "no need to sync "+new Date(f.lastModified());
    if (skipsymlinkfiles && Files.isPosixLink(f))
      return "symbolic link";
    return null;
  }








  void getFilesinDir(File d) {
    // JVector.addArray(tocheck,f.listFiles());
    File[] fs=d.listFiles();
    for (int i=0;i<fs.length;i++) {
      File f=fs[i];
      if (f.isDirectory())
        tocheck.add(0,f);
      else
        tocheck.add(f);
    }
  }

  String findLargest() {
    long largest=0;
    String sofar=null;
    for (int i=0;i<needSync.size();i++) {
      String fname=(String)needSync.get(i);
      File f=new File(topDir,fname);
      if (f.length()>largest) {
        largest=f.length();
        sofar=fname;
      }
    }
    return sofar;
  }

  static void echo(String s) {
    System.out.println(s);
  }

  String trim(File f) {
    String top=topDir.toString();
    if (!top.endsWith(""+File.separatorChar))
      top+=File.separatorChar;
    return JString.after(f.toString(),top);
  }

}

import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.net.URL;
import jlib.*;

import java.io.File;
import TrawlerStrategy;

public class Trawler implements ComLineProg {

  public static final URL notSimilar=JNet.url("http://default/");

  public String url;
  public int depth=2;
  public int maxperhost=1;
  public int max=10;
  public int alwaysparsesize=128*1024; // (the size of my old BBC Master :)
  public String exts="html"; // please provide in lower case!
  public File destdir=new File("trawled");
  public boolean leftoff=false;
  public boolean showstream=false;
  public boolean showprogress=true;
  public boolean showloads=false;
  public boolean dontlocalise=false;
  public boolean gui=false;
  public URL similar=notSimilar;
  public boolean subdirsOnly=false;
  public String forbiddenDomains="";

  public void run(List args) {

    System.out.println("Got dest dir \""+destdir+"\"");
    if (!leftoff) {
      Files.deltree(destdir);
    }
    // destdir.mkdirs();
    if (!destdir.exists()) {
      System.out.println("Creating directory "+destdir);
      destdir.mkdir();
    }
    TrawlerStrategy strat=new TrawlerStrategy(url,depth,maxperhost,max,
      alwaysparsesize,JString.splitv(exts,","),destdir,showstream,showprogress,
      !dontlocalise,similar,subdirsOnly,JString.splitv(forbiddenDomains,","));
    TrawletManager manager=new TrawletManager(strat,gui);

    Profile.clear();
    Profile.start("Whole trawl");

    manager.start();

    long time=Profile.stop("Whole trawl");
    System.out.println("Trawl took "+time/1000+" seconds.");

    System.exit(0); // Ensures exit even if Trawlets are left going!

  }

}

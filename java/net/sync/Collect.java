import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;
import java.net.Socket;

import jlib.*;

public interface Collect {

  void init();

  public List whatsrelevant(File dir);

  public List whatsnew(Date lastsyncdate,File dir);

  public String checkDir(File f);

  public String keepFile(File f);

}

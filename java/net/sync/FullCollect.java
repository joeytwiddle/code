import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;
import java.net.Socket;

import jlib.*;

public class FullCollect extends DefaultCollect {

  public FullCollect() {
  }

  public void init() {
    echo("FullCollect only ignores symlinked dirs and .sync files.");
    skipsyncfile=true;
    skipsymlinkfiles=false;
    skipsymlinkdirs=true;
    echo("And we skip KDE thumbnail dirs 'cos they crop up everywhere!");
    skipkdethumbnails=true;
  }

}

import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;
import java.net.Socket;

import jlib.*;

public class CodeCollect extends FullCollect {

  public CodeCollect() {
  }

  public void init() {

    echo("Setting up code-only config");

		maxsize=200000; // 200k

    skipsymlinkdirs=true;
    skipsyncfile=true;
    skipdifffile=true;
    skiphiddenfile=true;

    skiphiddendirs=true;
    skipverdirs=true;
    skiptrawleddirs=true;
    skipjavadocdirs=true;
    skipbrokenfile=true;
    skipbackups=true;
    skiplatexoutput=true;

    // Compiled code
    skipExts.put("class","Java bytecode classfile");

		// PRCS project files
    skipExts.put("prj","PRCS project files");

    // Human readable files
    skipExts.put("html","html file");
    skipExts.put("htm","html file");
    // skipExts.put("txt","txt file");

    // Latex created files
    if (skiplatexoutput) {
      skipTexOutput();
    }

    skipArchives();
    skipImages();

  }

}

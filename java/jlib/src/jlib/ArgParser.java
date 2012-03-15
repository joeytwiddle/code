package jlib;

import java.lang.String;
import java.util.Vector;
import java.util.List;
import java.io.File;

public class ArgParser {

  private List args;
  private String error="";

  public String command="";
  public String help="";
  public boolean problem=false; // === helpon

  public ArgParser(List as) { // makes a new copy - neccessary?
    args=new Vector();
    for (int i=0;i<as.size();i++)
      args.add(as.get(i));
    start();
  }

  public ArgParser(String[] as) {
    args=new Vector();
    for (int i=0;i<as.length;i++)
      args.add(as[i]);
    start();
  }

  public ArgParser(String s) {
    args=JString.splitv(s," ");
    start();
  }

  public void start() {
//    String program=JLib.getEnv("$_");
    String program="progname?";
    command+=program+" ";
    if (containsNoGen("-h"))
      problem=true;
    if (containsNoGen("--help"))
      problem=true;
    if (containsNoGen("/?"))
      problem=true;
  }

  public int left() {
    return args.size();
  }

  public String get() {
    return get(0);
  }
  /** Get the leftmost String, called name at the command line. **/
  public String get(String name) {
    return get(name,"string");
  }
  /** Reads the next String in as a file, and resolves its absolute path. **/
  public File getFile(String name) {
    String fname=get(name,"filename");
    if (fname==null)
      return null;
    try {
      return new File(fname).getAbsoluteFile().getCanonicalFile();
    } catch (Exception e) {
      return null;
    }
  }
  public File fileAfterOr(String prefix,String name,String alt) {
    String fname=Stringafter(prefix,alt,name);
//    if (fname==null)
//      return new File(alt).getAbsoluteFile();
//    else
      return new File(fname);
  }
  /** Get the leftmost String, called name, of the given type. **/
  public String get(String name,String type) {
    command+="<"+name+"> ";
    String got=get(0);
    if (got==null)
      help+="  "+type+" <"+name+"> required\n";
    else
      help+="  "+type+" <"+name+"> = \""+got+"\"\n";
    return got;
  }
  /** Get the leftmost String, called name.  If none left, return or. **/
  public String getOr(String name,String or) {
    return getOr(name,"String",or);
  }
  /** Get the leftmost String, called name, of the given type.  If none left, return or. **/
  public String getOr(String name,String type,String or) {
    command+="[<"+name+">] ";
    String got=getOr(0,or);
    help+="  "+type+" <"+name+"> = \""+got+"\"\n";
    return got;
  }

  public String getOption(String[] options) {
    command+="[";
    for (int i=0;i<options.length;i++) {
      command+=options[i];
      if (i<options.length-1)
        command+="|";
    }
    command+="] ";
    String got=get(0);
    if (!JVector.contains(options,got)) {
      problem=true;
      help+="I got \""+got+"\" but expected one of "+options+"\n";
      done();
    }
    return got;
  }

  public String getOption(String name,String[] options) {
    String list="";
    for (int i=0;i<options.length;i++) {
      list+=options[i];
      if (i<options.length-1)
        list+=" | ";
    }
    String got=get(0);
    if (!JVector.contains(options,got)) {
      problem=true;
      command+="<"+name+"> ";
      help+="  <"+name+"> = "+list+"\n";
      done();
    } else {
      command+=got+" ";
    }
    return got;
  }

  public String optionOr(String name,String[][] options,String or) {
    // Simplify and send to another optionOr
    String[] os=new String[options.length];
    String list="";
    for (int i=0;i<options.length;i++) {
      String[] pair=options[i];
      os[i]=pair[0];
      list+="    "+pair[0]+" : "+pair[1]+"\n";
    }
    String got=optionOr(name,os,or);
    if (got.equals(or))
      help+=list;
    return got;
  }

  public String optionOr(String name,String[] options,String or) {
    boolean orGiven=false;
    String list="";
    for (int i=0;i<options.length;i++) {
      list+=options[i];
      if (options[i].equals(or)) {
        list+="(=default)"; orGiven=true;
      }
      if (i<options.length-1)
        list+=" | ";
    }
    String got=getOr(0,or);
    if (!JVector.contains(options,got) && !or.equals(got))
      problem=true;
    if (got.equals(or) || problem) {
      command+="<"+name+"> ";
      help+="  <"+name+"> = "+list+/*
        (orGiven? " (*=default)" : "(default="+or+")" )+*/"\n";
    } else {
      command+=got+" ";
    }
    return got;
  }

  /** Pull the (i+1)th argument out of the list.
   *  Returns null if there are too few arguments left (or i is -ve).
  **/
  public String get(int i) {
    if (i>=0 && i<args.size()) {
      String s=(String)args.get(i);
      args.remove(i);
      return s;
    } else {
      problem=true;
      // help+="Compulsory argument missing\n";
      return null;
    }
  }
  public String getOr(int i,String or) {
    if (i>=0 && i<args.size()) {
      String s=(String)args.get(i);
      args.remove(i);
      return s;
    } else
      return or;
  }

  public boolean contains(String s) {
    return contains(s,null);
  }

  /** Pick out this argument if it exists, for the given meaning. **/
  public boolean contains(String s,String meaning) {
    command+="["+s+"] ";
    help+="  "+s+" : "+meaning+"\n";
    return containsNoGen(s);
  }

  public boolean containsNoGen(String s) {
    int i=find(s);
    if (i>-1) {
      args.remove(i);
      return true;
    }
    return false;
  }

  public int find(String s) {
    for (int i=0;i<args.size();i++)
      if (s.equals((String)args.get(i)))
        return i;
    return -1;
  }

  public String Stringafter(String s) {
    return Stringafter(s,null);
  }

  public String Stringafter(String s,String or) {
    return Stringafter(s,or,"no name given");
  }

  /** If this tag exists, take its value.
   *  @param or: the String returned if the tag-value pair is not present.
   *  For example:
   *    String name=argp.Stringafter("-user","none","Supply a user name");
   *    would be used like this:
   *    % app -user fred
   *    app: I got String >fred<
  **/
  public String Stringafter(String s,String or,String name) {
    return Stringafter(s,or,name,name);
  }

  public String Stringafter(String s,String or,String type,String name) {
    command+="["+s+" <"+type+">] ";
    String orstr=( or==null ? "(no default)" : "(default "+or+")" );
    help+="  "+s+" <"+type+"> : "+name+" "+orstr+"\n";
    int i=find(s);
    if (i>-1) {
      args.remove(i);
      return get(i);
    }
    return or;
  }

  public int intafter(String s,int or) {
    String x=Stringafter(s,""+or);
    try {
      int i=(new Integer(x)).intValue();
      return i;
    } catch (Exception e) {
    }
    return or;
  }

  public int intafter(String s,String m,int or) {
    String x=Stringafter(s,""+or,"int",m);
    try {
      int i=(new Integer(x)).intValue();
      return i;
    } catch (Exception e) {
    }
    return or;
  }

  public void say(String s) {
    help+=s+"\n";
  }

  public void done() {
    if (args.size()>0) {
      error+=args.size()+" arguments unused: ";
      for (int i=0;i<args.size();i++)
        error+=args.get(i).toString() + ( i<args.size()-1 ? " " : "" );
      error+="\n";
    }
    if (error.length()>0) {
      problem=true;
      System.out.print("Problem parsing command line arguments:\n"+error);
    }
    if (problem) {
      System.out.print("Usage: "+command+"\n"+help);
      System.exit(1);
    }
  }
	
	public List getRest(String type) {
		command+="[ <"+type+"...> ] ";
		List tmp=args;
		args=new Vector();
		return tmp;
	}

	public List getRest(String type,String about) {
		help+="  "+type+" : "+about+"\n";
		return getRest(type);
	}

}


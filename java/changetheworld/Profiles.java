package changetheworld;

import changetheworld.*;
import java.util.*;
import java.lang.*;
import jlib.*;

public class Profiles {

  public static Profiler dbread=new Profiler("JDBC DB reading",true);
  public static Profiler reflect=new Profiler("Reflection",true);
  public static Profiler myreflect=new Profiler("My reflection",true);
  public static Profiler jtype=new Profiler("JType casting",true);

  public static void clear() {
    dbread.clear();
    reflect.clear();
    myreflect.clear();
    jtype.clear();
  }

  public static void end() {
    dbread.end();
    reflect.end();
    myreflect.end();
    jtype.end();
  }

}

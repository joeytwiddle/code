package jlib;

import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;

public class RunComLineProg {

  public static void main(String[] args) {

    if (args.length<1) {
      System.out.println("java jlib.RunComLineProg <class> [ <args> ]");
      System.exit(1);
    }

    // First argument is class
    String cl=args[0];
    // Rest of arguments are given to ArgParser
    List apas=new Vector();
    for (int i=1;i<args.length;i++)
      apas.add(args[i]);
    ArgParser argparser=new ArgParser(apas);

    try {

      // Class c=Class.forName(cl);
      Class c=ClassLoader.getSystemClassLoader().loadClass(cl);
      ComLineProg initclp=(ComLineProg)c.newInstance();
      // System.out.println("Got initial "+initclp);

      Field[] fs=c.getDeclaredFields();
      // System.out.println(fs.length+" fields.");

      // First, read all optional arguments
      // Note: detection of optional / compulsory arguments ie.
      // detection of initialisation or not of field,
      // does not seem to correlate with =null in the case of
      // booleans and numbers
      // but the Object class for the base type may be used
      for (int i=0;i<fs.length;i++) {
        Field f=fs[i];
        Object initval=f.get(initclp);
        // System.out.println(""+f+" = "+initval);
        String type=f.getType().getName();

        if (initval!=null) {
          String name=f.getName();

          if (JTypes.isNumber(type) || type.equals("java.lang.String")) {
            String after=argparser.Stringafter("-"+name,""+initval,type,"provide this "+type);
            // System.out.println("- Searching for num/Str -"+name+" got "+after);
            if (after!=null)
              f.set(initclp,JTypes.fromString(after,type));
          } else if ( JTypes.isBool(type) ) {
            // System.out.println("- Searching for bool -"+name);
            try {
            boolean newbool=!((Boolean)f.get(initclp)).booleanValue();
            if (argparser.contains("-"+name,"turn bool "+name+" "+(newbool?"on":"off"))) {
              f.set(initclp,new Boolean(newbool));
            }
            } catch (Exception e) {
              System.out.println("Problem with bool: "+e);
            }
          } else {
            try {
              String str=argparser.Stringafter("-"+name,""+initval,type,"set this "+type);
              Object o=JTypes.fromString(str,type);
              if (o==null)
                System.out.println("JTypes doesn't know how to do "+type);
              else
                f.set(initclp,o);
            } catch (Exception e) {
              System.out.println("RunComLineProg(): trying to do "+type+" got "+e);
            }
          }

        }

      }

      try {
      // Then read all compulsory arguments
      for (int i=0;i<fs.length;i++) {
        Field f=fs[i];
        Object initval=f.get(initclp);
        String type=f.getType().getName();

        if (initval==null) {
          String name=f.getName();
          String str=argparser.get(name,type);
          // System.out.println("- Compulsory arg "+name+" got \""+str+"\"");
          Object o=JTypes.fromString(str,type);
          f.set(initclp,o);
        }
      }
      } catch (Exception e) {
        System.out.println("Compulsory bit: "+e);
      }

      argparser.done();

      // Create object
      Constructor[] cs=c.getConstructors();
      // System.out.println(cs.length+" constructors.");
      Constructor cn=cs[0];
      // ComLineProg clp=(ComLineProg)cn.newInstance(new Object[0]);
      ComLineProg clp=initclp;

      // Run program
      if (!argparser.problem)
        clp.run(new Vector());

    } catch (Exception e) {
      System.out.println("ComLineProg.main(): "+e);
    }

  }

}

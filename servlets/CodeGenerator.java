// Not sure what this one does!

import java.lang.reflect.*;
import java.sql.*;
import java.io.*;

import jlib.*;
import jlib.db.*;
import jlib.strings.FakePrintWriter;

public class CodeGenerator {

  public final static void main(String[] args) {
  
    String table="philosopher";
    String pageclass="generation.pages.Philosopher";
    String objectclass="generation.objects.Philosopher";

    boolean generateMultiPage=true;
    boolean generateObject=true;
    
    try {

    String classname=JString.afterlast(pageclass,".");

    PrintStream pageout=new PrintStream(new FileOutputStream(JString.replace(pageclass,".","/")+".java"));
    pageout.println("class "+classname+" extends Page {");

    PrintStream objout=new PrintStream(new FileOutputStream(JString.replace(objectclass,".","/")+".java"));
    FakePrintWriter objdbread=new FakePrintWriter();
    FakePrintWriter objfromform=new FakePrintWriter();
    objout.println("import java.sql.ResultSet;");
    objout.println("import jlib.db.*;");
    objout.println("import jlib.JLib;");
    objout.println("class "+classname+" extends DBObj {");
    objdbread.println("  "+classname+"(ResultSet res) {");
    objdbread.println("    try {");
    
    DB db=new DB();

    SimpleDBKey tmp=new SimpleDBKey("changetheworld",table,"odfkd");
    ResultSet res=db.getResultSet(tmp.location(),"SELECT * FROM "+table+";");

    ResultSetMetaData data=res.getMetaData();
    res.next();
    for (int i=1;i<=data.getColumnCount();i++) {
      Object o=res.getObject(i);
      String name=data.getColumnName(i);
      String type=o.getClass().getName();
      String nicejavatype=JTypes.niceJavaType(o.getClass());
      // pageout.println(type+" "+name+" = "+o);

      String component="jlib.web.components."+JString.afterlast(type,".")+"Field";

      pageout.println("  public "+component+" "+name+" = new "+component+"(\""+name+"\",\"none\");");

      objout.println("  public "+nicejavatype+" "+name+";");
      Class c2=null;
      try { // Test is base type
        c2=Class.forName(nicejavatype);
      } catch (Exception e) { }
      if (c2==null)
        objdbread.println("      "+name+" = res.get"+Character.toUpperCase(nicejavatype.charAt(0))+nicejavatype.substring(1)+"(\""+name+"\");");
      else
        objdbread.println("      "+name+" = ("+nicejavatype+") res.getObject(\""+name+"\");");
    }

    objdbread.println("    } catch (Exception e) {");
    objdbread.println("      JLib.report(\"Problem creating \"+this+\": \"+e);");
    objdbread.println("    }");
    objdbread.println("  }");

    objout.println();
    objout.print(objdbread.store);
    objout.println();
    objout.print(objfromform.store);
    objout.println();

    pageout.println("}");
    objout.println("}");

    } catch (Exception e) {
      System.out.println(""+e);
      e.printStackTrace();
    }

  }

}


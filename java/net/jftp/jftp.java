import jlib.*;
import java.lang.*;
import java.io.File;
import java.util.*;

  class FtpRecord {
    public String machine,user,pass;
    FtpRecord(String h,String u,String p) {
      machine=h;
      user=u;
      pass=p;
    }
  }

public class jftp extends Echo {

  static final String passwordFile=JLib.JPATH+"/data/secret.txt";

  static final private String key="lkjh(*6^%$HGFJKJHl(7tLKJblhFUT*^UIKJ";

  public static List readrecords() {
    List lines=Files.readlinesfromfile(passwordFile);
    List l=new Vector();
    if (lines==null)
      return l;
    for (int n=0;n<lines.size();n++) {
      String s=(String)lines.get(n);
      int i=s.indexOf(" ");
      int j=s.indexOf(" ",i+1);
      String machine=s.substring(0,i);
      String user=s.substring(i+1,j);
      String pass=s.substring(j+1);
      pass=decrypt(machine,user,pass);
      l.add(new FtpRecord(machine,user,pass));
    }
    return l;
  }

  public static void writerecords(List l) {
    String txt="";
    for (int i=0;i<l.size();i++) {
      FtpRecord r=(FtpRecord)l.get(i);
      txt+=r.machine+" "+r.user+" "+encrypt(r.machine,r.user,r.pass)+"\n";
    }
    Files.writeStringtofile(txt,passwordFile);
  }

  public static FtpRecord getrecord(List l,String h,String u) {
    for (int i=0;i<l.size();i++) {
      FtpRecord r=(FtpRecord)l.get(i);
      if (h.equals(r.machine)) {
        if (u==null)
          return r;
        if (u.equals(r.user))
          return r;
      }
    }
    return null;
  }

  static int hash(String s) {
    int h=0;
    for (int i=0;i<s.length();i++) {
      int j=(int)s.charAt(i);
      h+=j;
    }
    return h;
  }

  static char encrypt(char c,int i) {
/*    int j=(int)c;
    j=j-32;
    i=i % key.length();
    int d=(int)key.charAt(i);
    d=(d-32)%(90-32);
    return (char)(32 + (j+d)%(90-32)); */
    return c;
  }

  static char decrypt(char c,int i) {
/*    int j=(int)c;
    j=j-32;
    i=i % key.length();
    int d=(int)key.charAt(i);
    d=(d-32)%(90-32);
    int newj=(j-d);
    if (newj<0)
      newj=newj+(90-32);
    return (char)(32 + newj%(90-32)); */
    return c;
  }

  static String encrypt(String h,String u,String p) {
    int j=hash(h+u);
    String n="";
    for (int i=0;i<p.length();i++) {
      char c=p.charAt(i);
      char d=encrypt(c,j);
      n+=d;
      j+=c;
    }
    return n;
  }

  static String decrypt(String h,String u,String p) {
    int j=hash(h+u);
    String n="";
    for (int i=0;i<p.length();i++) {
      char c=p.charAt(i);
      char d=decrypt(c,j);
      n+=d;
      j+=d;
    }
    return n;
  }

  public static void main(String[] argv) {
    (new jftp()).go(argv);
  }

  public void go(String[] argv) {

    String machine=null;
    String user=null;

    ArgParser a=new ArgParser(argv);
    File localdir=a.fileAfterOr("-ld","localdir",".");
    String remotedir=a.fileAfterOr("-rd","remotedir",".")+"";
    boolean logging=a.contains("-log","show log");
    String arg1=a.get();
    a.command+="[<user>@]<machine> ";
    String[] ops={"get","put"};
    String com=a.getOption(ops);
    a.command+="<files> ";
    Vector files=new Vector();
    while (a.left()>0)
      files.add(a.get());
    a.done();

    if (arg1.indexOf("@")>0) {
      user=arg1.substring(0,arg1.indexOf("@"));
      machine=arg1.substring(arg1.indexOf("@")+1);
    } else {
      machine=arg1;
    }

    if (machine==null)
      CommandLine.error("You need to provide a machine");

    List records=readrecords();

    FtpRecord rec=getrecord(records,machine,user);
    if (rec==null) // Not found
      rec=new FtpRecord(machine,user,null);
    if (rec.user==null)
      rec.user=CommandLine.getString("User:");
    if (rec.pass==null) {
      rec.pass=CommandLine.getSecretString("Password:");
      records.add(rec);
      writerecords(records);
    }

    JFtp jftp=new JFtp(rec.machine,rec.user,rec.pass);

    System.out.println("Connecting...");
    jftp.open();

    jftp.changedir(remotedir);
//    System.out.println(""+jftp.getlist());

      System.out.print(jftp.log);

    for (int i=0;i<files.size();i++) {
      String fname=(String)files.get(i);
      if (com.equals("put")) {

        echo("Sending "+fname);
        jftp.writefile(fname);

      } else if (com.equals("get")) {

        echo("Getting "+fname);
        jftp.readfile(fname);

      } else
        System.out.println("Must provide put or get as first argument");
    }

    jftp.close();

    if (jftp.error || logging) {
      System.out.print(jftp.log);
      System.out.println("Bad one =(");
    }

  }

}
import java.net.Socket;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.util.jar.*;
import java.io.*;

import jlib.*;

class Share {

  public String name,user,machine;
  boolean writeAccess=true;
  public File localdir;

  Share(String n,String u,String m,boolean write,File ld) {
    name=n;
    user=u;
    machine=m;
    writeAccess=write;
    localdir=ld;
  }

  public String toString() {
    return name+" "+
      ( writeAccess ? "rw" : "ro" )+" "+
      user+"@"+machine+" "+
      localdir;
  }

  public static Share fromString(String s) {
    String[] ss=JString.split(s," ");
    boolean write;
    if (ss[1].equals("rw"))
      write=true;
    else if (ss[1].equals("ro"))
      write=false;
    else {
      JLib.error("Expected rw or ro but got \""+ss[1]+"\" in \""+s+"\"");
      return null;
    }
    String user=JString.before(ss[2],"@");
    String machine=JString.after(ss[2],"@");
    return new Share(ss[0],user,machine,write,new File(ss[3]));
  }

  int accessLevel() {
    if (writeAccess)
      return 2;
    else
      return 1;
  }

}











class ServerConfig {

  final static File storage=new File(JLib.JPATH+"/data/jsync-server.conf");

  String serverUser,serverMachine;
  List shares=new Vector();

  ServerConfig() {
    String u=JLib.getUserName();
    String m=JLib.getMachineName();
    setup(u,m);
  }

  ServerConfig(String u,String m) {
    setup(u,m);
  }

  void setup(String u,String m) {
    serverUser=u;
    serverMachine=m;
    readConfFile();
  }

  public Share getShare(String name) {
    for (int i=0;i<shares.size();i++) {
      Share s=(Share)shares.get(i);
      if (s.name.equals(name))
        return s;
    }
    return null;
  }

  String header() {
    return "# sync server access permissions for "+serverUser+"@"+serverMachine;
  }

  void readConfFile() {
    List lines=Files.readLinesFromFile(storage);
    if (lines==null) {
      System.out.println("Creating new config file "+storage);
      writeConfFile();
    } else {
      if (!lines.get(0).equals(header()))
        JLib.error("You are "+serverUser+"@"+serverMachine+" but "+storage+" says "+lines.get(0));
      for (int i=0;i<lines.size();i++) {
        String l=(String)lines.get(i);
        if (!l.trim().startsWith("#"))
          shares.add(Share.fromString(l));
      }
    }
  }

  void writeConfFile() {
    PrintWriter p=Files.printWriterToFile(storage);
    p.println(header());
    p.println("# share-name | read(ro)/write(rw) | user-name@host-name | directory");
    for (int i=0;i<shares.size();i++) {
      Share a=(Share)shares.get(i);
      p.println(a.toString());
    }
    p.close();
  }

  int getAccessLevel(String user,String machine,File f) {
    int level=0;
    for (int i=0;i<shares.size();i++) {
      Share a=(Share)shares.get(i);
      if ( ( a.user.equalsIgnoreCase(user) || a.user.equals("all") )
        && ( a.machine.equalsIgnoreCase(machine) || a.machine.equals("all") )
        && f.toString().startsWith(a.localdir.toString())
        && a.accessLevel()>=level )
          level=a.accessLevel();
    }
    return level;
  }
  boolean checkAccessLevel(String user,String machine,File f,int level) {
    return ( getAccessLevel(user,machine,f) >= level );
  }
  boolean checkReadAccess(String user,String machine,File f) {
    return checkAccessLevel(user,machine,f,1);
  }
  boolean checkWriteAccess(String user,String machine,File f) {
    return checkAccessLevel(user,machine,f,2);
  }

}

















class SyncSession {

  ServerConfig conf;
  SuperSocket sock;
  String user,machine;

  SyncSession(ServerConfig c,SuperSocket so,String u,String m) {
    conf=c;
    sock=so;
    user=u;
    machine=m;
  }

  void engage() throws Exception {

    sock.write("OK whaddya want?");

    String com;
    while (!(com=(String)sock.read()).equals("All done!")) {

      if (com.equals("What time do you make it?")) {

        echo(JLib.niceDate()+": Client requested the time.");
        sock.write(new Date());

      } else if (com.equals("What have you got for me?")) {

        Spec spec=new Spec((String)sock.read());
        Date lastGot=(Date)sock.read();
        File dir=(File)sock.read();
        sock.write("Wait");
        // echo("Client has requested info on "+dir);
        if (!checkReadAccess(dir)) {
          sock.write("Sorry, you do not have read access for "+dir);
        } else {
          Collect collect=spec.getCollectorOnServer();
          sock.write("OK");
          List results=collect.whatsnew(lastGot,dir);
          echo("Found "+results.size()+" new files for them:");
          echo(""+results);
          sock.write(results);
        }

      } else if (com.equals("What's my access")) {

        File f=(File)sock.read();
        int al=getAccessLevel(f);
        sock.write(new Integer(al));
        echo("They have access level "+al+" on "+f);

      } else if (com.equals("Please give me a file")) {

        File f=(File)sock.read();
        if (!checkReadAccess(f))
          sock.write("Sorry, you do not have read access for "+f);
        else {
          sock.write("Will do");
          echo(JLib.niceDate()+": Sending "+f);
          sock.writeFile(f);
        }

      } else if (com.equals("Please accept this file")) {

        File f=(File)sock.read();
        if (!checkWriteAccess(f))
          sock.write("Sorry, you do not have write access for "+f);
        else {
          sock.write("Most kind");
          echo(JLib.niceDate()+": Receiving "+f);
          sock.readFile(f);
        }

      } else if (com.equals("Please give me a jar of files")) {

        File topDir=(File)sock.read();
        List fileNames=(List)sock.read();
        if (!checkReadAccess(topDir,fileNames))
          sock.write("Sorry read access on some of those files is denied!");
        else {
          echo(JLib.niceDate()+": Sending jar of "+topDir+"/ "+fileNames);
          sock.write("On its way");
          File tmpJar=new File(JLib.JPATH,"tmp/syncserver-out.jar");
          JLib.makeJar(tmpJar,topDir,fileNames);
          sock.writeFile(tmpJar);
          // tmpJar.delete();
        }

      } else if (com.equals("Please accept this jar of files")) {

        File topDir=(File)sock.read();
        File tmpJar=new File(JLib.JPATH,"tmp/syncserver-in.jar");
        sock.readFile(tmpJar);
        JarFile jar=new JarFile(tmpJar);
        List fileNames=JLib.namesOfEntries(jar);
        jar.close();
        echo(JLib.niceDate()+": Received jar of "+topDir+"/ "+fileNames);
        if (!checkWriteAccess(topDir,fileNames))
          sock.write("Sorry write access to some of those files is denied!");
        else {
          sock.write("Why thank you");
          JLib.openJar(tmpJar,topDir);
        }
        // tmpJar.delete();

      /* } else if (com.equals("Can I use this collector?")) {

        JLib.error(this,"deprecated");
        Spec spec=new Spec((String)sock.read());
        sock.write(spec.getCollectorOnServer());
        sock.read("thanx"); */

      } else if (com.equals("Please give me byte[] collector class")) {

        Spec spec=new Spec((String)sock.read());
        echo("Sending client class "+spec);
        sock.write(spec.getClassBytes());

      } else {
        JLib.error("Got unknown request from client: "+com);
      }

    }

    sock.write("Bye");
    sock.read("Bye");

  }

  boolean checkReadAccess(File top,List names) {
    for (int i=0;i<names.size();i++)
      if (!checkReadAccess(new File(top,""+names.get(i))))
        return false;
    return true;
  }
  boolean checkWriteAccess(File top,List names) {
    for (int i=0;i<names.size();i++)
      if (!checkWriteAccess(new File(top,""+names.get(i))))
        return false;
    return true;
  }
  boolean checkReadAccess(File file) {
    return conf.checkReadAccess(user,machine,file);
  }
  boolean checkWriteAccess(File file) {
    return conf.checkWriteAccess(user,machine,file);
  }
  int getAccessLevel(File file) {
    return conf.getAccessLevel(user,machine,file);
  }

  static void echo(String s) {
    Server.echo(s);
  }

}














public class Server {

  public static void main(String[] args) {

    ArgParser a=new ArgParser(args);
    String com=a.getOr("optional command","start");

    if (com.equals("start")) {

      a.done();
      Server h=new Server();
      h.run();

    } else if (com.equals("list")) {

      echo(ServerConfig.storage+":");
      echo(JLib.execute("cat "+ServerConfig.storage));

    } else if (com.equals("share")) {

      boolean write=a.contains("-write");
      String name=a.get("name");
      String user=a.get("user");
      String machine=a.get("machine");
      File dir=a.getFile("dir");
      a.done();

      ServerConfig conf=new ServerConfig();
      Share already=conf.getShare(name);
      if (already!=null) {
        echo("Replacing share \""+name+"\":\n  "+already.toString());
        if (!conf.shares.remove(already))
          JLib.error("Failed to remove!");
      }
      Share newa=new Share(name,user,machine,write,dir);
      conf.shares.add(newa);
      conf.writeConfFile();
      echo("New share provided:\n  "+newa);

    } else if (com.equals("deny")) {

      String name=a.get("name");
      a.done();

      ServerConfig conf=new ServerConfig();
      Share already=conf.getShare(name);
      if (already==null)
        JLib.error("Could not find an access named "+name);
      conf.shares.remove(already);
      conf.writeConfFile();
      echo("Share removed:\n  "+already);

    } else {

      echo("Unrecognised command: "+com);
      a.done();

    }

  }

  void run() {

    echo("Reading server config...");

    ServerConfig conf=new ServerConfig();

    echo("Starting server on port "+JLib.defaultSyncPort+"...");

    while (true) {

      echo("");
      echo("Waiting for a client...");

      SuperSocket sock=new SuperSocket(JLib.defaultSyncPort);

      try {

      sock.read("Hello sync client here");
      sock.write(conf.serverMachine);
      sock.write("Who are you?");
      String user=(String)sock.read();
      String machine=(String)sock.read();

      echo(JLib.niceDate()+": Start session with "+user+"@"+machine);

      if ( user.equalsIgnoreCase(conf.serverUser)
        && machine.equalsIgnoreCase(conf.serverMachine) ) {
          sock.write("Sorry you can't sync when you are running the sync-server!");
          echo("Terminating connection since same user and machine!");
      } else {
        SyncSession session=new SyncSession(conf,sock,user,machine);
        session.engage();
      }

      sock.close();
      echo(JLib.niceDate()+": Finish session with "+user+"@"+machine);

      /*

      sock.read("I was last updated...");
      Date d=(Date)sock.read();
      sock.write("OK got it");
      echo("Last updated "+d);

      sock.read("Have you got anything for me?");
      Local l=new Local(d);
      l.tocheck.add(new File("."));
      List localfiles=l.whatsnew();
      System.out.println("I have "+localfiles.size()+" / "+l.possible+" relevant files.");
      if (localfiles.size()>0) {
        sock.write("Yup!");
        sock.write(localfiles);
      } else
        sock.write("Nada");

      sock.read("I have new files...");
      List remotefiles=(List)sock.read();
      sock.write("OK got it");

      // System.out.println("Got "+remotefiles);
      for (int i=0;i<remotefiles.size();i++) {
        File f=(File)remotefiles.get(i);
        echo("File "+f+" "+( !f.exists() ? "*new*" : "" ));
      }

      */

      } catch (Exception e) {
        echo("Server.run(): "+e);
        echo("Closing socket...");
        sock.close();
        echo("Trying to reopen service...");
      }

    }

  }

  static void echo(String s) {
    System.out.println(s);
  }

}

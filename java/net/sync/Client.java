import java.net.Socket;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;

import jlib.*;

class Join {

  public String name,server;
  int serverPort=JLib.defaultSyncPort;
  public File localDir,remoteDir;
  public Spec spec;

  public Date lastPut=null;
  public Date lastGot=null;
  public List needToGo=null;

//  public int totalChecked=-1;

  Join(String n,File ld,String s,File rd) {
    this(n,ld,s,rd,new Spec());
  }

  Join(String n,File ld,String s,File rd,Spec sp) {
    name=n;
    server=s;
    localDir=ld.getAbsoluteFile();
    // Does not seem to do what I want:
    // I want ld = "a/b/c/../e" to turn into localDir = "a/b/e".
    // This happens if user offers ../e when in c.
    // I expect problems if c ceases to exist.
    remoteDir=rd;
    spec=sp;
  }

  public String toString() {
    return name+" "+localDir+" "+server+" "+remoteDir+" "+spec.toString();
  }

  public static Join fromString(String s) {
    String[] ss=JString.split(s," ");
    return new Join(
      ss[0],
      new File(ss[1]),
      ss[2],
      new File(ss[3]),
      ( ss.length < 5 ? new Spec() : new Spec(ss[4]) )
    );
  }

  String onBreak() {
    return "Future syncs to "+localDir+" will use "+lastPutFile()+" and "+lastGotFile();
  }

  public File lastPutFile() {
    return new File(localDir,".lastput.sync");
  }

  public void setJustPut(Date d) {
    String s="Join "+name+" from "+localDir+" to "+server+":"+remoteDir+"\n"+
             "was last PUT "+d+" (local time)\n"+
             "Warning: do not modify this file or data will not sync properly!";
    Files.writeStringtoFile(s,lastPutFile());
    lastPutFile().setLastModified(d.getTime()+10*1000);
  }

  public Date getLastPut() {
    if (lastPut==null) {
      File f=lastPutFile();
      if (f.exists())
        lastPut=new Date(f.lastModified());
      else {
        Client.echo(name+" has never been put");
        lastPut=new Date(0);
      }
    }
    return lastPut;
  }

  public File lastGotFile() {
    return new File(localDir,".lastgot.sync");
  }

  public void setJustGot(Date d) {
    String s="Join "+name+" from "+localDir+" to "+server+":"+remoteDir+"\n"+
             "was last GOT "+d+" (server time)\n"+
             "Warning: do not modify this file or data will not sync properly!";
    Files.writeStringtoFile(s,lastGotFile());
    lastGotFile().setLastModified(d.getTime()+10*1000);
  }

  public Date getLastGot() {
    if (lastPut==null) {
      File f=lastGotFile();
      if (f.exists())
        lastGot=new Date(f.lastModified());
      else {
        Client.echo(name+" has never been got");
        lastGot=new Date(0);
      }
    }
    return lastGot;
  }

}


















class ClientConfig {

  final static File storage=new File(JLib.JPATH+"/data/jsync.conf");

  String user,machine;
  List joins=new Vector();

  ClientConfig() {
    String u=JLib.getUserName();
    String m=JLib.getMachineName();
    setup(u,m);
  }

  ClientConfig(String u,String m) {
    setup(u,m);
  }

  void setup(String u,String m) {
    user=u;
    machine=m;
    readConfFile();
  }

  String header() {
    return "# sync joins for "+user+"@"+machine;
  }

  public Join getJoin(String name) {
    for (int i=0;i<joins.size();i++) {
      Join s=(Join)joins.get(i);
      if (s.name.equals(name))
        return s;
    }
    return null;
  }

  void readConfFile() {
    List lines=Files.readLinesFromFile(storage);
    if (lines==null) {
      System.out.println("Creating new config file "+storage);
      writeConfFile();
    } else {
      if (!lines.get(0).equals(header()))
        JLib.error("You are "+user+"@"+machine+" but "+storage+" says "+lines.get(0));
      for (int i=0;i<lines.size();i++) {
        String l=(String)lines.get(i);
        if (!l.trim().startsWith("#"))
          joins.add(Join.fromString(l));
      }
    }
  }

  void writeConfFile() {
    PrintWriter p=Files.printWriterToFile(storage);
    p.print(toString());
    p.close();
  }

  public String toString() {
    StringBuffer s=new StringBuffer();
    s.append(header()+"\n");
    s.append("# join-name | local-dir | server | server-dir | (spec)\n");
    for (int i=0;i<joins.size();i++) {
      Join j=(Join)joins.get(i);
      s.append(j.toString()+"\n");
    }
    return s+"";
  }

}














public class Client {

  public static void main(String[] args) {

    ArgParser a=new ArgParser(args);

    boolean prompt=!a.contains("-noui","don't prompt for user interaction");
    boolean bring=a.contains("-bring","bring conflicting files here rather than send to server");
    boolean jar=a.contains("-jar","use compressed jar for transfer");
    int retries=a.intafter("-retries","number of connection attempts",10);
    // String[] options={"list","join","sync","syncall"};
    String[][] options={ {"list","List existing joins"},
                         {"join","Make a new join"},
                         {"break","Break a join"},
                         {"sync","Sync a join"},
                         {"syncall","Sync all joins"} };
    String command=a.optionOr("command",options,"syncall");
    ClientConfig conf=new ClientConfig();

    if (command.equals("list")) {

      a.done();

      echo(ClientConfig.storage+":");
      echo(""+conf.toString());
      // Unix only:
      // echo(JLib.execute("cat "+ClientConfig.storage));

    } else if (command.equals("join")) {

      a.say("You must provide the following to create a new join:");
      String joinName=a.get("join name");
      File localDir=a.getFile("local dir");
      String server=a.get("share server");
      File remoteDir=a.getFile("server dir");
      Spec spec=new Spec(a.Stringafter("-spec",new Spec().toString(),"specify collector classname"));
      a.done();

      Join already=conf.getJoin(joinName);
      if (already!=null) {
        echo("Replacing old join \""+joinName+"\":\n  "+already);
        if (!conf.joins.remove(already))
          JLib.error("Failed to remove!");
        echo(already.onBreak());
      }
      Join newj=new Join(joinName,localDir,server,remoteDir,spec);
      conf.joins.add(newj);
      conf.writeConfFile();

      echo("New sync joined:\n  "+newj);

    } else if (command.equals("break")) {

      String joinName=a.get("join name");
      a.done();

      Join already=conf.getJoin(joinName);
      if (already==null)
        JLib.error("There is no join "+joinName+" in "+ClientConfig.storage);
      else
        conf.joins.remove(already);
      echo(already.onBreak());
      conf.writeConfFile();

      echo("Sync join broken:\n  "+already);

    } else if (command.equals("sync")) {

      String joinName=a.get("join name");
      a.done();

      Join j=conf.getJoin(joinName);
      if (j==null)
        JLib.error("No such join: "+joinName);
      else {
        Client client=new Client(conf,j,prompt,bring,jar,retries);
        client.sync();
        client.close();
      }

    } else {

      a.done();

      if (conf.joins.size()==0)
        echo("You must make some joins first!");
      else {
        echo("Trying to sync all joins...");
        echo();
        for (int i=0;i<conf.joins.size();i++) {
          Join j=(Join)conf.joins.get(i);
          echo("Syncing "+j+" ("+(i+1)+"/"+conf.joins.size()+") ...");
          echo();
          Client client=new Client(conf,j,prompt,bring,jar,retries);
          client.sync();
          client.close();
          echo("Done syncing "+j);
          echo();
          if (i<conf.joins.size()-1)
            JLib.stopFor(5*1000);
        }
      }

    }

  }







  ClientConfig conf;
  boolean promptUser,jar;
  Join join;

  int retries;
  int retryTime=5*60; // every five minutes

  int conflictStrategy=2;
    // 0 = panic and quit
    // 1 = bring files to client for merging
    // 2 = send files to server for merging
  SuperSocket sock=null;
  String serverName=null;

  public Client(ClientConfig c,Join j,boolean p,boolean bring,boolean jr,int r) {
    conf=c;
    promptUser=p;
    join=j;
    if (bring)
      conflictStrategy=1;
    jar=jr;
    retries=r;
  }

  public void getSocket() {
    int attempt=0;
    int waitSeconds=5;
    while (sock==null) {

        try {

          echo("Connecting to server "+join.server+" on port "+join.serverPort+" ...");
          sock=new SuperSocket(join.server,join.serverPort);

          sock.write("Hello sync client here");
          serverName=(String)sock.read();
          sock.read("Who are you?");
          sock.write(conf.user);
          sock.write(conf.machine);
          sock.read("OK whaddya want?");

        } catch (Exception e) {

          sock=null;
          attempt++;
          if (attempt>retries)
            JLib.error("Given up after "+retries+" attempts.");
          echo(e.getMessage()+", will try again in "+waitSeconds+" seconds (at "+JLib.niceTime(new Date().getTime()+waitSeconds*1000)+")");
          echo();
          // "Failed to open socket to "+join.server+":"+join.serverPort+"."
          JLib.stopFor(waitSeconds*1000);
          waitSeconds=waitSeconds*2;
          if (waitSeconds>retryTime)
            waitSeconds=retryTime;

        }

    }
  }

  void requestUpdates(Date lastGot,File dir) {
    echo("Requesting updates to "+dir+" since last sync "+JLib.niceDate(lastGot));
    sock.write("What have you got for me?");
    sock.write(join.spec.toString());
    sock.write(lastGot);
    sock.write(dir);
    sock.read("Wait");
  }

  List collectUpdates() {
    String res=(String)sock.read();
    if (!res.equals("OK"))
      JLib.error("Asking for updates, got "+res);

    List l=(List)sock.read();
    return l;
  }

  Date getServerTime() {
    sock.write("What time do you make it?");
    return (Date)sock.read();
  }

  /** The extension to give conflicting files which are coming
   *  from server to client.
  **/
  String bringExtension() {
    return "."+serverName+"-sync";
  }
  /** The extension to give conflicting files which are going
   *  from client to server.
  **/
  String sendExtension() {
    return "."+conf.user+"-"+conf.machine+"-sync";
  }

  public void bring(List fs) {
    bring(fs,false);
  }
  public void bringJar(List fs) {
    File tmpJar=new File(JLib.JPATH,"tmp/sync-in.jar");
    sock.write("Please give me a jar of files");
    sock.write(join.remoteDir);
    sock.write(fs);
    sock.read("On its way");
    echo("Bringing jar from server...");
    sock.readFile(tmpJar);
    JLib.openJar(tmpJar,join.localDir);
    // tmpJar.delete();
    echo("Extracted jar to "+join.localDir+" containing "+fs);
  }
  public void bring(List fs,boolean conflict) {
    String extensionAdd= ( conflict ? bringExtension() : "" );
    for (int i=0;i<fs.size();i++) {
      String fn=(String)fs.get(i);
			try {
        File remotef=new File(join.remoteDir,fn);
        File f=new File(join.localDir,fn);
        if (conflict)
          f=new File(f.toString()+extensionAdd);
  
        sock.write("Please give me a file");
        sock.write(remotef);
        sock.read("Will do");
        sock.readFile(f);
  
        echo("Received "+f);
			} catch (Exception e) {
				Log.error(this,"Problem getting "+fn,e);
			}
    }
  }

  public void send(List fs) {
    send(fs,false);
  }
  /** Does not deal with conflicts - need to do some file renaming for that. **/
  public void sendJar(List fs) {
      File tmpJar=new File(JLib.JPATH,"tmp/sync-out.jar");
      JLib.makeJar(tmpJar,join.localDir,fs);
      sock.write("Please accept this jar of files");
      sock.write(join.remoteDir);
      sock.writeFile(tmpJar);
      sock.read("Why thank you");
      echo("Sent jar to server containing "+join.localDir+"/ "+fs);
      // tmpJar.delete();
  }
  public void send(List fs,boolean conflict) {
    String extensionAdd= ( conflict ? sendExtension() : "" );
    for (int i=0;i<fs.size();i++) {
      String fn=(String)fs.get(i);
			try {
        File remotef=new File(join.remoteDir,fn);
        File f=new File(join.localDir,fn);
        if (conflict)
          remotef=new File(remotef.toString()+extensionAdd);
  
        echo("Sending "+f);
  
        sock.write("Please accept this file");
        sock.write(remotef);
        sock.read("Most kind");
        sock.writeFile(f);
			} catch (Exception e) {
				Log.error(this,"Problem sending "+fn,e);
			}
     }
  }

  public int getAccessLevel(File f) {
    sock.write("What's my access");
    sock.write(f);
    return ((Integer)sock.read()).intValue();
  }

	private String printList(List l) {
		StringBuffer s=new StringBuffer();
		for (int i=0;i<l.size();i++) {
			s.append("  "+l.get(i)+"\n");
		}
		return s.toString();
	}

  public boolean sync() {

    getSocket();

    // We have to analyse incoming and outgoing to check for conflicts
    // It could be done more efficiently by checking only those outgoing files for conflicts at the other end (or vice-versa)

    StringBuffer summary=new StringBuffer();

    int accessLevel=getAccessLevel(join.remoteDir);
    if (accessLevel==0) {
      System.out.println("You do not have access to "+join.server+":"+join.remoteDir);
      return false;
    }
    else if (accessLevel==1)
      echo("You have read access on "+join.remoteDir);
    else if (accessLevel==2)
      echo("You have read-write access to "+join.remoteDir);

    Date lastGot=join.getLastGot();
    Collect collector=getCollector();
    // Start searching remotely
    requestUpdates(lastGot,join.remoteDir);
    // Search locally
    List outgoing=( accessLevel==2 ? getNeedToGo(collector) : new Vector() );
    // Collect remote search
    List incoming=collectUpdates();

    List conflicts=JList.intersection(incoming,outgoing);
    if (conflicts.size()>0) {
      echo("Removing conflicting files from O="+outgoing.size()+" and I="+incoming.size());
      JList.remove(outgoing,conflicts);
      JList.remove(incoming,conflicts);
      echo("leaving O="+outgoing.size()+" and I="+incoming.size());
      echo();
    }

    echo();
    echo("The following syncing is required:");
    int total=0;
    if (accessLevel==2) {
      echo("Outgoing: "+outgoing.size()); // +" / "+join.totalChecked);
      echo(printList(outgoing));
      total+=outgoing.size();
    }
    echo("Incoming: "+incoming.size());
    echo(printList(incoming));
    total+=incoming.size();

    if (conflicts.size()>0) {
      echo("Conflicts: "+conflicts.size());
      echo(printList(conflicts));
      total+=conflicts.size();
      if (conflictStrategy==0) {
        echo("Will give up!");
      } else if (conflictStrategy==1) {
        echo("Will bring conflicting files here, and give them extension "+bringExtension()+" so that you can merge them.");
      } else if (conflictStrategy==2) {
        echo("Will send conflicting files to "+join.server+" and give them extension "+sendExtension()+" so that they may be merged together there.");
      }
    }

    boolean ok=true;
    if (promptUser && total>0) { // Go straight through if 0 todo
      echo();
      ok=UI.askYesNo("Would you like to go ahead with the sync?");
    }
    if (!ok) {
      echo();
      echo("Aborting!");
      echo();
      return false;
    }

    echo();

    if (conflicts.size()>0) {

      if (conflictStrategy==0) {

        JLib.error("File conflicts - choose -here or -there");
        return false;

      } else if (conflictStrategy==1) {

        echo("Bringing conflicting files here for merging...");
        bring(conflicts,true);
        summary.append("Examine all sync files for "+conflicts+" from "+serverName+"\n");
        summary.append(""+conflicts);
        echo("You will be marked as fully synced, but the conflicting files should be rectified, and in doing so they will be synced at a later date.");

      } else if (conflictStrategy==2) {

        echo("Sending conflicting files there for merging...");
        send(conflicts,true);
        summary.append("Server should examine all sync files for "+conflicts+" from "+conf.user+"@"+conf.machine+"\n");
        summary.append(""+conflicts);
        echo("You will be marked as fully synced, but the conflicting files should be rectified, and in doing so they will be synced at a later date.");

      }

    }

    if (accessLevel>=1) {
      if (incoming.size()>0) {
        echo("Bringing new files so that we are up-to-date...");
        if (jar)
          bringJar(incoming);
        else
          bring(incoming,false);
      } else
        echo("Nothing to bring.");
    } else
      echo("You do not have access to bring anything.");

    echo();

    if (accessLevel==2) {
      if (outgoing.size()>0) {
        echo("Sending new files to keep "+serverName+" up-to-date...");
        if (jar)
          sendJar(outgoing);
        else
          send(outgoing,false);
      } else
        echo("Nothing to send.");
      echo();
    }

    // Have to do both of these after all files have moved.
    echo("Updating "+join.lastGotFile());
    join.setJustGot(getServerTime());
    if (accessLevel==2) {
      echo("Updating "+join.lastPutFile());
      join.setJustPut(new Date());
    }
    echo();

    String sum=summary.toString();
    if (sum.length()>0)
      echo(summary.toString());


    return true;

  }


  static void echo(String s) {
    System.out.println(s);
  }
  static void echo() { echo(""); }

  void close() {
    sock.write("All done!");
    sock.read("Bye");
    sock.write("Bye");
  }

  Collect getCollector() {
    return join.spec.getCollectorFromServer(sock);
    // new Collect();
  }
  public List getNeedToGo(Collect collect) {
    if (join.needToGo==null) {
      join.needToGo=collect.whatsnew(join.getLastPut(),join.localDir);
//      join.totalChecked=collect.possible;
    }
    return join.needToGo;
  }

}

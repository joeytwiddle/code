package jlib;

import java.net.*;
import java.lang.String;
import java.io.*;

public class SuperSocket {

  public boolean echoing=false;

  ObjectOutputStream out=null;
  ObjectInputStream in=null;

  // Only used to perform close at end
  Socket socket=null;
  ServerSocket serverSocket=null;
  Socket serverSocketSocket=null;

  // The thing you got when you expected something else
  public Object deviance;

  static public SuperSocket newSuperSocket(String server,int port) {
    try {
      return new SuperSocket(server,port);
    } catch (Exception e) {
      System.out.println("newSuperSocket("+server+","+port+"): "+e);
      return null;
    }
  }

  public SuperSocket(String server,int port) throws /*ProblemConnecting*/Exception {
    try {
      socket=new Socket(server,port);
      out=new ObjectOutputStream(socket.getOutputStream());
      in=new ObjectInputStream(socket.getInputStream());
    } catch (Exception e) {
      // System.out.println("SuperSocket("+server+","+port+"): "+e);
      throw e; // new ProblemConnectingException(e);
    }
  }

  /* public class ProblemConnectingException extends Exception {
    public ProblemConnectingException(Exception e) {
      super("Problem connecting: "+e);
    }
  } */

  public SuperSocket(int port) {
    try {
      serverSocket=new ServerSocket(port);
      serverSocketSocket=serverSocket.accept();
      out=new ObjectOutputStream(serverSocketSocket.getOutputStream());
      in=new ObjectInputStream(serverSocketSocket.getInputStream());
    } catch (Exception e) {
      System.out.println("SuperSocket("+port+"): "+e);
    }
  }

  public void write(Object o) {
    try {
      if (echoing)
        echo(">> "+o);
      out.writeObject(o);
      out.flush();
    } catch (Exception e) {
      System.out.println("SuperSocket.write(): "+e);
    }
  }

  public Object read() {
    Object got=quietread();
    if (echoing)
      echo("<< "+got);
    return got;
  }

  private Object quietread() {
    try {
      Object got=in.readObject();
      return got;
    } catch (Exception e) {
      System.out.println("SuperSocket.read(): "+e);
      return null;
    }
  }

  public boolean expect(Object o) {
    return read(o);
  }

  public boolean read(Object o) {
    Object got=read();
    if (got.equals(o)) {
      if (echoing)
        echo("(= "+got);
      return true;
    } else {
      deviance=got;
      if (echoing)
        echo("  ERROR: I expected "+o);
      JLib.report("SuperSocket: expected "+o+" got "+got);
      return false;
    }
  }

  public void echo(String s) {
    System.out.println(s);
  }

  public void close() {
    try {
    // Close if client
    if (socket!=null)
      socket.close();
    // Close if server
    if (serverSocketSocket!=null)
      serverSocketSocket.close();
    if (serverSocket!=null)
      serverSocket.close();
    } catch (Exception e) {
      JLib.error(this,"close",e);
    }
 }

 public void writeFile(File f) {
   try {
     InputStream in=new BufferedInputStream(new FileInputStream(f));
     int tot=0;
     boolean done=false;
     while (!done) {
       int a=in.available();
       if (a==-1)
         done=true;
       else {
         byte[] bs=new byte[a];
         int numread=in.read(bs);
         if (numread!=a)
           JLib.error("SuperSocket.sendFile(): numread!=a");
         write(bs);
         tot+=a;
         if (tot>=f.length())
           done=true;
       }
     }
     in.close();
   } catch (Exception e) { // In case of error, don't send any more file, but don't warn!
     JLib.report(e);
   }
   write("EOF");
 }

 public void readFile(File f) {
    try {
      f.getParentFile().mkdirs();
      BufferedOutputStream out=new BufferedOutputStream(new FileOutputStream(f));
      boolean done=false;
      while (!done) {
        Object o=read();
        if (o.equals("EOF"))
          done=true;
        else {
          byte[] bs=(byte[])o;
          out.write(bs);
          // echo(""+bs.length);
        }
      }
      out.close();
    } catch (Exception e) {
      JLib.report(e);
    }
  }

}
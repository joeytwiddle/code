import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;
import jlib.*;

public class ComSlave {

  public static void main(String[] args) {

    System.out.println(""+JLib.JPATH);

    ArgParser a=new ArgParser(args);
    String[] ops={ "host" , "use" };
    String mode=a.getOption("command",ops);
    String server=a.getOr("server","none");

    if (mode.equals("host")) {

      a.done();

      host(server);

    } else if (mode.equals("use")) {

      use(server);

    }

  }

  public static void use(String server) {

    String pass=CommandLine.getSecretString("passwd");

    SuperSocket sock=( server.equals("none") ?
        new SuperSocket(JLib.defaultSlavePort)
      : new SuperSocket(server,JLib.defaultSlavePort) );
    sock.write(pass);

    if (sock.read("OK")) {

      while (true) {

        String[] options={"send","shell"};
        String com=CommandLine.getOption(options);

        if (com.equals("send")) {

          File f=CommandLine.getFile("local filename");
          File remote=CommandLine.getFile("remote filename");
          sock.write("save file");
          sock.write(remote);
          sock.writeFile(f);

        } else if (com.equals("shell")) {

          System.out.println("Enter shell script, ending with empty line.");
          String tot="";
          while (true) {
            String nxt=CommandLine.readLine();
            if (nxt.equals(""))
              break;
            tot+=nxt+"\n";
          }
          File f=new File("./tmp.comslave");
          Files.writeStringToFile(tot,f);
          sock.write("run sh and return output");
          sock.writeFile(f);
          f.delete();
          System.out.println(""+sock.read());

        }

      }

    }

  }

  public static void host(String server) {

    String passwd=CommandLine.getSecretString("passwd");

    while (true) {

      SuperSocket sock=null;
      while (sock==null) {
        try {
         sock=( server.equals("none") ?
            new SuperSocket(JLib.defaultSlavePort)
          : new SuperSocket(server,JLib.defaultSlavePort) );
          if (sock.read(passwd))
            sock.write("OK");
          else {
            System.out.println("Invalid password: "+sock.deviance);
            sock.close();
            sock=null;
          }
        } catch (Exception e) {
          JLib.report(e);
          JLib.stopFor(1*1000);
        }
      }

      try {

        while (true) {

          String cmd=(String)sock.read();

          if (cmd.equals("save file")) {
            File f=(File)sock.read();
            sock.readFile(f);
          } else if (cmd.equals("run sh and return output")) {
            File f=new File("./comslaveshell.sh");
            sock.readFile(f);
            JLib.exec("chmod u+x "+f);
            System.out.println("Did:\n"+Files.readStringFromFile(f));
            String res=JLib.exec("sh -c "+f+"");
            System.out.println("Got:\n"+res);
            sock.write(res);
          }

        }

      } catch (Exception e) {
        JLib.report(e);
      }

      try {
        sock.close();
      } catch (Exception e) {
        JLib.report(e);
      }

    }

  }

}
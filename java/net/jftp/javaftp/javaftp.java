import com.fooware.net.*;
import jlib.Files;

public class javaftp {
  static FtpClient ftp;
  static void checkresponsefor(String action) {
      FtpResponse resp = ftp.getResponse();
      System.out.println(""+resp);
//      if (!resp.isPositiveCompletion()) {
//        System.out.println("Error with "+action+": "+resp);
//        System.exit(0);
//      }
  }
  public static void main(String[] args) {
    if (args.length<4) {
      System.out.println("javaftp <host> <username> <password> <files>");
    } else {
      String host=args[0];
      String user=args[1];
      String pass=args[2];
      
      ftp=new FtpClient();

      try {
        ftp.connect(host);
        checkresponsefor("connection");

      ftp.userName(user);
      checkresponsefor("user");

      ftp.password(pass);
      checkresponsefor("password");
      
      System.out.println("OKAY - logged in!");
      
      FtpWriter ftpout;
      
      ftp.representationType(ftp.ASCII_TYPE);
      ftp.dataPort();
      
      for (int i=3;i<args.length;i++) {
        String fname=args[i];
        System.out.println("Trying to store...");
        ftpout=ftp.store(fname);
        checkresponsefor("file "+fname);
        String s=Files.readStringfromfile(fname);
        System.out.println("Writing file...");
        ftpout.write(s);
        System.out.println("Done writing file...");
        ftpout.close();
        checkresponsefor("closing file "+fname);
      }
      
      ftp.logout();
      checkresponsefor("logout");

      } catch (Exception e) {
      	System.out.println("Exception in FTP process: "+e);
      	System.exit(0);
      }
      
    }
  }
}
import java.lang.*;
import java.io.*;

public class GG {

	public static final void main(String[] argv) {
		sendemail(argv[0]);
	}

  public static void sendemail( String email ) {
      try {
      String[] mailcommand = {"/usr/lib/sendmail", email};
      Process mailer = Runtime.getRuntime().exec( mailcommand ) ;
      PrintStream mail = new PrintStream( mailer.getOutputStream() ) ;
      mail.print( "Subject: blah blah blah\n" ) ;
      mail.print( "blah blah blah.....") ;
      mail.close() ;
        Thread.sleep( 400 ) ;
      } catch( Exception e ) {
				System.out.println(""+e);
        ;
      }
  }

}

import java.lang.*;

import jlib.*;

public class Customer {

  public static void main(String[] args) {

    while (true) {

      SuperSocket sock=new SuperSocket(3456);

      System.out.println("Got socket");

      String s=CommandLine.readLine();

      sock.write(s);

      Object o=sock.read();

      System.out.println(""+o);

      sock.close();

    }

  }
                                     15february2001
}
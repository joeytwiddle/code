/**
 * Example Client program using TCP.
 */

public class Tclient {

      final static  String    serverIPname = "10.0.0.16";     // server IP name
      final static  int       serverPort   = 3456;                      // server port number

  public static void main(String args[]) {
      java.net.Socket         sock = null;                              // Socket object for communicating
      java.io.PrintWriter     pw   = null;                              // socket output to server
      java.io.BufferedReader  br   = null;                              // socket input from server

      try {
            sock = new java.net.Socket(serverIPname,serverPort);       // create socket and connect
            pw   = new java.io.PrintWriter(sock.getOutputStream(), true);  // create reader and writer
            br   = new java.io.BufferedReader(new java.io.InputStreamReader(sock.getInputStream()));
            System.out.println("Connected to Server");

            pw.println("Message from the client");                      // send msg to the server
            System.out.println("Sent message to server");
            String answer = br.readLine();                              // get data from the server
            System.out.println("Response from the server >" + answer);

            pw.close();                                                 // close everything
            br.close();
            sock.close();
      } catch (Throwable e) {
            System.out.println("Error " + e.getMessage());
            e.printStackTrace();
      }
}
}
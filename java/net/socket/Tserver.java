/**
 * Example Server program using TCP.
 */

public class Tserver {

      final static      int   serverPort   = 3456;                      // server port number

public static void main(String args[]) {
      java.net.ServerSocket   sock = null;                              // original server socket
      java.net.Socket         clientSocket = null;                      // socket created by accept
      java.io.PrintWriter     pw   = null;                              // socket output stream
      java.io.BufferedReader  br   = null;                              // socket input stream

      try {
            sock = new java.net.ServerSocket(serverPort);               // create socket and bind to port
            System.out.println("waiting for client to connect");
            clientSocket = sock.accept();                               // wait for client to connect
            System.out.println("client has connected");
            pw   = new java.io.PrintWriter(clientSocket.getOutputStream(),true);
            br   = new java.io.BufferedReader(new java.io.InputStreamReader(clientSocket.getInputStream()));

            String msg = br.readLine();                                 // read msg from client
            System.out.println("Message from the client >" + msg);
            pw.println("Got it!");                                      // send msg to client

            pw.close();                                                 // close everything
            br.close();
            clientSocket.close();
            sock.close();
      } catch (Throwable e) {
            System.out.println("Error " + e.getMessage());
            e.printStackTrace();
      }
}
}

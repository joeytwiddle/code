import java.lang.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;

import jlib.Files;
import nuju.*;

public class Edit {

  public final static void main(String[] args) {

    try {

//    HTMLDocument doc=new HTMLDocument();
//    String fromfile=jlib.Files.readStringfromfile("/home/joey/.netscape/bookmarks.html");
//    doc.insertString(0,fromfile,new SimpleAttributeSet());

    JFrame frame=new JFrame("Joey's Test 1");

//    JEditorPane pane=new JEditorPane("http://hwi");
//    JEditorPane pane=new JEditorPane("http://hwi/servlet/changetheworld.CTW?command=display&page=/theindex.html");
    JEditorPane pane=new JEditorPane("file:///home/httpd/html/changetheworld/theindex.html");

    HTMLDocument doc=(HTMLDocument)pane.getDocument();

    Nuju.present(doc);
    System.out.println("BaseURL for document is: "+doc.getBase());
    Files.writeStringtofile(""+doc,"doc.txt");

    JEditorPane pane2=new JTextPane(doc);

    frame.getContentPane().add(pane);
//    frame.getContentPane().add(pane2);

    frame.setSize(400,400);

    frame.show();

    } catch (Exception e) {

      System.out.println("main(): "+e);

    }

  }

}
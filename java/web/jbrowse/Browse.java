import java.io.*;
import java.lang.*;
import java.util.*;
import java.net.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.text.html.parser.ParserDelegator;

import jlib.Files;
import nuju.*;

class MyParserCallback extends HTMLEditorKit.ParserCallback {

  URL baseUrl=null;
  Vector links=new Vector();

  MyParserCallback(URL url,String page) {
    super();
    try {
      baseUrl=url;
      Reader sr=new StringReader(page);
      (new ParserDelegator()).parse(sr, this, true);
    } catch (Exception e) {
      System.out.println("MyParserCallback(url,page): "+e);
    }
  }

  public void handleText(char[] data, int pos) {
    System.out.println("Text: "+new String(data));
  }

	public void handleStartTag(HTML.Tag t, MutableAttributeSet a, int pos) {
    if (t.toString().toLowerCase().equals("a")) {
      try {
        String lnk=a.getAttribute(HTML.Attribute.HREF).toString();
        URL nurl=new URL(baseUrl,lnk);
        System.out.println("Got URL \""+nurl+"\"");
        links.add(nurl);
      } catch (Exception e) {
        System.out.println("MyParserCallback.handleStartTag(): "+e);
      }
    }
    System.out.print("Tag: "+t+" attrs:");
    Enumeration e=a.getAttributeNames();
    while (e.hasMoreElements()) {
      Object key=e.nextElement();
      Object val=a.getAttribute(key);
      String keytype="("+key.getClass().getName()+")";
      String valtype="("+val.getClass().getName()+")";
      System.out.print(" "+keytype+key+"=\""+val+"\""+valtype);
    }
    System.out.print("\n");
  }
	
	public void handleEndTag(HTML.Tag t, int pos) {
	}
	
}

public class Browse {

  public static void show(Document doc) {
    Element[] es=doc.getRootElements();
    for (int i=0;i<es.length;i++) {
      System.out.print("RootElement: "+es[i]);
      show(es[i]," \\");
    }
  }
  public static void show(Element e) {
    show(e,"");
  }
  public static void show(Element e,String ind) {

      String simple="";

      int a=e.getStartOffset();
      int b=e.getEndOffset();
      if (e.isLeaf())
      try {
        simple=e.getDocument().getText(a,b-a);
      } catch (Exception ex) {
        System.out.println("show(): "+ex);
      }

//      String me=(""+e).trim();
      String me=e.getClass().getName();
      System.out.print(ind+me+" (\""+simple.trim()+"\") { attrs:");
      AttributeSet as=e.getAttributes();
//      System.out.print(as+" }\n");
      Enumeration en=as.getAttributeNames();
      while (en.hasMoreElements()) {
        Object key=en.nextElement();
        Object at=as.getAttribute(key);
        System.out.print(" "+key+"=\""+at+"\"");
      }
      System.out.print(" }\n");

      ind+="  ";

      for (int j=0;j<e.getElementCount();j++) {
        Element eb=e.getElement(j);
        show(eb,ind);
      }
      // ind-=;)
  }

  public final static void main(String[] args) {

    try {

//    String urlstr="file:///home/httpd/html/changetheworld/theindex.html";
    // String urlstr="http://hwi/";
    String urlstr=args[0];

    String page="";

    URL url=null;
    try {
      url=new URL(urlstr);
      Object o=url.getContent();
      System.out.println("Got "+o);
      InputStream b=(InputStream)o;
      StringBuffer s=new StringBuffer();
      while (b.available()>0)
        s.append((char)b.read());
      System.out.println(""+s);
      page=s.toString();
    } catch (Exception e) {
    }

    System.out.println("............");
    JEditorPane pane=new JEditorPane(urlstr);
    System.out.println("aaaaaaaaaaaaaaa");

    HTMLDocument doc=(HTMLDocument)pane.getDocument();
/*    HTMLDocument doctemp=new HTMLDocument();
    StyleSheet styles=doctmp.getStyleSheet();
    HTMLDocument doc=new HTMLDocument( */

    System.out.println("zzzzzzzzzz");

//    JEditorPane pane2=new JTextPane(doc);
//    JScrollPane scroll=new JScrollPane(pane);
    JFrame frame=new JFrame("Joey's Test 1");
    frame.getContentPane().add(pane);
    frame.setSize(400,400);
    frame.show();

    System.out.println("Trying parse...");
    MyParserCallback myparser=new MyParserCallback(url,page);
//    (new ParserDelegator()).parse(sr, , true);
    System.out.println("Parse done.");

//    Nuju.present(doc);

//    int l=doc.getLength();

//    System.out.println("The document ("+l+"): >"+doc.getText(0,doc.getLength())+"<");

//    show(doc);

//    System.out.println("Get text ("+10+"): >"+doc.getText(0,10)+"<");

/*    Element[] es=doc.getRootElements();
    for (int i=0;i<es.length;i++) {
      System.out.println(""+es[i]);
    }*/

    /* Gets no tags
    HTMLDocument.Iterator i=doc.getIterator(HTML.Tag.A);
    while (i.isValid()) {
      System.out.println("Hello =)");
      HTML.Tag h=i.getTag();
      System.out.println("Tag: "+h);
      i.next();
    } */

/*    JEditorPane pane=new JEditorPane(urlstr);

    HTMLDocument doc=(HTMLDocument)pane.getDocument();

    // Files.writeStringtofile(""+doc,"doc.txt");
    System.out.println("BaseURL for document is: "+doc.getBase());
    System.out.println("GetText: "+doc.getText(1,10)); */

 //   System.exit(0);

    } catch (Exception e) {

      System.out.println("main(): "+e);

    }

  }

}
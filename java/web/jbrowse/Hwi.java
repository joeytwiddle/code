import java.awt.*;
import java.awt.event.*;
import java.awt.Frame;
import java.awt.TextArea;
import java.awt.FlowLayout;
import java.awt.Dimension;
import javax.swing.JTable;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.table.TableColumn;
import javax.swing.JScrollPane;
import javax.swing.JEditorPane;
import java.lang.String;
import java.util.List;
import java.util.Vector;
import java.util.Enumeration;
import java.lang.String;
import java.net.URL;
import java.net.URLConnection;

import jlib.*;

public class Hwi extends Frame implements HyperlinkListener, CaretListener, MouseListener {

  URL url;
  TextField urltextfield;
  JEditorPane editpane;
  JScrollPane scrollpane;
  Label reportlabel;

  public Hwi(URL u) {
    url=u;
    start();
  }

  public static void main(String[] arg) {
    URL u=JNet.url(arg[0]);
    if (u==null)
      System.out.println("Couldn't resolve url "+arg[0]);
    else {
      Hwi h=new Hwi(u);
    }
  }

  void start() {
    try {
    setSize(500,500);
    setLayout(new FlowLayout());
      addWindowListener( new WindowAdapter() {
  	    public void windowClosing(WindowEvent e) {
  		    System.exit(0);
	      }
    	} );
      Label l=new Label("URL:");
    add(l);
      /* urltextfield=( new TextField(url.toString()) {
        protected void processTextEvent(TextEvent e) {
        }
      } );
      urltextfield.setColumns(50); */
      final Hwi h=this;
      urltextfield=( new SimpleTextField(url.toString(),70) {
        public void onEnter(String s) {
          h.newLocation(s);
        }
      } );
    add(urltextfield);
      editpane=new JEditorPane(url.toString());
    addEditPane();
    reportlabel=new Label("Starting....................................");
    // reportlabel.setSize(480,30);
    add(reportlabel);
    show();
    reportlabel.setText("Welcome.");
    analyseDocument(editpane.getDocument().getDefaultRootElement());
    } catch (Exception e) {
      System.out.println(""+e);
    }
  }

  public void addEditPane() {
      editpane.addHyperlinkListener(this);
      editpane.addCaretListener(this);
      editpane.addMouseListener(this);
      // editpane.setSize(500,460);
      scrollpane=new JScrollPane(editpane);
      scrollpane.setPreferredSize(new Dimension(460,400));
    add(scrollpane);
  }

  public void newLocation(String s) {
    URL u=JNet.url(s);
    String referer=urltextfield.getText();
    if (u!=null) {
      urltextfield.setText(u.toString());
      try {
        reportlabel.setText("Loading "+s+" ...");
        // editpane.setPage(u);
        // JNet.URLResponse r=new JNet.URLResponse(u,new JNet.Header(
        URLConnection con=u.openConnection();
        // con.setAllowUserInteraction(true);
        // con.setDefaultAllowUserInteraction(true);
        // con.setDoOutput(true);
        // con.setDoInput(true);
        con.setRequestProperty("referer",referer);
        // con.setDefaultRequestProperty("Accept","bugger/all");
        // con.setRequestProperty("Cookie","banasoup=heleleo");
        con.connect();
        // System.out.println("    : "+con.getRequestProperty("user-agent"));
        // System.out.println(" def: "+con.getDefaultRequestProperty("user-agent"));
        String content=JNet.streamFrom((java.io.InputStream)con.getContent());
        Files.writeStringtofile(content,"content.txt");
        reportlabel.setText("Loaded.  Giving to pane.");

        //editpane.setText(content);
        remove(scrollpane);
        getLayout().removeLayoutComponent(scrollpane);
        editpane=new JEditorPane("text/html",content);
        addEditPane();
        repaint();

        reportlabel.setText("Loaded.  Given to pane.");

        System.out.println("--- Headers for "+u);
        boolean done=false;
        for (int i=0;i<50 && !done;i++) {
          String k=con.getHeaderFieldKey(i);
          if (k==null && i>5)
            done=true;
          else
            System.out.println(k+": "+con.getHeaderField(k));
        }

        return;
      }
      catch (Exception e) {
        System.out.println(""+e);
      }
    }
    reportlabel.setText("Failed: "+s);
  }

  public void hyperlinkUpdate(HyperlinkEvent e) {
    System.out.println(""+e);
  }

  public void caretUpdate(CaretEvent e) {
    System.out.println(""+e);
    /*int start=JMaths.min(e.getDot(),e.getMark());
    int end=JMaths.max(e.getDot(),e.getMark());
    HTMLDocument doc=(HTMLDocument)editpane.getDocument();

    Element elc=doc.getCharacterElement(e.getMark());
    System.out.println("CharacterElement: "+elc);*/

/*    try {
      // String text=doc.getText(e.getMark(),e.getDot());
      // String text=editpane.getText().substring(start,end);
      String text=editpane.getSelectedText();
      System.out.println("Text: "+text);
      // String region=doc.getText(elc.getStartOffset(),elc.getEndOffset());
      String region=editpane.getText().substring(elc.getStartOffset(),elc.getEndOffset());
      System.out.println("Region: "+region);
    } catch (Exception ex) {
      System.out.println(""+ex);
    }*/

    /*try {
      AttributeSet as=elc.getAttributes();
      SimpleAttributeSet a=(SimpleAttributeSet)as.getAttribute(HTML.Tag.A);
      System.out.println("A: "+a);
      String loc=(String)a.getAttribute(HTML.Attribute.HREF);
      System.out.println("H: "+loc);
    } catch (Exception ex) {
      System.out.println(""+ex);
    }*/

      /*Enumeration names=as.getAttributeNames();
      while (names.hasMoreElements()) {
        Object o=names.nextElement();
        Object v=as.getAttribute(o);
        System.out.println("    Attr: "+o+" - "+v);
        System.out.println("      ( "+o.getClass().getName()+" , "+v.getClass().getName()+" )");
      }*/
    // analyseDocument(elc);

  }

  public void analyseDocument(Element el) {
    //System.out.println("ParagraphElement: "+el);
    List leaves=getLeafElements(el);
    System.out.println("Leaves:");
    for (int i=0;i<leaves.size();i++) {
      Element l=(Element)leaves.get(i);
      System.out.println("  "+l+" .name="+l.getName());
      AttributeSet a=l.getAttributes();
      Enumeration names=a.getAttributeNames();
      while (names.hasMoreElements()) {
        Object o=names.nextElement();
        Object v=a.getAttribute(o);
        System.out.println("    Attr: "+o+" - "+v);
        System.out.println("      ( "+o.getClass().getName()+" , "+v.getClass().getName()+" )");
      }
      Object lnk=a.getAttribute(HTML.Attribute.HREF);
    }
  }

  public void mouseClicked(MouseEvent e) {
    System.out.println(""+e);
    if (e.getClickCount()==2) {
      Caret c=editpane.getCaret();
      int start=JMaths.min(c.getDot(),c.getMark());
      int end=JMaths.max(c.getDot(),c.getMark());
      HTMLDocument doc=(HTMLDocument)editpane.getDocument();
      Element elc=doc.getParagraphElement(c.getDot());
      lookForLink(elc);
    }
  }
  public void mouseEntered(MouseEvent e) { }
  public void mouseExited(MouseEvent e) { }
  public void mousePressed(MouseEvent e) { }
  public void mouseReleased(MouseEvent e) { }

  void lookForLink(Element elc) {
      try {
        AttributeSet as=elc.getAttributes();
        SimpleAttributeSet a=(SimpleAttributeSet)as.getAttribute(HTML.Tag.A);
        // System.out.println("A: "+a);
        String loc=(String)a.getAttribute(HTML.Attribute.HREF);
        // System.out.println("H: "+loc);
        newLocation(JNet.url(url,loc).toString());
      } catch (Exception ex) {
        // System.out.println(""+ex);
      }
    for (int i=0;i<elc.getElementCount();i++)
      lookForLink(elc.getElement(i));
  }

  void addLeafElements(List l,Element e) {
    //if (e.isLeaf())
    //  l.add(e);
    //else {
      l.add(e);
      for (int i=0;i<e.getElementCount();i++)
        addLeafElements(l,e.getElement(i));
    //}
  }

  List getLeafElements(Element e) {
    List l=new Vector();
    addLeafElements(l,e);
    return l;
  }

}

import java.net.URL;
import java.util.List;
import java.util.Vector;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.text.html.parser.ParserDelegator;
import java.io.*;
import java.lang.*;
import java.util.*;
import java.net.*;

import jlib.Files;
import jlib.Profile;
import jlib.JString;
import jlib.JTypes;
import jlib.JList;
import jlib.JThread;
import jlib.JNet;

import Trawler;
import TrawletManager;

class MyParserCallback extends HTMLEditorKit.ParserCallback {

  Trawlet srcTrawlet;
  URL baseURL=null;
  /*List*/Vector/**/ links=new Vector();
  boolean copying;
  boolean opening;
  StringBuffer copy=new StringBuffer();
  String errors="";
  int ind=0;
  TrawletManager manager;
  int depth;
  String toCome="";

  MyParserCallback(TrawletManager m,Trawlet st,URL url,String page,int d,boolean c,boolean o) {
    super();
    manager=m;
    srcTrawlet=st;
    baseURL=url;
    depth=d;
    copying=c;
    opening=o;
    try {
      Reader sr=new StringReader(page);
      (new ParserDelegator()).parse(sr, this, true);
    } catch (Exception e) {
//      Trawler.echo("MyParserCallback(url,page): "+e);
    }
  }

  private void writeCopy(String s) {
    if (copying)
      copy.append(s);
  }

  public void handleComment(char[] data, int pos) {
    writeCopy(indent()+"<!-- "+data+" -->\n");
  }

	public void handleEndTag(HTML.Tag t, int pos) {
	  ind--;
	  if (t.equals(HTML.Tag.BODY) && errors.length()>0)
	    writeCopy("Trawler reports:<br>\n"+errors);
    String toDo="";
    if (t.toString().toLowerCase().equals("a")) {
      toDo=toCome;
      toCome="";
    }
	  writeCopy(indent()+"</"+t+">"+toDo+"\n");
	}
	
	public void handleError(String err, int pos) {
	  errors+=indent()+"Error: \""+err+"\"<br>\n";
	}
	
	public void handleSimpleTag(HTML.Tag t, MutableAttributeSet a, int pos) {
	  writeCopy(indent()+"<"+t+expand(a)+">\n");
	}
	
	public void handleStartTag(HTML.Tag t, MutableAttributeSet a, int pos) {
	  String tagout="<"+t+expand(a)+">";
    String d="";
    if (t.equals(HTML.Tag.A)) {
      try {
        String lnk=a.getAttribute(HTML.Attribute.HREF).toString();
        URL origurl=JNet.url(baseURL,lnk);

        if (origurl==null) {

          tagout="{"+lnk+"}";

        } else {

        if (opening)
          manager.goget(origurl,depth+1);
        else
          links.add(origurl);
        // if (manager.waiting.size()>0)

        if (manager.strat.localLink) {
          Trawlet tl=new Trawlet(origurl,null,-1);
          File f=manager.strat.fileName(tl).getAbsoluteFile();
          d+=f+"\n";
          URL lurl=JNet.url("file://"+f);
          d+=lurl+"\n";
          File srcf=manager.strat.fileName(srcTrawlet).getAbsoluteFile();
          d+=srcf+"\n";
          URL srclurl=JNet.url("file://"+srcf);
          d+=srclurl+"\n";
          tagout="<a href=\""+JNet.relative(srclurl,lurl)+"\">";
          toCome="&nbsp;(<a href=\""+origurl+"\">*</a>)";
        }

        }

      } catch (Exception e) {
        System.out.println("MyParserCallback.handleStartTag(): "+e+"\n"+d);
      }
    }
    // System.err.print(".");
	  writeCopy(indent()+tagout+"\n");
	  ind++;
  }
	
  public void handleText(char[] data, int pos) {
    writeCopy(indent()+data+"\n");
    JThread.sleep(1);
    System.err.print(".");
  }

  String indent() {
    return JString.repeat("  ",ind);
  }

	String expand(AttributeSet a) {
	  String s="";
	  if (a.getAttributeCount()==0)
	    return s;
	  Enumeration e=a.getAttributeNames();
	  while (e.hasMoreElements()) {
	    Object key=e.nextElement();
	    Object at=a.getAttribute(key);
	    s+=" "+key+"=\""+at+"\"";
	  }
	  return s;
	}
	
}


// package generation;

import jlib.JString;

import java.lang.String;
import javax.servlet.*;
import javax.servlet.http.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.Vector;
import java.util.*;
import java.text.SimpleDateFormat;
import java.lang.reflect.*;
import java.util.Hashtable;

import jlib.Log;
import jlib.db.*;
import jlib.multiui.*;

import appserver.*;

/** AppServer query response.
 *	Query can be:
 *		- Show me <page> with <parameter>s.
 *		- Regenerate <page> with <state> and call <component-action>.  (This could result in re-render of updated page, or a new page.)
 *		- Just generate <page> and render.
 *	Note: 1 occurs in Swing, 2 occurs but no page/state needed, just c-a, 3 does not occur in Swing.
**/

public class MainReal {

  private static final String baseURL="/generation/";
  
  Map parameters;
  PrintWriter out;
  
  public MainReal(Map p,PrintWriter o) {
    parameters=p;
    out=o;
  }
  
  public Field[] openFields(PageCreator o) {
    return o.getClass().getDeclaredFields();
  }
  
  // public void go(Map parameters,PrintWriter out) {
  public void go() {

    // out.println("hello");

    try {

      // Which application was requested?
      String application="generation";
      // Which page requested?
      String showpage=(String)parameters.get("page");

      String classname=application+".pages."+showpage;
      classname="filfirin."+showpage;
      Class c=Class.forName(classname);

      PageCreator page=(PageCreator)c.newInstance();

      Field[] fs=openFields(page);
      for (int i=0;i<fs.length;i++) {
        String name=fs[i].getName();
        Iterator iterator=parameters.keySet().iterator();
        while (iterator.hasNext()) {
          String key=(String)iterator.next();
          if (key.equals(name)) {
            // Match: drop parameter into page
            fs[i].set(page,parameters.get(key));
          }
        }
      } 

      // Read component states
      // ...


      // Execute required render

      // out.println("Rendering...");

      jlib.multiui.Page p=new Page();

      page.renderPage(p);

			State s=new State(page);
			for (int i=0;i<s.data.size();i++)
				p.println(i+": "+s.data.get(i));
			
      HTMLMedium m=new HTMLMedium();
      m.renderPage(p,out); 
      
    } catch (Exception ex) {
      out.println("Oh dear: "+ex);
    }
    
  }
  
}

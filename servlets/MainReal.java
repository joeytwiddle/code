// package generation;

import jlib.JString;

import java.lang.String;
import javax.servlet.*;
import javax.servlet.http.*;
import java.applet.*;
import java.util.Random;
import java.io.*;
import java.util.Vector;
import java.util.*;
import java.text.SimpleDateFormat;
import java.lang.reflect.*;
import java.util.Hashtable;

import jlib.*;
import jlib.db.*;
import jlib.multiui.*;

import appserver.*;

/** AppServer query response.
 *	Query can be:
 *		- Show me <page> with <parameter>s.
 *        newwindow=<pagename>&amp;<params>
 *		- On <page> with <state-changes>, call <component-action>.
 *											window=<windowref>
 *											value:<componentref>=<value>
 *											action:<componentref>=<whatever>
 *											expectvalue:<componentref>=<whatever> # needed for checkboxes which do not send anything if they are unchecked
 *			where ':' = Component.Delim
**/

class HtmlAppServer implements AppServer {
	MainReal mr;
	HtmlAppServer(MainReal m) {
		mr=m;
	}
	public void moveToPage(Page p) {
		mr.page=p;
		p.init();
	}
}

// class Instance {
	// public String ref=JString.random(5);
	// public boolean gone=false;
	// public String windowRef;
	// public Instance() { }
// }

public class MainReal {

  private static final String baseURL="/generation/";
  
	Session session;
  Map parameters;
  PrintWriter out;
  Page page=null;
			
  
  public MainReal(Session s,Map p,PrintWriter o) {
		session=s;
    parameters=p;
    out=o;
  }
  
  public Field[] openFields(Page o) {
    return o.getClass().getDeclaredFields();
  }

	public String[] getParamStarting(String s) {
		Object[] keys=parameters.keySet().toArray();
		for (int i=0;i<keys.length;i++) {
			String k=(String)keys[i];
			if (k.startsWith(s)) {
				String[] ans= { k , (String)parameters.get(k) };
				parameters.remove(k);
				return ans;
			}
		}
		return null;
	}

	public String getParam(String s) {
		Object[] keys=parameters.keySet().toArray();
		for (int i=0;i<keys.length;i++) {
			String k=(String)keys[i];
			if (k.equals(s)) {
				String ans=(String)parameters.get(k);
				parameters.remove(k);
				return ans;
			}
		}
		return null;
	}
  
  // public void go(Map parameters,PrintWriter out) {
  public void go() {

		// Log.clear();

    // String application="filfirin";
			
		// out.println("hello");

    try {

			// String[] newwindow=getParamStarting("newwindow"+Component.Delim);
			// String[] window=getParamStarting("window"+Component.Delim);

			// out.println("YO");

			String newwindow=getParam("newwindow");
			String window=getParam("window");
			String instance=getParam("instance");
			String[] action=getParamStarting("action"+Component.Delim);
			boolean doValues=true;

			if (instance!=null)
				if (session.instances.get(instance)!=null) {
					window=(String)session.instances.get(instance);
					action=null;
					doValues=false;
					out.println("Already visited instance "+instance+" and got page "+window);
				}
			
			if (newwindow!=null) {
			
		    String classname=/*application+"."+*/newwindow;
				// out.println("New window "+classname);
				try {
				  Class c=Class.forName(classname);
				  page=(Page)c.newInstance();
      	  page.init();
				} catch (Exception e) {
					out.println("Problem initiating page "+classname);
					Log.report("Problem initialiating page "+classname,e);
				}
				// session=new Session();
				
			} else if (window!=null) {
				
			  String windowref=window;
				// out.println("Retrieve window "+windowref);
				try {
				  page=session.getPage(windowref);
				  Log.report("Got window "+windowref); // +" = "+page);
					if (page==null)
						throw new Exception("Joey's own page == null exception");
				} catch (Exception e) {
					Log.error("Problem recovering window "+windowref,e);
				}

			  if (doValues) {
				  try {
			      
						Object[] keys=parameters.keySet().toArray();
					  
						// Check all expected valued (for checkboxes really)
			      for (int i=0;i<keys.length;i++) {
				      String key=(String)keys[i];
				      if (key.startsWith("expectvalue"+Component.Delim)) {
					      String componentref=JString.after(key,Component.Delim);
							  // Find actual value
							  boolean found=false;
							  for (int j=0;j<keys.length;j++) {
								  String key2=(String)keys[j];
								  if (key2.startsWith("value"+Component.Delim+componentref))
									  found=true;
							  }
							  if (!found)
								  ((UserEditableComponent)page.getComponent(componentref)).setValue(YesNoChoice.offVal);
						  }
					  }
					  
						// Write all provided values
			      for (int i=0;i<keys.length;i++) {
				      String key=(String)keys[i];
				      if (key.startsWith("value"+Component.Delim)) {
					      String componentref=JString.after(key,Component.Delim);
					      String value=(String)parameters.get(key);
					      Component c=page.getComponent(componentref);
					      // out.println("Reading "+value+" into "+c);
					      ((UserEditableComponent)c).setValue(value);
				      }
			      }
			    
					} catch (Exception e) {
					  Log.error(e,"Problem reading parameters into "+windowref);
			    }
				}

			  if (action!=null) {
				
				  String componentref=JString.after(action[0],Component.Delim);
			    try {
				    Component c=page.getComponent(componentref);
				    if (c==null) {
					    out.println("Could not find action component "+componentref+" in "+page);
				    } else {
							// c.setAppServer(new AppServer(this));
							c.appServer=new HtmlAppServer(this);
				      // out.println("Trying action on "+c);
				      // out.println("mode="+((filfirin.DB)page).mode);
							// this.page may change during execution of the following:
				      ((ActionComponent)c).action();
				      // out.println("mode="+((filfirin.DB)page).mode);
				    }
			    } catch (Exception e) {
				    out.println("Problem performing action "+componentref);
			    }
			  
				} else {
				  out.println("No action");
					page.init();
			  }

			}

			if (page!=null) {
			
			  // This is here because (at time of writing) a page of one ref may change state on an action
			  // making the old page redundant.  Redundant pages not yet sorted.
			  // To keep them active, we give the page a new name each time in case its state is changed.
			  // page.ref=JString.random(5); Above still true!
				// Well not we have instances.  Is it OK now?!
		  	
			  // session.remove(page);
		    // session.addPage(page);
		    session.putPage(page);
        if (session.instances.get(instance)==null)
					session.instances.put(instance,page.ref);
		    // out.println("Added page "+page.ref+" to session "+session+" "+session.pages.size());
          
				instance=JString.random(5);
			  try {
          // out.println("<form action=\"/servlets/Main?window="+page.ref+"\" method=\"post\">");
          out.println("<form action=\"/servlets/Main?instance="+instance+"\" method=\"post\">");
		      out.println("<input type=\"hidden\" name=\"window\" value=\""+page.ref+"\">");
		      HTMLMedium m=new HTMLMedium();
          m.renderPage(page,out);
        out.println("</form>");
			  } catch (Exception e) {
				  e.printStackTrace(out);
			  }

			}
    
    } catch (Exception ex) {
      out.println("Oh dear:");
			JLib.report(ex);
			ex.printStackTrace(out);
    }
    
	  out.print(JString.replace(Log.logiferror(),"\n","<br>\n"));

  }
  
}

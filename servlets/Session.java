import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;
import appserver.*;

public class Session {

	public String username;
	public List pages=new Vector();

	// For HTML specifically:
	public Map instances=new HashMap();

	public void addPage(Page page) {
    // State s=new State(page);
    // for (int i=0;i<s.data.size();i++)
      // page.println(i+": "+s.data.get(i));
		// Window w=new Window(page,new Vector());
		pages.add(page);
	}

	public void putPage(Page page) {
		pages.remove(page);
		pages.add(page);
	}

	public Page getPage(String ref) {
		for (int i=0;i<pages.size();i++) {
			Page page=(Page)pages.get(i);
			if (page.ref.equals(ref))
				return page;
		}
		Log.report(this,"Could not find page "+ref+" in "+pages);
		return null;
	}

}

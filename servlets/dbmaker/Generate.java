package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.multiui.*;
import jlib.db.*;

public class Generate extends Page {

	public static final void main(String[] args) {
		new nuju.Nuju(new Generate());
	}

	public static String classPath="/home/joey/j/code/java/servlets/";
	public static String dbmakerPath=classPath+"/dbmaker/";
	public static String dataPath=dbmakerPath+"/data/";

	public static String file2classPath(String file) {
		return JString.squeeze(JString.trim(JString.replace(JString.after(file,classPath),"/","."),"."),".");
	}

	private final Page page=this;

	public void init() {
		refresh();
	}

  public void refresh() {
    try {

			Log.report("hello");

			add(loadInitForm());

		} catch (Exception e) {
			Log.error(e);
		}

	}

  public Form loadInitForm() {
			Form form=new Form() {
			  public TextField nametxt=new TextField("Generation");
				public Button newButton=new Button("Load Init Class") {
				  public void action() {
					  page.clear();
						String srcClassName=nametxt.getValue();
						String path=dataPath+"init/"+srcClassName;
						String javaPath=file2classPath(path);
					  page.println(javaPath);
						try {
							Class c=Class.forName(javaPath);
							page.println("Loaded class "+c);
							page.add(new GenerateButton(c));
						} catch (Exception e) {
							Log.error(e);
						}
				  }
			  }; // This is dodgy because preRender happens every render time but components should be added once, and init() may not have been called.
				public void init() { // cannot be done in constructor - too early: both objects = null
					this.add(nametxt);
					this.add(newButton);
				}
			};
			form.init();
		return form;
	}

}

class GenerateButton extends Button {
				
	Class c;
	
	GenerateButton(Class cc) {
		super("Generate");
		c=cc;
	}
	
	public void action() {

		try {

		  // Generate proper Java classes
		  // Generate SQL DB
		  // Anything else needed to be known for DB r/w?
	  	
		  Field[] cfs=c.getFields();
		  for (int i=0;i<cfs.length;i++) {
			  Field cf=(Field)cfs[i];
			  Class cc=cf.getType();
		  	
				
				
			  Field[] fs=cc.getFields();
			  for (int j=0;j<fs.length;j++) {
				  Field f=fs[j];
				  boolean declared=f.getDeclaringClass().equals(cc);
					
			  }

		  }

		} catch (Exception e) {
			Log.error(e);
		}

	}
	
}


package filfirin;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.multiui.*;
import jlib.db.*;

public class DB extends Page {

	private static final String[] modes={"Displaying","Editing","Submitting","Deleting?","Deleting"};
	public static final int ViewMode=0;
	public static final int EditMode=1;
	public static final int SubmitMode=2;

  public String whichdb="generation";
  public String itemid="Philosopher:philno1";

  private int mode=ViewMode;
	private Table table;
	Text report=new Text("nothing");
	String tmp=ref;

	public void init() {
		refresh();
	}

  public void refresh() {
    try {
			comps.clear();
			println(modes[mode]+" item "+itemid+" from DB "+whichdb);
			println("Mode = "+mode);
			
      jlib.db.DB db=new jlib.db.DB();
      DBKey key=SimpleDBKey.fromString("(generation.objects)"+itemid);
      Object obj=db.get(key);
		  println("Got object "+obj);
      Field[] fs=obj.getClass().getFields();

			Table oldTable=table;
			table=new StreamedTable(2);
			
      for (int i=0;i<fs.length;i++) {
				if (i>0)
				  ((StreamedTable)table).newRow();	
				Field f=fs[i];
        String name=f.getName();
        boolean invisible=(
					   name.equals("password")
					|| name.equals("pass")
					|| name.equals("id")
				);
        Object value=f.get(obj);
				((StreamedTable)table).set(0,new Text(f.getName()));
				Component c=null;
				if (invisible)
					c=new Text("<hidden>");
				else {
					if (mode==ViewMode) {
						c=new Text("="+value);
				  } else if (mode==EditMode) {
						c=new TextField(f.getName(),""+value);
						c.ref=f.getName();
					} else if (mode==SubmitMode) {
						TextField prev=(TextField)oldTable.getComponent(f.getName());
						String nv=prev.content;
						c=new Text("\""+value+"\" -> \""+nv+"\"");
						f.set(obj,JTypes.fromString(nv,f.getType().getName()));
					}
				}
				((StreamedTable)table).set(1,c);
			}
			add(table);
			
			if (mode==ViewMode) {
				add( new Button("Edit") {
					public void action() {
						report.text=""+tmp;
						mode=EditMode;
						refresh();
					}
				} );
			} else if (mode==EditMode) {
				add( new Button("Submit") {
					public void action() {
						report.text=""+tmp;
						mode=SubmitMode;
						refresh();
					}
				} );
			} else if (mode==SubmitMode) {
			  db.writeBack((DBObj)obj);
				add( new Button("View") {
					public void action() {
						mode=ViewMode;
						refresh();
					}
				} );
			}

			add(report);

    } catch (Exception e) {
      println(this+": Exception "+e);
			JLib.report(e);
    }
  }

	void submit() {
		mode=SubmitMode;
		refresh();
	}

}

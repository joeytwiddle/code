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

/** Can be in state: show, edit, submit, delete, delete-confirm. **/

	public class EditForm extends Form {
		public void receive(Map map,Page p) {
			try {
      jlib.db.DB db=new jlib.db.DB();
      DBKey key=SimpleDBKey.fromString("(generation.objects)"+itemid);
      Object obj=db.get(key);
		  p.println("Was object "+obj);
      Field[] fs=obj.getClass().getFields();
			for (int i=0;i<fs.length;i++) {
				Field f=fs[i];
				Object o=map.get(f.getName());
				if (o!=null) {
					f.set(obj,o);
				}
			}
			p.println("Now object "+obj);
			mode=ViewMode;
			} catch (Exception e) {
			}
			renderPage(p);
		}
	};

public class DB extends PageCreator {

	private static final String[] modes={"Displaying","Editing","Submitting","Deleting?","Deleting"};
	public static final int ViewMode=0;
	public static final int EditMode=1;

  public String whichdb;
  public String itemid;
	public int mode=ViewMode;

	// public Table table;
	// It is not good to have table static, in case developer refers to it,
	// since it could be hacked about with by user.

	// Rather, in edit mode, the table becomes a form!

	public final Button editButton=new Button("Edit") {
		public void action() {
			mode=EditMode;
		}
	};

	public final Form editForm=new EditForm();

	public void edit() {
		mode=1;
	}

	public void submit() {
		mode=2;
		// How 2 get access to those dynamic text fields?
	}

  public void renderPage(Page p) {
    try {
			p.println(modes[mode]+" item "+itemid+" from DB "+whichdb);
			
      jlib.db.DB db=new jlib.db.DB();
      DBKey key=SimpleDBKey.fromString("(generation.objects)"+itemid);
      Object obj=db.get(key);
		  p.println("Got object "+obj);
      Field[] fs=obj.getClass().getFields();

			Table table=new StreamedTable(2);
			
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
					if (mode==0)
						c=new Text("="+value);
				  if (mode==1)
						c=new TextField(f.getName(),""+value);
				}
				((StreamedTable)table).set(1,c);
			}
			
			if (mode==EditMode) {
				p.add(editForm);
			} else {
				editForm.add(table);
				p.println("Editform = "+editForm);
				p.add(table);
			}

			switch (mode) {
				case 0:
					// p.add(new ParamChangeButton("Edit","mode","1"));
					// p.add(new Button("Edit",this.editPage()));
					p.add(editButton);
					break;
				// case 1:
					// p.add(new ParamChangeButton("Submit","mode","2"));
					// // p.add(new Button("Submit","submit"));
					// break;
			}

    } catch (Exception e) {
      p.println(this+": Exception "+e);
    }
  }

}

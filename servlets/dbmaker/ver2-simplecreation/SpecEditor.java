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

public class SpecEditor extends Page {

	public static String topdir="/home/joey/j/code/java/servlets/";
	public static String dbmakerdir=topdir+"dbmaker/";

	Spec spec;

	TextField spectxt;

	void loadSpec(String name) {
		try {
		  File f=new File(dbmakerdir,"specs/"+name+".spec");
		  FileInputStream fis=new FileInputStream(f);
		  ObjectInputStream ois=new ObjectInputStream(fis);
		  spec=(Spec)ois.readObject();
		} catch (Exception e) {
			Log.error(""+e);
		}
	}

	void saveSpec() {
		try {
			File f=new File(dbmakerdir,"specs/"+spec.name+".spec");
			FileOutputStream fos=new FileOutputStream(f);
			ObjectOutputStream oos=new ObjectOutputStream(fos);
			oos.writeObject(spec);
			oos.flush();
			fos.close();
		} catch (Exception e) {
			Log.error(""+e);
		}
	}

	// As you will see, the window is not very modular
	// It is constructed in realtime in the following function:

	public void init() {
		refresh();
	}

  public void refresh() {
    try {
			comps.clear();

			if (spec==null) {
				println("Creating new spec...");
				spec=new Spec("untitled");
			}

			// Unfortunately we must make spectxt a field of the Editor class in order for the anonymous Buttons to access it
			add(new Text("Viewing spec: "));
			spectxt=new TextField(spec.name);
			add(spectxt);
			add(new Button("Rename") {
				public void action() {
					spec.name=spectxt.getValue();
					// refresh();
				}
			} );
			add(new Button("Save") {
				public void action() {
					spec.name=spectxt.getValue();
					saveSpec();
					refresh();
				}
			} );
			add(new Button("Load") {
				public void action() {
					loadSpec(spectxt.getValue());
					refresh();
				}
			} );

			HorizontalSplit h=new HorizontalSplit();
			Container objects=new Container();
			Container tables=new Container();
			Container links=new Container();
			h.add(objects);
			h.add(tables);
			h.add(links);

			objects.println("Objects:");
			for (int i=0;i<spec.objects.size();i++) {
				Obj o=(Obj)spec.objects.get(i);
				objects.print(o.name);
				// Button editObj=new Button("Edit") {
					// public void action() {
					// 	
					// }
				// };
				Button editObj=new PopupWindowButton("Edit",new ObjEditor(o)); // How inefficient is this - I create a whole new Page, although I do not render it yet.
				objects.add(editObj);
				objects.nl();
			}
			// However, in this case we group the components together and can keep them local
			// The form really doesn't do much
			Form form=new Form() {
			  public TextField nametxt=new TextField("name");
				public Button newButton=new Button("New") {
				  public void action() {
					  spec.objects.add(new Obj(spec,nametxt.getValue()));
					  refresh();
				  }
			  }; // This is dodgy because preRender happens every render time but components should be added once, and init() may not have been called.
				public void init() { // cannot be done in constructor - too early: both objects = null
					this.add(nametxt); // this refers to the anonymous Form
					this.add(newButton); // I can't work out how to refer to Editor.add (if I needed to - I do have the right to!)  Actually being sent to either is fine (unless the objects.add(form) was on the other side of some other components)
				}
			};
			// objects.add(newButton);
			form.init();
			objects.add(form);

			tables.println("Tables:");

			links.println("Links:");

			add(h);

			add( new Button("Generate") {
				public void action() {
					spec.generate();
				}
			} );
			
    } catch (Exception e) {
      println(this+": Exception "+e);
			JLib.report(e);
    }
  }

}

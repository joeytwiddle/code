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

public class ObjEditor extends Page {

	Obj obj;

	public ObjEditor(Obj o) {
		obj=o;
	}

	public void init() {
		refresh();
	}

  public void refresh() {
    try {
			comps.clear();

			Form f=new Form() {
			  public TextField nametxt=new TextField(obj.name);
				public Button rename=new Button("Rename") {
				  public void action() {
					  obj.name=nametxt.getValue();
					  refresh();
				  }
				};
				public void init() {
					this.print("Viewing object: ");
					this.add(nametxt);
					this.add(rename);
					this.nl();
				}
			};
			f.init();
			add(f);

			Container properties=new Container();
			for (int i=0;i<obj.properties.size();i++) {
				Property p=(Property)obj.properties.get(i);
				properties.println(""+p);
			}
			add(properties);

			Form addproperty=new Form() {
				TextField name=new TextField("name");
				private String[] linkChoices=Property.choices;
				Choice c=new Choice(linkChoices);
				Choice c2=new Choice(obj.parentSpec.getTypeNames());
				Button b=new PopupWindowButton(">") {
					public Page destination() {
						// Pick up here
						Property property=Property.getNew(obj.parentSpec,name.getValue(),obj.parentSpec.getType(c2.getIndex()),c.getIndex());
						obj.properties.add(property);
						return new PropertyEditor(property);
					}
				};
				public void init() {
					this.print("Add property ");
					this.add(name);
					this.print(" of type ");
					this.add(c2);
					this.print(" with linkage ");
					this.add(c);
					this.add(b);
				}
			};
			addproperty.init();
			properties.add(addproperty);

    } catch (Exception e) {
      println(this+": Exception "+e);
			JLib.report(e);
    }
  }

}

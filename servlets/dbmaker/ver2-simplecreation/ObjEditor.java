package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.multiui.*;
import jlib.multiui.convenience.*;
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

					this.print(" extends ");
					for (int i=0;i<obj.extendsObjs.size();i++) {
						Obj o=(Obj)obj.extendsObjs.get(i);
						this.print(""+o.name);
						this.add(new DeleteButton("X",obj.extendsObjs,o) {
							public void action() {
								super.action();
								refresh();
							}
						} );
						this.print(" ");
					}
					this.add(new Choice(obj.parentSpec.getObjNamesAndnothing(),"nothing") {
						public void onChange() {
							obj.extendsObjs.add((Object)obj.parentSpec.objects.get(getIndex()-1));
							refresh();
						}
					} );
					this.add(new Button("Chng") { public void action() { refresh(); } });
					this.nl();
				}
			};
			f.init();
			add(f);

			Container properties=new Container();
			for (int i=0;i<obj.properties.size();i++) {
				Property p=(Property)obj.properties.get(i);
				properties.print(p.type.getName()+" "+p.name+" (relationship "+p.typeString()+") ");
				properties.add(new PopupWindowButton("Edit",new PropertyEditor(p)));
				class DeleteButton extends Button {
					Property pp;
					public DeleteButton(String n,Property pp2) {
						super(n);
						pp=pp2;
					}
					public void action() {
						obj.properties.remove(pp);
						refresh();
					}
				};
				DeleteButton b=new DeleteButton("Delete",p);
				properties.add(b);
				properties.nl();
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
						Property property=Property.getNew(obj.parentSpec,obj,name.getValue(),obj.parentSpec.getType(c2.getIndex()),c.getIndex());
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

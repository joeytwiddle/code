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

public class PropertyEditor extends Page {

	Property property;

	public PropertyEditor(Property p) {
		property=p;
	}

	public void init() {
		refresh();
	}

  public void refresh() {
    try {
			comps.clear();

			print("Viewing property: ");
		  add(new TextField(property.name) {
				public void setValue(String v) {
					property.name=v;
				}
			} );
			// print(" of type "+property.type);
			print(" of type ");
			add( new Choice(property.parentSpec.getTypeNames(),property.type.getName()) {
				public void onChange() {
					property.type=property.parentSpec.getType(getIndex());
				}
			} );
			nl();

			// Container name=new Container() {
			  // public TextField nametxt=new TextField(property.name);
				// public Button rename=new Button("Rename") {
				  // public void action() {
					  // property.name=nametxt.getValue();
					  // refresh();
				  // }
				// };
				// public void init() {
					// this.add(nametxt);
					// this.add(rename);
				// }
			// };
			// name.init();
			// add(name);

			YesNoChoice sep=new YesNoChoice("Store ID in the other table",property.seperateTable) {
				public void setBoolean(boolean b) {
					property.seperateTable=b;
				}
			};
			add(sep);
			
			YesNoChoice lb=new YesNoChoice("Link back in the other object",property.autoLinkBack) {
				public void setBoolean(boolean b) {
					property.autoLinkBack=b;
				}
			};
			add(lb);

			add(new Button("Submit changes"));
			
			// Container propertys=new Container();
			// for (int i=0;i<obj.propertys.size();i++) {
				// Property p=(Property)obj.propertys.get(i);
				// propertys.println(""+p);
			// }
			// add(propertys);
// 
			// Form addproperty=new Form() {
				// TextProperty name=new TextProperty("name");
				// private String[] choices=Property.choices;
				// Choice c=new Choice(choices);
				// Button b=new PopupWindowButton(">") {
					// public Page destination() {
						// Property f=new Property();
						// obj.propertys.add(f);
						// return new PropertyEditor(property);
					// }
				// };
				// public void init() {
					// this.add(new Text("Add property "));
					// this.add(name);
					// this.add(new Text(" of type "));
					// this.add(c);
					// this.add(b);
				// }
			// };
			// addproperty.init();
			// propertys.add(addproperty);

    } catch (Exception e) {
      println(this+": Exception "+e);
			JLib.report(e);
    }
  }

}

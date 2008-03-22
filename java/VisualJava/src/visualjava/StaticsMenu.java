package visualjava;

import javax.swing.JSeparator;

import org.neuralyte.Logger;
import org.neuralyte.common.swing.jmenus.DetachableJMenu;
import org.neuralyte.common.swing.jmenus.LazyJMenu;

/** joey Nov 2, 2004 2:11:30 AM */
public class StaticsMenu extends LazyJMenu {
    String className;
    StaticsMenu(String name, String _className) {
        super(name);
        className = _className;
    }
    public void generateChildren() {
        try {
        	add(DetachableJMenu.getDetacherFor(this));
        	add(new JSeparator());
        	// Logger.log("Generating statics from "+className+" for "+this);
        	MenuBuilder.addStaticsToMenu(this,Class.forName(className));
        } catch (Throwable e) { // I once got: "java.lang.NoClassDefFoundError: jlib/db/spec/old/DB (wrong name: jlib/db/spec/DB)" here.
        	org.neuralyte.Logger.error(e);
        }
    }
}

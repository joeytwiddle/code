package visualjava;

import org.neuralyte.common.swing.LazyJMenu;

/** joey Nov 2, 2004 2:11:30 AM */
public class StaticsMenu extends LazyJMenu {
    String className;
    StaticsMenu(String name, String _className) {
        super(name);
        className = _className;
    }
    public void generateChildren() {
        try {
            VisualJavaGUIStatics.addStaticsToMenu(this,Class.forName(className));
        } catch (Throwable e) { // I once got: "java.lang.NoClassDefFoundError: jlib/db/spec/old/DB (wrong name: jlib/db/spec/DB)" here.
            e.printStackTrace(System.err);
        }
    }
}

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
            VisualJavaGui.addStaticsToMenu(this,Class.forName(className));
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }
}

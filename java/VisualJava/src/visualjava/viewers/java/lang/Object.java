/*
 * Created on Jul 11, 2004
 *
 * To change the template for this generated file go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
package visualjava.viewers.java.lang;

import visualjava.AbstractViewer;

import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 * @author joey
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class Object extends AbstractViewer implements visualjava.Viewer {

	public Object(java.lang.Object _viewing) {
		viewing = _viewing;
	}

	public void refreshGui(JPanel panel) {
		super.refreshGui(panel);

	}

}

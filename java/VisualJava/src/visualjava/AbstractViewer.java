package visualjava;

import javax.swing.*;

/** joey Jul 11, 2004 6:00:18 AM */
public class AbstractViewer {

	protected Object viewing = null;

	public String getTitle() {
		return viewing.getClass().getName() + " \"" + viewing + "\"";
	}

	public void refreshGui(JPanel panel) {
		panel.add(new JLabel("Viewing " + viewing.getClass().getName() + " = \"" + viewing.toString() + "\""));
	}
}

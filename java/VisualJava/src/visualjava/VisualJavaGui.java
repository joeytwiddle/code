package visualjava;

import visualjava.Viewer;

import javax.swing.*;
import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.lang.reflect.Method;
import java.lang.reflect.Field;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

/** joey Jul 11, 2004 3:27:54 AM */
public class VisualJavaGui {

	public static void oldShowDeprecated(Object toShow) {
		java.lang.Class viewerType = VisualJava.viewerForObject(toShow);
		Viewer viewer = VisualJava.invokeViewer(viewerType,toShow);
		javax.swing.JFrame frame = new javax.swing.JFrame();
		javax.swing.JPanel panel = new javax.swing.JPanel();
		viewer.refreshGui(panel);
		frame.getContentPane().add(panel);
		frame.setSize(400,400);
		frame.setTitle(viewer.getTitle());
		frame.setVisible(true);
	}

}


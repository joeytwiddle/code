package visualjava;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.lang.reflect.Method;
import java.io.*;

/** joey Jul 11, 2004 3:26:31 AM */
public class VisualJava {

    public static Desktop desktop;

	public static void main(String[] args) {
		// visualjava.VisualJavaGui.oldShowDeprecated(new String("hello test"));
        JFrame f = new JFrame();
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setSize(600,400);
        desktop = new Desktop(f);
        f.setContentPane(desktop);
        f.setVisible(true);
        VisualJavaGUIStatics.addMenuBar(f);
        f.validate();
	}

	public static Class viewerForObject(Object toShow) {
		return viewerForType(toShow.getClass());
	}

	public static Class viewerForType(Class type) {
		Class testType = type;
		Class viewerType = null;
		while (viewerType == null && testType != null) {
			String test = "visualjava.viewers." + testType.getName();
			try {
				viewerType = Class.forName(test);
			} catch (Exception e) {
				// e.printStackTrace(System.err);
			}
			if (viewerType == null) {
				Class lastTestType = testType;
				testType = testType.getSuperclass();
				if (lastTestType == testType)
					break;
			}
		}
		return viewerType;
	}

	public static Viewer invokeViewer(Class viewerType,Object argument) {
		Class[] argTypes = { Object.class };
		java.lang.reflect.Constructor constructor = null;
		try {
			constructor = viewerType.getConstructor(argTypes);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		Object[] args = { argument };
		Viewer viewer = null;
		try {
			viewer = (Viewer)constructor.newInstance(args);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return viewer;
	}

}

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
        desktop.setSize(800,400);
        desktop.showObject("Hello");
        desktop.showObject(f);
        desktop.showObject(desktop);
        f.setContentPane(desktop);
        f.setVisible(true);
        VisualJavaGUIStatics.addMenuBar(f);
        f.validate();
	}

}

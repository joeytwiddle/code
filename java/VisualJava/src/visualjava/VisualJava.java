package visualjava;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.io.*;

/** joey Jul 11, 2004 3:26:31 AM */
public class VisualJava {

    public static Desktop desktop;

	public static void main(String[] args) {
		// visualjava.VisualJavaGui.oldShowDeprecated(new String("hello test"));
        final JFrame f = new JFrame();
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setSize(600,400);
        desktop = new Desktop(f);
        desktop.setSize(800,400);
        // f.setContentPane(desktop);
        f.getContentPane().setLayout(new BoxLayout(f.getContentPane(),BoxLayout.Y_AXIS));
        f.getContentPane().add(desktop);
        f.setVisible(true);
        desktop.showObject("Hello");
        desktop.showObject(f);
        desktop.showObject(desktop);
        try { desktop.displayMethod(Arrays.class.getMethod("asList",new Class[]{ new Object[0].getClass() }),null); }
        catch (Exception e) { }
        new Thread() { public void run() { VisualJavaGUIStatics.addMenuBar(f); } }.start();
        f.validate();
	}
	
	public static void showObject(Object obj) {
	    if (desktop == null) {
	        main(new String[0]);
	    }
	    desktop.showObject(obj);
	}

}

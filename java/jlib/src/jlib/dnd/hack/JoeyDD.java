package jlib.dnd.hack;

// javaJVMLocalObjectMimeType 

import java.awt.*;

import jlib.*;
import jlib.simple.*;
import jlib.dnd.hack.*;

/* Simplistic use of drag and drop to transport data between components in a single JVM.
 * Assumes only one mouse.  Does not interact with real DnD.
 * For any component c you wish to be a source for DnD, do DnDManager.setSource(c,Object o) where o is the information about the source which the target will receive.
 * Similarly, for a target component c do DnDManager.setTarget(c).
 * A target component must implement ComponentAcceptingDrop and hence provide a drop(Object) method to receive drops from DnD sources.
 */

class MyDropLabel extends Label implements ComponentAcceptingDrop {
	MyDropLabel(String s) {
		super(s);
	}
	public void drop(Object o) {
		System.out.println("MyDropLabel received:");
		System.out.println("    "+o);
	}
}

public class JoeyDD {

	public static void main(String[] argv) {
		Frame f=new Frame("D+D");
   	f.setSize(300,250);
		f.setLayout(new FlowLayout());
		
		Label src=new Label("Source");
		DnDManager.setSource(src,"The source object (a String)");
		f.add(src);
		
		MyDropLabel dest=new MyDropLabel("MyDropLabel");
		DnDManager.setTarget(dest);
		f.add(dest);
		
		Label normallabel=new Label("A normal Label");
		DnDManager.setTarget(normallabel,new Code() {
			public void execute() {
				System.out.print("Something was dropped on the normal Label:\n  "+input+"\n");
			}
		} );
		DnDManager.setSource(normallabel,"A source from the normal Label =)");
		f.add(normallabel);
		
		// f.add(new MySource(new CharArrayWriter()));
		// f.add(new MyDest());
		f.setVisible(true);
	}

}

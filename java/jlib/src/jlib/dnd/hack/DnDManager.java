package jlib.dnd.hack;

import java.awt.*;
import java.awt.dnd.*;
import java.awt.datatransfer.*;

import java.util.Hashtable;
import java.util.List;
import java.util.Iterator;

import java.io.*;
import java.io.IOException;

import javax.swing.JList;
import javax.swing.DefaultListModel;

import java.util.*;

import jlib.*;
import jlib.simple.*;


public class DnDManager {

	private static Object lastpickedup=null;

	private static Map code=new HashMap();

	// Making a component drag/drop-able

	/** Register any Component you wish to be a drag source here.
	 * <P>
	 * You must attach an Object which will be transferred to the target Component
	 * at the end of a drag-drop.
	 **/
	public static void setSource(Component c,Object o) {
		new MySource(c,o);
	}

	/** Register a ComponentAcceptingDrop as a drop target. **/
	public static void setTarget(ComponentAcceptingDrop c) {
		if (c instanceof Component)
			new MyDest((Component)c);
		else
			Log.warn("You must provide a Component as a drop target!");
	}

	/** Register any Component as a drop target, providing some Code which
	 * will be executed at the end of the drag-drop. **/
	public static void setTarget(java.awt.Component c,jlib.simple.Code cd) {
		code.put(c,cd); // Of course, rather than remember the c->cd link here, we could store cd alongside c in the MyDest
		new MyDest(c);
	}

	/** Internal - ignore. **/
	static void pickedUp(Object o) {
		lastpickedup=o;
	}

	/** Internal - ignore. **/
	static void droppedOn(Component c) {
		if (c instanceof ComponentAcceptingDrop)
			((ComponentAcceptingDrop)c).drop(lastpickedup);
		else {
			Code cd=(Code)code.get(c);
			cd.input=lastpickedup;
			cd.execute();
		}
	}

}



class MySource implements DragSourceListener, DragGestureListener {

	Object obj;
	DragSource dragSource;
	Component component;

	public MySource(Component c,Object o) {
		component=c;
		obj=o;
		dragSource=new DragSource();
		dragSource.createDefaultDragGestureRecognizer(component, DnDConstants.ACTION_MOVE, this);
	}

	public void dragExit(DragSourceEvent dse) { }
	public void dragOver(DragSourceDragEvent dsde) { }
	public void dragEnter(DragSourceDragEvent dsde) { }
	public void dragDropEnd(DragSourceDropEvent dsde) { }
	public void dropActionChanged(DragSourceDragEvent dsde) { }

	public void dragGestureRecognized(DragGestureEvent dge) {
		// causes exception: dragSource.startDrag (dge, DragSource.DefaultMoveDrop, null, this);
		dragSource.startDrag (dge, DragSource.DefaultMoveDrop, new StringSelection("dummy Transferable"), this);
		DnDManager.pickedUp(obj);
	}

}

/** Class used by DnDManager. **/

class MyDest implements DropTargetListener, DragGestureListener {

	DropTarget dropTarget;
	Component comp;

	public MyDest(Component c) {
		comp=(Component)c;
		dropTarget=new DropTarget(comp,this);
	}

	public void dragExit(DropTargetEvent dte) { }
	public void dragOver(DropTargetDragEvent dtde) { }
	public void dropActionChanged(DropTargetDragEvent dtde) { }
	public void dragGestureRecognized(DragGestureEvent dge) { }

	public void dragEnter(DropTargetDragEvent dtde) {
		dtde.acceptDrag(DnDConstants.ACTION_MOVE);
	}

	public void drop(DropTargetDropEvent dtde) {
		dtde.acceptDrop(DnDConstants.ACTION_MOVE);
		DnDManager.droppedOn(comp);
	}

}

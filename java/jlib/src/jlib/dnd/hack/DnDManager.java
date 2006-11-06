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

	// The hack: instead of passing objects using Java's weird interface (which supports exo-Java d+d).
	// we just keep a record of the last Object picked up (in JavaVM only - drags from outside jvm will
	// register last pickup with this system.).
	private static Object lastpickedup=null;

	// Map of Component -> Code to run when it is dropped on.
	private static Map code=new HashMap();

	// Map of registered source Component -> MySource
	private static Map registeredSrcs=new HashMap();

	// Making a component drag/drop-able

	/** You must use the DnDManager to register any Component you wish to be part of
	 *  DnD operation (either as a drag source or a drop destination).
	 * <P>
	 * You must attach an Object which will be transferred to the target Component
	 * at the end of a drag-drop.
	 * <P>
	 * If the state of the Component changes in such a way that it's attached Object
	 * should change, setSource should be called again, and will attach the new Object.
	 **/
	public static void setSource(Component c,Object o) {
		Object got=registeredSrcs.get(c);
		if ( got == null ) {
			registeredSrcs.put(c,new MySource(c,o));
		} else {
			((MySource)got).obj = o;
		}
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
		System.out.println("Dropping "+lastpickedup+" on "+c);
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
		dtde.getDropTargetContext().dropComplete(true);
	}

}

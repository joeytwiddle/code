package jlib.dnd.hack;

/** The component you wish to be a destination should implement this interface.  Furthermore, it should register itself with the DnDManager as a valid target. **/

public interface ComponentAcceptingDrop {
	public void drop(Object o); // Currently no info about the source of the drag, but that could be included within the passed Object.
}	

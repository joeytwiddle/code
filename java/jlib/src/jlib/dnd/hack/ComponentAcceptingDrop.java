package jlib.dnd.hack;

/** The component you wish to be a destination should implement this interface.  Furthermore, it should register itself with the DnDManager as a valid target. **/

public interface ComponentAcceptingDrop {
	
	/** The method which is executed when an Object is dropped on this Component.
	 * <P>
	 * Currently no info about the source of the drag is provided.
	 * If needed it should be provided with the passed Object.
	 **/
	public void drop(Object o);
}

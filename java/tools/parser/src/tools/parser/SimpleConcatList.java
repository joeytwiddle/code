package tools.parser;

import java.util.Vector;

/* Why not call it what it is?  A LiteralList. */

public class SimpleConcatList<T> extends Vector<T> implements ConcatList<T> {

	public SimpleConcatList<T> with(T item) {
		add(item);
		return this;
	}
	
}

package tools.parser;

import java.util.Vector;


public class SimpleConcatList<T> extends Vector<T> implements ConcatList<T> {

	public SimpleConcatList<T> with(T item) {
		add(item);
		return this;
	}
	
}

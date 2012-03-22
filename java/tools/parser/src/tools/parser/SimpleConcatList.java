package tools.parser;

import java.util.Vector;


public class SimpleConcatList<T> extends Vector implements ConcatList<T> {

	public ConcatList<T> with(T item) {
		add(item);
		return this;
	}
	
}

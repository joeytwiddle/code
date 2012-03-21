package tools.parser;

import java.util.Vector;


public class SimpleConcatList<T> extends Vector implements ConcatList<T> {

	ConcatList concat(T item) {
		add(item);
		return this;
	}
	
}

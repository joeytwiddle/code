package tools.parser;

import java.util.Vector;


public class ParseContext {

	private Grammar grammar;
	
	public Grammar getGrammar() {
		return grammar;
	}
	
	// Stats used always for standard failure reporting.
	public long closestToEnd = 9999999999999999l;
	public String closestFailure;
	public Vector<Type> path = new Vector<Type>();

}

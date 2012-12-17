package tools.parser;

import java.util.Vector;


public class ParseContext {

	private Grammar grammar;
	
	public ParseContext(Grammar grammar) {
		this.grammar = grammar;
	}
	
	public Grammar getGrammar() {
		return grammar;
	}
	
	// Stats used always for standard failure reporting.
	public long closestToEnd = 9999999999999999l;
	public String closestFailure;
	public Vector<Type> path = new Vector<Type>();

	// Grammar writers may use $1 $2 $3 even if some rule instances only have 1 match.
	// However they may disable if they don't want that flexibility.
	// Future: $_ or $* or $@ might be preferred.
	public boolean allowOutOfRangeArguments = true;

}

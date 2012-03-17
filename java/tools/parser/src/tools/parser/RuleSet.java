package tools.parser;

import java.lang.String;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Map;

public class RuleSet {

	String type;
	Vector<Vector<Type>> rules;
	Map replacements;

	RuleSet(String t) {
		this(t, new Vector<Vector<Type>>(), new Hashtable());
	}

	RuleSet(String t, Vector<Vector<Type>> rules, Map replacements) {
		type = t;
		this.rules = rules;
		this.replacements = replacements;
	}

	void add(Vector<Type> pattern) {
		rules.add(pattern);
	}
	/*
	 * void addreplacementtype(String target) { map.put(target,new Vector()); }
	 * void addreplacementbit(String target,Type t) { Vector rrs=map.get(target);
	 * rrs.add(t); }
	 */
}

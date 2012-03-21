package tools.parser;

import java.lang.String;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Map;

public class RuleSet {

	String type;
	Vector<Vector<Type>> rules;
	Map<String,Vector<Type>> replacements;

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

   public String toString() {
   	if (type != null && !type.equals("Anonymous")) {
   		return type;
   	} else {
   		StringBuffer out = new StringBuffer();
   		for (int i=0;i<rules.size();i++) {
   			Vector<Type> rule = rules.get(i);
   			for (int j=0;j<rule.size();j++) {
   				Type bit = rule.get(j);
   				out.append(bit.toString());
   				if (j < rule.size()-1) {
   					out.append(" ");
   				}
   			}
				if (i < rules.size()-1) {
					out.append(" | ");
				}
   		}
			return out.toString();
   	}
   }


}

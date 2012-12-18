package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.List;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Map;

/** Note that a RuleSet is not a set of many atom rules, like a whole
 * grammar, but just the rule definition for one atom.  Therefore it is
 * actually an OR rule (possibly with only one entry), containing many (or
 * one) AND clauses.
 *
 * Whether or not it has a name or replacements should be moved into a separate
 * class.
 *
 * TODO: These could be easier to follow if defined in their own classes, e.g.
 * MultiRule and SingleRule, or RuleBody and RuleClause.
 */

public class RuleSet {

	String type;
	List<List<Type>> rules;
	Map<String,List<Type>> replacements;

	public RuleSet(String t) {
		this(t, new Vector<List<Type>>());
	}

	public RuleSet(String t, List<List<Type>> rules) {
		this(t,rules,new Hashtable());
	}

	public RuleSet(String t, List<List<Type>> rules, Map replacements) {
		type = t;
		this.rules = rules;
		this.replacements = replacements;
	}

	// These are supposed to be a nice easy way for the grammar to initialise a ruleset and populate its list.
	// However they produce an inconsistency.  What if we want to pass a replacement map too?!
	// Recommendation: Separate the "rules" list into a separate class (e.g. RuleBody), so it can be instantiated separately and passed as one arg to RuleSet's constructor.
	// RuleSet could be renamed NamedRule (assuming only named rules have replacements) or AdvancedRule meaning a rule with replacements. 
	public RuleSet(String t, List<Type>... ls) {
		this(t, Arrays.asList(ls));
	}
	public RuleSet(List<Type>... ls) {
		this("Anonymous", ls);
	}
	
	void add(List<Type> pattern) {
		rules.add(pattern);
	}
	/*
	 * void addreplacementtype(String target) { map.put(target,new List()); }
	 * void addreplacementbit(String target,Type t) { List rrs=map.get(target);
	 * rrs.add(t); }
	 */

   public String toString() {
   	if (type != null && !type.equals("Anonymous")) {
   		return type;
   	} else {
   		StringBuffer out = new StringBuffer();
   		for (int i=0;i<rules.size();i++) {
   			List<Type> rule = rules.get(i);
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

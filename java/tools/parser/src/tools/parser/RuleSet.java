package tools.parser;

import java.lang.String;
import java.util.List;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Map;

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

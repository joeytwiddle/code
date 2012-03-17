package tools.parser;

import java.util.Vector;

import jlib.strings.SomeString;


public class GroupedDefn implements Type {

	private RuleSet ruleset;

   public GroupedDefn(RuleSet ruleset) {
   	this.ruleset = ruleset;
   }
   
	/** @deprecated **/
	public GroupedDefn(Vector<Type> justOneRule) {
		this.ruleset = new RuleSet("AnonymousRuleset");
		ruleset.rules.add(justOneRule);
   }

   /*
	public GroupedDefn(Vector<Vector<Type>> rules) {
		this.ruleset = new RuleSet("AnonymousRuleset");
		ruleset.rules = rules;
   }
   */

	@Override
   public Match match(SomeString s) {
		String type = "AnonymousType";
	   return Atom.matchAgainstRuleset(type, this, ruleset, s);
   }

	@Override
   public boolean replacementfor(Type o) {
	   // TODO Auto-generated method stub
	   return false;
   }

	
	
}

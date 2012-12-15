package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.Vector;

import tools.parser.*;
import tools.parser.extensions.*;

public class xmlGrm extends GrammarHelper {
  public static Grammar setupgrammar() {
    Grammar grammar = new Grammar();
    RuleSet ruleset;
    Vector<Type> rule;

    // Hmm interesting.  This could probably be a valid input.
    //
    // Although the vars do not have terminators, when they are followed immediately
    // by a single-char Text type, that char could be used as the implied
    // terminator.
    //
    // It might be possible for the grm grammar to seek and responds to this
    // pattern, so it needn't be a special case for the Parser.

    ruleset=new RuleSet("XML");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("type"," >"));
        rule.add(new Text(">"));
        rule.add(new Atom("Inside"));
        rule.add(new Text("</"));
        rule.add(new Var("type"," >"));
        rule.add(new Text(">"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("type"," >"));
        rule.add(new Atom("Param"));
        rule.add(new Text(">"));
        rule.add(new Atom("Inside"));
        rule.add(new Text("</"));
        rule.add(new Var("type"," >"));
        rule.add(new Text(">"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("type"," >"));
        rule.add(new Text("/>"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Param");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(" "));
        rule.add(new Var("param"," >"));
        rule.add(new Text("="));
        rule.add(new Var("value"," >"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Param"));
        rule.add(new Atom("Param"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Inside");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("XML"));
        rule.add(new Atom("Inside"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Var("text","<"));
      ruleset.add(rule);
    // Replacements


    return grammar;
  }
}

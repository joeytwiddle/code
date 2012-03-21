package tools.parser;

import java.lang.String;
import java.util.Vector;

import tools.parser.*;

public class marffGrm {
  public static void setupgrammar() {
    Grammar grammar = new Grammar();
    RuleSet ruleset;
    Vector<Type> rule;

    ruleset=new RuleSet("Main");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Definition"));
        rule.add(new Text("@data\n"));
        rule.add(new Atom("Data"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Definition");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DefBit"));
        rule.add(new Atom("Definition"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DefBit"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DefBit");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("@"));
        rule.add(new Atom("DefBit2"));
        rule.add(new Var("whatever","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
        rule.add(new Atom("DefBit2"));
    ruleset.replacements.put("java",rule);


    // That said: print whatever Comment or DefBit2 (whichever if matched) print,
    // but skip the "@", "\n" etc.

    ruleset=new RuleSet("Comment");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("CommentStart"));
        rule.add(new Var("comment","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("// "));
        rule.add(new Atom("CommentStart"));
        rule.add(new Var("comment"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("CommentStart");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("#"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("(*"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DefBit2");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DefRelation"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DefLink"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DefAttribute"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DefRelation");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("relation "));
        rule.add(new Var("relation","\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("	definitions.add(new Relation(\""));
        rule.add(new Var("relation"));
        rule.add(new Text("\");\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("DefLink");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("link "));
        rule.add(new Var("filename","\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("	definitions.add(new Link(\""));
        rule.add(new Var("filename"));
        rule.add(new Text("\");\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("DefAttribute");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("attribute "));
        rule.add(new Var("attrname"," "));
        rule.add(new Atom("OptKey"));
        rule.add(new Text(" "));
        rule.add(new Var("attrtype"," \n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("	definitions.add(new Attribute(\""));
        rule.add(new Var("attrname"));
        rule.add(new Text("\", ... ,\""));
        rule.add(new Var("attrtype"));
        rule.add(new Text("\");\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("OptKey");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(" key"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Data");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DataBit"));
        rule.add(new Atom("Data"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DataBit"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("DataBit");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Fields"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Fields");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Field"));
        rule.add(new Text(","));
        rule.add(new Atom("Fields"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Field"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements


    // Field = NumericalField | StringField

    ruleset=new RuleSet("Field");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("field",",\n"));
      ruleset.add(rule);
    // Replacements


  }
}

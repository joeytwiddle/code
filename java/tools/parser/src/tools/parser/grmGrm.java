package tools.parser;

import java.lang.String;
import java.util.Vector;

import tools.parser.*;

public class grmGrm {
  public static void setupgrammar() {
    Vector rulesets=Grammar.rulesets;
    RuleSet ruleset;
    Vector<Type> rule;

    // This grammar defines the file format for a grammar.
    // It can be used to parse .grm files.
    //
    // Export targets:
    //
    //   java - output the grammar as a Java RuleSet builder class for Parser.java
    //
    //   hugs - output the grammar in Haskell, for import by hwi.ath.cx's parser.hs
    //
    //   pojo (experimental and fugly) - create a set of bean-like classes to hold grammar atoms in Java
    //

    // TODO:
    //
    //   Allow easier definition of repeat and optional elements with + * and [ ... ]
    //   We could also create more advanced rules with e.g. ( ... ) | ...
    //
    //   The Var type is not very powerful.  An anonymous regexp match might be
    //   preferable.  e.g. /[$@_a-zA-Z][$@_a-zA-Z0-9]*/
    //
    //
    // CONSIDER:
    //
    //   Allow Comments without blank lines (inbetween other Atoms / almost anywhere)

    // SIGNIFICANT CHANGES:
    //
    //    2012-03-16 Exclude Vars now *fail* if 0 chars are matched.  Rules which
    //    previously found 0-length matches acceptable should be updated to accept
    //    them explicitly.
    //
    //    2012-03-15 Introduced Include Vars (VarAccept).
    //

    ruleset=new RuleSet("Main");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("GrammarDefn"));
      ruleset.add(rule);
    // Replacements


    // java: "package grammars;\n\n" GrammarDefn

    ruleset=new RuleSet("GrammarDefn");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Grm"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("package tools.parser;\n\nimport java.lang.String;\nimport java.util.Vector;\n\nimport tools.parser.*;\n\npublic class grmGrm {\n  public static void setupgrammar() {\n    Vector rulesets=Grammar.rulesets;\n    RuleSet ruleset;\n    Vector<Type> rule;\n\n"));
        rule.add(new Atom("Grm"));
        rule.add(new Text("  }\n}\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("module Grammar where\n\ndata Type = Atom String | Var String | Str String\n          | VarExcl String String\n  deriving (Eq)\n\ndata Match = No | Yes Type String [Match] String\n  deriving (Eq)\n\ntype RuleSet = [[Type]]\n\ntype Rule = ( Type , RuleSet , [Replacement] )\n\ntype Replacement = ( String , [Type])\n\n\nrules = [ "));
        rule.add(new Atom("Grm"));
        rule.add(new Text(" ]\n"));
    ruleset.replacements.put("hugs",rule);


    ruleset=new RuleSet("Grm");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmBit"));
        rule.add(new Atom("Grm"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmBit"));
      ruleset.add(rule);
    // Replacements


    // Grm = GrmBit+

    // TODO: We can try adding ! after Whitespace.  If we have matched some Whitespace then it can't be a comment or an AtomDef, so recursion back through here can quit easily.
    // No dude it won't ever test Comment or AtomDef if Whitespace matched, it returns on a whole match.  Therefore ! is only useful if it comes before the end of the line.

    ruleset=new RuleSet("GrmBit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Whitespace"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("AtomDef"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("Comment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("#"));
        rule.add(new Var("comment","\n"));
        rule.add(new Text("\n"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("#\n"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("    //"));
        rule.add(new Var("comment"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("    //"));
        rule.add(new Var("comment"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("pojo",rule);


    ruleset=new RuleSet("OptComment");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    // hugs: "-- " <comment> "\n"

    ruleset=new RuleSet("NL");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OptSpc");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("SpcBit"));
        rule.add(new Atom("OptSpc"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("SpcBit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(" "));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("\t"));
      ruleset.add(rule);
    // Replacements


    // Note that two AtomDefs without an empty line between them will not parse!

    // AtomName = <atomname/"^.<>\n\" =">
    ruleset=new RuleSet("AtomName");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("atomtype","^.<>\n\t\r \"+*()![]{}"));
      ruleset.add(rule);
    // Replacements

    // AtomName = <atomtype~"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_">

    ruleset=new RuleSet("AtomDef");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("AtomName"));
        rule.add(new Text(" = "));
        rule.add(new Atom("Defn"));
        rule.add(new Atom("OptReplacements"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("    ruleset=new RuleSet(\""));
        rule.add(new Atom("AtomName"));
        rule.add(new Text("\");\n      rulesets.add(ruleset);\n      rule=new Vector<Type>();\n"));
        rule.add(new Atom("Defn"));
        rule.add(new Text("      ruleset.add(rule);\n"));
        rule.add(new Atom("OptReplacements"));
        rule.add(new Text("\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("  ( Atom \""));
        rule.add(new Atom("AtomName"));
        rule.add(new Text("\",[\n    [ "));
        rule.add(new Atom("Defn"));
        rule.add(new Text(" ]\n    ] , [\n"));
        rule.add(new Atom("OptReplacements"));
        rule.add(new Text("\n  ] ) ,\n"));
    ruleset.replacements.put("hugs",rule);

    rule=new Vector<Type>();
        rule.add(new Text("interface "));
        rule.add(new Atom("AtomName"));
        rule.add(new Text(" {\n\n}\n  class AnImplmentation {\n"));
        rule.add(new Atom("Defn"));
        rule.add(new Text("  }\n\n"));
    ruleset.replacements.put("pojo",rule);


    // Replacements are the lines defining export targets: java, hugs, pojo

    ruleset=new RuleSet("OptReplacements");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\n"));
        rule.add(new Atom("Replacements"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("    // Replacements\n"));
        rule.add(new Atom("Replacements"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("Replacements");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ManyReplacements"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Replacement"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ManyReplacements");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Replacement"));
        rule.add(new Text("\n"));
        rule.add(new Atom("Replacements"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("Replacement"));
        rule.add(new Text(",\n"));
        rule.add(new Atom("Replacements"));
    ruleset.replacements.put("hugs",rule);


    // Note that use of + and * make it harder to place things *inbetween* the
    // elements during output (list join), so the above hugs output cannot currently
    // be easily reproduced with + or * as in the new rule below!

    // Replacements = ( Replacement "\n" )+
    // Replacements = Replacement ( "\n" Replacement )+ | ""

    ruleset=new RuleSet("Replacement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("target","<>\n\" :"));
        rule.add(new Text(": "));
        rule.add(new Atom("Defn"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("    rule=new Vector<Type>();\n"));
        rule.add(new Atom("Defn"));
        rule.add(new Text("    ruleset.replacements.put(\""));
        rule.add(new Var("target"));
        rule.add(new Text("\",rule);\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("      ( \""));
        rule.add(new Var("target"));
        rule.add(new Text("\" , [ "));
        rule.add(new Atom("Defn"));
        rule.add(new Text(" ] ) "));
    ruleset.replacements.put("hugs",rule);


    ruleset=new RuleSet("Defn");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DefnOr"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DefnAnd"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
      ruleset.add(rule);
    // Replacements


    // RelativeElement is only used for output, not matching.

    ruleset=new RuleSet("DefnBit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RelativeElement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("BasicElement"));
        rule.add(new Atom("OptRepeatMarker"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("BasicElement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Variable"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("ActiveReplacement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Text"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("GroupElement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("AtomRef"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Regexp"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("OptionalElement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("MagicTokenOfDoom"));
      ruleset.add(rule);
    // Replacements


    //              | RepeatElement

    // CONSIDER: Could be put = BasicElement RepeatMarker | ... at the top?
    // No, again that's inf recursive.  But we could try:
    // BasicElement = BasicElement2 RepeatMarker | BasicElement2

    // This might be called "exclusion" in Prolog.  *yawn*
    // If the magic token is reached, none of later options in a DefnBit will be attempted.
    // In other words, ! commits us to the current line, or failure.

    ruleset=new RuleSet("MagicTokenOfDoom");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("!"));
      ruleset.add(rule);
    // Replacements


    // This can make things more efficient, by allowing us to exclude searches that
    // we know are invalid.
    // But should it be a full commit?  I.e.:
    //   1) If rest of arguments fail, fall back out and try next in parent.
    //   2) If rest of arguments fail, report error and stop parsing!

    // I can envisage another token which might be useful.  Let's call it % for now.
    // It can be used to mark a force-fail match, e.g. in the following
    //   cat = % Dog
    //       | BlackCat | WhiteCat | GreyCat | OtherCat
    // % Dog means drop out immediately with failure if we match a Dog.  It can be
    // used a an heuristic in some situations, where we expect a Dog is much more
    // likely, so generally more efficient to check that than to check all the cats
    // first.

    // Perhaps ! should be renamed . so % can be renamed ! (since it basically means "not").

    ruleset=new RuleSet("GroupElement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("("));
        rule.add(new Atom("OWS"));
        rule.add(new Atom("Defn"));
        rule.add(new Atom("OWS"));
        rule.add(new Text(")"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.add( new GroupedDefn((RuleSet) new Runner(){ Object run(){\n          RuleSet ruleset = new RuleSet(\"Anonymous\");\n        Vector<Type> rule = new Vector<Type>();\n"));
        rule.add(new Atom("Defn"));
        rule.add(new Text("        ruleset.add(rule);\n          return ruleset;\n        } }.run() ) );\n"));
    ruleset.replacements.put("java",rule);


    // Too recursive I suspect:
    //RepeatElement = DefnBit "*"
    // Since RepeatElement was hard to define for "*" and "+", we parse repeat
    // through OptRepeatMarker, a single char after an atom match.

    ruleset=new RuleSet("OptionalElement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("["));
        rule.add(new Atom("OWS"));
        rule.add(new Atom("Defn"));
        rule.add(new Atom("OWS"));
        rule.add(new Text("]"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OptRepeatMarker");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RepeatMarker"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RepeatMarker");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RepeatMarker2"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),\""));
        rule.add(new Atom("RepeatMarker2"));
        rule.add(new Text("\"));\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("RepeatMarker2");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ZeroOrMore"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("OneOrMore"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ZeroOrMore");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("*"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("*"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Atom("0,-1"));
    ruleset.replacements.put("javaB",rule);


    ruleset=new RuleSet("OneOrMore");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("+"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("+"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Atom("1,-1"));
    ruleset.replacements.put("javaB",rule);


    ruleset=new RuleSet("ZeroOrOne");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("OptionalElement"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("0,1"));
    ruleset.replacements.put("javaB",rule);


    // javaB: pass min and max rather than symbols

    ruleset=new RuleSet("DefnOr");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Atom("Whitespace"));
        rule.add(new Text("|"));
        rule.add(new Atom("Whitespace"));
        rule.add(new Atom("Defn"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Text("      ruleset.add(rule);\n      rule=new Vector<Type>();\n"));
        rule.add(new Atom("Defn"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Text("] ,\n      [ "));
        rule.add(new Atom("Defn"));
    ruleset.replacements.put("hugs",rule);

    rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Text("  }\n  class AnotherImplementation {\n"));
        rule.add(new Atom("Defn"));
    ruleset.replacements.put("pojo",rule);


    ruleset=new RuleSet("DefnAnd");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Text(" "));
        rule.add(new Atom("Defn"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Atom("Defn"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Text(", "));
        rule.add(new Atom("Defn"));
    ruleset.replacements.put("hugs",rule);


    ruleset=new RuleSet("Variable");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Var"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("VarDeny"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("VarAccept"));
      ruleset.add(rule);
    // Replacements


    // I think Var is a VarReference - cannot be used for parsing (no terminal
    // condition!) but is used in replacements.

    // TODO: Perhaps make it explicit that Var and Re

    ruleset=new RuleSet("Var");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("varname","<>\n\"/ "));
        rule.add(new Text(">"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.add(new Var(\""));
        rule.add(new Var("varname"));
        rule.add(new Text("\"));\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("Var \""));
        rule.add(new Var("varname"));
        rule.add(new Text("\""));
    ruleset.replacements.put("hugs",rule);

    rule=new Vector<Type>();
        rule.add(new Text("    String "));
        rule.add(new Var("varname"));
        rule.add(new Text(";\n"));
    ruleset.replacements.put("pojo",rule);


    ruleset=new RuleSet("VarDeny");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("varname","<>\n\"/ ~"));
        rule.add(new Text("/\""));
        rule.add(new Var("denied","\""));
        rule.add(new Text("\">"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.add(new Var(\""));
        rule.add(new Var("varname"));
        rule.add(new Text("\",\""));
        rule.add(new Var("denied"));
        rule.add(new Text("\"));\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("VarExcl \""));
        rule.add(new Var("varname"));
        rule.add(new Text("\" \""));
        rule.add(new Var("denied"));
        rule.add(new Text("\""));
    ruleset.replacements.put("hugs",rule);

    rule=new Vector<Type>();
        rule.add(new Text("    String "));
        rule.add(new Var("varname"));
        rule.add(new Text(";\n"));
    ruleset.replacements.put("pojo",rule);


    ruleset=new RuleSet("VarAccept");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("varname","<>\n\"/ ~"));
        rule.add(new Text("~\""));
        rule.add(new Var("accepted","\""));
        rule.add(new Text("\">"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.add(new Var(\""));
        rule.add(new Var("varname"));
        rule.add(new Text("\",null,\""));
        rule.add(new Var("accepted"));
        rule.add(new Text("\"));\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("VarIncl \""));
        rule.add(new Var("varname"));
        rule.add(new Text("\" \""));
        rule.add(new Var("denied"));
        rule.add(new Text("\""));
    ruleset.replacements.put("hugs",rule);

    rule=new Vector<Type>();
        rule.add(new Text("    String "));
        rule.add(new Var("varname"));
        rule.add(new Text(";\n"));
    ruleset.replacements.put("pojo",rule);


    ruleset=new RuleSet("AtomRef");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("AtomName"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.add(new Atom(\""));
        rule.add(new Atom("AtomName"));
        rule.add(new Text("\"));\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("Atom \""));
        rule.add(new Atom("AtomName"));
        rule.add(new Text("\""));
    ruleset.replacements.put("hugs",rule);

    rule=new Vector<Type>();
        rule.add(new Text("    "));
        rule.add(new Atom("AtomName"));
        rule.add(new Text(" arg1;\n"));
    ruleset.replacements.put("pojo",rule);


    // pojo: "  /* Possible " <atomtype> " */\n"

    ruleset=new RuleSet("Text");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\""));
        rule.add(new Var("text","\""));
        rule.add(new Text("\""));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("\"\""));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.add(new Text(\""));
        rule.add(new Var("text"));
        rule.add(new Text("\"));\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("Str \""));
        rule.add(new Var("text"));
        rule.add(new Text("\""));
    ruleset.replacements.put("hugs",rule);

    rule=new Vector<Type>();
        rule.add(new Text("    /* Matched: \""));
        rule.add(new Var("text"));
        rule.add(new Text("\" */\n"));
    ruleset.replacements.put("pojo",rule);


    ruleset=new RuleSet("WhitespaceBit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\n"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(" "));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("\t"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("\r"));
      ruleset.add(rule);
    // Replacements


    // Whitespace = WhitespaceBit Whitespace
    //            | WhitespaceBit

    ruleset=new RuleSet("Whitespace");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("WhitespaceBit"));
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"+"));
      ruleset.add(rule);
    // Replacements

    // Whitespace = <whitespace~"\n \t\r">

    // Optional whitespace:

    // OWS = Whitespace
    //     | ""

    ruleset=new RuleSet("OWS");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("WhitespaceBit"));
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"*"));
      ruleset.add(rule);
    // Replacements




    // == Experimental Stuff ==

    // This is used in c.grm.  It allows for more complex output, but calling the
    // code inside `...`s with the match Object available as a local field.

    ruleset=new RuleSet("ActiveReplacement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("`"));
        rule.add(new Var("java","`"));
        rule.add(new Text("`"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("      rule.add( new ActiveReplacement() { public String replace() { "));
        rule.add(new Var("java"));
        rule.add(new Text(" } } );\n"));
    ruleset.replacements.put("java",rule);


    // I think RelativeElement might be a dodgy way to access variables in parent/child types.
    // ^SearchUp->SearchDown.DirectDown

    ruleset=new RuleSet("RelativeElement");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RelUp"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("RelDown"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RelUp");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Ref"));
        rule.add(new Text("^"));
        rule.add(new Atom("BasicElement"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        { Vector realrule=rule; rule=new Vector<Type>(); "));
        rule.add(new Atom("BasicElement"));
        rule.add(new Text(" realrule.add(new RelElement('^',"));
        rule.add(new Atom("Ref"));
        rule.add(new Text(",(Type)rule.get(0))); rule=realrule; }\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("RelDown");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Ref"));
        rule.add(new Text("."));
        rule.add(new Atom("BasicElement"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        { Vector realrule=rule; rule=new Vector<Type>(); "));
        rule.add(new Atom("BasicElement"));
        rule.add(new Text(" realrule.add(new RelElement('.',"));
        rule.add(new Atom("Ref"));
        rule.add(new Text(",(Type)rule.get(0))); rule=realrule; }\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("Ref");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("ref"," \"\n.^"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("\""));
        rule.add(new Var("ref"));
        rule.add(new Text("\""));
    ruleset.replacements.put("java",rule);




    // Untested...

    // TODO: Gah presumably we need to escape any "s in the output of RegexpBody
    ruleset=new RuleSet("Regexp");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("/"));
        rule.add(new Atom("RegexpBody"));
        rule.add(new Text("/"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("new Regexp(\""));
        rule.add(new Atom("RegexpBody"));
        rule.add(new Text("\")"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("RegexpBody");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RegexpBit"));
        rule.add(new Atom("RegexpBody"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RegexpBit");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\\"));
        rule.add(new Atom("AnyOneChar"));
        rule.add(new Atom("RegexpBit"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("["));
        rule.add(new Atom("RegexpCharBlock"));
        rule.add(new Text("]"));
        rule.add(new Atom("RegexpBit"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Var("regexpbit","\\/[\n"));
        rule.add(new Atom("RegexpBit"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RegexpCharBlock");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\\"));
        rule.add(new Atom("AnyOneChar"));
        rule.add(new Atom("RegexpCharBlock"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Var("regexpcharblock","]\n"));
        rule.add(new Atom("RegexpCharBlock"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements



    ruleset=new RuleSet("DummyTestRule");
      rulesets.add(ruleset);
      rule=new Vector<Type>();
        rule.add( new GroupedDefn((RuleSet) new Runner(){ Object run(){
          RuleSet ruleset = new RuleSet("Anonymous");
        Vector<Type> rule = new Vector<Type>();
        rule.add(new Atom("DummyTestContents"));
        ruleset.add(rule);
          return ruleset;
        } }.run() ) );
      ruleset.add(rule);
    // Replacements


  }
}

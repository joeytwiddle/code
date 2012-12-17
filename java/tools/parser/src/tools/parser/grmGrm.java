package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.Vector;

import tools.parser.*;
import tools.parser.extensions.*;

public class grmGrm extends GrammarHelper {
  public static Grammar setupgrammar() {
    Grammar grammar = new Grammar();
    RuleSet ruleset;
    Vector<Type> rule;

    // This grammar defines the file format for a grammar.
    // It can be used to parse .grm files.
    //
    // Export targets ("replacements"/output types - really need to standardise this term!):
    //
    //   java - output the grammar as a Java RuleSet builder class for Parser.java
    //
    //   hugs - output the grammar in Haskell, for import by hwi.ath.cx's parser.hs
    //
    //   pojo (experimental and fugly) - create a set of bean-like classes to hold grammar atoms in Java
    //

    // SIGNIFICANT CHANGES:
    //
    // 2012-03-16 Exclude Vars now *fail* if 0 chars are matched.  Rules which
    //            previously found 0-length matches acceptable should be updated to
    //            accept them explicitly.
    //
    // 2012-03-15 Introduced Include Vars (VarAccept).
    //
    // 2012-??-?? Allowed easier definition of repeat and optional elements with + * [ ... ] and ( ... )
    //            See OneOrMore ZeroOrMore OptionalElement and GroupElement

    // TODO:
    //
    //   The Var type is not very powerful.  An anonymous regexp match might be
    //   preferable.  e.g. /[$@_a-zA-Z][$@_a-zA-Z0-9]*/
    //
    //   On the other hand, the simplicity of our Vars may be a feature.
    //
    // Provide special marker _ or . in replacements which signify whatever
    // was matched.  Would allow:
    //   A = B | C
    //   out: "blah: " _
    // instead of:
    //   A = B | C
    //   out: "blah: " B C
    // which is sucky.
    // We could also provide $1, $2, $3 like Coffescript's, in which case $0 or $_
    // might be a preferable marker for "all".
    //
    // CONSIDER:
    //
    //   Done? Allow Comments without blank lines (inbetween other Atoms / almost anywhere)
    //   TODO? Allow Comments everywhere!  (Inbetween macros, at the end of rule lines, ...)
    //
    // CONSIDER: A special grammar rule type which can be given custom decision code
    // *inside* the grammar, e.g. $(...) where ... contains some text-matching code
    // (an implementation of the Java parser's Type.match() interface).
    //
    // TODO: Allow importing of other grammars.  So that, e.g. the Expression
    // grammar could be imported by many other grammars.  (It might also be nice if
    // we could extend the Expression grammar to e.g. CExpression and
    // JavaScriptExpression, so that we can handle minor differences, presumably by
    // overwriting existing atoms.)
    // Importing and extending would differ in that importing will place the grammar
    // rules into a namespace (so we might refer to JavaExpression.Expression),
    // whereas extending would bring the rules into the local namespace, so they may
    // be overridden.

    // CONSIDER: MagicTokenOfDoom could be more powerful than
    // MagicTokenOfEfficiency, in that the former would commit to the current line
    // not just for the current atom but for the whole parse.  So if the line does
    // not match after a MagicTokenOfDoom has been passed, we can display an error
    // "expected <NextType> but got <remaining_string>".
    //
    // Possible magic tokens:
    //   Taken: * + ( ) [ ] $ = # @
    //   Available: ! ^ % & ~ _ - /
    // ^ is light on the eye (except in Lucida), so may be suitable for the most common MagicTokenOfEfficiency.
    // But - _ and ~ are also quite light, and & is friendly and suggestive of continuation
    // ! and ^ are suitable for the DoNotMatch condition.
    // The association of ! with warnings makes it a good candidate for MagicTokenOfDoom.
    // We can also consider different meanings for &, && and &&&.
    //   !   = not
    //   !!  = MagicTokenOfEfficiency
    //   !!! = MagicTokenOfDoom

    // TODO: Do we want to demand spaces separate the elements of atom rules?  If
    // so, we should check that this is enforced by the grammar (it should be
    // tightened up), or risk people creating grammars without spaces!

    // Efficiency in parsers.  One great inefficiency in our existing system is that
    // often the same matches will be attempted on the same text many times.
    //
    // For example when matching Defn = DefnOr | DefnAnd | DefnBit against a single
    // item, three matches before DefnBit will be made, the first two failing after
    // the match because there are no extra arguments for DefnOr or DefnAnd.
    //
    // Obviously memoing can solve this, which we got for free when we were in
    // Haskell.  We could set up our own cache to catch common/likely repeats or all
    // repeats.  (E.g. it would not be too hard to detect possible repeat atoms in a
    // single ruleset, and decide to cache just them, but harder if the repeats are
    // in sub-atoms, though they could be caught by caching every match!)
    // Failed matches could also be cached.  Some failures are fast, some not.
    //
    // Alternatively we should structure our grammars in an ugly post-"fix-it"
    // style, e.g.
    //
    //   Defn = DefnBit OptDefnPostAndOr
    //
    // where OptDefnPostAndOr if matched would have to manage the existing
    // translation of DefnBit similarly to how * and [] do now.
    //
    // OptDefnPostAndOr = DefnPostOr | DefnPostAnd | ""
    // DefnPostOr = "|" Defn
    // DefnPostAnd = Defn

    ruleset=new RuleSet("Main");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("GrammarDefn"));
      ruleset.add(rule);
    // Replacements


    // java: "package grammars;\n\n" GrammarDefn

    ruleset=new RuleSet("GrammarDefn");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Grm"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("package tools.parser;\n\nimport java.lang.String;\nimport java.util.Arrays;\nimport java.util.Vector;\n\nimport tools.parser.*;\nimport tools.parser.extensions.*;\n\npublic class grmGrm extends GrammarHelper {\n  public static Grammar setupgrammar() {\n    Grammar grammar = new Grammar();\n    RuleSet ruleset;\n    Vector<Type> rule;\n\n"));
         rule.add(new Atom("Grm"));
         rule.add(new Text("    return grammar;\n  }\n}\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("module Grammar where\n\ndata Type = Atom String | Var String | Str String\n          | VarExcl String String\n  deriving (Eq)\n\ndata Match = No | Yes Type String [Match] String\n  deriving (Eq)\n\ntype RuleSet = [[Type]]\n\ntype Rule = ( Type , RuleSet , [Replacement] )\n\ntype Replacement = ( String , [Type])\n\n\nrules = [ "));
         rule.add(new Atom("Grm"));
         rule.add(new Text(" ]\n"));
    ruleset.replacements.put("hugs",rule);


    ruleset=new RuleSet("Grm");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmHeaderBit"));
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"*"));
        rule.add(new Atom("GrmBit"));
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"*"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("GrmHeaderBit");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmAt"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Whitespace"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("GrmAt");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmOption"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmImport"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmExtend"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("GrmOtherAt"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("GrmOption");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("@OPTION"));
        rule.add(new Atom("HorizSpace"));
        rule.add(new Var("optname"," \t\n\r"));
        rule.add(new Atom("HorizSpace"));
        rule.add(new Atom("OptOptionArgs"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("    grammar.setOption_"));
         rule.add(new Var("optname"));
         rule.add(new Text("("));
         rule.add(new Atom("OptOptionArgs"));
         rule.add(new Text(");\n"));
    ruleset.replacements.put("java",rule);

    //# Had trouble referring to grouped atom when OptOptionArgs was inline.
    // java: "    grammar.setOption_" <optname> "(" OptionArgs ");\n"
    //# Alternative, less hard-coded into grammar object.
    // java: "    grammar.setOption(\"" <optname> "\", { " OptionArgs " } );\n"

    ruleset=new RuleSet("OptOptionArgs");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add( new GroupedDefn((RuleSet) new Runner(){ Object run(){
          RuleSet ruleset = new RuleSet("Anonymous");
        Vector<Type> rule = new Vector<Type>();
        rule.add(new Atom("OptionArgs"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("HorizSpace"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
        ruleset.add(rule);
          return ruleset;
        } }.run() ) );
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OptionArgs");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("MultipleOptionArgs"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("OptionArg"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("MultipleOptionArgs");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("OptionArg"));
        rule.add(new Atom("HorizSpace"));
        rule.add(new Atom("OptionArgs"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("OptionArg"));
         rule.add(new Text(", "));
         rule.add(new Atom("OptionArgs"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("OptionArg");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("arg"," \t\n\r"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("GrmImport");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("@IMPORT"));
        rule.add(new Atom("HorizSpace"));
        rule.add(new Var("namespace"," \t\n\r"));
        rule.add(new Atom("HorizSpace"));
        rule.add(new Text("\""));
        rule.add(new Var("filename","\"\n"));
        rule.add(new Text("\""));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("    grammar.importNamespace(\""));
         rule.add(new Var("namespace"));
         rule.add(new Text("\",\""));
         rule.add(new Var("filename"));
         rule.add(new Text("\");\n"));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("GrmExtend");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("@EXTEND"));
        rule.add(new Atom("HorizSpace"));
        rule.add(new Text("\""));
        rule.add(new Var("filename","\"\n"));
        rule.add(new Text("\""));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("GrmOtherAt");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("@"));
        rule.add(new Var("option_name"," \n"));
        rule.add(new Atom("OptHorizSpace"));
        rule.add(new Var("rest","\n"));
        rule.add(new Atom("NL"));
      ruleset.add(rule);
    // Replacements
    //# For prototyping, for now, we allow grammar to insert any Java it likes.
    rule=new Vector<Type>();
        rule.add(new Text("    "));
         rule.add(new Var("rest"));
    ruleset.replacements.put("java",rule);


    // TODO: We can try adding ! after Whitespace.  If we have matched some Whitespace then it can't be a comment or an AtomDef, so recursion back through here can quit easily.
    // No dude it won't ever test Comment or AtomDef if Whitespace matched, it returns on a whole match.  Therefore ! is only useful if it comes before the end of the line.

    ruleset=new RuleSet("GrmBit");
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    // hugs: "-- " <comment> "\n"

    ruleset=new RuleSet("AtomName");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("PackagedAtomName"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("PlainAtomName"));
      ruleset.add(rule);
    // Replacements


    //# WARNING: Although I want to use "." to refer to modular rules, it is already used for RelDown!
    ruleset=new RuleSet("PackagedAtomName");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("PackageName"));
        rule.add(new Text("."));
        rule.add(new Atom("PlainAtomName"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("PlainAtomName");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Identifier"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("PackageName");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Identifier"));
      ruleset.add(rule);
    // Replacements


    // Identifier = <identifier/"^.<>\n\" =">
    // Identifier = <identifier/"^<>\n\t\r \"+*()![]{}">
    ruleset=new RuleSet("Identifier");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("identifier",null,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"));
      ruleset.add(rule);
    // Replacements


    // Note that two AtomDefs without an empty line between them will not parse!

    ruleset=new RuleSet("AtomDef");
      grammar.addRuleset(ruleset);
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
         rule.add(new Text("\");\n      grammar.addRuleset(ruleset);\n      rule=new Vector<Type>();\n"));
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
         rule.add(new Text(" {\n\n}\n  class AnImplementation implements "));
         rule.add(new Atom("AtomName"));
         rule.add(new Text(" {\n"));
         rule.add(new Atom("Defn"));
         rule.add(new Text("  }\n\n"));
    ruleset.replacements.put("pojo",rule);

    // BUG: We cannot make a second reference to AtomName

    // Replacements are the lines defining export targets: java, hugs, pojo

    ruleset=new RuleSet("OptReplacements");
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ManyReplacements"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Comment"));
        rule.add(new Atom("Replacements"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Replacement"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ManyReplacements");
      grammar.addRuleset(ruleset);
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
    // Really?  Doesn't the "\n" just get output again literally?

    // Replacements = ( Replacement "\n" )+
    // Replacements = Replacement ( "\n" Replacement )+ | ""

    ruleset=new RuleSet("Replacement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("target","<>\n\" :"));
        rule.add(new Text(": "));
        rule.add(new Atom("ReplacementDefn"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("    rule=new Vector<Type>();\n"));
         rule.add(new Atom("ReplacementDefn"));
         rule.add(new Text("    ruleset.replacements.put(\""));
         rule.add(new Var("target"));
         rule.add(new Text("\",rule);\n"));
    ruleset.replacements.put("java",rule);

    rule=new Vector<Type>();
        rule.add(new Text("      ( \""));
         rule.add(new Var("target"));
         rule.add(new Text("\" , [ "));
         rule.add(new Atom("ReplacementDefn"));
         rule.add(new Text(" ] ) "));
    ruleset.replacements.put("hugs",rule);


    ruleset=new RuleSet("Defn");
      grammar.addRuleset(ruleset);
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


    ruleset=new RuleSet("DefnOr");
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("DefnBit"));
        rule.add(new Text(" "));
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"+"));
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


    // ReplacementElement is only used for output/replacement lines, not the rule definition.
    // However at the moment we have included it in DefnBit.
    // Perhaps we should create DefnBit and OutDefnBit, the latter containing those
    // few extra rules which can only be used for outputs.
    // Yes we should.

    // Efficient
    // DefnBit = BasicElement OptRepeatMarker

    // Inefficient but atomic output
    ruleset=new RuleSet("DefnBit");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RepeatedElement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("BasicElement"));
      ruleset.add(rule);
    // Replacements


    // Would-be best-of-both but not yet supported
    // DefnBit = BasicElement OptRepeatMarker

    ruleset=new RuleSet("BasicElement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("VarAccept"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("VarDeny"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Text"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("GroupElement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("MagicSymbol"));
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
    // Replacements


    ruleset=new RuleSet("ReplacementDefn");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add( new GroupedDefn((RuleSet) new Runner(){ Object run(){
          RuleSet ruleset = new RuleSet("Anonymous");
        Vector<Type> rule = new Vector<Type>();
        rule.add(new Atom("OptSpc"));
        rule.add(new Atom("ReplacementElement"));
        rule.add(new Atom("OptSpc"));
        ruleset.add(rule);
          return ruleset;
        } }.run() ) );
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"*"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ReplacementElement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ArgReplacement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("RelativeElement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("ActiveReplacement"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("VarRef"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("Text"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("AtomRef"));
      ruleset.add(rule);
    // Replacements


    // CONSIDER: Could we put = BasicElement RepeatMarker | ... at the top?
    // No, again that's inf recursive.  But we could try:
    // BasicElement = BasicElement2 RepeatMarker | BasicElement2

    // If the magic token is reached, none of later options in a DefnBit will be attempted.
    // In other words, ! commits us to the current line, or failure.
    // Prolog has something similar called "cut", but that prevents backtracking to
    // the left of it, whilst allowing re-attempts on elements to its right.

    // Prolog's cut affects search strategy (excluding some parts of the tree), but
    // my current concern is in producing parse errors as opposed to parse failures.
    // The difference sounds like we need to "commit" to a rule, say "if we get here
    // then we must not fail".

    ruleset=new RuleSet("MagicSymbol");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("MagicTokenOfDoom"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("%"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("$"));
      ruleset.add(rule);
    // Replacements

    // Don't put "#" here - it may eat through comments!

    ruleset=new RuleSet("MagicTokenOfDoom");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("!"));
      ruleset.add(rule);
    // Replacements


    // Token "." or ";" could mean "definite".  If reached, it indicates everything
    // before it has been parsed correctly, and there should be no alternative to
    // what has been parsed.  If the parser then decides to reject this match, i.e.
    // fall back and try something else earlier, the symbol can cause an exit with
    // an error.  This would be difficult to use correctly on low-level elements,
    // but should be possible on the top layer.

    // This can make things more efficient, by allowing us to exclude searches that
    // we know are invalid.
    // But should it be a full commit?  I.e.:
    //   1) If rest of arguments fail, fall back out and try next in parent.
    //   2) If rest of arguments fail, report error and stop parsing!

    // Alternatively, "=!" could mean "=" but if a match is found, it should never
    // be un-done!  It is simpler but knows only itself, not the context in which it
    // was included.

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
      grammar.addRuleset(ruleset);
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


    ruleset=new RuleSet("RepeatedElement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("BasicElement"));
        rule.add(new Atom("RepeatMarker"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("BasicElement"));
         rule.add(new Text("        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),\""));
         rule.add(new Atom("RepeatMarker"));
         rule.add(new Text("\"));\n"));
    ruleset.replacements.put("java",rule);

    // java: "        rule.add( new RepeatedRule(" BasicElement ") );\n"

    ruleset=new RuleSet("OptionalElement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("["));
        rule.add(new Atom("OWS"));
        rule.add(new Atom("Defn"));
        rule.add(new Atom("OWS"));
        rule.add(new Text("]"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Atom("Defn"));
         rule.add(new Text("        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),0,1));\n"));
    ruleset.replacements.put("java",rule);

    // java: "        rule.add(new RepeatedRule(" Defn ",0,1));\n"

    ruleset=new RuleSet("RepeatMarker");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ZeroOrMore"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("OneOrMore"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OptRepeatMarker");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RepeatMarkerOld"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RepeatMarkerOld");
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("ZeroOrMore"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("OneOrMore"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("ZeroOrMore");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("*"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("*"));
    ruleset.replacements.put("java",rule);

    // javaB: 0,-1

    ruleset=new RuleSet("OneOrMore");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("+"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("+"));
    ruleset.replacements.put("java",rule);

    // javaB: 1,-1

    //# I am not sure whether to include ZeroOrOne in the grammar, since
    //# OptionalElement already performs this function.
    //# E.g. [X] achieves what X? would.
    // ZeroOrOne = "?"
    // java: "?"
    // javaB: 0,1

    // javaB: unused example of passing min and max rather than symbols

    // I think Var is a VarReference - cannot be used for parsing (no terminal
    // condition!) but is used in replacements.

    ruleset=new RuleSet("VarRef");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("<"));
        rule.add(new Var("varname","<>\n\"/ ~"));
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
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
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

    // CONSIDER: To avoid this stupid \" marker, we could ensure \" is interpreted
    // by the Text reader.
    //
    // We could also offer TextApos, which would be a '...' string instead of "..."
    //
    // Note that TextApos would need a different reader setup (if we adopt the same
    // as JS and sh, that "s can exist inside a '...' but we then must escape \')
    //
    // Introduction of '...' seems unneccessary, and could be saved for something
    // special!  But replacement of \" with \" seems desirable.

    ruleset=new RuleSet("Text");
      grammar.addRuleset(ruleset);
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




    ruleset=new RuleSet("NL");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("\r\n"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("\n"));
      ruleset.add(rule);
    // Replacements


    // HorizSpace = " " | "\t"
    ruleset=new RuleSet("HorizSpace");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("horizspace",null," \t"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OptHorizSpace");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("HorizSpace"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("OptSpc");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("SpcBit"));
        rule.add(new Atom("OptSpc"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("SpcBit");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text(" "));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text("\t"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("WhitespaceBit");
      grammar.addRuleset(ruleset);
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

    // Whitespace = WhitespaceBit+
    ruleset=new RuleSet("Whitespace");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("whitespace",null," \n\t\r"));
      ruleset.add(rule);
    // Replacements


    // Optional whitespace:

    // OWS = Whitespace
    //     | ""

    ruleset=new RuleSet("OWS");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("WhitespaceBit"));
        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),"*"));
      ruleset.add(rule);
    // Replacements




    // == Experimental Stuff ==

    // This is used in c.grm.  It allows for more complex output, but calling the
    // code inside `...`s with the match Object available as a local field.

    ruleset=new RuleSet("ActiveReplacement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("`"));
        rule.add(new Var("java","`"));
        rule.add(new Text("`"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("      rule.add( new ActiveReplacement() { public String replace(OutputContext ctx) { "));
         rule.add(new Var("java"));
         rule.add(new Text(" } } );\n"));
    ruleset.replacements.put("java",rule);


    // RelativeElement is a way to access variables in parent/child types.
    // ^SearchUp->SearchDown.DirectDown

    ruleset=new RuleSet("RelativeElement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RelUp"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Atom("RelDown"));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RelUp");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Ref"));
        rule.add(new Text("^"));
        rule.add(new Atom("ReplacementElement"));
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
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("Ref"));
        rule.add(new Text("."));
        rule.add(new Atom("ReplacementElement"));
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
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Var("ref"," \"\n.^"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("\""));
         rule.add(new Var("ref"));
         rule.add(new Text("\""));
    ruleset.replacements.put("java",rule);


    ruleset=new RuleSet("ArgReplacement");
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Text("$"));
        rule.add(new Var("num",null,"0123456789_"));
      ruleset.add(rule);
    // Replacements
    rule=new Vector<Type>();
        rule.add(new Text("        rule.add( new ArgReplacement("));
         rule.add(new Var("num"));
         rule.add(new Text(") );\n"));
    ruleset.replacements.put("java",rule);




    // Untested...

    // TODO: Gah presumably we need to escape any "s in the output of RegexpBody
    ruleset=new RuleSet("Regexp");
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
      rule=new Vector<Type>();
        rule.add(new Atom("RegexpBit"));
        rule.add(new Atom("RegexpBody"));
      ruleset.add(rule);
      rule=new Vector<Type>();
        rule.add(new Text(""));
      ruleset.add(rule);
    // Replacements


    ruleset=new RuleSet("RegexpBit");
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
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
      grammar.addRuleset(ruleset);
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


    return grammar;
  }
}

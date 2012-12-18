package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.Vector;
import org.neuralyte.literal.LiteralMap;

import tools.parser.*;
import tools.parser.extensions.*;

public class grmGrm extends GrammarHelper {
  public static Grammar setupgrammar() {
    Grammar grammar = new Grammar( new LiteralMap()    // This grammar defines the file format for a grammar.
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

    .with("Main", new RuleSet("Main", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("GrammarDefn") } )
      )
    ))

    // java: "package grammars;\n\n" GrammarDefn

    .with("GrammarDefn", new RuleSet("GrammarDefn", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Grm") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("package tools.parser;\n\nimport java.lang.String;\nimport java.util.Arrays;\nimport java.util.Vector;\n\nimport tools.parser.*;\nimport tools.parser.extensions.*;\n\npublic class grmGrm extends GrammarHelper {\n  public static Grammar setupgrammar() {\n    Grammar grammar = new Grammar();\n    RuleSet ruleset;\n    Vector<Type> rule;\n\n") , new Atom("Grm") , new Text("    return grammar;\n  }\n}\n")))
        .with("hugs", Arrays.asList(new Text("module Grammar where\n\ndata Type = Atom String | Var String | Str String\n          | VarExcl String String\n  deriving (Eq)\n\ndata Match = No | Yes Type String [Match] String\n  deriving (Eq)\n\ntype RuleSet = [[Type]]\n\ntype Rule = ( Type , RuleSet , [Replacement] )\n\ntype Replacement = ( String , [Type])\n\n\nrules = [ ") , new Atom("Grm") , new Text(" ]\n")))
        .with("javadecl", Arrays.asList(new Text("package tools.parser;\n\nimport java.lang.String;\nimport java.util.Arrays;\nimport java.util.Vector;\nimport org.neuralyte.literal.LiteralMap;\n\nimport tools.parser.*;\nimport tools.parser.extensions.*;\n\npublic class grmGrm extends GrammarHelper {\n  public static Grammar setupgrammar() {\n    Grammar grammar = new Grammar( new LiteralMap()") , new Atom("Grm") , new Text("\n    );\n    return grammar;\n  }\n}\n")))
    ))

    .with("Grm", new RuleSet("Grm", Arrays.asList(
        Arrays.asList( new Type[]{ new RepeatedRule(new Atom("GrmHeaderBit"),"*"), new RepeatedRule(new Atom("GrmBit"),"*") } )
      )
    ))

    .with("GrmHeaderBit", new RuleSet("GrmHeaderBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("GrmAt") } ),
        Arrays.asList( new Type[]{ new Atom("Whitespace") } ),
        Arrays.asList( new Type[]{ new Atom("Comment") } )
      )
    ))

    .with("GrmAt", new RuleSet("GrmAt", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("GrmOption") } ),
        Arrays.asList( new Type[]{ new Atom("GrmImport") } ),
        Arrays.asList( new Type[]{ new Atom("GrmExtend") } ),
        Arrays.asList( new Type[]{ new Atom("GrmOtherAt") } )
      )
    ))

    .with("GrmOption", new RuleSet("GrmOption", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("@OPTION"), new Atom("HorizSpace"), new Var("optname"," \t\n\r"), new Atom("HorizSpace"), new Atom("OptOptionArgs"), new Atom("NL") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("    grammar.setOption_") , new Var("optname") , new Text("(") , new Atom("OptOptionArgs") , new Text(");\n")))
    //# Had trouble referring to grouped atom when OptOptionArgs was inline.
    // java: "    grammar.setOption_" <optname> "(" OptionArgs ");\n"
    //# Alternative, less hard-coded into grammar object.
    // java: "    grammar.setOption(\"" <optname> "\", { " OptionArgs " } );\n"
        .with("javadecl", Arrays.asList(new Text("    // @todo grammar.setOption_") , new Var("optname") , new Text("(") , new Atom("OptOptionArgs") , new Text(");\n")))
    ))

    .with("OptOptionArgs", new RuleSet("OptOptionArgs", Arrays.asList(
        Arrays.asList( new Type[]{ new GroupedDefn(new RuleSet("Anonymous", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptionArgs") } ),
        Arrays.asList( new Type[]{ new Atom("HorizSpace") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      ))) } )
      )
    ))

    .with("OptionArgs", new RuleSet("OptionArgs", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MultipleOptionArgs") } ),
        Arrays.asList( new Type[]{ new Atom("OptionArg") } )
      )
    ))

    .with("MultipleOptionArgs", new RuleSet("MultipleOptionArgs", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptionArg"), new Atom("HorizSpace"), new Atom("OptionArgs") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Atom("OptionArg") , new Text(", ") , new Atom("OptionArgs")))
    ))

    .with("OptionArg", new RuleSet("OptionArg", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("arg"," \t\n\r") } )
      )
    ))

    .with("GrmImport", new RuleSet("GrmImport", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("@IMPORT"), new Atom("HorizSpace"), new Var("namespace"," \t\n\r"), new Atom("HorizSpace"), new Text("\""), new Var("filename","\"\n"), new Text("\""), new Atom("NL") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("    grammar.importNamespace(\"") , new Var("namespace") , new Text("\",\"") , new Var("filename") , new Text("\");\n")))
        .with("javadecl", Arrays.asList(new Text("    // @todo grammar.importNamespace(\"") , new Var("namespace") , new Text("\",\"") , new Var("filename") , new Text("\");\n")))
    ))

    .with("GrmExtend", new RuleSet("GrmExtend", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("@EXTEND"), new Atom("HorizSpace"), new Text("\""), new Var("filename","\"\n"), new Text("\""), new Atom("NL") } )
      )
    ))

    .with("GrmOtherAt", new RuleSet("GrmOtherAt", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("@"), new Var("option_name"," \n"), new Atom("OptHorizSpace"), new Var("rest","\n"), new Atom("NL") } )
      ),
        /* Replacements */
        new LiteralMap()
    //# For prototyping, for now, we allow grammar to insert any Java it likes.
        .with("java", Arrays.asList(new Text("    ") , new Var("rest")))
        .with("javadecl", Arrays.asList(new Text("    // @todo ") , new Var("rest")))
    ))

    // TODO: We can try adding ! after Whitespace.  If we have matched some Whitespace then it can't be a comment or an AtomDef, so recursion back through here can quit easily.
    // No dude it won't ever test Comment or AtomDef if Whitespace matched, it returns on a whole match.  Therefore ! is only useful if it comes before the end of the line.

    .with("GrmBit", new RuleSet("GrmBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Whitespace") } ),
        Arrays.asList( new Type[]{ new Atom("Comment") } ),
        Arrays.asList( new Type[]{ new Atom("AtomDef") } )
      )
    ))

    .with("Comment", new RuleSet("Comment", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("#"), new Var("comment","\n"), new Text("\n") } ),
        Arrays.asList( new Type[]{ new Text("#\n") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("    //") , new Var("comment") , new Text("\n")))
        .with("pojo", Arrays.asList(new Text("    //") , new Var("comment") , new Text("\n")))
        .with("javadecl", Arrays.asList(new Text("    //") , new Var("comment") , new Text("\n")))
    ))

    .with("OptComment", new RuleSet("OptComment", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Comment") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    // hugs: "-- " <comment> "\n"

    .with("AtomName", new RuleSet("AtomName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("PackagedAtomName") } ),
        Arrays.asList( new Type[]{ new Atom("PlainAtomName") } )
      )
    ))

    //# WARNING: Although I want to use "." to refer to modular rules, it is already used for RelDown!
    .with("PackagedAtomName", new RuleSet("PackagedAtomName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("PackageName"), new Text("."), new Atom("PlainAtomName") } )
      )
    ))

    .with("PlainAtomName", new RuleSet("PlainAtomName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Identifier") } )
      )
    ))

    .with("PackageName", new RuleSet("PackageName", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Identifier") } )
      )
    ))

    // Identifier = <identifier/"^.<>\n\" =">
    // Identifier = <identifier/"^<>\n\t\r \"+*()![]{}">
    .with("Identifier", new RuleSet("Identifier", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("identifier",null,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") } )
      )
    ))

    // Note that two AtomDefs without an empty line between them will not parse!

    .with("AtomDef", new RuleSet("AtomDef", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("AtomName"), new Atom("OptHorizSpace"), new Text("="), new Atom("OptHorizSpace"), new Atom("RuleSet"), new Atom("OptReplacements"), new Atom("NL") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("    ruleset=new RuleSet(\"") , new Atom("AtomName") , new Text("\");\n      grammar.addRuleset(ruleset);\n      rule=new Vector<Type>();\n") , new Atom("RuleSet") , new Text("      ruleset.add(rule);\n") , new Atom("OptReplacements") , new Text("\n")))
        .with("hugs", Arrays.asList(new Text("  ( Atom \"") , new Atom("AtomName") , new Text("\",[\n    [ ") , new Atom("RuleSet") , new Text(" ]\n    ] , [\n") , new Atom("OptReplacements") , new Text("\n  ] ) ,\n")))
        .with("pojo", Arrays.asList(new Text("interface ") , new Atom("AtomName") , new Text(" {\n\n}\n  class AnImplementation implements ") , new Atom("AtomName") , new Text(" {\n") , new Atom("RuleSet") , new Text("  }\n\n")))
    // BUG: We cannot make a second reference to AtomName
        .with("javadecl", Arrays.asList(new Text("    .with(\"") , new ArgReplacement(1) , new Text("\", new RuleSet(\"") , new Atom("AtomName") , new Text("\", ") , new Atom("RuleSet") , new Atom("OptReplacements") , new Text("\n    ))\n")))
    ))

    // Replacements are the lines defining export targets: java, hugs, pojo

    .with("OptReplacements", new RuleSet("OptReplacements", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("YesReplacements") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("YesReplacements", new RuleSet("YesReplacements", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\n"), new Atom("Replacements") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("    // Replacements\n") , new Atom("Replacements")))
        .with("javadecl", Arrays.asList(new Text(",\n        /* Replacements */\n        new LiteralMap()\n") , new Atom("Replacements")))
    ))

    .with("Replacements", new RuleSet("Replacements", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ManyReplacements") } ),
        Arrays.asList( new Type[]{ new Atom("Comment"), new Atom("Replacements") } ),
        Arrays.asList( new Type[]{ new Atom("Replacement") } )
      )
    ))

    .with("ManyReplacements", new RuleSet("ManyReplacements", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Replacement"), new Text("\n"), new Atom("Replacements") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("hugs", Arrays.asList(new Atom("Replacement") , new Text(",\n") , new Atom("Replacements")))
    ))

    // Note that use of + and * make it harder to place things *inbetween* the
    // elements during output (list join), so the above hugs output cannot currently
    // be easily reproduced with + or * as in the new rule below!
    // Really?  Doesn't the "\n" just get output again literally?

    // Replacements = ( Replacement "\n" )+
    // Replacements = Replacement ( "\n" Replacement )+ | ""

    .with("Replacement", new RuleSet("Replacement", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("target","<>\n\" :"), new Text(": "), new Atom("ReplacementDefn") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("    rule=new Vector<Type>();\n") , new Atom("ReplacementDefn") , new Text("    ruleset.replacements.put(\"") , new Var("target") , new Text("\",rule);\n")))
        .with("hugs", Arrays.asList(new Text("      ( \"") , new Var("target") , new Text("\" , [ ") , new Atom("ReplacementDefn") , new Text(" ] ) ")))
        .with("javadecl", Arrays.asList(new Text("        .with(\"") , new Var("target") , new Text("\", Arrays.asList(") , new Atom("ReplacementDefn") , new Text("))")))
    ))

    // The only difference between a RuleSet and a Defn is how we output it.
    .with("RuleSet", new RuleSet("RuleSet", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Defn") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("javadecl", Arrays.asList(new Text("Arrays.asList(\n        ") , new Atom("Defn") , new Text("\n      )")))
    ))

    .with("Defn", new RuleSet("Defn", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DefnOr") } )
      )
    ))

    //# We had to split DefnAnd and DefnOr into chains, so we can output separated by commas.
    //# TODO: A convenience way to do this to any RepeatElement e.g. (A B)+ is desirable!  At the moment we don't even have any way to refer to a matched list.  Perhaps $* iff the whole rule is one list...?  :f  Perhaps ReplacementArgs $1 $2 could refer to items in the rule not in the match?  Mmm there's really no difference.  :P  So we *can* refer to a list with $n?  So our goal is ... $3.join(", ") or join($3,", ") ?  Somehow I prefer the former; we can throw a "method join not found" error rather than "wrong args for join call".
    //# Now DefnAnd and DefnOr can both return a single DefnBit, it is not a special case.  (DefnOr can return a single DefnAnd.)
    //     | DefnAnd
    //     | DefnBit

    // DefnOr = DefnBit OWS "|" OWS Defn
    // java: DefnBit "      ruleset.add(rule);\n      rule=new Vector<Type>();\n" Defn
    // hugs: DefnBit "] ,\n      [ " Defn
    // pojo: DefnBit "  }\n  class AnotherImplementation {\n" Defn

    //# CONSIDER: Rename DefnOr to RuleBody and DefnAnd to RuleClause?
    .with("DefnOr", new RuleSet("DefnOr", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DefnAnd"), new Atom("DefnOrMaybeMore") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("javadecl", Arrays.asList(new Text("Arrays.asList( new Type[]{ ") , new Atom("DefnAnd") , new Text(" } )") , new Atom("DefnOrMaybeMore")))
    ))
    // Java requires us to be specific about the array types here.

    .with("DefnOrMaybeMore", new RuleSet("DefnOrMaybeMore", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DefnOrMore") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("DefnOrMore", new RuleSet("DefnOrMore", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OWS"), new Text("|"), new Atom("OWS"), new Atom("DefnOr") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("      ruleset.add(rule);\n      rule=new Vector<Type>();\n") , new Atom("DefnOr")))
        .with("hugs", Arrays.asList(new Text("] ,\n      [ ") , new Atom("DefnOr")))
        .with("pojo", Arrays.asList(new Text("  }\n  class AnotherImplementation {\n") , new Atom("DefnOr")))
        .with("javadecl", Arrays.asList(new Text(",\n        ") , new Atom("DefnOr")))
    ))

    // DefnAnd = DefnBit " "+ Defn
    // java: DefnBit Defn
    // hugs: DefnBit ", " Defn
    // javadecl: ", " DefnAnd
    .with("DefnAnd", new RuleSet("DefnAnd", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DefnBit"), new Atom("MaybeMore") } )
      )
    ))
    // javadecl: "Arrays.asList( new Type[]{ " DefnBit MaybeMore " } )"
    // Java requires us to be specific about the array types here.

    .with("MaybeMore", new RuleSet("MaybeMore", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MoreAnd") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("MoreAnd", new RuleSet("MoreAnd", Arrays.asList(
        Arrays.asList( new Type[]{ new RepeatedRule(new Text(" "),"+"), new Atom("DefnAnd") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("Java", Arrays.asList(new Atom("DefnAnd")))
        .with("hugs", Arrays.asList(new Text(", ") , new Atom("DefnAnd")))
        .with("javadecl", Arrays.asList(new Text(", ") , new Atom("DefnAnd")))
    ))

    // ReplacementElement is only used for output/replacement lines, not the rule definition.
    // However at the moment we have included it in DefnBit.
    // Perhaps we should create DefnBit and OutDefnBit, the latter containing those
    // few extra rules which can only be used for outputs.
    // Yes we should.

    // Efficient
    // DefnBit = BasicElement OptRepeatMarker

    // Inefficient but atomic output
    .with("DefnBit", new RuleSet("DefnBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("RepeatedElement") } ),
        Arrays.asList( new Type[]{ new Atom("BasicElement") } )
      )
    ))

    // Would-be best-of-both but not yet supported
    // DefnBit = BasicElement OptRepeatMarker

    .with("BasicElement", new RuleSet("BasicElement", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("VarAccept") } ),
        Arrays.asList( new Type[]{ new Atom("VarDeny") } ),
        Arrays.asList( new Type[]{ new Atom("Text") } ),
        Arrays.asList( new Type[]{ new Atom("GroupElement") } ),
        Arrays.asList( new Type[]{ new Atom("MagicSymbol") } ),
        Arrays.asList( new Type[]{ new Atom("AtomRef") } ),
        Arrays.asList( new Type[]{ new Atom("Regexp") } ),
        Arrays.asList( new Type[]{ new Atom("OptionalElement") } )
      )
    ))

    // ReplacementDefn = (OptSpc ReplacementElement OptSpc)*
    .with("ReplacementDefn", new RuleSet("ReplacementDefn", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptSpc"), new Atom("ReplacementElement"), new Atom("OptSpc"), new Atom("ReplacementElementMaybeMore") } )
      )
    ))

    .with("ReplacementElementMaybeMore", new RuleSet("ReplacementElementMaybeMore", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("OptSpc"), new Atom("ReplacementElementMore") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("ReplacementElementMore", new RuleSet("ReplacementElementMore", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ReplacementDefn") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("javadecl", Arrays.asList(new Text(", ") , new Atom("ReplacementDefn")))
    ))

    .with("ReplacementElement", new RuleSet("ReplacementElement", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ReplacementExpression") } ),
        Arrays.asList( new Type[]{ new Atom("ReplacementObject") } ),
        Arrays.asList( new Type[]{ new Atom("ActiveReplacement") } ),
        Arrays.asList( new Type[]{ new Atom("VarRef") } ),
        Arrays.asList( new Type[]{ new Atom("Text") } )
      )
    ))

    // A sub-class of ReplacementElement, those which may match a complex object in
    // the original rule (which may expose helper child functions).
    .with("ReplacementObject", new RuleSet("ReplacementObject", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ArgReplacement") } ),
        Arrays.asList( new Type[]{ new Atom("RelativeElement") } ),
        Arrays.asList( new Type[]{ new Atom("AtomRef") } )
      )
    ))

    // ReplacementExpression.  E.g. $1.join(", ") or InputAtom.toLowerCase()
    // ReplacementExpression is controversial in terms of complexity, but it should
    // allow us to output delimited lists without having to split rules into chains,
    // which is ugly and verbose and makes grammars less intuitive.
    // BUG TODO: Beware different meanings of "." e.g. OtherModule.InputAtom.toLowerCase()
    //           When trying to parse the AtomRef, it should take the first two
    //           words but actively reject the last word "toLowerCase" because of
    //           the presence of "(".  This will require a lookahead feature, to
    //           check the char following the atomref without consuming it.  :f
    //           A space then a "(" indicates not a call but a following GroupElement.
    .with("ReplacementExpression", new RuleSet("ReplacementExpression", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("not_yet_supported") } )
      )
    ))
    // ReplacementExpression = ReplacementObject "." FunctionCall

    // FunctionCall = <funcname/"("> "(" FuncArgs ")"

    // FuncArgs = OptHorizSpace (OneFuncArg OptHorizSpace "," OptHorizSpace)* OneFuncArg | OptHorizSpace
    // FuncArgs = OptHorizSpace (OneFuncArg OptHorizSpace [","] OptHorizSpace)*
    // This last rule appears simpler.  The optional "," allows parsing of the last
    // arg the same as all previous args.  It is sort-of safe in that it won't be
    // backtracked over due to being inside the ()s.  However it would happily
    // parse multiple UNdelimited args, which should not really be considered valid.

    // So TODO:
    // It seems we need a delimited-list-parsing construct as well as a
    // delimited-list-producing replacement.  For example:
    //
    //   FuncArgs = listOf(OptHorizSpace OneFuncArg, OptHorizSpace ",")
    //
    // means "parse a list of the first rule, delimited by instances of the last
    // rule".  Some alternative syntax:
    //
    //   FuncArgs = [[ OptHorizSpace OneFuncArg , OptHorizSpace "," ]]
    //
    //   FuncArgs = [[ OptHorizSpace OneFuncArg :: OptHorizSpace "," ]]
    //
    // How would the replacement for this look?
    //
    //   target: [[ OneFuncArg , ";\n" ]]
    //
    // For the first arg, we will have to match against each instance of the
    // OneFuncArg atom in the sub-matches found.  The secondary arg can act the same
    // way, making it more powerful than join(";\n") which can only output a string.
    // (In other words our delimiters can be complex rules, and their output too.)

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

    .with("MagicSymbol", new RuleSet("MagicSymbol", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("MagicTokenOfCommitment") } ),
        Arrays.asList( new Type[]{ new Atom("ReservedMagicSymbols") } )
      )
    ))
    // Don't put "#" here - it may eat through comments!

    // These are symbols we may want to use in future
    .with("ReservedMagicSymbols", new RuleSet("ReservedMagicSymbols", Arrays.asList(
        Arrays.asList( new Type[]{ new Text(":") } ),
        Arrays.asList( new Type[]{ new Text("%") } ),
        Arrays.asList( new Type[]{ new Text("$") } ),
        Arrays.asList( new Type[]{ new Atom("MagicTokenOfDoom") } )
      )
    ))

    .with("MagicTokenOfDoom", new RuleSet("MagicTokenOfDoom", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("!") } )
      )
    ))

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

    // Presumably we want to report a problem at the first atom after the symbol
    // which fails to match.

    // I can envisage another token which might be useful.  Let's call it % for now.
    // It can be used to mark a force-fail match, e.g. in the following
    //   cat = % Dog
    //       | BlackCat | WhiteCat | GreyCat | OtherCat
    // % Dog means drop out immediately with failure if we match a Dog.  It can be
    // used a an heuristic in some situations, where we expect a Dog is much more
    // likely, so generally more efficient to check that than to check all the cats
    // first.

    // Perhaps ! should be renamed . so % can be renamed ! (since it basically means "not").

    // OK I'm calling our parse-error hinter MagicTokenOfCommitment

    .with("MagicTokenOfCommitment", new RuleSet("MagicTokenOfCommitment", Arrays.asList(
        Arrays.asList( new Type[]{ new Text(";") } )
      )
    ))

    .with("GroupElement", new RuleSet("GroupElement", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("("), new Atom("OWS"), new Atom("RuleSet"), new Atom("OWS"), new Text(")") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.add( new GroupedDefn((RuleSet) new Runner(){ Object run(){\n          RuleSet ruleset = new RuleSet(\"Anonymous\");\n        Vector<Type> rule = new Vector<Type>();\n") , new Atom("RuleSet") , new Text("        ruleset.add(rule);\n          return ruleset;\n        } }.run() ) );\n")))
        .with("javadeclold", Arrays.asList(new Text("new GroupedDefn((RuleSet) new Runner(){ Object run(){\n          return new RuleSet(\"Anonymous\", ") , new Atom("RuleSet") , new Text(");\n        } }.run() )")))
        .with("javadecl", Arrays.asList(new Text("new GroupedDefn(new RuleSet(\"Anonymous\", ") , new Atom("RuleSet") , new Text("))")))
    ))

    .with("RepeatedElement", new RuleSet("RepeatedElement", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("BasicElement"), new Atom("RepeatMarker") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Atom("BasicElement") , new Text("        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),\"") , new Atom("RepeatMarker") , new Text("\"));\n")))
        .with("javadecl", Arrays.asList(new Text("new RepeatedRule(") , new Atom("BasicElement") , new Text(",\"") , new Atom("RepeatMarker") , new Text("\")")))
    ))

    // TODO: Surely like GroupElement, OptionalElement should be able to contain a
    // detailed Defn, not just one lone element.  This needs fixing in the
    // RepeatedRule class!
    .with("OptionalElement", new RuleSet("OptionalElement", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("["), new Atom("OWS"), new Atom("BasicElement"), new Atom("OWS"), new Text("]") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Atom("Defn") , new Text("        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),0,1));\n")))
    // java: "        rule.add(new RepeatedRule(" BasicElement ",0,1));\n"
        .with("javadecl", Arrays.asList(new Text("new RepeatedRule(") , new Atom("BasicElement") , new Text(",0,1)")))
    ))

    .with("RepeatMarker", new RuleSet("RepeatMarker", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ZeroOrMore") } ),
        Arrays.asList( new Type[]{ new Atom("OneOrMore") } )
      )
    ))

    .with("OptRepeatMarker", new RuleSet("OptRepeatMarker", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("RepeatMarkerOld") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("RepeatMarkerOld", new RuleSet("RepeatMarkerOld", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("RepeatMarker2") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.set(rule.size()-1, new RepeatedRule((Type)rule.lastElement(),\"") , new Atom("RepeatMarker2") , new Text("\"));\n")))
    ))

    .with("RepeatMarker2", new RuleSet("RepeatMarker2", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("ZeroOrMore") } ),
        Arrays.asList( new Type[]{ new Atom("OneOrMore") } )
      )
    ))

    .with("ZeroOrMore", new RuleSet("ZeroOrMore", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("*") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("*")))
    ))
    // javaB: 0,-1

    .with("OneOrMore", new RuleSet("OneOrMore", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("+") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("+")))
    ))
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

    .with("VarRef", new RuleSet("VarRef", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("<"), new Var("varname","<>\n\"/ ~"), new Text(">") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.add(new Var(\"") , new Var("varname") , new Text("\"));\n")))
        .with("hugs", Arrays.asList(new Text("Var \"") , new Var("varname") , new Text("\"")))
        .with("pojo", Arrays.asList(new Text("    String ") , new Var("varname") , new Text(";\n")))
        .with("javadecl", Arrays.asList(new Text("new Var(\"") , new Var("varname") , new Text("\")")))
    ))

    .with("VarDeny", new RuleSet("VarDeny", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("<"), new Var("varname","<>\n\"/ ~"), new Text("/\""), new Var("denied","\""), new Text("\">") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.add(new Var(\"") , new Var("varname") , new Text("\",\"") , new Var("denied") , new Text("\"));\n")))
        .with("hugs", Arrays.asList(new Text("VarExcl \"") , new Var("varname") , new Text("\" \"") , new Var("denied") , new Text("\"")))
        .with("pojo", Arrays.asList(new Text("    String ") , new Var("varname") , new Text(";\n")))
        .with("javadecl", Arrays.asList(new Text("new Var(\"") , new Var("varname") , new Text("\",\"") , new Var("denied") , new Text("\")")))
    ))

    .with("VarAccept", new RuleSet("VarAccept", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("<"), new Var("varname","<>\n\"/ ~"), new Text("~\""), new Var("accepted","\""), new Text("\">") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.add(new Var(\"") , new Var("varname") , new Text("\",null,\"") , new Var("accepted") , new Text("\"));\n")))
        .with("hugs", Arrays.asList(new Text("VarIncl \"") , new Var("varname") , new Text("\" \"") , new Var("denied") , new Text("\"")))
        .with("pojo", Arrays.asList(new Text("    String ") , new Var("varname") , new Text(";\n")))
        .with("javadecl", Arrays.asList(new Text("new Var(\"") , new Var("varname") , new Text("\",null,\"") , new Var("accepted") , new Text("\")")))
    ))

    .with("AtomRef", new RuleSet("AtomRef", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("AtomName") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.add(new Atom(\"") , new Atom("AtomName") , new Text("\"));\n")))
        .with("hugs", Arrays.asList(new Text("Atom \"") , new Atom("AtomName") , new Text("\"")))
        .with("pojo", Arrays.asList(new Text("    ") , new Atom("AtomName") , new Text(" arg1;\n")))
        .with("javadecl", Arrays.asList(new Text("new Atom(\"") , new Atom("AtomName") , new Text("\")")))
    ))

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

    .with("Text", new RuleSet("Text", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\""), new Var("text","\""), new Text("\"") } ),
        Arrays.asList( new Type[]{ new Text("\"\"") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.add(new Text(\"") , new Var("text") , new Text("\"));\n")))
        .with("hugs", Arrays.asList(new Text("Str \"") , new Var("text") , new Text("\"")))
        .with("pojo", Arrays.asList(new Text("    /* Matched: \"") , new Var("text") , new Text("\" */\n")))
        .with("javadecl", Arrays.asList(new Text("new Text(\"") , new Var("text") , new Text("\")")))
    ))



    .with("NL", new RuleSet("NL", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\r\n") } ),
        Arrays.asList( new Type[]{ new Text("\n") } )
      )
    ))

    // HorizSpace = " " | "\t"
    .with("HorizSpace", new RuleSet("HorizSpace", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("horizspace",null," \t") } )
      )
    ))

    .with("OptHorizSpace", new RuleSet("OptHorizSpace", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("HorizSpace") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("OptSpc", new RuleSet("OptSpc", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("SpcBit"), new Atom("OptSpc") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("SpcBit", new RuleSet("SpcBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Text(" ") } ),
        Arrays.asList( new Type[]{ new Text("\t") } )
      )
    ))

    .with("WhitespaceBit", new RuleSet("WhitespaceBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\n") } ),
        Arrays.asList( new Type[]{ new Text(" ") } ),
        Arrays.asList( new Type[]{ new Text("\t") } ),
        Arrays.asList( new Type[]{ new Text("\r") } )
      )
    ))

    // Whitespace = WhitespaceBit Whitespace
    //            | WhitespaceBit

    // Whitespace = WhitespaceBit+
    .with("Whitespace", new RuleSet("Whitespace", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("whitespace",null," \n\t\r") } )
      )
    ))

    // Optional whitespace:

    // OWS = Whitespace
    //     | ""

    .with("OWS", new RuleSet("OWS", Arrays.asList(
        Arrays.asList( new Type[]{ new RepeatedRule(new Atom("WhitespaceBit"),"*") } )
      )
    ))



    // == Experimental Stuff ==

    // This is used in c.grm.  It allows for more complex output, but calling the
    // code inside `...`s with the match Object available as a local field.

    .with("ActiveReplacement", new RuleSet("ActiveReplacement", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("`"), new Var("java","`"), new Text("`") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("      rule.add( new ActiveReplacement() { public String replace(OutputContext ctx) { ") , new Var("java") , new Text(" } } );\n")))
        .with("javadecl", Arrays.asList(new Text("new ActiveReplacement() { public String replace(OutputContext ctx) { ") , new Var("java") , new Text(" } }")))
    ))

    // RelativeElement is a way to access variables in parent/child types.
    // ^SearchUp->SearchDown.DirectDown

    .with("RelativeElement", new RuleSet("RelativeElement", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("RelUp") } ),
        Arrays.asList( new Type[]{ new Atom("RelDown") } )
      )
    ))

    .with("RelUp", new RuleSet("RelUp", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Ref"), new Text("^"), new Atom("ReplacementElement") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        { Vector realrule=rule; rule=new Vector<Type>(); ") , new Atom("BasicElement") , new Text(" realrule.add(new RelElement('^',") , new Atom("Ref") , new Text(",(Type)rule.get(0))); rule=realrule; }\n")))
        .with("javadecl", Arrays.asList(new Text("new RelElement('^',") , new Atom("Ref") , new Text(",") , new Atom("ReplacementElement") , new Text(")")))
    ))

    .with("RelDown", new RuleSet("RelDown", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Ref"), new Text("."), new Atom("ReplacementElement") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        { Vector realrule=rule; rule=new Vector<Type>(); ") , new Atom("BasicElement") , new Text(" realrule.add(new RelElement('.',") , new Atom("Ref") , new Text(",(Type)rule.get(0))); rule=realrule; }\n")))
        .with("javadecl", Arrays.asList(new Text("new RelElement('.',") , new Atom("Ref") , new Text(",") , new Atom("ReplacementElement") , new Text(")")))
    ))

    .with("Ref", new RuleSet("Ref", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("ref"," \"\n.^") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("\"") , new Var("ref") , new Text("\"")))
        .with("javadecl", Arrays.asList(new Text("\"") , new Var("ref") , new Text("\"")))
    ))

    .with("ArgReplacement", new RuleSet("ArgReplacement", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("$"), new Var("num",null,"0123456789_") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("        rule.add( new ArgReplacement(") , new Var("num") , new Text(") );\n")))
        .with("javadecl", Arrays.asList(new Text("new ArgReplacement(") , new Var("num") , new Text(")")))
    ))



    // Untested...

    // TODO: Gah presumably we need to escape any "s in the output of RegexpBody
    .with("Regexp", new RuleSet("Regexp", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("/"), new Atom("RegexpBody"), new Text("/") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("new Regexp(\"") , new Atom("RegexpBody") , new Text("\")")))
        .with("javadecl", Arrays.asList(new Text("new Regexp(\"") , new Atom("RegexpBody") , new Text("\")")))
    ))

    .with("RegexpBody", new RuleSet("RegexpBody", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("RegexpBit"), new Atom("RegexpBody") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("RegexpBit", new RuleSet("RegexpBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\\"), new Atom("AnyOneChar"), new Atom("RegexpBit") } ),
        Arrays.asList( new Type[]{ new Text("["), new Atom("RegexpCharBlock"), new Text("]"), new Atom("RegexpBit") } ),
        Arrays.asList( new Type[]{ new Var("regexpbit","\\/[\n"), new Atom("RegexpBit") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("RegexpCharBlock", new RuleSet("RegexpCharBlock", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("\\"), new Atom("AnyOneChar"), new Atom("RegexpCharBlock") } ),
        Arrays.asList( new Type[]{ new Var("regexpcharblock","]\n"), new Atom("RegexpCharBlock") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))



    // TESTS

    .with("DummyTestRule", new RuleSet("DummyTestRule", Arrays.asList(
        Arrays.asList( new Type[]{ new GroupedDefn(new RuleSet("Anonymous", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DummyTestContents") } )
      ))) } )
      )
    ))

    .with("ManyNothingsProbablyStackOverflow", new RuleSet("ManyNothingsProbablyStackOverflow", Arrays.asList(
        Arrays.asList( new Type[]{ new RepeatedRule(new RepeatedRule(new Atom("Something"),0,1),"*") } )
      )
    ))


    );
    return grammar;
  }
}

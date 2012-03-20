package tools.parser;

import java.lang.String;
import java.util.Vector;

import tools.parser.*;

public class dlangGrm {
  public static void setupgrammar() {
    Vector rulesets=Grammar.rulesets;
    RuleSet ruleset;
    Vector<Type> rule;

    // DLang - A dumb language
    //
    // Export targets:
    //
    // dintj - runtime structures suitable for interpreting by Java dlang.interpreter.*

    // TODO:
    //   Array dereference (aka List shortcuts)
    //   Member dereference (fields and methods of objects).  Similar for statics...
    //   .. namespaces
    //   Constructor definition and invocation
    //   Templating (generics)
    //   Anonymous implementations?  Or first order functions...
    //   Shortcuts for hashmaps
    //   Loops
    //   Define terminator symbol, e.g. "\n" | ";"

    // This is not exactly minimal, but we'd like a nicer way to access the last
    // element in an array than l[l.length-1]

  }
}

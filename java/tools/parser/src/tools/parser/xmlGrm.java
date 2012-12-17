package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.Vector;
import org.neuralyte.literal.LiteralMap;

import tools.parser.*;
import tools.parser.extensions.*;

public class xmlGrm extends GrammarHelper {
  public static Grammar setupgrammar() {
    Grammar grammar = new Grammar( new LiteralMap()    // Hmm interesting.  This could probably be a valid input.
    //
    // Although the vars do not have terminators, when they are followed immediately
    // by a single-char Text type, that char could be used as the implied
    // terminator.
    //
    // It might be possible for the grm grammar to seek and responds to this
    // pattern, so it needn't be a special case for the Parser.

    .with("XML", new RuleSet("XML", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("<"), new Var("type"," >"), new Text(">"), new Atom("Inside"), new Text("</"), new Var("type"," >"), new Text(">") } ),
      Arrays.asList( new Type[]{ new Text("<"), new Var("type"," >"), new Atom("Param"), new Text(">"), new Atom("Inside"), new Text("</"), new Var("type"," >"), new Text(">") } ),
      Arrays.asList( new Type[]{ new Text("<"), new Var("type"," >"), new Text("/>") } )
      )
    ))

    .with("Param", new RuleSet("Param", Arrays.asList(
        Arrays.asList( new Type[]{ new Text(" "), new Var("param"," >"), new Text("="), new Var("value"," >") } ),
      Arrays.asList( new Type[]{ new Atom("Param"), new Atom("Param") } )
      )
    ))

    .with("Inside", new RuleSet("Inside", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("") } ),
      Arrays.asList( new Type[]{ new Atom("XML"), new Atom("Inside") } ),
      Arrays.asList( new Type[]{ new Var("text","<") } )
      )
    ))


    );
    return grammar;
  }
}

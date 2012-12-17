package tools.parser;

import java.lang.String;
import java.util.Arrays;
import java.util.Vector;
import org.neuralyte.literal.LiteralMap;

import tools.parser.*;
import tools.parser.extensions.*;

public class marffGrm extends GrammarHelper {
  public static Grammar setupgrammar() {
    Grammar grammar = new Grammar( new LiteralMap()    .with("Main", new RuleSet("Main", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Definition"), new Text("@data\n"), new Atom("Data") } )
      )
    ))

    .with("Definition", new RuleSet("Definition", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DefBit"), new Atom("Definition") } ),
        Arrays.asList( new Type[]{ new Atom("DefBit") } )
      )
    ))

    .with("DefBit", new RuleSet("DefBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Comment") } ),
        Arrays.asList( new Type[]{ new Text("@"), new Atom("DefBit2"), new Var("whatever","\n"), new Text("\n") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Atom("Comment") , new Atom("DefBit2")))
    ))

    // That said: print whatever Comment or DefBit2 (whichever if matched) print,
    // but skip the "@", "\n" etc.

    .with("Comment", new RuleSet("Comment", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("CommentStart"), new Var("comment","\n"), new Text("\n") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("// ") , new Atom("CommentStart") , new Var("comment") , new Text("\n")))
    ))

    .with("CommentStart", new RuleSet("CommentStart", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("#") } ),
        Arrays.asList( new Type[]{ new Text("(*") } )
      )
    ))

    .with("DefBit2", new RuleSet("DefBit2", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DefRelation") } ),
        Arrays.asList( new Type[]{ new Atom("DefLink") } ),
        Arrays.asList( new Type[]{ new Atom("DefAttribute") } )
      )
    ))

    .with("DefRelation", new RuleSet("DefRelation", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("relation "), new Var("relation","\n") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("	definitions.add(new Relation(\"") , new Var("relation") , new Text("\");\n")))
    ))

    .with("DefLink", new RuleSet("DefLink", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("link "), new Var("filename","\n") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("	definitions.add(new Link(\"") , new Var("filename") , new Text("\");\n")))
    ))

    .with("DefAttribute", new RuleSet("DefAttribute", Arrays.asList(
        Arrays.asList( new Type[]{ new Text("attribute "), new Var("attrname"," "), new Atom("OptKey"), new Text(" "), new Var("attrtype"," \n") } )
      ),
        /* Replacements */
        new LiteralMap()
        .with("java", Arrays.asList(new Text("	definitions.add(new Attribute(\"") , new Var("attrname") , new Text("\", ... ,\"") , new Var("attrtype") , new Text("\");\n")))
    ))

    .with("OptKey", new RuleSet("OptKey", Arrays.asList(
        Arrays.asList( new Type[]{ new Text(" key") } ),
        Arrays.asList( new Type[]{ new Text("") } )
      )
    ))

    .with("Data", new RuleSet("Data", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("DataBit"), new Atom("Data") } ),
        Arrays.asList( new Type[]{ new Atom("DataBit") } )
      )
    ))

    .with("DataBit", new RuleSet("DataBit", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Fields") } )
      )
    ))

    .with("Fields", new RuleSet("Fields", Arrays.asList(
        Arrays.asList( new Type[]{ new Atom("Field"), new Text(","), new Atom("Fields") } ),
        Arrays.asList( new Type[]{ new Atom("Field"), new Text("\n") } )
      )
    ))

    // Field = NumericalField | StringField

    .with("Field", new RuleSet("Field", Arrays.asList(
        Arrays.asList( new Type[]{ new Var("field",",\n") } )
      )
    ))


    );
    return grammar;
  }
}

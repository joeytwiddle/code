package tools.parser;

import java.io.PrintStream;

/* It is a bit fiddly to make our replacement rules use this instead of a normal Type, but they should!
 * Once we have done that we can swap the inheritance order of Type and ReplacementType. */

public interface ReplacementType {

	  void renderMatchAs(OutputContext ctx, Match parentMatch, String target, PrintStream out);

}

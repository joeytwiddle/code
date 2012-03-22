package tools.parser;

import java.io.PrintStream;

/* It is a bit fiddly to make our replacement rules use this instead of a normal Type, but they should! */

public interface ReplacementType {

	  void renderMatchAs(Match parentMatch, String target, PrintStream out);

}

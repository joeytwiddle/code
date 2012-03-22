package tools.parser;

/* Magic types may appear in the replacement but may need to find their equivalent
 * in the list of sub-matches before they can be rendered.
 */

public interface MagicType extends Type {

	  /**
	   * This is used in Match.renderIn() to decide which matched type to use when
	   * performing a replacement.  It is only needed a couple of "difficult" types
	   * like Type and Var, most others can be rendered without this lookup.
	   *
	   * Method for matching the new structural types [...]* (...)+ are not yet
	   * specified, but as ever leaving the output target unset will cause all
	   * sub-matches to be shown.
	   *
	   * If we refer to them positionally (which is logical since they are unnamed)
	   * then we could provide the neccessary extra options that these types need,
	   * namely join for lists, e.g. $2.join(", ")
	   * and maybe even conditionals for optionals, e.g. $2 ? $2 : "null"
	  **/
	  boolean replacementfor(Type o);

}

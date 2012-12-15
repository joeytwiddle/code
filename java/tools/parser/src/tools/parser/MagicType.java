package tools.parser;

/**
 * Magic types are types that may appear in the replacement and will need to
 * find their corresponding entry in the list of matches.
 * 
 * @see Match.grabUnusedMatchMatching
 */
public interface MagicType extends Type {

	/**
	 * Given that we are a type specified in a replacement rule (e.g. a Var or
	 * Atom), can we consume a submatch of the given type in the match?
	 *
	 * Alternatively, we could have overriden .equals().
	 */
	boolean replacementfor(Type o);
}

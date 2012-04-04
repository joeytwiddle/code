package tools.parser;

/**
 * Magic types are types that may appear in the replacement but that need to
 * find their equivalent in the list of sub-matches before they can be rendered.
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

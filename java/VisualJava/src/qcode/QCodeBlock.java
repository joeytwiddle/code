package qcode;

interface QCodeBlock extends QIO {
    // We hold not a tree, but an ordered forest?
    java.util.List<QStatementObj> children = null;
}

// The "parent"
interface QStatementObj extends QIO {
}

// There are two types of QStatementObj:
// A QStatement: a single operation, depth 0
// A QSpecialStatement: variable depth, needs renaming
//   things like if and while statements, which contain one or more QCodeBlocks.

// Where do anon functions come into this?  They may be placed
// into a variable in a QStatement, or created and immediately
// consumed anonymously during a function call.

// The Scala guy said they fused objects and functions, made them effectively indistinguishable / transferrable.
// I am trying to interpret this as:
//   There is no real difference between a function with some closures (scope variables),
//   and a method tied to a particular instance object.
//   and an Object?
// Currying can also come into play here!  There are a lot of equivalences.  (fn a) b === (fn' b) a

// Additionally:
// In some languages, operators and functions are equivalent.  In those cases, operators are sometimes functions but with inner-binding-pattern.
// We could also extend this to right-bound functions/operators, e.g.: carrot isVegetable

interface QStatement extends QStatementObj {
}

interface QSpecialStatement extends QStatementObj {
}



interface QIfStatement extends QSpecialStatement {
}

interface QLoop extends QSpecialStatement {
}
interface QForLoop extends QLoop {
}
interface QWhileLoop extends QLoop {
}

interface QFunctionCall extends QStatement {

}

abstract class QAssignment implements QStatement {
    // var name
    // expression
}

class QExpression {
}


/* We could also consider:
 * Sometimes in apps we want to offer the user a simpler but still reasonably powerful way to enter expressions,
 * e.g. wireshark filters expose a lot of data to the user (in a property tree) and lets them perform expressions on them
 * Other times the app might want to be more restrictive, allow the user to select/create/provide not an expression but just a property from the tree (e.g. to select a new field for primary sorting).
 * When writing the above, we might also consider if they are compatible with any UI library classes we might create to satisfy the above two developer uses.
 * (In fact filters themselves are a subset of expressions, specifically expressions with a boolean return.)
 *
 * In these cases, the referenced values tend to be properties of some object
 * or tree, and less often global, local, or scope/instance variables.
 */



/*
 * For my own lang, which is again a separate project (one of many formats for serialisation),
 * I think I want a Coffeescript meets Haskell feel.
 *
 * I don't want a keyword for functions.
 * 
 *   f = (x,y) -> g z
 *
 *   f x y = g z
 *
 *   f(x,y) = g(z)    - I rather like this, mathematical
 * 
 *   f(x,y) = g z     - We hate using () because they are
 *                    - hard to reach.  Remap to [].
 *
 * This would be invalid:
 *
 *   f(x,y) + 1 = anything
 * 
 * unless we want to allow crazy backwards assignment.  ;)
 * 
 * I really like the latter two.  It does not parse as a valid call or normal
 * assignment, so let is read as a function definition!
 *
 * Who knows whether it is mutable?  :P
 *
 * If re-assigned, in this Haskell case, the second would need to become
 * another fresh function definition with a generated (anon) name, e.g.
 * f2 or %module_%class_%method_staticfns_f_assignment_2.
 *
 * Btw in the examples above, z and g must be visible somehow, and x and y are
 * not needed (in Haskell might not be calculated).
 *
 * Also what do the above do with regard to anonymous functions?
 *
 *   js:  ls.map( function(x){ return x+2; } );
 *
 *   cs:  map (x) -> x+2, ls
 *
 *   hs:  map (\x -> x+2) ls
 *
 *   us:  map f(x) = x+2, ls
 *        map (_(x) = x+2) ls
 *        map (x) -> x+2, ls
 *        map x -> x+2, ls
 *        
 *        map (x -> x+2) ls
 *        map(x -> x+2, ls)
 *        map(x -> x+2) ls
 *
 * onf.us.List.map (x -> x+2) ls
 *
 */

/*
 * Now we digress into a
 * different matter entirely, mathematical optimization:
 *
 * sum [] = 0
 * sum h:t = h + sum t
 *
 * f i = i
 * g i = 2
 *
 * list1 = map [1..10] f
 * list2 = map [1..10] g
 *
 * -- list1 should take longer than list2 to calculate.
 *
 * We don't need to "run" g in the same way we run f.
 * If we take into account that n X m where both are known numbers is easier to calculate than f(1) + ... + f(n)
 *
 * If instead:
 *
 *   g i = 5 + h i
 *
 * Then whether or not we can cache depends on whether h is state-dependent or not.
 * Or perhaps whether it is obviously optimizable/constant/static wrt its input.
 *
 */


// CONSIDER: Should we also provide QWhitespace, QComment and QMetaData (extendable)?

// P will be the first implementation of a language, the prototype.
// R,S,T and U=universal will be better implementations.

// Where is the cross-platform lib, written in ideal-lang?
// Where lives the core lib, providing a base for the cross-platform in every lang?


/* Should we allow macros?  Macros are awesome?
 * Or something greater even than that?  Special operators / language extensions?
 * They let us do, e.g.
 *   report populationCount
 * would translate to
 *   console.log("populationCount="+niceString(populationCount))
 * 
 * What does:
 * delete account['sp4mmer939'] 
 */

/*
 * Imports may be locally named, otherwise adopt their default names and can be globs.
 * Imports may be packages or classes or functions / procedures.  Maybe even methods?
 *
 * Defaults:
 *
 *   $log = import System.out.println
 *   ULib = org.neuralyte.lang.ulib
 *   import ULib.core.*
 *
 *   import java.io.String
 *   import java.util.Hashtable
 *   $util = java.util
 *
 * Conflicts in the above:
 *
 *   For globals/constants, should we use $mod or Mod or $MOD ?
 *
 * To what extend can locals override them?
 *
 *   Is assignment enough to work with, do we really need 'import' ?
 *   Is import a keyword or an operator?  Is the RHS a String?  It looks like a property reference!
 *
 * Examples:
 *
 *   import org.neuralyte.groovygui.*
 *   import ...
 *
 * A function bound to an instance:
 *
 *   $log = new org.neuralyte.logger.SimpleLogger("main").log
 *
 */


/*
 * In some contexts it would be nice to be able to create properties which are actually function calls, i.e. call-on-demand.
 * E.g. player.state ~= player.getState()
 * Here ` indicates the following expression should be lazily evaluated, and/or not evaluated until the time of read.
 * Again we need power-operator like delete.
 */

/* Contexts
 * Sometimes the little worker needs to ask the big boss how to perform a particular task.
 * 
 * In the renderer,
 *   camera passes context (transformToCamera)
 *   ship passes context (transformShipToWorld,transformShipToCamera)
 *   mesh pass transformMeshToShip,transformMeshToCamera
 *
 * In the VM / QLang
 *
 * VMContext:
 *   vmIsRunningWithReferenceCounting
 * CodeContext@
 *   thisCodeBlockRequiresScoping   => what the vm can/must do, and how exporting will work in certain languages
 *
 * Rather than define an interface for each property, we could let their access be hit or miss, i.e. the interface is satisfied by the presence of a prooperty of that name in any object (/class?).
 *
 * This is effectively a shortcut for declaring a HasPropertyXYZTypeK.
 *
 * Alternatively, it can be implemented weakly using a hashtable lookup of strings.
 *
 * Do we pass one context object to functions?
 * No surely we pass many?
 *
 *   lightingContext, meshContext, renderConfig, ... ?
 *
 * Perhaps some of those do belong in one renderContext or just 'context', but
 * some of them do not.
 *
 */


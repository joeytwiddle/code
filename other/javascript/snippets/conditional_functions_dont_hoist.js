// If a function is defined inside a loop or a conditional block, then the function's var/name does hoist, but the function value does not (and effectively becomes an assignment).  Although it does depend which engine you are using!

// <i_am_a_banana> ljharb: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/function#Conditionally_created_functions
// <i_am_a_banana> DeltaHeavy: Functions can be conditionally declared, that is, a function statement can be nested within an if statement, however the results are inconsistent across implementations and therefore this pattern should not be used in production code. For conditional function creation, use function expressions instead.
// <joeytwiddle> In my opinion `if (bar) { function foo(){} }` should be equivalent to `if (bar) { var foo = function(){}; }` and apparently that is how Chrome and Firefox behave, but not Safari and Edge
// <____> joeytwiddle: but in a function declaration, the whole function is hoisted
// <joeytwiddle> according to MDN, not if that function appears inside an if

// Here is the MDN example, with logging:

var hoisted = "foo" in this;
console.log(`BEFORE: 'foo' name ${hoisted ? "is" : "is not"} hoisted. typeof foo is ${typeof foo}`);
if (true) {
  function foo(){ return 1; }
}
console.log(`AFTER:  'foo' name ${hoisted ? "is" : "is not"} hoisted. typeof foo is ${typeof foo}`);

// If you run this in Chrome or Firefox, the var will be hoisted, but the function value will not.
// Thus the function will not be defined until after the conditional if statement has happened.
// Remove the if, and it acts differently.

// Let's assume we are going to declare two functions, 'first' and 'second',
// but we don't want to evaluate either of them right now.

// In fact, by wrapping one in ()s and forgetting the semicolon, we can
// accidentally evaluate it.  But this is rather contrived!

var a = 8, b = 16;

// No trailing semicolon is needed after a function declaration

function first(){
	console.log("First function was called");
	return 32;
}

(a + b).toFixed(2);

// However a function expression can be inadvertantly called if the following
// line begins with parentheses

(function second(){
	console.log("Second function was called");
	return 64;
})

(a + b).toFixed(2);

// In fact, because we forgot the semicolon, 'second' will be called, (a+b)
// will be discarded, and toFixed will be run on the number 64!


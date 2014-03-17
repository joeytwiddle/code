// A second order function, is one that operates on other functions.
//
// For example, this once function accepts a function fn as input, and returns
// a new function, which is like fn but will only ever occur once.

function once(fn) {

	var called = false;

	return function(){

		if (!called) {
			fn(); // Call the function!
			called = true; // But never call it again!
		} else {
			console.log("Not calling fn again!");
		}

	};

}


var oldFunc = function(){
	alert("HELLO WORLD!");
};

var newFunc = once(oldFunc);

newFunc();   // HELLO WORLD!
newFunc();   // Not calling fn again!
newFunc();   // Not calling fn again!



// Messing with Array.prototype breaks even Object.keys, so not doing it.  (I had always assumed Object.keys would check hasOwnProperty.)
// Hmm what?  When was I using Object.keys on an Array?  OK I wasn't.  Object.keys was *producing* an array, which was fine and valid, but Chrome devtools was *displaying* the prototype functions when I logged the array.

var oldLog = console.log;
console.log = function(text){
	document.write("<TT>"+text+"</TT><BR>\n");
	oldLog.apply(console,arguments);
};

var testNum = 3 * Math.random() | 0;

console.log("Performing test number "+testNum);

if (testNum == 0) {
	Array.prototype.first = function(){
		return this[0];
	};

} else if (testNum == 1) {

	// Apparently Array.prototype.first = ...; is unsafe BUT Object.defineProperties(Array.prototype, { first: ... }); is safe.
	// So if we do this, everything below works completely normally.

	Object.defineProperties(Array.prototype, {
		first: {
			// enumerable: false,     // Can it be seen by for..in loops?  Default = false
			// writable:   true,      // Default = fale
			// configurable: false,   // Default = false
			value: function(){
				return this[0];
			}
		}
	});

} else if (testNum == 2) {

	Object.defineProperty(Array.prototype, "first", {
		value: function(){
			return this[0];
		}
	});

}

var myArray = [1,2,3];

if (myArray.first() === 1) {
	console.log("Works");
} else {
	console.error("Test failed!");
}

console.log("keys="+Object.keys(myArray));   // Shows three items, array has length=3
console.log("keys:",Object.keys(myArray));   // Shows three items *and* shows the prototype functions, array has length=3

console.log("mapped: "+myArray.map(function(x) {
	return x+1;
}));

for (var k in myArray) {
	console.log("for in yields k="+k);
	// Test 0 will show an extra thing here!
}

myArray.forEach(function(item){
	console.log("forEach yields item="+item);
});



/*

var o = {};

o.a = 1;
// is equivalent to :
Object.defineProperty(o, "a", {value : 1,
                               writable : true,
                               configurable : true,
                               enumerable : true});


// On the other hand,
Object.defineProperty(o, "a", {value : 1});
// is equivalent to :
Object.defineProperty(o, "a", {value : 1,
                               writable : false,
                               configurable : false,
                               enumerable : false});

Note that if someone has already messed with Object.prototype, then omitting any of the settings leaves room to inherit them!  To avoid that, you can explicitly provide them all, or you can provide just `__proto__: null` to protect the rest and get the defaults.

*/


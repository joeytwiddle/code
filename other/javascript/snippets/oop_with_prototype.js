// I want to put as much as possible in .prototype
// Because I suspect this will be more efficient than calling a lot of code to
// build up the members of an object at instantiation time.



function Animal() {
	this.uniqueID = "" + (Math.random()*10000 | 0);
	// This means we must always call the parent constructor!
}
Animal.prototype = {
	dna: "GATAKA",
	location: 0,
	topSpeed: 5,
	walk: function(){
		location += topSpeed;
	}
};



/* Method 1 */
function Mammal()   {
	this.topSpeed = 20;
	this.layEgg = function() { };
	// So immediately we are writing the members of the subclass differently from the parent!
}
Mammal.prototype = Object.create(Animal.prototype);

function Cougar() { }
Cougar.prototype = Object.create(Mammal.prototype);
// TODO: Test this; is it true?
Cougar.prototype.constructor = Cougar;   // Otherwise instances of Cat would have a constructor of Mammal

function Sloth() { }
Sloth.prototype = Object.create(Mammal.prototype);



/* Method 2
// Object.extend() is from prototype.js.  Here we make our own version:
if (!Object.extend) {
	Object.extend = function() {
		var src = arguments[0];
		for (var i=1; i<arguments.length; i++) {
			var src = arguments[i];
			for (var prop in src) {
				dest[prop] = src[prop];
			}
		}
		return dest;
	};
}

function Mammal() { }
Object.extend(Mammal.prototype,Animal.prototype,{
	topSpeed: 20,
	layEgg: function(){ }
});

function Cougar() { }
Object.extend(Cougar.prototype,Mammal.prototype,{
	topSpeed: 60
});

function Sloth() { }
Object.extend(Sloth.prototype,Mammal.prototype,{
	topSpeed: 5
});
*/



/* Method 3 */
// Oh I see, we're supposed to do it like this:

Mammal.prototype = new Animal;            // Hook up Animal into Mammal's prototype chain
// What did that actually do?
// Is it any different from ...
// Mammal.prototype.prototype = Animal.prototype;  // ?
Mammal.prototype.constructor = Mammal;    // I saw someone refactor this into: Mammal.DeriveFrom(Animal);
function Mammal() {
	Animal.call(this);
	this.topSpeed = 20;
	this.layEgg = function() {
		// If we wanted to do super here (we can't for layEgg ofc):
		Animal.prototype.layEgg.call(this);
	};
}

function Cougar() {
	Mammal.call(this);
}
Cougar.prototype = new Mammal;
Cougar.prototype.constructor = Cougar;
Cougar.prototype.topSpeed = 60;

function Sloth() {
	Mammal.call(this);
}
Sloth.prototype = new Mammal;
Sloth.prototype.constructor = Sloth;
// If we want to introduce new members using an object literal instead of a
// load of assignments, we can use ES5's defineProperties function:
Object.defineProperties(Sloth.prototype,{
	topSpeed: 5
});



/* Method 4

// Perhaps we can tackle this with Object.create() after all:

Mammal = Object.create(Animal, {
	topSpeed: 20,
	layEgg: function() {
	}
});

*/


console.log("Mammal.prototype.prototype = "+Mammal.prototype.prototype);

var clive = new Cougar();

console.log("clive.topSpeed = "+clive.topSpeed);
console.log("clive.layEgg = "+clive.layEgg);
console.log("clive.uniqueID = "+clive.uniqueID);

var cindy = new Cougar();

console.log("cindy.uniqueID = "+cindy.uniqueID);

var steve = new Sloth();

console.log("steve.topSpeed = "+steve.topSpeed);
console.log("steve.layEgg = "+steve.layEgg);
console.log("steve.uniqueID = "+steve.uniqueID);



/* Conclusions:
 * We can do single-inheritance using the prototype chain and Method 3.
 * We can do multiple inheritance using Method 2, or perhaps 4.
 * We still haven't dealt with calling the constructor in Method 2/4.
 * In fact even using Method 2, I am worried I will forget to call the super constructor!
 * So I suggest we create our own class-creating wrapper for whatever method we choose?  (Problem: This will get in the way of someone who actually wants to call the super constructor with some sensible selected arguments!)
 * I think one general thing I might like my class framework to do, is to offer .className or some such, in case holder is interested in what type of object they are holding!
 * OTOH maybe the naming of the constructor can give us that (also for parents in the prototype).
 */


/*
if (!this.Class) {
	this.Class = {};
}
*/



// Some helper functions: http://www.ruzee.com/blog/2008/12/javascript-inheritance-via-prototypes-and-closures
// Similar: http://ejohn.org/blog/simple-javascript-inheritance/




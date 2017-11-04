A = function(){};
A.prototype = {
	foo: 1,
};

B = function(){};
B.prototype = Object.create(A.prototype);

C = function(){};
C.prototype = Object.create(B.prototype);

var o = new C();
//var o = Object.create(C);

console.log("o.constructor:", o.constructor);
console.log("o.constructor.name:", o.constructor.name);
console.log("o.constructor === C:", o.constructor === C);

/*
console.log("o.foo:", o.foo);
console.log("o.constructor:", o.constructor);
console.log("o.constructor.prototype:", o.constructor.prototype);
console.log("o.constructor.prototype.constructor:", o.constructor.prototype.constructor);
console.log("o.constructor.prototype.constructor.prototype:", o.constructor.prototype.constructor.prototype);
console.log("o.constructor.prototype.constructor.prototype.constructor:", o.constructor.prototype.constructor.prototype.constructor);
console.log("o.constructor.prototype.constructor.prototype.constructor.prototype:", o.constructor.prototype.constructor.prototype.constructor.prototype);
*/

var inspect = o;
var count = 5;

while (true) {
	console.log("inspecting %s with constructor %s", inspect, inspect.constructor);
	if (inspect.hasOwnProperty('foo')) {
		console.log("Found foo on %s with constructor %s", inspect, inspect.constructor);
		break;
	}
	//if (inspect.constructor === Object) {
		//console.log("Got all the way up to Object, now giving up.");
		//break;
	//}
	console.log("Moving through constructor %s to prototype %s", inspect.constructor, inspect.constructor.prototype);
	inspect = inspect.constructor.prototype;
	if (count-- === 0) {
		break;
	}
}


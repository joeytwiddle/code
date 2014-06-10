// Constructor
function ArrayMaker(arg1, arg2) {
	this.someProperty = 'whatever';
	this.theArray = [ this, arg1, arg2 ];
}

// Methods
ArrayMaker.prototype = {
	someMethod: function () {
		alert( 'someMethod called');
	},
	getArray: function () {
		return this.theArray;
	}
};

// Instantiation
var am = new ArrayMaker( 'one', 'two' );
var other = new ArrayMaker( 'first', 'second' );

alert("array="+other.getArray());


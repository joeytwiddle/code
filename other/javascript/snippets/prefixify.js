// By Huffjackson
var prefixify = function(op) {
  if ( op === '.' ) op = 'a[b]'
  else              op = 'a ' + op + ' b'
  return Function('a', 'b', 'return ' + op);
};



// Let's try using it
var multiply = prefixify("*");
var add      = prefixify("+");

var addTwoAndMultiplyByTen = function(n) { return multiply(10,add(2,n)) };

console.log( addTwoAndMultiplyByTen(5) );

// OK that was dull



// Prefixify allows us to make functions out of operators
// Functions can be passed around and used later

var equal = prefixify("==");
var lessThan = prefixify("<");
var greaterThan = prefixify(">");

// Let's use it with shesek's partial application:

function compareByField(field) {
  return function(op) {
    return function(value) {
      return function(object) {
        return op(object[field], value);
      };
    };
  };
}

// We have now declared so many functions, we can query our database on
// different fields, and make different comparisons, without writing a single
// function or loop!  :)

var db = [ {name:"sam",age:30}, {name:"fred",age:40}, {name:"dave",age:50} ];

var matchName  = compareByField("name")(equal);

console.log("People named dave:", db.filter(matchName("dave")) );

var compareAge = compareByField("age");
var isYounger  = compareAge(lessThan);
var hasAge     = compareAge(equal);
var isOlder    = compareAge(greaterThan);

console.log("Youngsters: ", db.filter(isYounger(40)) );
console.log("Middle-aged:", db.filter(hasAge(40)) );
console.log("Oldies:     ", db.filter(isOlder(40)) );



// Traditionally we would have written:

function comparebyfield(field,op,value,object) {
  return op(object[field], value);
}

// which we could call like this:

comparebyfield("age", equal, 30, "sam");

// But with partial application to shesek's function above, this is now:

compareByField("age")(equal)(30)("sam");



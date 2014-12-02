// Joey's online examples:
// Prototype: http://jsfiddle.net/SGDB3/2/
// Construction: http://jsfiddle.net/vXesT/

// Prototype:
function a () {}
a.prototype = {
    fnA = function () {},
    propA = 500
};

function b () {}
b.prototype = new a;
b.prototype.constructor = b;
b.prototype.fnB = function () {};
b.prototype.propB = 300;

function c () {}
c.prototype = new b;
c.prototype.constructor = c;
c.prototype.fnC = function () {};
c.prototype.propC = 200;



// Object.create:
function a() {}
a.prototype = {
    fnA = function () {},
    propA = 500
};

var b = Object.create(a.prototype);
b.fnB = function () {};
b.propB = 300;

var c = Object.create(b.prototype);
c.fnC = function () {};
c.propC = 200;

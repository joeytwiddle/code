function Person() {
};
Person.prototype.kick = function(type) {
	print(type + " kick!");
};

function Norris() {
};
Norris.prototype = new Person();
Norris.prototype.kick = function(hardness) {
	print("*");
	Object.getPrototypeOf(this).kick(hardness + " roundhouse");
};

function MegaNorris() {
};
MegaNorris.prototype = new Norris();
new MegaNorris().kick("very hard");

<Maxdamantus> You'd get problems when Norris' kick wants to take an argument that isn't simply passed to the "super".
<Maxdamantus> Whenever it's not simply: .foo = function(a, b, c){ Object.getPrototypeOf(this).foo(a, b, c, "d"); }

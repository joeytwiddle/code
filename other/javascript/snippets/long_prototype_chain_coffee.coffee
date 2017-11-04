# OK well epiphany time
# All this time I thought obj.constructor was standard in Javascript for objects created with new
# But it turns out 

class A
	foo: 1

class B extends A

class C extends B

o = new C()
#var o = Object.create(C)

console.log("o.constructor:", o.constructor)
console.log("o.constructor.name:", o.constructor.name)
console.log("o.constructor == C:", o.constructor == C)

###
console.log("o.foo:", o.foo)
console.log("o.constructor:", o.constructor)
console.log("o.constructor.prototype:", o.constructor.prototype)
console.log("o.constructor.prototype.constructor:", o.constructor.prototype.constructor)
console.log("o.constructor.prototype.constructor.prototype:", o.constructor.prototype.constructor.prototype)
console.log("o.constructor.prototype.constructor.prototype.constructor:", o.constructor.prototype.constructor.prototype.constructor)
console.log("o.constructor.prototype.constructor.prototype.constructor.prototype:", o.constructor.prototype.constructor.prototype.constructor.prototype)
###

inspect = o
count = 5

while true

	console.log("inspecting %s with constructor %s", inspect, inspect.constructor)
	if inspect.hasOwnProperty('foo')
		console.log("Found foo on %s with constructor %s", inspect, inspect.constructor)
		break

	#if inspect.constructor == Object
		#console.log("Got all the way up to Object, now giving up.")
		#break

	# This does not work
	# We need to go to inspect.constructor.prototype.__proto__
	# which is the same as inspect.__proto__
	# (and is what I was trying to avoid)
	console.log("Moving through constructor %s to prototype %s", inspect.constructor, inspect.constructor.prototype)
	inspect = inspect.constructor.prototype

	if count-- == 0
		break



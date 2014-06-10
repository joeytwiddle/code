
JaySync = {

    // JaySync.series() - Accepts a list of functions to be called in series,
    // followed by any number of initial arguments (to the first function).
    // Each function may accept a variable number of arguments, but the last
    // argument must be the callback function, which should be called on the
    // function's completion.

    series: function() {
        var firstArgs = Array.prototype.slice.call(arguments,0);
        var list = firstArgs.shift();
        function doNext() {
            var argsReceived = arguments;
            var next = list.shift();
            if (next) {
                var nextArgs = Array.prototype.slice.call(argsReceived,0);
                nextArgs.push(doNext);
                next.apply(this,nextArgs);
            }
        }
        doNext.apply(this,firstArgs);
    },

    // Parallel simply executes all the functions in the list, calling whenDone
    // only when every function has completed (called its 'next()').
    // Each function must call its first arg 'next' function, *once only*, on
    // success, failure or timeout.

    parallel: function(list,whenDone) {
        var numToDo = list.length;
        function whenDoneFunc(i) {
            return function() {
                numToDo--;
                if (numToDo == 0 && whenDone) {
                    whenDone();
                }
            };
        }
        for (var i=0;i<list.length;i++) {
            // list[i](whenDoneFunc(i));
            var func = list[i];
            var next = whenDoneFunc(i);
            func(next);
        }
    }

};

// Examples

JaySync.series([
    function(x,y,next) { console.log("First inputs:",x,y); next(x,y); },
    function(x,y,next) { console.log("Doing: "+x+" * "+y); next(x*y); },
    function(z) { console.log("Result: "+z); }
],5,2);

JaySync.series([
    function(next) { var x=5; var y=2; console.log("First inputs:",x,y); next(x,y); },
    function(x,y,next) { console.log("Doing: "+x+" * "+y); next(x*y); },
    function(z) { console.log("Result: "+z); }
]);

JaySync.parallel([
    function(next) { setTimeout(next,500); },
    function(next) { setTimeout(next,1000); }
],function(){ console.log("All parallel functions completed."); });

// The goal of this is to write async code which requires a lot of callbacks
// without indenting.  But we are losing a major advantage compared to normal
// nested callback dode: we lack closure of any vars declared.
// To compensate, we allow multiple vars to be passed forwards through
// arguments, which is specific about which vars are needed, but a little
// verbose.


// By Cuteness (@me) on Dwitter's Discord

let _debugPrint = (...a) => console.log(...a);
let Cls = class Cls {
    constructor () {
        _debugPrint('a new Cls instance created');
    }
    toString () {
        _debugPrint('.toString() called');
        return "";
    }
    valueOf () {
        _debugPrint('.valueOf() called');
        return false;
    }
};
let obj = new Cls();
_debugPrint(Boolean(obj.toString())); // false
_debugPrint(Boolean(obj.valueOf())); // false
_debugPrint(Boolean(obj)); // true
_debugPrint(new Boolean(false)); // true

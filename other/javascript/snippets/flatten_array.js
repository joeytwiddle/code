function flatten(arr) { return arr.reduce(function (flat, toFlatten) { return flat.concat(Array.isArray(toFlatten) ? flatten(toFlatten) : toFlatten); }, []); };   // qswz

var flatten = a => Array.isArray(a)?[].concat(...a.map(flatten)):a;

var flatten = arr => arr.reduce((a, b) => a.concat(b && b.reduce ? flatten(b) : b), [])


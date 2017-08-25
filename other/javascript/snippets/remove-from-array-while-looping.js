'use strict';

console.log("We want every loop to loop 'a', 'b' and 'c', even if we remove 'b' during the loop.");

console.log();

(function () {
  console.log("// BAD: list.forEach");
  var list = ['a', 'b', 'c'];
  list.forEach(item => {
    console.log(item);
    if (item === 'b') {
      list.splice(list.indexOf(item), 1);
    }
  });
}());

console.log();

(function () {
  console.log("// BAD: for item of list");
  var list = ['a', 'b', 'c'];
  for (var item of list) {
    console.log(item);
    if (item === 'b') {
      list.splice(list.indexOf(item), 1);
    }
  }
}());

console.log();

(function () {
  console.log("// GOOD: Array.from(list).forEach");
  var list = ['a', 'b', 'c'];
  Array.from(list).forEach(item => {
    console.log(item);
    if (item === 'b') {
      list.splice(list.indexOf(item), 1);
    }
  });
}());

console.log();

(function () {
  console.log("// GOOD: for item of Array.from(list)");
  var list = ['a', 'b', 'c'];
  for (var item of Array.from(list)) {
    console.log(item);
    if (item === 'b') {
      list.splice(list.indexOf(item), 1);
    }
  }
}());

console.log();

(function () {
  console.log("// OK I GUESS: for i = list.length; i--");
  var list = ['a', 'b', 'c'];
  for (var i = list.length; i--; ) {
    var item = list[i];
    console.log(item);
    if (item === 'b') {
      list.splice(list.indexOf(item), 1);
    }
  }
}());


'use strict';

var list0 = ['a', 'b', 'c'];
list0.forEach(item => {
  console.log(item);
  if (item === 'b') {
    list0.splice(list0.indexOf(item), 1);
  }
});

console.log();

var list2 = ['a', 'b', 'c'];
for (var item of list2) {
  console.log(item);
  if (item === 'b') {
    list2.splice(list2.indexOf(item), 1);
  }
}

console.log();

var list1 = ['a', 'b', 'c'];
Array.from(list1).forEach(item => {
  console.log(item);
  if (item === 'b') {
    list1.splice(list1.indexOf(item), 1);
  }
});

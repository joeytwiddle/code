'use strict';

const array = [1, 2, 3];
const object = {a: 'A', b: 'B', c: 'C'};

// Not needed
for (let item of array) {
  console.log(item);
}

// You can use const!
for (const item of array) {
  console.log(item);
}

// Not needed
for (let k in object) {
  console.log(k);
}

// You can use const!
for (const k in object) {
  console.log(k);
}

// But for a traditional for loop
for (let i = 0; i < array.length; i++) {
  const item = array[i];
  console.log(item);
}

// You must use let!
for (const i = 0; i < array.length; i++) {
  const item = array[i];
  console.log(item);
}

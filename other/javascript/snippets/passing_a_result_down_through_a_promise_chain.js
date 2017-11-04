// <grill> how exactly would one go about chaining together a bunch of promises in such a way that previous results would be available in later "links" of the chain

// With three mutable variables in shared scope
let a, b, c;
taskA().then(_a => {
  a = _a;
  return taskB(a);
}).then(_b => {
  b = _b;
  return taskC(a, b);
}).then(_c => {
  c = _c;
  // got c
  // do something with state and c
}).catch(error => {
  // handle the error
});

// With scope and indentation (I prefer this to avoid the confusing variables)
taskA().then(a => {
  // got a
  return taskB(a).then(b => {
    // got b
    return taskC(a, b).then(c => {
      // got c
      // do something with all the data
    });
}).catch(error => {
  // handle the error
});

// With coroutines and the 'co' library: https://github.com/tj/co
co(function *() {
  const a = yield taskA();
  const b = yield taskB(a);
  const c = yield taskC(a, b);
  // do something with all the data
}).catch(error => {
  // handle the error
});

// With async-await, very similar
Promise.resolve(async () => {
  const a = await taskA();
  const b = await taskB(a);
  const c = await taskC(a, b);
  // do something with all the data
}).catch(error => {
  // handle the error
});

// grr12314's explodey head approach
[
  taskA,
  taskB,
  taskC
].reduce(
  (chain, func) => chain.then(
    (arr) => func(arr).then(
      funcresult => arr.concat([funcresult])
    )
  ),
  Promise.resolve([])
).catch(error => {
  // handle the error
});
// In this case, the func must pick the variables it wants out of the array it was passed.
// But alternatively we could call `func.apply(null, arr)` to spread the vars over arguments.

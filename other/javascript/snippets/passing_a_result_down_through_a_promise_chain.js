// With an impure state object (or you could use 3 vars here)
const state = {};
taskA().then(a => {
  state.a = a;
  return taskB(state.a);
}).then(b => {
  state.b = b;
  return taskC(state.a, state.b);
}).then(c => {
  // got c
  // do something with state and c
}).catch(error => {
  // handle the error
});

// With scope and indentation (my recommendation if you can't do any of those below)
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

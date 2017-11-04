// With an impure state object (or you could use 3 vars here)
const state = {};
taskA().then(firstData => {
  state.firstData = firstData;
  return taskB(state.firstData);
}).then(secondData => {
  state.secondData = secondData;
  return taskC(state.firstData, state.secondData);
}).then(thirdData => {
  // got thirdData
  // do something with state and thirdData
}).catch(error => {
  // handle the error
});

// Passing a fresh new pure state object each time (cjohnson's suggestion?)
Promise.resolve().then(() => {
  return taskA().then(firstData => ({firstData: firstData})
).then(state => {
  return taskB(state.firstData).then(secondData => Object.assign({}, state, {secondData: secondData});
}).then(state => {
  return taskC(state.firstData, state.secondData).then(thirdData => Object.assign({}, state, {thirdData: thirdData});
}).then(state => {
  // got everything
  // do something with state
}).catch(error => {
  // handle the error
});

// With scope and indentation (my recommendation if you can't do any of those below)
taskA().then(firstData => {
  // got firstData
  return taskB(firstData).then(secondData => {
    // got secondData
    return taskC(firstData, secondData).then(thirdData => {
      // got thirdData
      // do something with all the data
    });
}).catch(error => {
  // handle the error
});

// If the promises can run in parallel
// (This isn't really solving the problem: results do not get passed down the chain)
Promise.all([
  taskA(),
  taskB(),
  taskC()
]).then(([firstData, secondData, thirdData]) => {
  // do something with all the data
}).catch(error => {
  // handle the error
});

// With coroutines and the 'co' library: https://github.com/tj/co
co(function *() {
  const firstData = yield taskA();
  const secondData = yield taskB(firstData);
  const thirdData = yield taskC(firstData, secondData);
  // do something with all the data
}).catch(error => {
  // handle the error
});

// With async-await
Promise.resolve(async () => {
  const firstData = await taskA();
  const secondData = await taskB(firstData);
  const thirdData = await taskC(secondData);
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

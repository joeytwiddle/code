// With an impure state object (or you could use 3 vars here)
const state = {};
someFunctionThatReturnsAPromise().then((firstData) => {
  state.firstData = firstData;
  return anotherFunctionThatReturnsAPromise();
}).then((secondData) => {
  state.secondData = secondData;
  return yetAnotherFunctionThatReturnsAPromise();
}).then((thirdData) => {
  // got thirdData
  // do something with stateData and thirdData
}).catch(error => {
  // handle the error
});

// With scope and indentation
someFunctionThatReturnsAPromise().then((firstData) => {
  // got firstData
  return anotherFunctionThatReturnsAPromise().then((secondData) => {
    // got secondData
    return yetAnotherFunctionThatReturnsAPromise().then((thirdData) => {
      // got thirdData
      // do something with all the data
    });
}).catch(error => {
  // handle the error
});

// If the promises can run in parallel
// (This isn't really solving the problem: results do not get passed down the chain)
Promise.all([
  someFunctionThatReturnsAPromise(),
  anotherFunctionThatReturnsAPromise(),
  yetAnotherFunctionThatReturnsAPromise()
]).then(([firstData, seconData, thirdData]) => {
  // do something with all the data
}).catch(error => {
  // handle the error
});

// With coroutines and the 'co' library: https://github.com/tj/co
co(function *() {
  const firstData = yield someFunctionThatReturnsAPromise();
  const secondData = yield anotherFunctionThatReturnsAPromise();
  const thirdData = yield yetAnotherFunctionThatReturnsAPromise();
  // do something with all the data
}).catch(error => {
  // handle the error
});

// With async-await
Promise.resolve(async () => {
  const firstData = await someFunctionThatReturnsAPromise();
  const secondData = await anotherFunctionThatReturnsAPromise();
  const thirdData = await yetAnotherFunctionThatReturnsAPromise();
  // do something with all the data
}).catch(error => {
  // handle the error
});

// grr12314's explodey head approach
[
  someFunctionThatReturnsAPromise,
  anotherFunctionThatReturnsAPromise,
  yetAnotherFunctionThatReturnsAPromise
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

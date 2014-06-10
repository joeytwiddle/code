# artificial delay; mocks an asynchronous task
delay = (f) ->
  setTimeout f, Math.random() * 100 * Math.random() * 10

# mini-async.js lib
async =
  whilst: (a, b, c) ->
    `a() ? b(function(e){return e ? c(e) : async.whilst(a,b,c)}) : c()`
  series: (a, cb) ->
    a.push cb ?= ->
    (next = (err, result) ->
      a = [a[a.length-1]] if err
      a.shift()(next, err, result)
      return
    )()
    return
  parallel: (a, cb) ->
    a.push cb ?= ->
    next = (err) ->
      a.shift()
      if err or 1 is a.length
        a[a.length-1](err)
        a = []
      return
    for i of a when i < a.length-1
      a[i](next)
    return

# series
console.log "series started."
async.series [
  (next, err, result) -> delay -> console.log ['a', next, err, result]; next(err, 1)
  (next, err, result) -> delay -> console.log ['b', next, err, result]; next(err, 2)
  (next, err, result) -> delay -> console.log ['c', next, err, result]; next(err, 3)
  (next, err, result) -> console.log ['d', next, err, result]; next(err, 4)
],(next, err, result) ->
  return console.log ['series complete with error', next, err, result] if err
  console.log ["series complete.", next, err, result]

# parallel
console.log "parallel started."
async.parallel [
  (next) -> delay -> console.log ['e', next]; next()
  (next) -> delay -> console.log ['f', next]; next()
  (next) -> console.log ['g', next]; next()
],(err) ->
  return console.log ['parallel complete with error', err] if err
  console.log ["parallel complete.", err]

console.log 'start whilst'
count = 0
async.whilst (-> console.log 'my a'; count < 5 ), 
  ((cb) -> console.log 'my b'; count++; delay cb; return ),
  ((err) -> console.log 'my c'; return )



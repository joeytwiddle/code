<jaawerth> the funniest is the horrible but entertaining things you can do when you use ES6 in non-strict mode
<jaawerth> lilke one of my favorites:
<jaawerth> n> let i = 0; with(new Proxy({}, {has: () => true, get: (t,k,p) => String(k).repeat(i++)})) { a + b + c }

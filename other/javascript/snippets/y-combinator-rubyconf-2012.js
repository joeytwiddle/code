// https://www.youtube.com/watch?v=FITJMJjASUs#Ruby_Conf_12_-_Y_Not-_Adventures_in_Functional_Programming_by_Jim_Weirich

// For Vim:
// set makeprg=node\ %
// cope
// nmap <F5> :w<CR>:make<CR><CR>

// fact = (n) => n === 0 ? 1 : n * fact(n - 1)

fact_improver = (partial) => (n) => n === 0 ? 1 : n * partial(n - 1)

y = (imp) => (
    (gen) => gen(gen)
    //gen => imp((v) => gen(gen)(v))
)(
    gen => imp((v) => gen(gen)(v))
)

// fact is the fixed point of fact_improver
fact = y(fact_improver)

console.log(
    // ((apf) => apf(apf))((apf) => (n) => n === 0 ? 1 : n * apf(apf)(n - 1))(7)
    fact(5)
)

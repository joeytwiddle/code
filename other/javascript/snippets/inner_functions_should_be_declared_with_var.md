TLDR: Yes, `var` is better.

In terms of implementation, the behaviour is indentical, *provided it is at the top-level of the function body.*  But beneath an if-condition or a for loop, `var` should always be used, or you get [ambiguous](https://github.com/joeytwiddle/code/blob/bb102b2d4677ef28065f075f656915df8d7114a5/other/javascript/snippets/declare_callback_after_passing_it.html) behaviour.

Matias did mention that Crockford's advice is to use `var isSpamKeyword = ...` always.  See "Function Declarations" [here](http://javascript.crockford.com/code.html).
- It makes it clearer that this is a local function that probably makes use of closure.
- It is possible to move the var inside an if-condition or a loop body without making any changes.

That said, I still tend to declare functions as functions (not vars) but I think that may be die-hard behaviour inherited from other languages, and perhaps I (and we) should try to adopt the recommend `var` approach for JS in future!

The one advantage of declaring as a function is that, thanks to hoisting, you can declare the function *below* the code that uses it.  This can be useful to make the flow of code in the file match the flow of execution.

    function context() {
        initiateAsyncAction( callback );

        function callback(result, error) { // will be hoisted to the top of its context function
            // process
        }
    }

This is not so easy to achieve with vars.

// ==UserScript==
// @name         Pause Activity When Unfocused
// @namespace    http://tampermonkey.net/
// @version      0.0.2
// @description  When page is no longer focused, delay all calls to requestAnimationFrame and setTimeout until the page is refocused.  Useful to prevent CPU overheat when developing games.
// @author       joeytwiddle
// @match        http://*/*
// @match        https://*/*
// @grant        unsafeWindow
// ==/UserScript==

setTimeout(function() {
    'use strict';

    var paused = false;

    unsafeWindow.addEventListener('blur', function () {
        paused = true;
        console.log('Window activity paused');
    });
    unsafeWindow.addEventListener('focus', function () {
        paused = false;
        console.log('Window activity resumed');
        if (delayedCall) {
            delayedCall.realFunc.apply(delayedCall.this, delayedCall.arguments);
            delayedCall = null;
        }
    });

    console.log('Wrapping functions...');

    delayWhenUnfocused('setTimeout');
    delayWhenUnfocused('requestAnimationFrame');
    // This is a polyfill that people occasionally use:
    delayWhenUnfocused('requestAnimFrame');

    var delayedCall = null;

    function delayWhenUnfocused (fnName) {
        var realFunc = unsafeWindow[fnName];
        if (!realFunc) {
            return;
        }
        var modifiedFunc = function () {
            if (paused) {
                delayedCall = {this: this, arguments: arguments, realFunc: realFunc};
            } else {
                return realFunc.apply(this, arguments);
            }
        };
        unsafeWindow[fnName] = modifiedFunc;
        console.log('Replaced ' + fnName + '.');
    }
}, 2000);

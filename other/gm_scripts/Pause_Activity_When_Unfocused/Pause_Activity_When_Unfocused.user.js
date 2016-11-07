// ==UserScript==
// @name         Pause Activity When Unfocused
// @namespace    http://tampermonkey.net/
// @version      0.0.4
// @description  Postpone calls to requestAnimationFrame and setTimeout when the page loses focus.  Useful to prevent CPU overheat when developing games or graphics.
// @author       joeytwiddle
// @match        http://*/*
// @match        https://*/*
// @grant        unsafeWindow
// ==/UserScript==

var delayBeforePausing = 60 * 1000;

function setupListeners () {
    'use strict';

    var paused = false;
    var timeoutId = null;
    var delayedCalls = [];

    unsafeWindow.addEventListener('blur', function () {
        clearTimeout(timeoutId);
        timeoutId = setTimeout(function () {
            paused = true;
            console.log('Window activity paused');
            timeoutId = null;
        }, delayBeforePausing);
    });
    unsafeWindow.addEventListener('focus', function () {
        if (timeoutId) {
            clearTimeout(timeoutId);
            timeoutId = null;
        }
        if (paused) {
            console.log('Window activity resumed');
            paused = false;
        }
        while (delayedCalls.length > 0) {
            var delayedCall = delayedCalls.shift();
            try {
                delayedCall.realFunc.apply(delayedCall.this, delayedCall.arguments);
            } catch (e) {
                //console.error("Error while replaying delayed call:");
                console.error(e);
            }
        }
    });

    console.log('Wrapping functions...');

    delayWhenUnfocused('setTimeout');
    delayWhenUnfocused('requestAnimationFrame');
    // This is a polyfill that people occasionally use:
    delayWhenUnfocused('requestAnimFrame');

    function delayWhenUnfocused (fnName) {
        var realFunc = unsafeWindow[fnName];
        if (!realFunc) {
            return;
        }
        var modifiedFunc = function () {
            if (paused) {
                delayedCalls.push( {this: this, arguments: arguments, realFunc: realFunc} );
            } else {
                return realFunc.apply(this, arguments);
            }
        };
        unsafeWindow[fnName] = modifiedFunc;
        console.log('Replaced ' + fnName + '.');
    }
}

//setTimeout(setupListeners, 20);
setupListeners();

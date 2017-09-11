// ==UserScript==
// @name         Pause Activity When Unfocused
// @namespace    http://tampermonkey.net/
// @version      0.0.3
// @description  Postpone calls to requestAnimationFrame and setTimeout when the page loses focus.  Useful to prevent CPU overheat when developing games or graphics.
// @author       joeytwiddle
// @match        http://*/*
// @match        https://*/*
// @grant        unsafeWindow
// ==/UserScript==

var delayBeforePausing = 1.5 * 60 * 1000;

var slowAnimations = false;   // Dangerous (at least for YouTube); see below.

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
//setupListeners();

// Don't run on all websites; only pages where we are visiting the root page
// (Intended to catch our game but not our management sites)
if (document.location.pathname === '/') {
    setupListeners();
}

// Bonus!
// Warning: This breaks YouTube :O  I see the video but none of the other features on the page (comments, titlebar, thumbs all missing).
if (slowAnimations) {
    unsafeWindow.requestAnimationFrame = function (callback) {
        unsafeWindow.setTimeout(callback, 1000 / 20);
    };
}

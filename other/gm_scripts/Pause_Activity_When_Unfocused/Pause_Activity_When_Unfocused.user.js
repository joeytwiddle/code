// ==UserScript==
// @name         Pause Activity When Unfocused
// @namespace    https://openuserjs.org/users/joeytwiddle
// @version      0.1
// @description  Pauses calls to requestAnimationFrame when the tab is not in focus.
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
        //console.log('Window activity paused');
    });
    unsafeWindow.addEventListener('focus', function () {
        paused = false;
        //console.log('Window activity resumed');
        if (delayedCall) {
            delayedCall.realFunc.apply(delayedCall.this, delayedCall.arguments);
            delayedCall = null;
        }
    });

    //console.log('Wrapping functions...');

    delayWhenUnfocused('requestAnimFrame');
    delayWhenUnfocused('requestAnimationFrame');

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
        //console.log('Replaced ' + fnName + '.');
    }
}, 2000);

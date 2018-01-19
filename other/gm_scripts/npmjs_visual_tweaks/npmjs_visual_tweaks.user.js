// ==UserScript==
// @name         npmjs.com visual tweaks
// @namespace    http://tampermonkey.net/
// @version      0.2
// @description  Makes npmjs.com fonts more like GitHub's
// @author       joeytwiddle
// @match        https://www.npmjs.com/package/*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';

    // I want to scale down the fonts and everything else a bit.  This was an easy way to do that.
    GM_addStyle('.container { transform: scale(0.92); transform-origin: 50% 0; }');

    GM_addStyle('body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol", "Source Sans Pro", "Lucida Grande", sans-serif; }');
    GM_addStyle('body { font-family: line-height: 1.5;}');

    GM_addStyle('pre, code, kbd, samp { font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, Courier, monospace; }');

    // A snippet of code appearing within a paragraph
    GM_addStyle('code { font-size: 88%; }');
    // A line of code on its own
    GM_addStyle('pre > code { font-size: 100%; }');
    // A block of code
    GM_addStyle('pre { font-size: 82%; line-height: 1.4; }');

    // A snippet of code appearing within a paragraph
    GM_addStyle('code { padding: 0.2em 0.2em; }');
    // A line of code on its own
    GM_addStyle('pre > code { padding: 1em 2em; }');

    // Not black text, but darker than the default #333
    //GM_addStyle('.markdown p, .markdown li, code { color: #24292e; }');
    // Just black
    GM_addStyle('.markdown p, .markdown li, code { color: #000; }');

    // More of a gap after each paragraph?  Wasn't actually needed.  The problem was wrapping due to insufficient width.
    //GM_addStyle('div, .highlight { padding-bottom: 20px; }');

    GM_addStyle('.highlight, code { background-color: #f6f8fa; }');

    // Fill (almost) the whole width of the screen, like Wide-Github does.
    GM_addStyle('.container { width: 98%; max-width: 100%; }');
})();

// ==UserScript==
// @name         Google Visited Links Color
// @namespace    GVLC
// @version      0.1
// @description  Make it easier to see which links you have already visited on google search results
// @author       joeytwiddle
// @license      MIT
// @include      https://www.google.*/search?*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';
    // const visitedLinkColor = '#9a8bb1';
    // const visitedLinkColor = '#9e5fbd';
    // const visitedLinkColor = '#8c59a6';
    // const visitedLinkColor = '#946ba8';
    // const visitedLinkColor = '#986aaf';
    // The normal blue, heavily grey-ified
    const visitedLinkColor = '#75738c';
    GM_addStyle('.mblink:visited, a:visited { color: ' + visitedLinkColor + '; }');
    // Your code here...
})();

// ==UserScript==
// @name         Google Visited Links Color
// @namespace    GVLC
// @version      0.1.1
// @description  Make it easier to see which links you have already visited on google search results
// @author       joeytwiddle
// @license      MIT
// @include      https://www.google.*/search?*
// @grant        GM_addStyle
// ==/UserScript==

// The year is 2018.  Google search results have made their links dark blue (#1a0dab) and their visited links dark pink (#609).
// It it somewhat difficult to tell them apart.  This userscript attempts to make the colours easier to distinguish.

(function() {
    'use strict';
    //const visitedLinkColor = '#9a8bb1';
    //const visitedLinkColor = '#9e5fbd';
    //const visitedLinkColor = '#8c59a6';
    //const visitedLinkColor = '#946ba8';
    // A nice faint pink.  Works well (clear separation) but looks a bit odd.
    //const visitedLinkColor = '#986aaf';
    //GM_addStyle('.mblink:visited, a:visited { color: ' + visitedLinkColor + '; }');

    // Back to default web colours, applying "least surprise" design principle
    // Default web colours are (I think): #00C (link) and #609 (visited link)
    // Here we are using slightly adjusted versions
    GM_addStyle(`
      a:link, .w, #prs a:visited, #prs a:active, .q:active, .q:visited, .kl:active, .tbotu {
        color: #0000aa;
      }
      .mblink:visited, a:visited {
        color: #880099;
      }
    `);
})();

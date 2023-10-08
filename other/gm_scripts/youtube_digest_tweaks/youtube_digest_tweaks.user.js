// ==UserScript==
// @name         YouTube Digest Tweaks
// @namespace    http://tampermonkey.net/
// @version      0.2
// @description  Make YouTube Digest more pleasant to look at
// @author       joeytiwddle
// @match        https://www.youtube.com/*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';

    GM_addStyle(`
        /* Reduce large padding around sections */
        .chat-gpt-container #answer .summaryCollapseGroup .collapseItem > div {
            padding: 0;
            /* There was a larger gap below the text than above it, so let's reduce that */
            /* margin-bottom: -5px; */
            /* Curiously, this also works */
            margin-top: -3px;
            border-bottom: 0;
        }
        .chat-gpt-container #answer .summaryCollapseGroup .collapseItem .title h3 {
            /* Do not make section titles bold */
            font-weight: 400;
            margin: 1em 0;
        }
        .chat-gpt-container #answer .summaryCollapseGroup .content {
            /* Reduce large padding around content summary */
            margin: 0;
            padding: 0;
        }
        .chat-gpt-container #answer .summaryCollapseGroup .content .content {
            /* Add small vertical and medium horizontal padding (indentation) */
            padding: 0.2em 1em;
        }
    `);
})();

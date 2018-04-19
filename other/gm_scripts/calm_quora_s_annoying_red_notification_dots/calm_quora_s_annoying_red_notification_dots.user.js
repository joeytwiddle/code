// ==UserScript==
// @name         Calm Quora's annoying red notification dots
// @namespace    joeytwiddle
// @version      0.0.3
// @license      MIT
// @description  The red notifications on Quora are too glaring, appear too frequently, and do not go away easily enough.  Let's make them grey so they aren't such a bother.
// @author       joeytwiddle
// @match        https://www.quora.com/*
// @grant        GM_addStyle
// ==/UserScript==

GM_addStyle('.SiteHeaderBadge, .WriteNavbadge, .red_badge { background: #ddd !important; background-color: #ddd !important; color: #666 !important; transform: scale(0.8); opacity: 0.5; }');

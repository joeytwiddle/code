// ==UserScript==
// @name         Remove Quora's stupid ducking notification badges
// @namespace    ducking-idiots
// @version      0.0.1
// @license
// @description  Every time you visit the site there are annoying red notification counters.  They lie, and you can't get rid of them.  I never asked for this ship.  Way to encourage engagement guys!
// @author       joeytwiddle
// @match        https://www.quora.com/*
// @grant        GM_addStyle
// ==/UserScript==

GM_addStyle('.SiteHeaderBadge, .WriteNavbadge, .red_badge { display: none !important; }'); // ducking idiots

// Yes that's right I got really annoyed that some developers added red dots to their website. :-P

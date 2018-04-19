// ==UserScript==
// @name         Calm Quora's annoying red notification dots
// @namespace    ducking-idiots
// @version      0.0.2
// @license
// @description  Every time you visit the site there are annoying notification badges.  But they lie, and you can't get rid of them.  I never asked for this ship.  Way to encourage engagement guys!
// @author       joeytwiddle
// @match        https://www.quora.com/*
// @grant        GM_addStyle
// ==/UserScript==

//GM_addStyle('.SiteHeaderBadge, .WriteNavbadge, .red_badge { display: none !important; }'); // ducking idiots

// Yes that's right I got really annoyed that some developers added red dots to their website.  :-P

// Alright I have calmed down now.  I'm going to make them faint grey instead, and smaller:

GM_addStyle('.SiteHeaderBadge, .WriteNavbadge, .red_badge { background: #ddd !important; background-color: #ddd !important; color: #666 !important; transform: scale(0.8); opacity: 0.5; }'); // still ducking idiots though

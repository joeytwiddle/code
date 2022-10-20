// ==UserScript==
// @name         Discord Tweaks
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      0.0.1
// @description  Tweak Discord UI: reduce spacing between messages
// @author       joeytwiddle
// @match        https://discord.com/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=discord.com
// @license      MIT
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';

    // Around 21/10/2022 Discord put a larger vertical gap between messages.  This reduces the gap size similar to how it was before.
    GM_addStyle(`
        .group-spacing-16 .groupStart-3Mlgv1 {
            margin: 0.25em;
        }
    `);
})();

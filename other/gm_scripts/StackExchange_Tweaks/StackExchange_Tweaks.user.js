// ==UserScript==
// @name           StackExchange Tweaks
// @namespace      SET
// @description    Minor visual tweas to StackExchange
// @version        1.0.0
// @include        http://stackoverflow.com/*
// @include        https://stackoverflow.com/*
// @include        http://*.stackexchange.com/*
// @include        https://*.stackexchange.com/*
// @grant          GM_addStyle
// ==/UserScript==

// Swap the positions of the notifications block and the username/stats block
var secondaryNav = document.querySelector('.secondary-nav')
secondaryNav.parentNode.insertBefore(secondaryNav, secondaryNav.parentNode.firstChild)

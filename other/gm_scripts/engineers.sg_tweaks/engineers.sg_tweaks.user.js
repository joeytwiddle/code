// ==UserScript==
// @name         Engineers.SG Tweaks
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  Removes 0-padding of hour on AM/PM times
// @author       joeytwiddle
// @match        https://engineers.sg/events
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    Array.from(document.querySelectorAll('.event-item > div > p > strong')).forEach(elem => {
        elem.textContent = elem.textContent.replace(/0(\d:\d\d [AP]M)/, '$1')
    });
})();

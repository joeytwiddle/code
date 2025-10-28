// ==UserScript==
// @name         Auto-close Zoom join pages
// @namespace    http://tampermonkey.net/
// @version      2025-10-28
// @description  Closes a Zoom join page after a few minutes, so you don't have to manually close it after the call
// @author       joeytwiddle
// @license      ISC
// @match        https://*.zoom.us/j/*
// @include      https://*.zoom.tld/j/*
// @include      https://zoom.tld/j/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=zoom.us
// @grant        window.close
// ==/UserScript==

(function() {
    'use strict';

    const $ = s => document.querySelector(s);

    const isZoomJoinPage = $('#zoom-ui-frame') || $('#zoom-ui-container');

    if (isZoomJoinPage) {
        // Close the window later
        setTimeout(() => {
            window.close();
        }, 60 * 1000);

        // Advertise that this script is running
        // The short delay makes the change more noticeable
        setTimeout(() => {
            const existingTextElement = $('#zoom-ui-frame h2')
            const ourTextElement = document.createElement('h2');
            ourTextElement.textContent = 'This page will auto-close soon';
            existingTextElement.parentNode.appendChild(ourTextElement);
        }, 1.5 * 1000);
    }
})();

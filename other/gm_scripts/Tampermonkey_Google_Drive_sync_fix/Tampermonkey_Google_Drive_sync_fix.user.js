// ==UserScript==
// @name         Tampermonkey Google Drive sync fix [adopted]
// @namespace    https://github.com/Tampermonkey/tampermonkey/issues/904
// @version      0.1.2
// @description  Tampermonkey keeps asking us to select an account when we open the browser.  But this script by Jack'lul will do it for us!
// @author       jacklul <jacklul.github.io>
// @license      ISC
// @match        https://accounts.google.com/signin/oauth*tampermonkey*
// @match        https://accounts.google.com/o/oauth2/v2/auth*tampermonkey*
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    let tryClick = setInterval(function() {
        const el = document.querySelector('div[data-authuser="0"] #profileIdentifier');

        if (el) {
            const isBusy = document.querySelector('div[role="presentation"][tabindex="-1"]');
            const hasBackButton = document.querySelector('div[role="button"]#next');

            if (isBusy) {
                console.log('Page is busy...');
            }

            // Close page if there was "unknown error"
            if (hasBackButton) {
                window.close();
            }

            // Try clicking until page gets busy
            if (!isBusy) {
                console.log('Clicking...');
                el.click();
            }
        }
    }, 500);
})();

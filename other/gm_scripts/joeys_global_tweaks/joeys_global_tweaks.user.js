// ==UserScript==
// @name         Joey's Global Tweaks
// @namespace    http://tampermonkey.net/
// @version      2024-07-28-2
// @description  CSS tweaks for various extensions or common elements
// @author       joeytwiddle
// @match        http://*/*
// @match        https://*/*
// @icon         https://commons.wikimedia.org/wiki/File:Globe_icon_2.svg
// @grant        GM_addElement
// ==/UserScript==

(function() {
    'use strict';

	startWatchingForChatGPTSummary();

	function startWatchingForChatGPTSummary() {
		runWhenReady('#readergpt__shadowRoot', addStylesToChatGPTSummary);
	}

	function addStylesToChatGPTSummary() {
		addStyleToShadowDOM(document.querySelector('#readergpt__shadowRoot'), `
			/* For "ChatGPT Summary - summarize assistant" extension
			   https://chromewebstore.google.com/detail/chatgpt-summary-summarize/mikcekmbahpbehdpakenaknkkedeonhf
			*/
			.readergpt__container {
				width: 75vw !important;
				height: 75vh !important;
				/* To prevent the unneccessary horizontal scrollbar */
				overflow-x: hidden;
			}
			#readergpt__readableText {
				/* On my machine, the default margin of 16px cuts off the last line of text, let alone showing the white border. */
				margin-bottom: 56px;
			}
		`);

		if (document.location.hostname.match(/youtube/)) {
			// It is common on YouTube that I will navigate to another video and run the summarize assistant there too.
			// In which case, we will need to add the CSS styles again.
			//
			// BUG TODO: Of course this doesn't work, because it sees the selector immediately, and triggers a second time!
			//
			//startWatchingForChatGPTSummary();
		}
	}

	function addStyleToShadowDOM(shadowWrapper, css) {
		GM_addElement(shadowWrapper.shadowRoot, 'style', { textContent: css });
		// Alternatives: https://stackoverflow.com/questions/60263139/how-to-use-gm-addstyle-in-tampermonkey-on-shadow-root-open
		// Alternatives: https://stackoverflow.com/questions/23683439/gm-addstyle-equivalent-in-tampermonkey
		// Alternatives: https://github.com/Tampermonkey/tampermonkey/issues/1714
	}

	function runWhenReady(readySelector, callback) {
		var numAttempts = 0;
		var tryNow = function() {
			var elem = document.querySelector(readySelector);
			if (elem) {
				callback(elem);
			} else {
				numAttempts++;
				if (numAttempts >= 34) {
					console.warn('Giving up after 34 attempts. Could not find: ' + readySelector);
				} else {
					setTimeout(tryNow, 250 * Math.pow(1.1, numAttempts));
				}
			}
		};
		tryNow();
	}
})();

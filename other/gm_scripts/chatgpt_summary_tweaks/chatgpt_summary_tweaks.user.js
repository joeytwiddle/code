// ==UserScript==
// @name         ChatGPT Summary Tweaks
// @namespace    http://tampermonkey.net/
// @version      2024-08-15
// @description  CSS tweaks for ChatGPT Summary extension
// @author       joeytwiddle
// @match        http://*/*
// @match        https://*/*
// @icon         https://commons.wikimedia.org/wiki/File:Globe_icon_2.svg
// @grant        GM_addElement
// ==/UserScript==

(function() {
    'use strict';

	// For "ChatGPT Summary - summarize assistant" extension
	// https://chromewebstore.google.com/detail/chatgpt-summary-summarize/mikcekmbahpbehdpakenaknkkedeonhf

	startWatchingForChatGPTSummary();

	function startWatchingForChatGPTSummary() {
		runWhenReadyUniq('#readergpt__shadowRoot', addStylesToChatGPTSummary);
	}

	function addStylesToChatGPTSummary() {
		addStyleToShadowDOM(document.querySelector('#readergpt__shadowRoot'), `
			.readergpt__container {
				width: 75vw !important;
				height: 75vh !important;
				/* To prevent the unneccessary horizontal scrollbar */
				overflow-x: hidden;
			}
			#readergpt__readableText {
				/* On my machine, the default margin of 16px cuts off the last line of text, let alone showing the white border. */
				margin-bottom: 56px;

				/* No, let's remove the white margin, and go for padding instead. */
				margin: 0;
				margin-bottom: 36px;
				padding: 24px;
			}
		`);

		if (document.location.hostname.match(/youtube/)) {
			// It is common on YouTube that I will navigate to another video and run the summarize assistant there too.
			// In which case, we will need to add the CSS styles again.
			startWatchingForChatGPTSummary();
		}
	}

	function addStyleToShadowDOM(shadowWrapper, css) {
		GM_addElement(shadowWrapper.shadowRoot, 'style', { textContent: css });
		// Alternatives: https://stackoverflow.com/questions/60263139/how-to-use-gm-addstyle-in-tampermonkey-on-shadow-root-open
		// Alternatives: https://stackoverflow.com/questions/23683439/gm-addstyle-equivalent-in-tampermonkey
		// Alternatives: https://github.com/Tampermonkey/tampermonkey/issues/1714
	}

	// Modified version of runWhenReady() which does not re-trigger on the same element
	var lastElementHandled = null;
	function runWhenReadyUniq(readySelector, callback) {
		var numAttempts = 0;
		var tryNow = function() {
			var elem = document.querySelector(readySelector);
			if (elem && elem !== lastElementHandled) {
				lastElementHandled = elem;
				callback(elem);
			} else {
				numAttempts++;
				if (numAttempts >= 5000) {
					console.warn('Giving up after 5000 attempts. Could not find: ' + readySelector);
				} else {
					setTimeout(tryNow, 250 * Math.pow(1.1, numAttempts));
				}
			}
		};
		tryNow();
	}
})();

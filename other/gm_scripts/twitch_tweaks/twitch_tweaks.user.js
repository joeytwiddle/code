// ==UserScript==
// @name         Twitch Tweaks
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      0.0.2
// @description  Make theatre mode toggle fullscreen
// @author       joeytwiddle
// @match        https://www.twitch.tv/*
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

	// Options

	const makeTheatreModeButtonToggleFullscreen = true;

	//

	// BUG: The first time we click this button, it does go fullscreen, but then it disappears!
	// (Presumably it's a kind of fullscreen that Twitch thinks Theatre Mode does not belong in.)
	// Then when I use Twitch's button to exit fullscreen, the Theatre button re-appears, but no longer toggles fullscreen.
	// I guess that's because it's a new button, without our event listener attached.

	if (makeTheatreModeButtonToggleFullscreen) {
		const theatreModeButtonSelector = '[data-a-target="player-theatre-mode-button"]';

		waitForElement(theatreModeButtonSelector, (element) => {
			// Actually there seem to be two of these (perhaps for different page widths?)
			Array.from(document.querySelectorAll(theatreModeButtonSelector)).forEach(element => {
				console.info("Adding event listener to", element);
				element.addEventListener('click', () => {
					console.info("Toggling fullscreen");
					// Hides the theatre button
					// Although hitting Firefox's own fullscreen button doesn't do that
					// Also this only seems to work once
					// But at least it works the first time!
					toggleFullScreen();
					//
					// Hides chat
					//const fullscreenButton = document.querySelector('[data-a-target="player-fullscreen-button"]')
					//fullscreenButton.click();
				}, true);
			});
		});
	}

	// From https://stackoverflow.com/a/19442811
	function toggleFullScreen() {
       if (!document.fullscreenElement &&
        !document.mozFullScreenElement && !document.webkitFullscreenElement) {
         if (document.documentElement.requestFullscreen) {
           document.documentElement.requestFullscreen();
         } else if (document.documentElement.mozRequestFullScreen) {
           document.documentElement.mozRequestFullScreen();
         } else if (document.documentElement.webkitRequestFullscreen) {
           document.documentElement.webkitRequestFullscreen(Element.ALLOW_KEYBOARD_INPUT);
         }
       } else {
          if (document.cancelFullScreen) {
             document.cancelFullScreen();
          } else if (document.mozCancelFullScreen) {
             document.mozCancelFullScreen();
          } else if (document.webkitCancelFullScreen) {
            document.webkitCancelFullScreen();
          }
       }
     }

	function waitForElement(selector, optionsOrCallback) {
		var callback = typeof optionsOrCallback === 'function' ? optionsOrCallback : optionsOrCallback.callback;

		var timeoutSeconds = optionsOrCallback.timeoutSeconds || 120;
		var retrySeconds = optionsOrCallback.retrySeconds || 5;

		var startTime = Date.now();

		var tryForElement = function() {
			var element = document.querySelector(selector);
			if (element) {
				callback(element);
			} else {
				if (Date.now() < startTime + timeoutSeconds * 1000) {
					setTimeout(tryForElement, retrySeconds * 1000);
				} else {
					throw new Error('Could not find element "' + selector + '" within ' + timeoutSeconds + ' seconds');
				}
			}
		};

		tryForElement();
	}
})();

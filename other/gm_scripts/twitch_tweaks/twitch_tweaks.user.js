// ==UserScript==
// @name         Twitch Tweaks
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      0.2.0
// @description  Make theatre mode toggle fullscreen
// @author       joeytwiddle
// @license      ISC
// @match        https://www.twitch.tv/*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';

	// Options

	const quietenPrimeOffersRedDot = true;

	const makeTheatreModeButtonToggleFullscreen = true;

	const addClipsTab = true;

	//

	if (quietenPrimeOffersRedDot) {
		//GM_addStyle(`.prime-offers__pill { display: none }`);
		GM_addStyle(`.prime-offers__pill { filter: saturate(0%) brightness(250%) }`);
	}

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

	if (addClipsTab) {
		setTimeout(checkForMissingClipsLink, 1 * 1000);
		// TODO: Only try again after a click, keypress, or link is followed
		setInterval(checkForMissingClipsLink, 15 * 1000);
	}

	function checkForMissingClipsLink() {
		if (document.querySelector('a[tabname="videos"]') && !document.querySelector('a[tabname="clips"]')) {
			const videosLink = document.querySelector('a[tabname="videos"]');
			const videosNode = videosLink.parentNode; // The container li

			const streamerName = videosLink.pathname.split('/')[1];
			const range = '7d'; // or '24hr' or '30d' or 'all'
			const clipsPath = `/${streamerName}/clips?featured=false&filter=clips&range=${range}`;

			const clipsNode = videosNode.cloneNode(true);
			const clipsLink = clipsNode.querySelector('a');
			//console.log('clipsNode:', clipsNode);
			clipsLink.querySelector('p').textContent = 'Clips';
			clipsLink.setAttribute('tabname', 'clips'); // Doesn't work without setAttribute?
			clipsLink.setAttribute('data-a-target', 'channel-home-tab-Clips');
			clipsLink.href = clipsPath;
			// Switch the styling from selected to not selected
			// TODO: If we actually are on the clips page, then do the opposite, and remove eGimjL from the Videos link
			// In case they change in future, can we auto-detect what eGimjL and kzdBhB are?  (By counting the number of occurrences.)
			const nodeWithSelectionStyle = clipsNode.querySelector('.ScTextWrapper-sc-iekec1-1');
			if (nodeWithSelectionStyle) {
				nodeWithSelectionStyle.className = nodeWithSelectionStyle.className.replace('eGimjL', 'kzdBhB');
			}

			videosNode.parentNode.insertBefore(clipsNode, videosNode.nextSibling);
		}
	}
})();

// ==UserScript==
// @name         Messenger Old Favicon
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      1.0.0
// @license      ISC
// @description  Make Facebook Messenger's icon blue again
// @author       joeytwiddle
// @match        https://www.messenger.com/*
// @grant        none
// ==/UserScript==

(function () {
	"use strict";

	// Note: This file follows the same pattern as Twitter_Favicon_No_Red_Notification_Dot.user.js

	const debugLogging = false;

	const iconWithoutPip = "data:image/png;base64,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";

	const iconWithPip = "data:image/png;base64,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";

	const head = document.getElementsByTagName("head")[0];

	function checkTheFavicon() {
		debugLogging && console.log("Checking favicon...");

		const currentIcons = head.querySelectorAll('[rel~="icon"]');
		debugLogging &&
			Array.from(currentIcons).forEach((icon) =>
				console.log("icon:", icon.href)
			);

		const currentIcon = currentIcons[currentIcons.length - 1];
		if (!currentIcon) {
			console.warn("messenger_old_favicon: No currentIcon to check");
			return;
		}

		if (currentIcon.href === iconWithoutPip || currentIcon.href === iconWithPip) {
			return;
		}

		const withPip = !currentIcon.href.includes("/YQbyhl59TWY.ico");
		setFavicon(currentIcon, withPip);
	}

	function setFavicon(currentIcon, withPip) {
		// Create an entirely new element
		/*
		const newIcon = document.createElement('link');
		newIcon.setAttribute('rel', 'shortcut icon');
		//newIcon.setAttribute('type', 'image/x-icon');
		//newIcon.setAttribute('href', 'data:image/x-icon;base64,______');
		newIcon.setAttribute('href', 'https://abs.twimg.com/favicons/twitter.ico');

		for (let i = currentIcons.length; i--;) {
			currentIcons[i].parentNode.removeChild(currentIcons[i]);
		}
		head.appendChild(newIcon);
		*/

		// Reuse the existing element
		const newHref = withPip ? iconWithPip : iconWithoutPip;
		const currentHref = currentIcon.getAttribute("href");
		if (newHref !== currentHref) {
			currentIcon.setAttribute("href", newHref);
		}
	}

	checkTheFavicon();
	new MutationObserver((mutations) => checkTheFavicon()).observe(head, {
		childList: true,
		subtree: true,
	});
})();
// ==UserScript==
// @name         Download Photo from Google Maps
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  Press ';' to download a photo while browsing Google Maps
// @author       itsnotlupus
// @contributors joeytwiddle
// @license      ISC
// @match        https://maps.google.com/*
// @match        https://www.google.*/maps/*
// @match        https://www.google.*.*/maps/*
// @match        https://www.google.co.uk/maps/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=maps.google.com
// @grant        GM_download
// ==/UserScript==

(function() {
    'use strict';

	// This will save the image directly to your Downloads folder (if your userscript environment supports it).
	// Otherwise it will open a new tab, for you to save the image manually.
	const useGMDownload = true;

	document.body.addEventListener('keydown', (evt) => {
		if (evt.key === ';') {
			const imgElem = document.body.querySelector('#magnifier img');
			const imgUrl = imgElem.src.replace(/=w.*$/, '=s8192');
			//console.log('imgUrl:', imgUrl);

			if (useGMDownload && typeof GM_download !== 'undefined') {
				const locationElem = document.querySelector('h1');
				const location = locationElem ? locationElem.textContent.trim() : 'Google Maps';
				// Make a unique filename, by attaching the date and time
				const timeNow = new Date().toISOString().split('T').join('-').split('.')[0];
				const fileName = `${location}.${timeNow}.jpg`;

				GM_download({
					url: imgUrl,
					name: fileName,
					onerror: (err) => {
						console.error(err);
						alert(err);
					},
				});
			} else {
				window.open(imgUrl, '_blank');
			}
		}
	}, true)
})();

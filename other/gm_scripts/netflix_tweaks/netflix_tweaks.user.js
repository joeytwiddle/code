// ==UserScript==
// @name         Netflix Tweaks
// @namespace    joeytwiddle
// @version      0.0.1
// @description  Hide top recommendation/advert
// @author       You
// @match        https://www.netflix.com/*
// @icon         https://www.google.com/s2/favicons?domain=netflix.com
// @grant        GM_addStyle
// @run-at       document-start
// ==/UserScript==

(function() {
    'use strict';

	const hideTopRecommendation = true;
	//const videoFilter = 'brightness(1.3) contrast(0.85) saturate(0.8)';
	const videoFilter = 'brightness(1.3) contrast(0.9) saturate(0.85)';

	if (typeof hideTopRecommendation !== 'undefined' && hideTopRecommendation) {
		GM_addStyle(`
			.volatile-billboard-animations-container {
				display: none;
			}
		`);
	}

	if (typeof videoFilter === 'string') {
		GM_addStyle(`
			video {
				filter: ${videoFilter};
			}
		`);
	}

})();

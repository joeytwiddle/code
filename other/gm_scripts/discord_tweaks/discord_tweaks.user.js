// ==UserScript==
// @name         Discord Tweaks
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      0.1.2
// @description  Reduce gap between messages, optionally unbrighten name of selected channel
// @author       joeytwiddle
// @match        https://discord.com/*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=discord.com
// @license      MIT
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';

	//// Options

	// Around 21/10/2022 Discord put a larger vertical gap between messages.
	// his reduces the gap size similar to how it was before.
	const reduceGapBetweenMessages = true;

	// When a channel has activity (new messages), Discord makes the name of that channel bold and white, which is fine.
	// But Discord also does that for the selected channel, which makes it look like it has new activity, when maybe it doesn't.
	// I can't stand that behaviour, so I disable it.
	const doNotBrightenSelectedChannel = false;

	//// End Options

	//

	if (reduceGapBetweenMessages) {
		GM_addStyle(`
			/* Original fix for 21/10/2022 */
			.group-spacing-16 .groupStart-3Mlgv1 {
				margin: 0.25em;
			}
			/* New CSS classes.  It looks like [role=article] will be the most resilient. I haven't seen it used for any other elements on a standard page, but it could be used in some Discord UI / menus. */
			[role=article], .message-2CShn3, .cozy-VmLDNB, .zalgo-26OfGz {
				padding-top: 0rem;
				padding-bottom: 0rem;
			}
		`);
	}

	if (doNotBrightenSelectedChannel) {
		// The problem is this CSS rule, specifically the .modeSelected selector, which highlights the currently selected channel the same as channels with unread activity.
		/*
			.modeConnected-NrO4cP .name-28HaxV,
			.modeConnected-NrO4cP:hover .name-28HaxV,
			.modeSelected-3DmyhH .name-28HaxV,
			.modeSelected-3DmyhH:hover .name-28HaxV,
			.modeUnread-3Cxepe .name-28HaxV,
			.modeUnread-3Cxepe:hover .name-28HaxV,
			.notInteractive-2tFrlE.modeConnected-NrO4cP .name-28HaxV,
			.notInteractive-2tFrlE.modeSelected-3DmyhH .name-28HaxV {
				color: var(--interactive-active);
			}
		*/

		// Let's try to fix it
		GM_addStyle(`
			.modeSelected-3DmyhH .name-28HaxV {
				color: var(--channels-default);
			}
			/* But if it really IS unread, then we will brighten the channel name */
			.modeSelected-3DmyhH.modeUnread-3Cxepe .name-28HaxV {
				color: var(--interactive-active);
			}
		`);
	}
})();

// ==UserScript==
// @name         Calm Quora's annoying red notification dots
// @namespace    joeytwiddle
// @version      1.0.1
// @license      MIT
// @description  The red notifications on Quora are too glaring, appear too frequently, and do not go away easily enough.  Let's make them grey so they aren't such a bother.  Also the popups and the adverts can take a hike.
// @author       joeytwiddle
// @match        https://www.quora.com/*
// @grant        GM_addStyle
// ==/UserScript==

// Make the red notification dots grey instead
GM_addStyle('.SiteHeaderBadge, .WriteNavbadge, .red_badge { background: #ddd !important; background-color: #ddd !important; color: #666 !important; transform: scale(0.8); opacity: 0.5; }');

// Don't show any popups
GM_addStyle('.Growl { display: none !important; }');

// Just make the whole damn header grey!
// As requested by Keeni: https://greasyfork.org/en/forum/discussion/37380/x
// In fact I think I will just apply this to anything I find on the site which is red!
GM_addStyle('.SiteHeader, .questions_to_answer_icon { filter: saturation(0%); }');

// Quora's advertisements appear visually a lot like the answers, so it is easy to read an advert and get confused because it's talking about a completely tangential topic.
// For some reason, I decided not to remove the adverts completely, but to de-emphasise them and differentiate them.
// The padding is needed so that the text doesn't appear to close to the edge of the red box.
// A hint of Quora red:
//const cssForAdverts = { backgroundColor: 'hsl(3, 25%, 90%)', opacity: 0.3, padding: '1em' };
// No background, just de-emphasise.  The padding is not strictly necessary but it can help to differentiate.
//const cssForAdverts = { opacity: 0.3, padding: '1em' };
const cssForAdverts = { opacity: 0.1, padding: '0em' };
jQuery('.advertiser_endpoint').closest('.outer_content_box').css(cssForAdverts)

// Curiously, window.jQuery didn't work in TamperMonkey for Chrome.  unsafeWindow.jQuery did, but I don't want the extra @grant

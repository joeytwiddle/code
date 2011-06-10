// ==UserScript==
// @name           URLs Need Titles
// @namespace      UNT
// @description    When you paste a URL to a friend, it's better it contain the title of the page.  This script adds these missing titles for common websites using # part of URL.
// @include        http://*/*
// @exclude        https://*/*
// ==/UserScript==


function setTitle(title) {
	if (title)
		title = title.replace(/ /g,'_').replace(/^[\r\n_]*/,'').replace(/[\r\n_]*$/,''); // "_"s paste better into IRC, since " "s become "%20"s which are hard to read.  The second and third parts trim "_"s and newlines from the start and end of the string.
	if (title) {
		if (!document.location.hash) {
			document.location.replace(document.location.href + '#' + title); // Does not alter browser history
			// document.location.hash = title; // Crashes Chrome less often
		}
	}
}


function check() {

	// TODO: Check if location.pathname is cross-browser

	// if (hostMatches("<maybe.>youtube.*") && pathMatches("^/watch")) {
	if (document.location.host.match(/(^|\.)youtube\./)) {
		if (document.location.pathname == "/watch") {
			var title = document.getElementsByTagName("h1")[0].textContent
				|| document.title.replace(/^YouTube - /,'')
				|| null;
			setTitle(title);
		}
	}

	if (document.location.host.match(/(^|\.)xkcd\.com$/)) {
		if (document.location.pathname.match(/[0-9]+\/$/)) {
			var title = (""+document.title).replace(/^[^:]*: /,'','g');
			setTitle(title);
		}
	}

}


// YouTube used to crash in Chrome unless we waited a bit.
setTimeout(check,5000);



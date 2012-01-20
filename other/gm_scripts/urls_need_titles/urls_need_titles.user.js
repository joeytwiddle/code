// ==UserScript==
// @name           URLs Need Titles
// @namespace      UNT
// @description    When you paste a URL to a friend, it's better it contain the title of the page.  This script adds these missing titles for common websites using # part of URL.
// @include        http://*/*
// @exclude        https://*/*
// ==/UserScript==

var rules = [

	/* A rule:
		{
			hostMatch: "youtube.TLD",   youtube.ath.cx, somewhere.youtube.com
			                            TLD is replaced by 1 or two toplevel domain elements.
			pathMatch: "/watch",        This regex will be wrapped with ^..$ so use
			                            .* at either end for wildcards.
			get: ...                    A function which returns a sensible title for this page.
		}
	*/

	{
		hostMatch: "youtube.TLD",
		pathMatch: "/watch",
		get: function(){
			return document.getElementsByTagName("h1")[0].textContent || document.title.replace(/^YouTube - /,'');
		}
	},

	{
		hostMatch: "xkcd.TLD",
		pathMatch: ".*[0-9]+/",
		get: function(){
			return (""+document.title).replace(/^[^:]*: /,'','g');
		}
	},

	{
		hostMatch: "imdb.TLD",
		pathMatch: ".*title.*",
		get: function(){
			return (""+document.title).replace(/ - IMDb/,'','g');
		}
	},

	{
		hostMatch: "pouet.net",
		pathMatch: "",
		get: function(){ return document.title }
	},

	{
		hostMatch: "userscripts.org",
		pathMatch: "/scripts/show/.*",
		get: function(){ return document.title.replace(" for Greasemonkey",''); }
	}

];

function check() {
	rules.forEach(checkRule);
}

function checkRule(rule) {
	// TODO: Check if location.pathname is cross-browser and/or standards-compliant
	var hostRegexp = rule.hostMatch.replace(/\.TLD$/, "(\.[^.]*$|\.[^.]*\.[^.]*$)");
	if (document.location.host.match(hostRegexp)) {
		if (rule.pathMatch) {
			var pathRegexp = '^' + rule.pathMatch + '$';
			if (!document.location.pathname.match(pathRegexp)) {
				return false;
			}
		}
		var newTitle = rule.get();
		if (newTitle == '' || newTitle == null) {
			GM_log("Failed to get new title for "+document.location+" from "+document.title);
		}
		setTitle(newTitle);
	}
}

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


// 2010/11: Waiting a bit can prevent crashing (e.g. YouTube in Chrome).
setTimeout(check,5000);


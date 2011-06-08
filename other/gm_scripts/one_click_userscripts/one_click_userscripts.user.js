// ==UserScript==
// @name           One Click Userscripts
// @namespace      OCU
// @description    Don't use Greasemonkey to load your userscripts, use this lighter interface instead!  Also works as a bookmarklet.
// @include        *
// ==/UserScript==

// == TODO ==
// Respect Include/Exclude rules.
// Could be renamed: All My Userscripts


// == Config ==

/*
var defaultScripts = [
	"http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js",
	"http://hwi.ath.cx/code/other/gm_scripts/wikiindent/wikiindent.user.js",
];
*/

var defaultScripts = [
	"fastjslogger",
	"faviconizetheweb",
	// "wikiindent",
	// "make_bookmarklet_from_us",
	"title_youtube_locations",
	"google_preview_pane",
	"delicious_link_tooltop",
].map(hwiScript);

var allowBrowserToCacheScripts = false;

function hwiScript(scriptName) {
	return "http://hwi.ath.cx/code/other/gm_scripts/" + scriptName + "/" + scriptName + ".user.js";
}



// == Main ==

function loadUserscripts() {

	var scriptsToLoad;

	try {
		scriptsToLoad = JSON.parse(GM_getValue("OCU_scriptList",null));
		GM_log("[OCU] Loaded config: "+scriptsToLoad);
	} catch (e) { }

	if (!scriptsToLoad || !scriptsToLoad.length) {
		GM_log("[OCU] No config found, falling back to default script list.");
		scriptsToLoad = defaultScripts;
		// Some sites (e.g. wikitravel) replace the default JSON with an incomplete implementation.
		if (this.JSON && JSON.stringify) {
			GM_setValue("OCU_scriptList",JSON.stringify(scriptsToLoad));
		}
	}

	scriptsToLoad.forEach(considerScript);

}

function loadScript(url,thenCallFn) {
	var scr = document.createElement("script");
	if (!allowBrowserToCacheScripts) {
		var sep = ( url.indexOf('?')>=0 ? '&' : '?' );
		url = url + sep+"dummy="+Math.random(100000);
	}
	scr.src = url;
	if (thenCallFn) {
		// Fixed: scr.onload=thenCallFn fails in Greasemonkey with "Component is not available"
		scr.addEventListener('load',thenCallFn,false);
		scr.addEventListener('error',thenCallFn,false);
	}
	document.body.appendChild(scr);
}

function considerScript(url) {
	// Check includes/excludes
	// But don't do it every time, at least cache the data for occasional update.
	if (true) {
		loadScript(url);
	}
}



// == Load fallback GM API library in case we need it. ==

loadScript(hwiScript("fallbackgmapi"),loadUserscripts);


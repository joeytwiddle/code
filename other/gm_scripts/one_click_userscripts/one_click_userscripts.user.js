// ==UserScript==
// @name           One Click Userscripts
// @namespace      OCU
// @description    Don't use Greasemonkey to load your userscripts, use this lighter interface instead!     Also works as a bookmarklet.
// @include        *
// ==/UserScript==

// TODO:
//   - Respect Include/Exclude rules.


// == Config ==

/*
var defaultScripts = [
	"http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js",
	"http://hwi.ath.cx/code/other/gm_scripts/wikiindent/wikiindent.user.js",
];
*/

var defaultScripts = [
	// "http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js",
	"fastjslogger",
	"google_preview_pane",
	"delicious_link_tooltop",
	"faviconizetheweb",
	"wikiindent",
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
		GM_setValue("OCU_scriptList",JSON.stringify(scriptsToLoad));
	}

	scriptsToLoad.forEach(loadScript);

}

function loadScript(url,thenCallFn) {
	var scr = document.createElement("script");
	if (!allowBrowserToCacheScripts) {
		var sep = ( url.indexOf('?')>=0 ? '&' : '?' );
		url = url + sep+"dummy="+Math.random(100000);
	}
	scr.src = url;
	if (thenCallFn) {
		scr.onload = thenCallFn;
		scr.onerror = thenCallFn;
	}
	document.body.appendChild(scr);
}



// == Load fallback GM API library in case we need it. ==

loadScript(hwiScript("fallbackgmapi"),loadUserscripts);


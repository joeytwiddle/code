// ==UserScript==
// @name           FallbackGMAPI
// @namespace      FallbackGMAPI
// @description    Default functions for GM_ API library for wider browser compatibility and execution of userscripts from bookmarklets.
// @downstreamURL  http://userscripts.org/scripts/source/123254.user.js
// @include        http://nowhere.example.com/@require_me_to_make_your_userscript_compatible_in_many_browsers
// @copyright      2009, 2010, 2011 James Campos, Paul Clark
// @license        cc-by-3.0; http://creativecommons.org/licenses/by/3.0/
// @exclude        *
// ==/UserScript==

// This is a shim to provide the Greasemonkey API functions if they are not already available.
// Goals:
// - Making userscripts work when they are loaded via a bookmarklet.
// - Making userscripts work better in Google Chrome (installed as extensions or run from bookmarklets).
// - Making userscripts work better in other browsers (I have not actually been testing this!).

// Notes:
// - To use in document scope, just load this script before any userscript JS.
// - To use as a Chrome or Greasemonkey extension, you need to copy paste from
//   this library into the top of your userscript.
//   (Alternatively, in Chrome, you might be able to load this library into
//   page scope, before continuing your userscript with a callback, but this
//   will not work in Greasemonkey due to the sandbox.)

(function(){

var exportTarget = window;

if (typeof GM_log === 'undefined') {
	if (this.console && typeof console.log === "function") {
		console.log("[FBGM] Implementing fallback GM_log");
	}
	// Without 'this' sometimes Bookmarklets in Chrome can't see GM_log!
	exportTarget.GM_log = function(data) {
		if (this.console && console.log) {
			console.log(data);
		} else {
			// We cannot log to Firefox's error console.
			if (window.navigator.vendor === "") {
				setTimeout(function(){ throw new Error(""+data); },0);
			}
		}
		// Many browsers no longer display the status or even the statusbar
		window.status = ""+data;
	};
}

var weAreInUserscriptScope = (typeof GM_log != 'undefined');
if (window.navigator.vendor.match(/Google/) && weAreInUserscriptScope) {
	var div = document.createElement("div");
	div.setAttribute("onclick", "return window;");
	exportTarget.unsafeWindow = div.onclick();
}

if (typeof GM_addStyle == 'undefined') {
	// GM_log("[FBGM] Implementing fallback GM_addStyle");
	exportTarget.GM_addStyle = function(css) {
		var style = document.createElement('style');
		style.textContent = css;
		document.getElementsByTagName('head')[0].appendChild(style);
	};
}

if (typeof GM_openInTab == 'undefined') {
	// GM_log("[FBGM] Implementing fallback GM_openInTab");
	exportTarget.GM_openInTab = function(url) {
		return window.open(url, "_blank");
	};
}

if (typeof GM_registerMenuCommand == 'undefined') {
	// GM_log("[FBGM] Implementing fallback GM_registerMenuCommand");
	exportTarget.GM_registerMenuCommand = function(name, funk) {
		// TODO
	};
}

// Implement GM_get/setValue using localStorage if needed.

// DONE: It would be more appropriate to use globalStorage if it is available.
// TODO: We should separate keys by script namespace/name/number, or data may
// overlap with other userscripts or site scripts.

// This throws an error in FF4: GM_setValue.toString().indexOf("not supported")>=0

if (typeof GM_setValue == 'undefined' || window.navigator.vendor.match(/Google/)) {

	/*
	var storage = this.globalStorage || this.localStorage || this.sessionStorage;
	var name = "some unlabeled";
	*/

	// Removed "globalStorage" storage because in FF4 it offers it but errors when we try to use it.
	// Chrome doesn't offer it ATM.
	var storageTypes = ["localStorage","sessionStorage"];
	var storage,name;
	for (var i=0;i<storageTypes.length;i++) {
		if (this[storageTypes[i]]) {
			storage = this[storageTypes[i]];
			name = storageTypes[i];
			break;
		}
	}

	if (storage) {
		try {
			storage.length; storage.getItem("dummy");
		} catch (e) {
			alert("Tried to use "+name+" storage but it failed on us!");
			storage = null;
		}
	}

	if (storage) {

		/*
		var allStorages = [this.globalStorage,this.localStorage,this.sessionStorage];
		// Removed "Global" storage because in FF4 it offers it but errors when we try to use it.
		// Chrome doesn't offer it ATM.
		var names = ["Local","Session"];
		var i = (allStorages.indexOf ? allStorages.indexOf(storage) : 0);
		var name = (i >= 0 ? names[i] : "unknown");
		*/

		GM_log("[FBGM] Implementing fallback GM_get/setValue using "+name+" storage.");

		exportTarget.GM_setValue = function(name, value) {
			value = (typeof value)[0] + value;
			storage.setItem(name, value);
		};

		// This causes a security error in FF4 with GM present.  (I presume that
		// means while running in GM?  Or does it occur when we run in the page
		// too?)
		exportTarget.GM_getValue = function(name, defaultValue) {
			var originalValue = storage.getItem(name);
			if (!originalValue)
				return defaultValue;
			var type = originalValue[0];
			var value = originalValue.substring(1);
			switch (type) {
				case 'b':
					return value == 'true';
				case 'n':
					return Number(value);
				case 's':
					return value;
				default:
					return originalValue;
			}
		};

		exportTarget.GM_deleteValue = function(name) {
			storage.removeItem(name);
		};

		exportTarget.GM_listValues = function() {
			var list = [];
			for (var i=0;i<storage.length;i++) {
				list.push(storage.key(i));
			}
			GM_log("[FBGM] localstorage is holding "+storage.length+" records.");
			return list;
		};

		/*

		*/

		// We leave a warning that, unlike Firefox's GM_set/getValue, our data
		// will only persist while on this domain:

		exportTarget.GM_setValue.restrictedToDomain = true;
		exportTarget.GM_getValue.restrictedToDomain = true;

	} else {
		GM_log("[FBGM] Warning: Could not implement GM_get/setValue.");
	}

}

// I don't know a need for this at the moment.  My browsers either give me
// get/set/list or nothing at all.
// If we are adding this into a userscript after data already exists, we might
// find some of the existing names through GM_getValue.
// TODO: UNTESTED!
if (typeof GM_listValues == 'undefined') {
	GM_log("[FBGM] Implementing fallback GM_listValues using intercepts.");
	var original_GM_setValue = GM_setValue;
	var original_GM_deleteValue = GM_deleteValue;
	exportTarget.GM_setValue = function(name, value) {
		original_GM_setValue(name,value);
		var values = JSON.parse(GM_getValue("#_LISTVALUES") || "{}");
		values[name] = true;
		original_GM_setValue("#_LISTVALUES",JSON.stringify(values));
	};
	exportTarget.GM_deleteValue = function(name) {
		original_GM_deleteValue(name);
		var values = JSON.parse(GM_getValue("#_LISTVALUES") || "{}");
		delete values[name];
		original_GM_setValue("#_LISTVALUES",JSON.stringify(values));
	};
	exportTarget.GM_listValues = function() {
		var values = JSON.parse(GM_getValue("#_LISTVALUES") || "{}");
		var list = [];
		for (var key in values) {
			list.push(key);
		}
		GM_log("[FBGM] GM_listValues is holding "+list.length+" records.");
		return list;
	};
}

if (typeof GM_xmlhttpRequest == "undefined") {
	GM_log("[FBGM] Implementing fallback GM_xmlhttpRequest, will only work for local domain.");
	exportTarget.GM_xmlhttpRequest = function(details) {
		var req = new XMLHttpRequest();
		req.open(details.method, details.url);
		req.send(null);
		req.onreadystatechange = function() {
			details.onload(req);
			/*
			if (req.readyState == 4) {
				if (req.status == 200) {
					details.onload(req);
				}
			}
			*/
		}
	};
	// Most browsers now restrict XMLHttpRequest to local domain requests only.
	// We leave a warning that this restriction is in effect:
	exportTarget.GM_xmlhttpRequest.localDomainOnly = true;
}

})();

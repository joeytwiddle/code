// @name           Cross-site XHR via JSON callback proxy
// @description    Include this script to get a cross-site GM_xmlhttpRequest via the proxy.
// @copyright      2010 Paul Clark
// @license        cc-by-3.0; http://creativecommons.org/licenses/by/3.0/

// DONE: Some scripts provide an onreadystatechange function but not onload.
//       So we should fake an onreadystatechange event for them.

// We always replace Google Chrome's GM_xmlhttpRequest because it is not cross-site.
if (!this.GM_xmlhttpRequest || window.navigator.vendor.match(/Google/)) {
	GM_xmlhttpRequest = function(details) {
		var proxyHost = "hwi.ath.cx:8124";
		// We don't want to send functions to the proxy, so we remove them from the details object.
		var onloadCallback = details.onload;
		var onerrorCallback = details.onerror;
		var onreadystatechangeCallback = details.onreadystatechange;
		delete details.onload;
		delete details.onerror;
		delete details.onreadystatechange;
		// Insert a callback into the root window, anonymised by a random key.
		var callbackName = "xss_xhr_via_jsonp_callback_" + parseInt(Math.random()*987654321);
		var callbackFunction = function(responseDetails) {
			if (onreadystatechangeCallback) {
				responseDetails.readyState = 4;
				onreadystatechangeCallback(responseDetails);
			}
			if (onloadCallback) {
				onloadCallback(responseDetails);
			}
		};
		if (!window.navigator.vendor.match(/Google/) /* || !weAreInUserscriptScope */) {
			// This works fine in Firefox GM, or in Chrome's content scope.
			window[callbackName] = callbackFunction;
		} else {
			// But the window seen from Chrome's userscript scope is sandboxed,
			// and many updates are not shared between scopes.
			// So we must get Chrome's unsafeWindow (the real content window).
			var div = document.createElement("div");
			div.setAttribute("onclick", "return window;");
			unsafeWindow = div.onclick();
			// And place the callback in that.
			unsafeWindow[callbackName] = callbackFunction;
		}
		// Request an XHR response from the proxy, which should return some javascript to call the callback.
		var reqStrung = JSON.stringify(details);
		var params = "details="+encodeURIComponent(reqStrung)+"&callback="+callbackName;
		var script = document.createElement("script");
		script.type = "text/javascript";
		script.src = "http://" + proxyHost + "/xhrasjson?" + params;
		document.getElementsByTagName("head")[0].appendChild(script);
		// The callback should run on a successful response.  But we need to handle errors too.
		// script.onload = function(e) { GM_log("Script has loaded."); };
		script.onerror = function(e) {
			var responseDetails = {};
			responseDetails.status = 12345;
			if (onreadystatechangeCallback) {
				responseDetails.readyState = 4;
				onreadystatechangeCallback(responseDetails); // This gets called even on error, right?
			}
			if (onerrorCallback) {
				onerrorCallback(responseDetails);
			}
			throw new Error("Failed to get JSONed XHR response from "+proxyHost+" - the server may be down.");
		};
	};
}

/*
// TODO: We could similarly make a cross-site implementation of XMLHttpRequest.
CrossSiteXMLHttpRequestViaJSON = function() {
};
*/


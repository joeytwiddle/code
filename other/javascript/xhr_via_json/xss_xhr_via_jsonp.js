// @name           Cross-site XHR via JSON callback proxy
// @description    Include this script to get a cross-site GM_xmlhttpRequest via the proxy.
// @copyright      2010 Paul Clark
// @license        cc-by-3.0; http://creativecommons.org/licenses/by/3.0/

// TODO: Some scripts provide an onreadystatechange function but not onload.
//       So we should fake an onreadystatechange event for them.

// We always replace Google Chrome's GM_xmlhttpRequest because it is not cross-site.
if (!this.GM_xmlhttpRequest || window.navigator.vendor.match(/Google/)) {
	var GM_xmlhttpRequest = function(request) {
		var proxyHost = "hwi.ath.cx:8124";
		// We don't want to send functions to the proxy, so we remove them from the request object.
		var onload = request.onload;
		var onerror = request.onerror;
		var onreadystatechange = request.onreadystatechange;
		delete request.onload;
		delete request.onerror;
		delete request.onreadystatechange;
		// Insert the callback into the root window, anonymised by a random key.
		var callbackName = "xss_xhr_via_jsonp_callback_" + parseInt(Math.random()*987654321);
		if (!window.navigator.vendor.match(/Google/) /* || !weAreInUserscriptScope */) {
			// This works fine in Firefox GM, or in Chrome's content scope.
			window[callbackName] = function(response) { onload(response); };
		} else {
			// But the window seen from Chrome's userscript scope is sandboxed,
			// and does not always update from one to the other.
			// So we must get Chrome's unsafeWindow (the real content window).
			var div = document.createElement("div");
			div.setAttribute("onclick", "return window;");
			unsafeWindow = div.onclick();
			// And place the callback in that.
			unsafeWindow[callbackName] = function(response) { onload(response); };
		}
		// Request an XHR response from the proxy, which should return some javascript to call the callback.
		var reqStrung = JSON.stringify(request);
		var params = "request="+encodeURIComponent(reqStrung)+"&callback="+callbackName;
		var script = document.createElement("script");
		script.type = "text/javascript";
		script.src = "http://" + proxyHost + "/xhrasjson?" + params;
		// script.onload = function(e) { GM_log("Script has loaded."); };
		script.onerror = function(e) {
			onerror();
			throw new Error("Failed to get JSONed XHR response from "+proxyHost+" - the server may be down.");
		};
		document.getElementsByTagName("head")[0].appendChild(script);
	};
}

/*
// CONSIDER: We could similarly make a cross-site XMLHttpRequest.
CrossSiteXMLHttpRequestViaJSON = function() {
};
*/


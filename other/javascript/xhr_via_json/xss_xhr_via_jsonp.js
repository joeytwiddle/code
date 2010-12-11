/* Include this script to get a cross-site GM_xmlhttpRequest via the proxy. */

// We always replace Google Chrome's GM_xmlhttpRequest because it is not cross-site.
if (!this.GM_xmlhttpRequest || window.navigator.vendor.match(/Google/)) {
	this.GM_xmlhttpRequest = function(request) {
		var proxyHost = "hwi.ath.cx:8124";
		// We don't want to send functions to the proxy!  TODO: onerror, ... ?
		var onload = request.onload;
		delete request.onload;
		// Insert the callback into the root window, anonymised by a random key.
		var callbackName = "xss_xhr_via_jsonp_callback_" + parseInt(Math.random()*987654321);
		// TODO BUG: Sometimes the callback script cannot find this function!
		// Gah Chrome doesn't give us access to the real window, only access via
		// the DOM.  So this trick currently works from a bookmarklet, but not
		// from a Chrome userscript extension.
		// Script injection is possible as in stackoverflow.com/questions/2303147
		// but then the problem is making a callback to the userscript-scope
		// functions.
		// Nasty workaround: Run userscripts in content scope (outside the
		// sandbox), along with GM_ compatibility functions (since we have most
		// of these now).
		// TODO: Another nasty workaround: Inject into the page as above, but
		// since we can't callback directly to userscript-scope, instead drop the
		// response data into the DOM, and start a timer here here to call the
		// callback when the response data arrives.
		window[callbackName] = function(response) { onload(response); };
		// Request an XHR response from the proxy, which should return some javascript to call the callback.
		var reqStrung = JSON.stringify(request);
		var params = "request="+encodeURIComponent(reqStrung)+"&callback="+callbackName;
		var script = document.createElement("script");
		script.type = "text/javascript";
		script.src = "http://" + proxyHost + "/xhrasjson?" + params;
		script.onerror = function(e) { console.log("Failed to get response from "+proxyHost+" - the server may be down."); };
		document.getElementsByTagName("head")[0].appendChild(script);
	};
}

/*
// CONSIDER: We could similarly make a cross-site XMLHttpRequest.
CrossSiteXMLHttpRequestViaJSON = function() {
};
*/


/* Include this script to get a cross-site GM_xmlhttpRequest via the proxy. */

// We always replace Google Chrome's GM_xmlhttpRequest because it is not cross-site.
if (!this.GM_xmlhttpRequest || window.navigator.vendor.match(/Google/)) {
	this.GM_xmlhttpRequest = function(request) {
		var proxyHost = "hwi.ath.cx:8124";
		// We don't want to send functions to the proxy!  TODO: more than onload?  onerror, ... ?
		var onload = request.onload;
		delete request.onload;
		// Insert the callback into the root window, anonymised by a random key.
		var callbackName = "xss_xhr_via_jsonp_callback_" + parseInt(Math.random()*987654321);

		if (!window.navigator.vendor.match(/Google/) /* || !weAreInUserscriptScope */) {

			// This works fine in Firefox, or in Chrome's content scope.
			window[callbackName] = function(response) { onload(response); };

		} else {

			// But the window in Chrome userscripts is sandboxed, and does not
			// always update from one to the other.
			// So we must get Chrome's unsafeWindow (the real content window).
			var div = document.createElement("div");
			div.setAttribute("onclick", "return window;");
			unsafeWindow = div.onclick();

			unsafeWindow[callbackName] = function(response) { onload(response); };

			/*
			// In Chrome userscripts/extensions, we need to perform two workarounds:
			// 1) We can't simply place a function into the content-scope, we have to inject it.
			// 2) That injected function can't call our callback directly, so
			//    instead it leaves a message in messageElement, which we pick up
			//    here in userscript-scope with a timer.
			var messId = callbackName+"_message_element";
			// var messageElement = document.createElement("span"); messageElement.style.display = "none"; messageElement.id = messId; document.body.appendChild(messageElement);
			var createMessageElementString = '(function(){ var messageElement = document.createElement("span"); messageElement.style.display = "none"; messageElement.id = "'+messId+'"; document.body.appendChild(messageElement); })();';
			var setupCallbackFunction = 'window["'+callbackName+'"] = function(response) { document.getElementById("'+messId+'").responseMessage = JSON.stringify(response); window.status = ("Set responseMessage of '+messId+'"); };';
			var injectedScriptString = createMessageElementString + setupCallbackFunction;
			var injectedScriptElement = document.createElement("script");
			// injectedScriptElement.innerHTML = injectedScriptString;
			injectedScriptElement.appendChild(document.createTextNode(injectedScriptString));
			// (document.body || document.head || document.documentElement).appendChild(injectedScriptElement);
			document.getElementsByTagName("head")[0].appendChild(injectedScriptElement);
			var delay = 100;
			function checkForMessage() {
				// TODO BUG: I swear this was working fine in content scope, but
				// when we run as a userscript, it never sees the .responseMessage
				// appear.  And now I've broken it for content scope too.  =/
				var messageElement = document.getElementById(messId);
				if (!messageElement) {
					log("Error: Could not find messageElement with id="+messId);
				} else {
					log("messageElement .id="+messageElement.id+" and .responseMessage="+messageElement.responseMessage);
					if (messageElement.responseMessage !== undefined) {
						onload(JSON.parse(messageElement.responseMessage));
						return;
					}
				}
				delay = delay * 2;
				setTimeout(checkForMessage,delay);
			}
			setTimeout(checkForMessage,delay);
			*/

		}

		// Request an XHR response from the proxy, which should return some javascript to call the callback.
		var reqStrung = JSON.stringify(request);
		var params = "request="+encodeURIComponent(reqStrung)+"&callback="+callbackName;
		var script = document.createElement("script");
		script.type = "text/javascript";
		script.src = "http://" + proxyHost + "/xhrasjson?" + params;
		// script.onload = function(e) { log("Script has loaded."); };
		script.onerror = function(e) { log("Failed to get JSONed XHR response from "+proxyHost+" - the server may be down."); };
		document.getElementsByTagName("head")[0].appendChild(script);
	};
}

/*
// CONSIDER: We could similarly make a cross-site XMLHttpRequest.
CrossSiteXMLHttpRequestViaJSON = function() {
};
*/


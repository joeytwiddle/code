// ==UserScript==
// @name           No IFrames
// @namespace      ....
// @include        http://*.wikia.com/*
// @include        *
// @version        0.0.1
// ==/UserScript==

// TODO: rename IFrameBlock

var keepLocalIframes = true;
var dontBlockJustRemove = false;

// TODO: Turn this into something more like FlashBlock.  e.g. Only act if the
// target domain is different from the parent domain, then show the target
// domain and a button which can be pressed to display the IFrame.

var removed = 0;

var iframes = document.getElementsByTagName("IFRAME");
// console.log("Removing "+iframes.length+" IFrames...");
for (var i=iframes.length-1;i>=0;i--) {
	var iframe = iframes[i];
	if (keepLocalIframes) {
		if (iframe.src.match(/^about:/))
			continue;
		if (iframe.src.match("^"+document.location.protocol+"//"+document.location.hostname+"/"))
			continue;
	}
	if (dontBlockJustRemove) {
		// console.log("Removing IFrame: "+iframe.src);
		iframe.parentNode.removeChild(iframe);
	} else {
		iframe.oldSrc = iframe.src;
		iframe.src = 'about:blank';
		removed++;
		// console.log("IFrame's window = " + iframe.contentWindow);
		// console.log("IFrame's document = " + iframe.contentDocument);
		// if (iframe.contentDocument)
			// console.log("IFrame's document body = " + iframe.contentDocument.body);
	}
}
console.log("[NIF] Removed "+removed+" IFrames.");

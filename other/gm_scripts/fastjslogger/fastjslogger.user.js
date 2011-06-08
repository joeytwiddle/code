// ==UserScript==
// @name           FastJSLogger
// @namespace      FastJSLogger
// @description    Attempts to redirect console.log to a div floating on the page.
// @include        *
// ==/UserScript==
// Dropped: @require        http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js

// This fails to intercept GM_log calls from other userscripts.
// However it intercepts everything when we run bookmarklets.

// TODO: We may be able to capture errors by overriding setTimeout,
// setInterval, XMLHttpRequest and any other functions which use callbacks,
// with our own versions which attempt the callback within a try-catch which
// logs and throws any intercepted exceptions.
// Unfortunately wrapping a try-catch around the main scripts in the document
// (which have alread run) is a bit harder from here.  ;)

(function(){



var st = document.createElement("style");
st.type = "text/css";
st.innerHTML = ".logDiv { position: fixed; right: 8px; top: 8px; width: 320px; height: 120px; background-color: #ffffcc; color: black; font-size: 85%; z-index: 10000; overflow: auto; }";
document.getElementsByTagName("head")[0].appendChild(st);

var logDiv = document.createElement("pre");
logDiv.id = 'fastJSLogger';
logDiv.className = 'logDiv';
logDiv.style.display = 'none';
document.body.appendChild(logDiv);

function addCloseButtonTo(elem) {

	var b = document.createElement("button");
	b.textContent = "X";
	b.style.zIndex = 2000;

	b.onclick = function() {
		GM_log("[",b,"] Destroying:",elem);
		elem.style.display = 'none';
		elem.parentNode.removeChild(elem);
	};

	b.style.float = 'right';   // BUG: not working in FF4!
	elem.appendChild(b);

	// b.style.position = 'absolute';
	// b.style.right = '4px';
	// b.style.top = '4px';
	// document.body.appendChild(b);
	//// BUG: b is not removed from DOM on click!

}
addCloseButtonTo(logDiv);

var oldConsole = this.console; // When running as a userscript in Chrome, cannot see this.console!
var oldGM_log = this.GM_log;

var target = ( this.unsafeWindow ? this.unsafeWindow : window );

var preventInfLoop = null;

target.console = {};
target.console.log = function(a,b,c) {

	if (a+b+c === preventInfLoop) {
		return;
	}
	preventInfLoop = a+b+c;

	// Some of the browsers dislike use of .call and .apply here, e.g. GM in FF4.
	// So to avoid "oldConsole.log is not a function":
	if (oldConsole && oldConsole.log) {
		oldConsole.log(a,b,c);
	}
	if (oldGM_log) {
		oldGM_log(a,b,c);
	}

	// We cannot do arguments.join (FF4)
	var out = "";
	for (var i=0;i<arguments.length;i++) {
		if (i>0) {
			out += " ";
		}
		out += (""+arguments[i]);
	}
	logDiv.appendChild(document.createElement("br"));
	logDiv.appendChild(document.createTextNode(out));

	// Make visible if hidden
	logDiv.style.display = '';
	// Scroll to bottom
	logDiv.scrollTop = logDiv.scrollHeight;

};

target.console.log("FastJSLogger loaded this="+this+" GM_log="+typeof this.GM_log);

//// Not really needed
// this.GM_log = target.console.log;



// Try to intercept errors
// Strategy: Whenever a callback is placed, we should wrap it!

var oldSetTimeout = window.setTimeout;
window.setTimeout = function(fn,ms) {
	var wrappedFn = function(){
		try {
			fn();
		} catch (e) {
			console.log("[ERROR] "+e+" from "+fn);
			throw e;
		}
	};
	oldSetTimeout(wrappedFn,ms);
};

// TODO: all DOM events!  XHR.



})();

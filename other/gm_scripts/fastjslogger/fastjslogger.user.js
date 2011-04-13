// ==UserScript==
// @name           FastJSLogger
// @namespace      FastJSLogger
// @description    Attempts to redirect console.log to a div floating on the page.
// @include        *
// ==/UserScript==
// Dropped: @require        http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js

// This fails to intercept GM_log calls from other userscripts.
// However it intercepts everything when we run bookmarklets.

var st = document.createElement("style");
st.type = "text/css";
st.innerHTML = ".logDiv { position: fixed; right: 4px; top: 4px; width: 320px; height: 120px; background-color: #ffffcc; color: black; font-size: 70%; z-index: 10000; overflow: auto; }";
document.getElementsByTagName("head")[0].appendChild(st);

var logDiv = document.createElement("div");
logDiv.id = 'fastJSLogger';
logDiv.className = 'logDiv';
logDiv.style.display = 'none';
document.body.appendChild(logDiv);

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
	logDiv.style.display = '';
	if (oldConsole && oldConsole.log) {
		oldConsole.log(a,b,c); // to avoid "oldConsole.log is not a function"
	}
	if (oldGM_log) {
		oldGM_log(a,b,c);
	}
	logDiv.appendChild(document.createElement("br"));
	// We cannot do arguments.join (FF4)
	var out = "";
	for (var i=0;i<arguments.length;i++) {
		if (i>0) {
			out += " ";
		}
		out += (""+arguments[i]);
	}
	logDiv.appendChild(document.createTextNode(out));
};

target.console.log("FastJSLogger loaded this="+this+" GM_log="+this.GM_log);

this.GM_log = target.console.log;


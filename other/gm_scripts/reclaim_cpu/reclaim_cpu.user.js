// ==UserScript==
// @name           Reclaim CPU
// @namespace      http://userscripts.org/users/89794   (joeytwiddle)
// @description    Destroys plugins and disables javascript timers on the page, when it has been left idle, so Firefox stops taking CPU when you are not using it.  Useful if you have a slow PC, or you like to open 20 tabs and then do something else without closing them.  It will only stop pages which automatically refresh themselves, if you set the checkSeconds config variable to trigger cleanup <I>before</I> the page reloads itself!
// @include        *
// @exclude        http://userscripts.org/*
// ==/UserScript==

////////
// @exclude        http://*.userscripts.org/*
// @exclude        http://*.youtube.*/*
// @exclude        http://*.other.sites/where/using/cpu/is/a/good/thing

// Fatness not handled by FatFox script (do this yourself!):
// Firefox has an option to stop animated gifs.
// In about:config set image.animation_mode = "once".

//// Config ////

var onlyIfIdle = true;

var idleSeconds = 60; // Page will cleanup if it has not been in view for this many seconds.

var checkSeconds = 15; // When onlyIfIdle==false, will trigger cleanup after
							  // exactly this time.  When onlyIfIdle==true, will
							  // trigger cleanup after at least twice this time.

/*

DONE: Rename to CPU Reclaimer.

DONE: As well as killing JS timers, kill flash apps (remove all object and
embed tags I guess)!

TODO: Optionally remove all gif's.  (Presuming we can't just stop them.)

TODO: Delay the script disabling until user has stopped using the page.  Detect
user-idle by checking if his mouse x/y has changed.  Hmm that might trigger
when not wanted, e.g. when user is just watching a video.  Can we check if the
user's focus has moved away from the current tab to another tab, or off the
browser window entirely?
Maybe we can have a global GM_setvar which can track the last tab(window) to
detect an event (e.g. mouse movement, but we should make this kbd-only
compatible too!).

DONE: Rather than removing elements, replace them with "[ELEMENT REMOVED]".
DONE: We could offer a link/button which the user can click if they want to
restore the element.

TODO: Make the new element match the size of the old element, so the page shape
does not change.

TODO: Is it possible to give the user a way to restart the javascript timers in
the same way?

TODO: Doing this on Google Maps page appears to cause some new transfers to
take place!  Mmm I think maybe Google Maps is pure Javascript anyway?  If so,
it might not be hogging the CPU anyway.

*/

var report = "";





function cleanupCPUHoggers() {
	clearJavascriptTimers();
	removeNastyElements();

	window.status = report;
	if (GM_log) {
		// GM_log("On \""+document.title+"\": "+report);
		GM_log("On \""+document.title+"\": "+report);
	}

	/*
	var d = document.createElement('DIV');
	d.innerHTML = "<style type='text/css'> .fatfoxinfo{ background-color: white; color: black; position: fixed; right: 4px; top: 4px; } </style><div class='fatfoxinfo'>Events stopped.</div>";
	// " + escapeHTML(report) + "
	// document.body.appendChild(d);
	document.body.insertBefore(d,document.body.firstChild);
	*/

}

function clearJavascriptTimers() {
	// Kill any existing Javascript timers:
	// (Thanks to Joel's Bookmarklets at squarefree.com)
	var c, tID, iID;
	tID = setTimeout(function(){}, 0);
	for (c=1; c<1000 && c<=tID; ++c)
		clearTimeout(tID - c);
	iID = setInterval(function(){},1000);
	for (c=0; c<1000 && c<=iID; ++c)
		clearInterval(iID - c);
	// alert("tID was "+tID+" iID was "+iID);
	report += "Stopped timers ("+tID+","+iID+")";
}

function removeNastyElements() {
	// Clear embedded plugins (Flash/Java-applet/...)
	report += " Removed elements";
	removeElemsWithTag("object");
	removeElemsWithTag("embed");
	// removeElemsMatching( function(x){ return x.tagName == "embed" } );
	report += ".";
}

function removeElemsWithTag(tag) {
	var nasties = unsafeWindow.document.getElementsByTagName(tag);
	report += ", "+nasties.length+" "+tag+"s"; // .map("QWERTYUIOPASDFGHJKLZXCVBNM","qwertyuiopasdfghjklzxcvbnm");
	for (var i=nasties.length-1;i>=0;i--) {
		var lastLength = nasties.length;
		var node = nasties[i];
		// We must call a seperate function, to make a new context for the inner
		// function which contains unique variables.
		destroyNasty(node);
	}
}

function destroyNasty(node) {

	//// Grab stuff needed for restoring.
	// var elemHTML = node.outerHTML; // In all browsers except Mozilla :P
	var attribs = "";
	for (var j=0;j<node.attributes.length;j++) {
		var attr = node.attributes[j];
		attribs += ' ' + attr.name + '="' + attr.value.replace(/"/g,'%22') + '"';
	}
	var elemHTML = "<"+node.tagName+""+attribs+"/>";
	var sis = node.nextSibling;

	//// Main bit: remove the offending element:
	var dad = node.parentNode;
	dad.removeChild(node);

	//// Put a Restore link in its place:

	var newNode = document.createElement('SPAN');
	// newNode.appendChild(document.createTextNode("[REMOVED HEAVY ELEMENT "+elemHTML));
	newNode.appendChild(document.createTextNode("["));

	var restoreElement = function(evt) {
		// TODO: We should really create the element type directly, without
		// creating a new SPAN.
		var restoredNode = document.createElement('SPAN');
		restoredNode.innerHTML = elemHTML;
		newNode.parentNode.insertBefore(restoredNode,newNode);
		newNode.parentNode.removeChild(newNode);
		window.status = "Restored: "+elemHTML;
	};
	var restoreLink = unsafeWindow.document.createElement("A");
	restoreLink.textContent = "Restore Removed Plugin";
	restoreLink.onclick = restoreElement;
	restoreLink.title = elemHTML;
	restoreLink.href = "javascript:(function(){})();"; // Just to prevent browser changing page.
	newNode.appendChild(restoreLink);

	newNode.appendChild(document.createTextNode("]"));
	dad.insertBefore(newNode,sis);

}


//// Not GM compatible: 
// window.top.document.cleanupCPUHoggers = cleanupCPUHoggers;
// setTimeout('window.top.document.cleanupCPUHoggers();',idleSeconds*1000);
//// GM double-plus good:
// setTimeout(cleanupCPUHoggers,idleSeconds*1000);

/*
// Old style:
setTimeout(' (function () { var c, tID, iID; tID = setTimeout(function(){}, 0); for (c=1; c<1000 && c<=tID; ++c) { clearTimeout(tID - c); } iID = setInterval(function(){},1000); for (c=0; c<1000 && c<=iID; ++c) { clearInterval(iID - c); } window.status = "Scripts stopped.  ("+tID+","+iID+")"; })(); ',idleSeconds*1000);
*/



var mouseHasLeft = false;
var lastSawMouse = new Date().getTime();

var idleInfo = document.createElement('DIV');
document.body.insertBefore(idleInfo,unsafeWindow.document.body.firstChild);

var watchMouseMove = function(e) {
	mouseHasLeft = false;
	lastSawMouse = new Date().getTime();
	// idleInfo.textContent = "(" + e.pageX + "," + e.pageY + ")";
	window.status = "(" + e.pageX + "," + e.pageY + ")";
	idleInfo.textContent = "Mouse is back.";
};
// document.body.addEventListener("mousemove", watchMouseMove, false);
window.addEventListener("mousemove", watchMouseMove, false);
window.addEventListener("focus", watchMouseMove, false); // Catches when use re-enters window with keyboard.

var watchMouseLeave = function(e) {
	mouseHasLeft = true;
	lastSawMouse = new Date().getTime();
	window.status = "Mouse has left the window.";
	idleInfo.textContent = "Mouse has left the window.";
}
window.addEventListener("mouseout", watchMouseLeave, false);

// TODO TEST: Is catching the window fine-grained enough?  If we switch to other tabs, does original tab still think it is alive?
// BUG: If I Alt-Tab to get on/off the window, no events are fired.

/*
document.body.addEventListener("mousemove", function(ev){
	window.status = "hello";
}, false);
*/

function checkIdle() {
	var now = new Date().getTime();
	var idleTime = (now-lastSawMouse)/1000;
	idleInfo.textContent = "Last saw mouse "+ idleTime +"s ago.";
	if (idleTime > idleSeconds) {
	} else {
		setTimeout(checkIdle,5000);
	}
}

setTimeout(checkIdle,5000);


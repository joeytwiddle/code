// ==UserScript==
// @name           Reclaim CPU
// @namespace      http://userscripts.org/users/89794   (joeytwiddle)
// @description    Disables all javascript timers on the page after 60 seconds, so Firefox stops chugging at the CPU.
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
var secondsBeforeStoppingJavascript = 60;
// var secondsBeforeStoppingJavascript = 15;

/*

TODO: Rename to CPU Reclaimer.

TODO: As well as killing JS timers, kill flash apps (remove all object tags I
guess)!

TODO: Optionally remove all gif's.  (Presuming we can't just stop them.)

TODO: Delay the script disabling until user has stopped using the page.  Detect
user-idle by checking if his mouse x/y has changed.

TODO: Rather than removing elements, replace them with "[ELEMENT REMOVED]".

TODO: Doing this on Google Maps page appears to cause some new transfers to
take place!  Mmm I think maybe Google Maps is pure Javascript anyway?  If so,
it might not be hogging the CPU anyway.

*/

var report = "";



function cleanupCPUHoggers() {
	clearJavascriptTimers();
	removeNastyElements();

	window.status = report;

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
	var nasties = document.getElementsByTagName(tag);
	report += ", "+nasties.length+" "+tag+"s"; // .map("QWERTYUIOPASDFGHJKLZXCVBNM","qwertyuiopasdfghjklzxcvbnm");
	for (var i=0;i<nasties.length;i++) {
		var lastLength = nasties.length;
		var node = nasties[i];
		node.parentNode.removeChild(node);
		// DONE TEST: has nasties.length now changed?!
		if (nasties.length < lastLength) {
			i--;
			// GM_log('nasties.length did indeed drop.');
			// Yup this happens!
		}
	}
}


//// Not GM compatible: 
// window.top.document.cleanupCPUHoggers = cleanupCPUHoggers;
// setTimeout('window.top.document.cleanupCPUHoggers();',secondsBeforeStoppingJavascript*1000);
//// GM double-plus good:
setTimeout(cleanupCPUHoggers,secondsBeforeStoppingJavascript*1000);

/*
*/

/*
// This works:
setTimeout(' (function () { var c, tID, iID; tID = setTimeout(function(){}, 0); for (c=1; c<1000 && c<=tID; ++c) { clearTimeout(tID - c); } iID = setInterval(function(){},1000); for (c=0; c<1000 && c<=iID; ++c) { clearInterval(iID - c); } window.status = "Scripts stopped.  ("+tID+","+iID+")"; })(); ',secondsBeforeStoppingJavascript*1000);
*/



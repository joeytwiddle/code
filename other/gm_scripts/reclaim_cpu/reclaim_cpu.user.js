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

var bool onlyIfIdle = true;

var checkSeconds = 15; // When onlyIfIdle==false, will trigger cleanup after
							  // exactly this time.  When onlyIfIdle==true, will
							  // trigger cleanup after at least twice this time.

/*

TODO: Rename to CPU Reclaimer.

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
TODO: We could offer a link/button which the user can click if they want to
restore the element.

TODO: Doing this on Google Maps page appears to cause some new transfers to
take place!  Mmm I think maybe Google Maps is pure Javascript anyway?  If so,
it might not be hogging the CPU anyway.

*/

var report = "";



/*
// Detect if the browser is IE or not.
// If it is not IE, we assume that the browser is NS.
var IE = document.all?true:false

// If NS -- that is, !IE -- then set up for mouse capture
if (!IE) document.captureEvents(Event.MOUSEMOVE)

// Set-up to use getMouseXY function onMouseMove
document.onmousemove = getMouseXY;

// Temporary variables to hold mouse x-y pos.s
var tempX = 0
var tempY = 0

// Main function to retrieve mouse x-y pos.s

function getMouseXY(e) {
  if (IE) { // grab the x-y pos.s if browser is IE
    tempX = event.clientX + document.body.scrollLeft
    tempY = event.clientY + document.body.scrollTop
  } else {  // grab the x-y pos.s if browser is NS
    tempX = e.pageX
    tempY = e.pageY
  }  
  // catch possible negative values in NS4
  if (tempX < 0){tempX = 0}
  if (tempY < 0){tempY = 0}  
  // show the position values in the form named Show
  // in the text fields named MouseX and MouseY
  document.Show.MouseX.value = tempX
  document.Show.MouseY.value = tempY
  return true
}
*/



function cleanupCPUHoggers() {
	clearJavascriptTimers();
	removeNastyElements();

	window.status = report;
	if (GM_log) {
		// GM_log("On \""+document.title+"\": "+report);
		GM_log("On \""+window.top.document.title+"\": "+report);
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
	var nasties = document.getElementsByTagName(tag);
	report += ", "+nasties.length+" "+tag+"s"; // .map("QWERTYUIOPASDFGHJKLZXCVBNM","qwertyuiopasdfghjklzxcvbnm");
	for (var i=nasties.length-1;i>=0;i--) {
		var lastLength = nasties.length;
		var node = nasties[i];
		var attribs = "";
		for (var j=0;j<node.attributes.length;j++) {
			var attr = node.attributes[j];
			attribs += ' ' + attr.name + '=' + attr.value;
		}
		// var text = "<FONT size='+1'><B>[Removed "+tag+" Element]</B></FONT>";
		var text = "[REMOVED ELEMENT <"+tag+" "+attribs+"/>]";
		var sis = node.nextSibling;
		var dad = node.parentNode;
		dad.removeChild(node);
		var textNode = document.createTextNode(text);
		dad.insertBefore(textNode,sis);
		/*
		//// No longer needed now we reversed the order of the loop.
		// DONE TEST: has nasties.length now changed?!
		nasties = document.getElementsByTagName(tag); // Sometimes this is needed :f
		if (nasties.length < lastLength) {
			i--;
			// GM_log('nasties.length did indeed drop.');
			// Yup this happens!
		}
		*/
		// nasties = document.getElementsByTagName(tag);
		// GM_log("After doing "+i+" nasties.length="+nasties.length);
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



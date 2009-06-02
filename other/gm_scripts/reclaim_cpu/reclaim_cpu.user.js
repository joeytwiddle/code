// ==UserScript==
// @name           Reclaim CPU
// @namespace      http://userscripts.org/users/89794   (joeytwiddle)
// @description    Stops Firefox from using up CPU unneccessarily, by removing plugins and disabling javascript timers on tabs which have been left idle.  Useful if you have a slow PC, or you like to open 20 tabs and then do something else without closing them.  Allows the user to restart the plugins when they return to the tab, but not the timers.
// @include        *
// @exclude        http://www.last.fm/*
// @exclude        http://www.deezer.com/*
// @exclude        http://*.other.sites/where/we/want/to/leave/the/page/running
// ==/UserScript==

////// Some sites you might like to exclude: //////
// @exclude        http://userscripts.org/*
// @exclude        http://*.userscripts.org/*
// @exclude        http://youtube.*/*
// @exclude        http://*.youtube.*/*

//// Putting it neatly in a whole function() block broke it!
// function() {

////// Config //////

var idleSeconds = 10;       // Page will cleanup if mouse has been outside it
                            // for this many seconds.  Or if you set
                            // detectWhenIdle=false, page will cleanup this
                            // many seconds after loading.

var detectWhenIdle = true;  // detectWhenIdle is much more user-friendly,
                            // although a bit more complicated.
                            // If set to false, cleanup will be performed
                            // automatically after idleSeconds.
                            // If set to true, cleanup is performed after
                            // user has left page idle for idleSeconds.

var reTrigger = false;      // Resume idle detection if the user restores a
                            // hidden element.

var beAggressive = true;    // If set, will cleanup a newly loaded page if it
                            // receives no mouse signals at all after loading.
                            // This means you must move your mouse a tiny amount
                            // over the page after it has loaded, or cleanup
                            // will occur!
                            // But without beAggressive, tabs opened in the
                            // background, or pages which finished loading
                            // after you switched away from Firefox, will not
                            // be marked idle.

                            // When attempting idle detection, if the user
                            // does not move the mouse over the page at all
                            // after it has loaded, Reclaim_CPU
                            // he is watching/reading the page or doing
                            // something else, and we have no knowledge of
                            // whether the mouse is inside the window or not.
                            // beAggressive==true means we will cleanup in this
                            // situation.  You really want this on if you tend
                            // to open 10 tabs, but read only 5 of them.  It is
                            // easy to prevent unwanted triggering, just move
                            // your mouse once over the document you are
                            // reading, *after* it has finished loading.  (We
                            // often do this naturally during normal browsing.)
                            // But beware this could trigger badly on pages
                            // which automatically reload or change URL, e.g.
                            // to present a slideshow.  If so, increase
                            // idleSeconds.

// TODO: var removeAllImages = true; // This might free up some memory.

var stopJavascriptTimers = true;
var removePlugins = true;
var showInfoInTitle = true; // Handy for debugging/watching Reclaim_CPU working.

/* ////// Documentation //////

// As far as Reclaim_CPU is concerned, moving the mouse anywhere outside the
// page is "going idle".  So don't hover on the toolbar if you don't want the
// page to cleanup.  I think it might also be true that if you don't move your
// mouse at all after the page has completed loading (when the script starts),
// it will think you are idle.

// NOTE: One source of CPU hog not handled by Reclaim_CPU script is animated
// gifs.  Firefox has an option to stop animated gifs after one cycle.  You may
// wish to visit about:config and set image.animation_mode = "once".

// Other sources of CPU hog not handled by this script are of course extensions
// and Firefox itself.

// NOTE: Reclaim_CPU will only cleanup pages which automatically refresh
// themselves, IF the idleSeconds triggers cleanup *before* the page reloads
// itself!

// NOTE: If you use Flashbock to load a plugin *after* Reclaim CPU has cleaned
// up the page, it won't know, so it won't clean it up if you go idle later.

*/



/* ////// Developer notes / Changes / Todos //////

TODO: We should delay initialization of the script until the document.onload
event or maybe a short time after with setTimeout.  This will ensure script does
not trigger idle detection when in fact page was still loaded (browser was frozen).

TODO: OK so we clean up all frames where GM is loaded.  But what if an iframe
is created at runtime, with a flash element inside it.  Will our script cleanup
the element?  I fear not.

DONE: Rename to CPU Reclaimer.

DONE: As well as killing JS timers, kill flash apps (remove all object and
embed tags I guess)!

TODO: Optionally remove all gif's.  (Presuming we can't just stop their
animation.)

DONE: Delay the script disabling until user has stopped using the page.  Detect
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

DONE: Will now optionally trigger if the mouse is outside the window when the
script inits.

DONE: What if an <embed> plugin is living inside an IFrame on the page?
Will it be found and destroyed ok?  Yes GM scripts run for each IFrame.

TODO: Provide way to disable from in-page.  Maybe you started listening to a
video/audio plugin, and you do want to leave it running whilst working
elsewhere.  (Little widget next to each plugin?)  (Would we ever want to leave
JS running?  Well anyway in that case it might not be so disruptive to disable
script and reload page).  I am turning reTrigger OFF for this reason!

DONEish The beAggressive issue could be helped by adding a little message to
the page: "User appears idle.  Closing plugins and stopping scripts in 7
seconds..."  Well now showInfoInTitle can help with this.

TODO: Is it silly to clear timers without also clearing events.  It would be
very easy for a mouseover event to start a new timeout chain.  But how likely
is that?  Weigh that against how destructive it would be to remove all events
from a page you wanted to use.

*/



////// Functions which perform the page cleanup. //////

var report;

function cleanupCPUHoggers() {
	report = "";

   if (stopJavascriptTimers)
      clearJavascriptTimers();
   if (removePlugins)
      removeNastyElements();

	window.status = report;
	if (GM_log) {
		// GM_log("On \""+document.title+"\": "+report);
		GM_log(report);
	}

	/*
	var d = document.createElement('DIV');
	d.innerHTML = "<style type='text/css'> .fatfoxinfo{ background-color: white; color: black; position: fixed; right: 4px; top: 4px; } </style><div class='fatfoxinfo'>Events stopped.</div>";
	// " + escapeHTML(report) + "
	// document.body.appendChild(d);
	document.body.insertBefore(d,document.body.firstChild);
	*/

   if (showInfoInTitle) { document.title = "[Clean] " + document.title.replace(/^\[Idling\] /,""); }

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
   // Haha I really don't know what tID and iID mean, but I keep printing them
   // out until I reach enlightenment.
}

function removeNastyElements() {
	// Clear embedded plugins (Flash/Java-applet/...)
	report += (report==''?'R':" and r") + "emoved ";
	removeElemsWithTag("object");
	report += " and ";
	removeElemsWithTag("embed");
	report += " and ";
	removeElemsWithTag("applet");
	// removeElemsMatching( function(x){ return x.tagName == "embed" } );
	report += " from \""+document.title+"\"";
   if (document != top.document) {
      report += " (child frame of \""+top.document.title+"\")";
   }
   report += ".";
}

function removeElemsWithTag(tag) {
	var nasties = unsafeWindow.document.getElementsByTagName(tag);
	report += nasties.length+" "+tag.toUpperCase()+"s"; // .map("QWERTYUIOPASDFGHJKLZXCVBNM","qwertyuiopasdfghjklzxcvbnm");
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
   var sizeConstraints = "";
   if (node.width && node.height) {
      sizeConstraints = "width:"+node.width+"px; height:"+node.height+"px;";
   }
	var dad = node.parentNode;

	//// Put a Restore link in its place:

	var newNode = document.createElement('DIV');
   // BUG: vertical-align is not working.
   // sizeConstraints only works when newNode is a DIV, not a SPAN.
	newNode.innerHTML = "<STYLE type='text/css'> .WhiteOnRed{ color:#660000; background-color:#eeeeee; padding: 2px; font-weight: bold; vertical-align: middle; "+sizeConstraints+" } </STYLE>";
	newNode.className = "WhiteOnRed";
	newNode.appendChild(document.createTextNode("["));

	var restoreElement = function(evt) {
		// TODO: We should really create the element type directly, without
		// creating a new SPAN.
		var restoredNode = document.createElement('SPAN');
		restoredNode.innerHTML = elemHTML;
		newNode.parentNode.insertBefore(restoredNode,newNode);
		newNode.parentNode.removeChild(newNode);
		window.status = "Restored: "+elemHTML;
		if (reTrigger) {
			initTimer();
		}
	};
	var restoreLink = unsafeWindow.document.createElement("A");
	restoreLink.textContent = "Restore Removed Plugin";
	restoreLink.onclick = restoreElement;
	restoreLink.title = elemHTML;
	restoreLink.href = "javascript:(function(){})();"; // Just to prevent browser changing page.
	restoreLink.className = "WhiteOnRed";
	newNode.appendChild(restoreLink);

	newNode.appendChild(document.createTextNode("]"));
	dad.insertBefore(newNode,sis);

   // newNode.style.width = node.width+'px';
   // newNode.style.height = node.height+'px';
   // restoreLink.style.width = node.width+'px';
   // restoreLink.style.height = node.height+'px';

	//// Main bit: remove the offending element:
	dad.removeChild(node);

}



////// Functions which perform idle page detection. //////

function initTimer() {

	if (!detectWhenIdle) {

		//// Cleanup automatically at fixed time after page has loaded.
		setTimeout(cleanupCPUHoggers,idleSeconds*1000);

	} else {

		//// Detect when page is idle (no longer in focus), and cleanup then.

		var mouseHasLeft = false;
		var timerRunning = false;
		var lastSawMouse = new Date().getTime();

		// var idleInfo = document.createElement('DIV');
		// document.body.insertBefore(idleInfo,unsafeWindow.document.body.firstChild);

		function checkIdle() {
			timerRunning = false;
			var now = new Date().getTime();
			var idleTime = (now-lastSawMouse)/1000;
			// idleInfo.textContent = "Last saw mouse "+ idleTime +"s ago.";
			if (mouseHasLeft && idleTime > idleSeconds) {
				cleanupCPUHoggers();
				// DONE: We should also disable all the event watchers.
				window.removeEventListener("mousemove", watchMouseMove, true);
				window.removeEventListener("focus", watchMouseMove, true);
				window.removeEventListener("mouseout", watchMouseLeave, true);
			} else {
				if (mouseHasLeft) {
					// GM_log("Unusual: Mouse has left but checkIdle() has triggered with idleTime only "+idleTime+".  Waiting..."); // Not so unusual really - it just means the user has wiggled their mouse over the edge: out, in, out.
					if (timerRunning) {
						GM_log("Blimey: Another timer started while checkIdle() was running!");
					} else {
						timerRunning = true;
						setTimeout(checkIdle,idleSeconds*1000); // Let's check again later
					}
				} else {
					// GM_log("Stopped timer loop since mouse is back.");
				}
			}
		}
		// DONE: We don't actually need to run the checkIdle timer at all, until the mouse has left!

		var watchMouseMove = function(e) {
         if (mouseHasLeft && showInfoInTitle) { document.title = document.title.replace(/^\[Idling\] /,""); }
			mouseHasLeft = false;
			lastSawMouse = new Date().getTime();
			// idleInfo.textContent = "(" + e.pageX + "," + e.pageY + ")";
			// window.status = "Mouse moved (" + e.pageX + "," + e.pageY + ")";
			// window.status = "Mouse has entered the "+e.target.tagName;
			// idleInfo.textContent = "Mouse is back.";
			return false;
		};

		var watchMouseLeave = function(e) {
			// Oh of course - this is getting triggered for mouseout of all elements!
			// But fortunately it was immediately being disabled by watchMouseMove.
			// Well anyway let's fix that:
			if (!e.target.tagName || e.target.tagName=='HTML' || e.target.tagName=='BODY') {
         } else {
				return false;
         }
			if (!mouseHasLeft) {
				if (timerRunning) {
					// Don't start a timer - we have one!
				} else {
					setTimeout(checkIdle,(idleSeconds+0.5)*1000);
					timerRunning = true;
				}
            if (showInfoInTitle) { document.title = "[Idling] " + (document.title.replace(/^\[Idling\] /,"")); }
			}
			mouseHasLeft = true;
			lastSawMouse = new Date().getTime();
			// window.status = "Mouse has left the "+e.target.tagName;
			// idleInfo.textContent = "Mouse has left the window.";
			// GM_log("Mouse has left the "+e.target.tagName);
			return false;
		}

      // for (var i=0;i<document.body.childNodes.length;i++) {
         // var x = document.body.childNodes[i];
         var x = document.body;
         x.addEventListener("mouseover", watchMouseMove, false);
         x.addEventListener("focus", watchMouseMove, false); // Catches when user re-enters window with keyboard?
         x.addEventListener("mouseout", watchMouseLeave, false);
         // These work better in konq?  They seem to do all depths.
         // x.onmouseover = watchMouseMove;
         // x.onfocus = watchMouseMove;
         // x.onmouseout = watchMouseLeave;
         document.addEventListener("load",function(evt){GM_log("onload has evt="+evt+" mouse=("+evt.pageX+","+evt.pageY+")");},false);
      // }

      // It seems on Konqueror, the last event I caught firing when the mouse
      // was moved out of the window was a mouseover on BODY, not leaving it.
      // I think maybe i get the leave before the enter O_o
      // Hmm I can also produce mouseover BODY by scroll with the wheel =/

		// DONE TEST: Is catching the window fine-grained enough?  If we switch to other tabs, does original tab still think it is alive?  Yes, and No.  It works fine.  window only refers to current tab, not whole browser window.  ;)
		// BUG: If I Alt-Tab to get on/off the window, no events are fired.

		/*
		document.body.addEventListener("mousemove", function(ev){
			window.status = "hello";
		}, false);
		*/

		// setTimeout(checkIdle,5000);

		if (beAggressive) {
         if (showInfoInTitle) { document.title = "[Idling] " + (document.title.replace(/^\[Idling\] /,"")); }
			mouseHasLeft = true;
			timerRunning = true;
			setTimeout(checkIdle,(idleSeconds-0.5)*1000); // -0.5 so it should run twice at least
		}

	}

}

initTimer();


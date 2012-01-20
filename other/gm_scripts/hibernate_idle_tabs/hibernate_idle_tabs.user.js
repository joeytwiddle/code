// ==UserScript==
// @name           Hibernate Idle Tabs
// @namespace      HIT
// @description    If a tab is idle for a long time, navigate to a light holding page until it is used again, to recover memory.
// @include        *
// ==/UserScript==


/* +++ Documentation +++ */
//
// On a normal page, checks to see if the user goes idle.  (Mouse movements,
// key actions and page focus reset the idle timer.)  If the page is left idle
// past the timeout, then the window navigates to a lighter holding page,
// hopefully freeing up memory in the browser.
//
// On a holding page, if the user focuses the window, the window navigates back
// to the original page.  (This can be canceled in Chrome by clicking on
// another tab, but not by paging to another tab with the keyboard!)
//
// In order for the holding page to present the same favicon as the original
// page, we must capture this image before leaving the original page, and pass
// it to the holding page as a CGI parameter.
//
// (A simpler alternative might be to aim for a 404 on the same domain and use
// that as the holding page.)



/* +++ Config +++ */

var hibernateIfIdleForMoreThan = 4*60*60; // 4 hours for me
// var hibernateIfIdleForMoreThan = 10; // for testing
var restoreTime = 0.2; // in seconds

// GM_log("Hibernate Idle Tabs is DISABLED."); return;

//// When we hibernate, we want to go to a mostly-blabk valid webpage somewhere
//// where the userscript can run again.
//// This could be a file:///... URL, but atm we use an http://... URL.
//// Userscripts do not run on about:blank in Firefox 6.0 or Chromium 2011!
// var holdingPage = "about:blank";
//// hwi.ath.cx is not always online!
var oldHoldingPage = "http://hwi.ath.cx/hibernated_tab.html";
//// Let's just spam Google.  This is not really satisfactory, since it provides an image and unneeded CSS!
var holdingPage = "http://www.google.com/hibernated_tab";   // Returns a 404 page we can work on :)



// TODO: Some users may want the hibernated page to restore when the window is focused (when the tab is switched to), rather than waiting for a mouseover.
// TODO: Expose a function to allow a bookmarklet to force-hibernate the current tab?



/* +++ Main +++ */

var onHoldingPage = document.location.href.match(holdingPage+"?") != null;

if (!onHoldingPage) {   // Only need this for a few days.
	onHoldingPage = document.location.href.match(oldHoldingPage+"?") != null;
}

function handleNormalPage() {

	whenIdleDo(hibernateIfIdleForMoreThan,hibernatePage);

	function hibernatePage() {
		var url = document.location.href;
		var targetHost = url.replace(/.*:\/\//,'').replace(/\/.*/,'');
		loadFaviconIntoCanvas(targetHost,reallyHibernatePage);
		function reallyHibernatePage(canvas) {
			document.body.appendChild(canvas);
			var params = {
				hibernate_url: document.location.href,
				hibernate_title: document.title
			};
			if (canvas) {
				try {
					var faviconDataURL = canvas.toDataURL("image/png");
					params.hibernate_favicon_url = faviconDataURL;
				} catch (e) {
					var extra = ( window != top ? " (running in frame or iframe)" : "" );
					GM_log("[HIT] Got error"+extra+": "+e+" doc.loc="+document.location.href);
				}
			}
			var queryString = buildQueryParameterString(params);
			document.location = holdingPage + "?" + queryString;
		}
	}

}

function handleHoldingPage() {

	var params = getQueryParameters();

	// setHibernateStatus("Holding page for " + params.hibernate_title + "\n with URL: "+params.hibernate_url);
	// var mainReport = params.hibernate_title + " (Holding Page)";
	var mainReport = "(" + params.hibernate_title + " - hibernated)";
	setHibernateStatus(mainReport);

	try {
		var faviconDataURL = params.hibernate_favicon_url;
		if (!faviconDataURL) {
			// If we do not have a favicon, it is preferable to present an empty/transparent favicon, rather than leave the browser to show the favicon of the particular site we have chosen to put the holding page on.
			faviconDataURL = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAEklEQVQ4jWNgGAWjYBSMAggAAAQQAAF/TXiOAAAAAElFTkSuQmCC";
		}
		writeFaviconFromDataString(faviconDataURL);
	} catch (e) {
		GM_log(""+e);
	}

	function restoreTab(evt) {
		var url = decodeURIComponent(params.hibernate_url);
		setHibernateStatus("Returning to: "+url);
		document.location.replace(url);
		evt.preventDefault();   // Accept responsibility for the double-click.
		return false;   // Prevent browser from doing anything else with it (e.g. selecting the word under the cursor).
	}

	checkForActivity();

	function checkForActivity() {

		var countdownTimer = null;

		// listen(document.body,'mousemove',startCountdown); // In Firefox this ignore mousemove on empty space (outside the document content), so trying window...
		listen(window,'mousemove',startCountdown); // Likewise for click below!
		// listen(document.body,'blur',clearCountdown); // Does not fire in Chrome?
		listen(window,'blur',clearCountdown); // For Chrome
		listen(window,'mouseout',clearCountdown);
		// listen(document.body,'click',clearCountdown);
		listen(window,'click',clearCountdown);
		listen(window,'dblclick',restoreTab);

		function startCountdown(e) {
			if (countdownTimer != null) {
				// There is already a countdown running - do not start.
				return;
			}
			var togo = restoreTime*1000;
			function countDown() {
				setHibernateStatus(mainReport +
						'\n' + "Tab will restore in "+((togo/1000)|0)+" seconds." +
						'  ' + "Click or defocus to pause." +
						'  ' + "Or double click to restore now!"
						);
				if (togo <= 0) {
					restoreTab();
				} else {
					togo -= 1000;
					if (countdownTimer)
						clearTimeout(countdownTimer);
					countdownTimer = setTimeout(countDown,1000);
				}
			}
			countDown();
		}

		function clearCountdown(ev) {
			if (countdownTimer) {
				clearTimeout(countdownTimer);
			}
			countdownTimer = null;
			var evReport = "";
			if (ev) {
				evReport = " by "+ev.type+" on "+this;
			}
			var report = mainReport + '\n' + "Paused" + evReport + "";
			setHibernateStatus(report);
		}

	}

}

if (onHoldingPage) {
	handleHoldingPage();
} else {
	handleNormalPage();
}



/* +++ Library Functions +++ */

function listen(target,eventType,handler,capture) {
	target.addEventListener(eventType,handler,capture);
}

function ignore(target,eventType,handler,capture) {
	target.removeEventListener(eventType,handler,capture);
}

// Given an object, encode its properties and values into a URI-ready CGI query string.
function buildQueryParameterString(params) {
	return Object.keys(params).map( function(key) { return key+"="+encodeURIComponent(params[key]); } ).join("&");
}

// Returns an object whose keys and values match those of the CGI query string of the current document.
function getQueryParameters() {
	var queryString = document.location.search;
	var params = {};
	queryString.replace(/^\?/,'').split("&").map( function(s) {
		var part = s.split("=");
		var key = part[0];
		var value = decodeURIComponent(part[1]);
		params[key] = value;
	});
	return params;
}

function whenIdleDo(idleTimeoutSecs,activateIdleEvent) {

	var timer = null;
	var pageLastUsed = new Date().getTime();

	function setNotIdle() {
		pageLastUsed = new Date().getTime();
	}

	function checkForIdle() {
		var msSinceLastUsed = new Date().getTime() - pageLastUsed;
		if (msSinceLastUsed > idleTimeoutSecs * 1000) {
			activateIdleEvent();
		}
		setTimeout(checkForIdle,idleTimeoutSecs/5*1000);
	}

	setTimeout(checkForIdle,idleTimeoutSecs*1000);

	listen(document.body,'mousemove',setNotIdle);
	listen(document.body,'focus',setNotIdle);
	listen(document.body,'keydown',setNotIdle);

}



/* +++ Local Convenience Functions +++ */

var statusElement = null;
function checkStatusElement() {
	if (!statusElement) {
		while (document.body.firstChild) {
			document.body.removeChild(document.body.firstChild);
		}
		statusElement = document.createElement("div");
		document.body.insertBefore(statusElement,document.body.firstChild);
	}
}

function setHibernateStatus(msg) {
	msg = ""+msg;
	document.title = msg;

	checkStatusElement();
	statusElement.textContent = msg;
	statusElement.innerText   = msg;   // IE
}






/* +++ Favicon, Canvas and DataURL Magic +++ */

function loadFavicon(targetHost,callback) {
	var favicon = document.createElement('img');
	favicon.addEventListener('load',function() {
		callback(favicon);
	});

	var targetProtocol = document.location.protocol || "http:";

	// If there is a <link rel="icon" ...> in the current page, then I think that overrides the site-global favicon.
	// NOTE: This is not appropriate if a third party targetHost was requested, only if they really wanted the favicon for the current page.
	var foundLink = null;
	var linkElems = document.getElementsByTagName("link");
	for (var i=0;i<linkElems.length;i++) {
		var link = linkElems[i];
		if (link.rel === "icon" || link.rel === "shortcut icon") {
			foundLink = link;
			break;
		}
	}
	if (foundLink) {
		favicon.addEventListener('error',function(){ callback(favicon); });
		favicon.src = foundLink.href;
		// NOTE: If we made the callback interface pass favicon as 'this' rather than an argument, then we wouldn't need to wrap it here (the argument may be evt).
		return;
	}

	var extsToTry = ["jpg","gif","png","ico"];   // iterated in reverse order
	function tryNextExtension() {
		var ext = extsToTry.pop();
		if (ext == null) {
			GM_log("Ran out of extensions to try for "+targetHost+"/favicon.???");
			// We run the callback anyway!
			callback(null);
		} else {
			favicon.src = targetProtocol+"//"+targetHost+"/favicon."+ext;
		}
	}
	favicon.addEventListener('error',tryNextExtension);
	tryNextExtension();
	// When the favicon is working we can remove the canvas, but until then we may as well keep it visible!
}

function writeFaviconFromCanvas(canvas) {
	var faviconDataURL = canvas.toDataURL("image/png");
	// var faviconDataURL = canvas.toDataURL("image/x-icon;base64");
	// GM_log("Got data URL: "+faviconDataURL.substring(0,10+"... (length "+faviconDataURL.length+")");
	writeFaviconFromDataString(faviconDataURL);
}

function writeFaviconFromDataString(faviconDataURL) {

	var d = document, h = document.getElementsByTagName('head')[0];

	// Create this favicon
	var ss = d.createElement('link');
	ss.rel = 'shortcut icon';
	ss.type = 'image/x-icon';
	ss.href = faviconDataURL;
	/*
	// Remove any existing favicons
	var links = h.getElementsByTagName('link');
	for (var i=0; i<links.length; i++) {
		if (links[i].href == ss.href) return;
		if (links[i].rel == "shortcut icon" || links[i].rel=="icon")
			h.removeChild(links[i]);
	}
	*/
	// Add this favicon to the head
	h.appendChild(ss);

	// Force browser to acknowledge
	// I saw this trick somewhere.  I don't know what browser requires it.  But I just left it in!
	var shim = document.createElement('iframe');
	shim.width = shim.height = 0;
	document.body.appendChild(shim);
	shim.src = "icon";
	document.body.removeChild(shim);

}

function loadFaviconIntoCanvas(targetHost,callback) {

	// GM_log("Getting favicon for: "+targetHost);

	var canvas = document.createElement('canvas');
	var ctx = canvas.getContext('2d');

	loadFavicon(targetHost,gotFavicon);

	function gotFavicon(favicon) {
		if (favicon) {
			// GM_log("Got favicon from: "+favicon.src);
			canvas.width = favicon.width;
			canvas.height = favicon.height;
			ctx.drawImage( favicon, 0, 0 );
		}
		callback(canvas);
	}

}

/* This throws a security error from canvas.toDataURL(), I think because we are
trying to read something from a different domain than the script!
In Chrome: "SECURITY_ERR: DOM Exception 18"
*/
// loadFaviconIntoCanvas(document.location.host,writeFaviconFromCanvas);

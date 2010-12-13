// ==UserScript==
// @name           FaviconizeTheWeb
// @description    Adds a favicon for the website next to every external link on the page.  Clearly shows when you will be leaving the current site, and where you will be going!
// @namespace      FTW
// @include        *
//// Delicious already provide favicons on some pages (via yimg).
// @exclude        http://*delicious.com/search*
// @version        1.3
// ==/UserScript==
// Based on FaviconizeGoogle.



// == Config == //

var placeFaviconAfter = false;
var placeFaviconInsideLink = false;
var scaleIcon = 0.75;
var startAt = 50;
var timeoutMillis = 10;
var batchSize = 20;

var alwaysUseGoogle = true;





// == Library Functions == //

var oldSetTimeout = setTimeout;
function setTimeout(callbackFn,delay) {
	// Rather dodgy implementation of batch processing ;)
	if (Math.random()>(1.0/batchSize)) // 0.0 locks up the browser on large pages with many favicons
		callbackFn();
	else
		oldSetTimeout(callbackFn,delay);
}

function getElementsByTagNameAndClassName(tN,cN) {
	return filterListBy( document.getElementsByTagName(tN), function(x){ return x.className==cN } );
}

function getElementsByClassName(cN) {
	return getElementsByTagNameAndClassName("*",cN);
}

function filterListBy(l,c) {
	var ret = new Array();
	for (var i=0;i<l.length;i++) {
		var it = l[i];
		if (c(it)) {
			ret[ret.length] = it;
		}
	}
	return ret;
}

function iterateAndDo(list,iterationFn,delay,i) {
	if (!delay)
		delay=20;
	if (!i)
		i=0;
	function doOne() {
		if (i < list.length) {
			iterationFn(list[i]);
			i++;
			setTimeout(doOne,delay);
		}
	}
	doOne();
}



// == Main == //

function createFaviconFor(url) {

	var host = url.replace(/^[^\/]*:\/\//,'').replace(/\/.*$/,'');
	// if (host == document.location.host) {
		// return null;
	// }
	var img = document.createElement('IMG');
	// img.src = 'http://'+host+'/favicon.ico';

	var imageExtensions = ( alwaysUseGoogle ? [] : ['ico','png','gif','jpg'] );
	function tryExtension() {
		var ext = imageExtensions.pop();
		if (ext) {
			img.src = 'http://'+host+'/favicon.'+ext;
		} else {
			img.title = "Fail to find favicon for "+host;
			img.src = 'http://www.google.com/s2/favicons?domain=' + host; // Google's cache will sometimes provide a favicon we would have missed, e.g. if the site uses .png instead of .ico.  Thanks to NV for suggesting this, and to Google.
			// @consider We could also generate an md5sum and request a gravatar, which might simply allow human recognition of repeats.
		}
	}
	img.addEventListener('error',tryExtension,true);
	tryExtension();

	img.title = ''+host;
	// img.width = 16*scaleIcon;
	// img.height = 16*scaleIcon;
	img.className = 'ftwFavicon';
	// img.style.border = 0;
	// Favicon image elements are hidden until they have fully loaded
	// img.style.display = 'none';
	img.addEventListener('load',function(){ img.style.display = ''; },false);
	return img;
}



var count = 0;

var lastURL = null;

function checkLink(link) {

	if (!link.href) {
		return;
	}
	// Skip relative and same-host links:
	if (link.href.match(/^[/]/) || link.href.match("://"+document.location.host)) {
		return;
	}
	// Skip Javascript links and relative (same-page) anchors
	if (link.href.match(/^#/) || link.href.match(/^javascript:/)) {
		return;
	}
	// Do not faviconize links which contain (or are only) an image
	if (link.getElementsByTagName("IMG").length > 0) {
		return;
	}

	// Let's not create a duplicate favicon if the page offers the same link with two different <A>s:
	var sameAsLast = (link.href == lastURL);
	if (sameAsLast)
		return;
	lastURL = link.href;

	var img = createFaviconFor(link.href);
	img.style.display = 'none';
	var loadListener = function(img){return function(){ img.style.display = ''; };}(img);
	img.addEventListener('load',loadListener,false);
	var targetNode = link;
	if (placeFaviconInsideLink) {
		if (placeFaviconAfter) {
			targetNode.appendChild(img);
		} else {
			targetNode.insertBefore(img,targetNode.firstChild);
		}
	} else {
		if (placeFaviconAfter) {
			targetNode.parentNode.insertBefore(img,targetNode.nextSibling);
		} else {
			targetNode.parentNode.insertBefore(img,targetNode);
		}
	}

	// For Delicious.  Their low-detail links have float:left set, so our
	// images appear to their right, unless...
	if (document.location.href.indexOf("delicious")>=0 && /(^|\s)taggedlink($|\s)/.test(link.className)) {
		img.style.float = 'left';
	}

	// If any website puts a border on their images, we want to remove it
	// img.style.border = '0px';

	count++;
	return;

}

function doIt() {

	// GM_log("doIt() was called!");

	var style = document.createElement('STYLE');
	var padSide = (placeFaviconAfter?'left':'right');
	style.innerHTML = ".ftwFavicon { margin-"+padSide+": "+(scaleIcon/3)+"em; opacity: 0.7; width: "+scaleIcon+"em; height: "+scaleIcon+"em; vertical-align: 0em; }";
	// vertical-align: middle; <-- appears to make alignment worse in Chrome!
	// Settled for vertical-align: 0em; which fits since capital text seem to be about 0.75em tall
	document.getElementsByTagName('head')[0].appendChild(style);

	// var links = document.evaluate("//a[@class='l']",document,null,6,null);
	// var links = filterListBy(document.links, function(x){ return x.className=='l'; } );
	// var links = document.links.filter( function(x){ return x.className=='l'; } );
	// var links = getElementsByTagNameAndClassName("A",'l');
	// Allows it to work on any sites:
	// if (links.length == 0)
	var links = document.getElementsByTagName("A");

	// GM_log("Got links = "+links.snapshotLength);

	iterateAndDo(links,checkLink,timeoutMillis);

}


// doIt();

/* Adding lots of little images to the page can lock up slow browsers a bit, so
 * we wait a bit before starting.
 */

// window.addEventListener('load',function(){
	// setTimeout(doIt,startAt);
// },false);

/* This method ensures a few small chunks of interaction time before we start. */
function startCountdown(total,delay,fnToDo) {
	function doCountdown(msToGo) {
		if (msToGo > 0) {
			setTimeout(function(){
				doCountdown(msToGo-delay);
			},delay);
		} else {
			fnToDo();
		}
	}
	doCountdown(total);
}
startCountdown(startAt,timeoutMillis,doIt);


// ==UserScript==
// @name           FaviconizeTheWeb
// @namespace      FTW
// @include        *
// ==/UserScript==

// Based on FaviconizeGoogle.

// My Bookmarklet does it thus:
// javascript:(function(){for (var i=0;i<document.links.length;i++) {var link = document.links[i];if (link.href.match('^javascript:') || link.href.match('^#')) {continue;}var host = link.href.replace(/^[^\/]*:\/\//,'').replace(/\/.*$/,'');if (host == document.location.host) {continue;}var img = document.createElement('IMG');img.src = 'http://'+host+'/favicon.ico';link.parentNode.insertBefore(img,link);}})();

// Or my Google userscript was thus:

// DONE: Provided more options where to place favicon: by the link or by the
// url, before or after, inside or outside the link.  However in my opinion
// they all suck except the default. ;)

var placeFaviconAfter = false;
var placeFaviconInsideLink = false;
var scaleIcon = 0.75;
var delayExecution = 3000;

function doIt() {

GM_log("doIt() was called!");

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

function createFaviconFor(url) {
	var host = url.replace(/^[^\/]*:\/\//,'').replace(/\/.*$/,'');
	// if (host == document.location.host) {
		// return null;
	// }
	var img = document.createElement('IMG');
	img.src = 'http://'+host+'/favicon.ico';
	img.width = 16*scaleIcon;
	img.height = 16*scaleIcon;
	img.className = 'ftwFavicon';
	img.border = 0;
	// Favicon image elements are hidden until they have fully loaded
	img.style.display = 'none';
	img.addEventListener('load',function(){ img.style.display = ''; },false);
	return img;
}

function getElementsByTagNameAndClassName(tN,cN) {
	return filterListBy( document.getElementsByTagName(tN), function(x){ return x.className==cN } );
}

function getElementsByClassName(cN) {
	return getElementsByTagNameAndClassName("*",cN);
}

var style = document.createElement('STYLE');
var padSide = (placeFaviconAfter?'left':'right');
style.innerHTML = ".ftwFavicon { padding-"+padSide+": 2px; vertical-align: middle; opacity: 0.6; }";
document.getElementsByTagName('head')[0].appendChild(style);

// var links = document.evaluate("//a[@class='l']",document,null,6,null);
// var links = filterListBy(document.links, function(x){ return x.className=='l'; } );
// var links = document.links.filter( function(x){ return x.className=='l'; } );
// var links = getElementsByTagNameAndClassName("A",'l');
// Allows it to work on any sites:
// if (links.length == 0)
var links = document.getElementsByTagName("A");

// GM_log("Got links = "+links.snapshotLength);

// for (var i=0;i<links.snapshotLength;i++) {
	// var link = links.snapshotItem(i);
var lastURL = null;
for (var i=0;i<links.length;i++) {
	var link = links[i];
	if (link.href.match('^javascript:') || link.href.match('^#')) {
		continue;
	}
	//// Skip relative and same-host links:
	if (link.href.match(/^[/]/) || link.href.match("://"+document.location.host)) {
		continue;
	}

	// Let's not create a duplicate favicon if the page offers the same link with two different <A>s:
	var sameAsLast = (link.href == lastURL);
	if (sameAsLast)
		continue;
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
}

}

// doIt();
//// Like most things dammit, adding a few images to the page seems to lock up
//// my Firefox a bit.  So we delay execution of the script, so the user can
//// actually use their browser for a moment.
window.addEventListener('load',function(){
	// setTimeout(doIt,delayExecution);
	//// This method is less realtime based, and more freetime based, i.e. it
	//// gives the user given ms of UNLOCKED FIREFOX TIME before taking action.
	function doCountdown(fnToDo,msToGo) {
		if (msToGo > 0) {
			setTimeout(function(){
				doCountdown(fnToDo,msToGo-200);
			},200);
		} else {
			fnToDo();
		}
	}
	doCountdown(doIt,delayExecution);
},false);


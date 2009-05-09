// ==UserScript==
// @name           FaviconizeGoogle
// @namespace      http://userscripts.org/users/89794
// @description    Adds favicons next to Google search results.
// @include        http://google.*/search?*
// @include        http://www.google.*/search?*
// ==/UserScript==

// DONE: Provided more options where to place favicon: by the link or by the
// url, before or after, inside or outside the link.  However in my opinion
// they all suck except the default. ;)

// Broken images would be messy, but Firefox seems to hide them after a while
// anyway.  We do still see the gap from the image's padding though!
// It might be desirable to check each image actually exists/loads, or remove it.
// Is that possible, without making an http request ourselves?

var placeFaviconByUrl = false;
var placeFaviconAfter = false;
var placeFaviconInsideLink = false;

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

// var links = document.evaluate("//a[@class='l']",document,null,6,null);
var links = filterListBy(document.links, function(x){ return x.className=='l'; } );
// var links = document.links.filter( function(x){ return x.className=='l'; } );

// GM_log("Got links = "+links.snapshotLength);

var style = document.createElement('STYLE');
var padSide = (placeFaviconAfter?'left':'right');
style.innerHTML = ".favicon { padding-"+padSide+": 4px; vertical-align: middle; }";
document.getElementsByTagName('head')[0].appendChild(style);

// for (var i=0;i<links.snapshotLength;i++) {
	// var link = links.snapshotItem(i);
for (var i=0;i<links.length;i++) {
	var link = links[i];
	// if (link.href.match('^javascript:') || link.href.match('^#')) {
		// continue;
	// }
	var host = link.href.replace(/^[^\/]*:\/\//,'').replace(/\/.*$/,'');
	// if (host == document.location.host) {
		// continue;
	// }
	var img = document.createElement('IMG');
	img.src = 'http://'+host+'/favicon.ico';
	img.width = '16';
	img.height = '16';
	img.className = 'favicon';
	img.border = 0;
	var targetNode = (placeFaviconByUrl ? link.parentNode.parentNode.getElementsByTagName('cite')[0] : link);
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


// ==UserScript==
// @name           FaviconizeGoogle
// @namespace      http://userscripts.org/users/89794   (joeytwiddle)
// @description    Adds favicons next to Google search results.  Also works for startpage.com
// @homepage       https://greasyfork.org/en/scripts/7664-faviconizegoogle
// @downstreamURL  http://userscripts.org/scripts/source/48636.user.js
// @version        1.3.3
// @include        /https?:\/\/((www\.)?|encrypted\.|news\.)google\.[a-z]{2,3}(\.[a-z]{2})?\/(search|webhp|\?gws_rd|\?gfe_rd)?.*/
// @include        /https?:\/\/(www\.|[a-z0-9-]*\.)?startpage.com\/.*/
// @grant          none
// ==/UserScript==

var placeFaviconByUrl      = false;   // The little green link below the article title
var placeFaviconAfter      = false;   // Display after the link instead of before it
var placeFaviconInsideLink = false;   // Makes the favicon clickable but may also get underlined

// Some alternatives/remixes:
// - https://github.com/NV/faviconize-google.js (Chrome extension)
// - https://greasyfork.org/en/scripts/12395-google-favicons (works with Endless Google)
// - https://gist.github.com/Sir-Cumference/223d36cbec6473b0e6927e5c50c11568 (very short code, @match works with Greasemonkey)

// DONE: Provided more options where to place favicon: by the link or by the
// url, before or after, inside or outside the link.  However in my opinion
// they all suck except the default. ;)

// Broken images would be messy, but Firefox seems to hide them after a while
// anyway.  We do still see the gap from the image's padding though!
// It might be desirable to check each image actually exists/loads, or remove it.
// Is that possible, without making an http request ourselves?

// Third-party host URL detection is implemented leniently, and accordingly
// hostname extraction implemented aggressively, which results in favicons
// being given to unexpected things like bookmarklets which contain a site url.

/*
function filterListBy (l,c) {
	var ret = new Array();
	for (var i=0;i<l.length;i++) {
		var it = l[i];
		if (c(it)) {
			ret[ret.length] = it;
		}
	}
	return ret;
}
*/

function createFaviconFor (url) {
	var host = url.replace(/^[^\/]*:\/\//,'').replace(/\/.*$/,'');
	// if (host == document.location.host) {
		// return null;
	// }
	// Use protocol (http/https) of current page, to avoid mixed-content warnings/failures.
	var protocol = document.location.protocol.replace(/:$/, '');
	//console.log("[FaviconizeGoogle.user.js] protocol:" ,protocol);
	var img = document.createElement('IMG');
	//img.src = protocol + '://'+host+'/favicon.ico';
	img.src = '//www.google.com/s2/favicons?domain=' + host; // Google's cache will sometimes provide a favicon we would have missed, e.g. if the site uses .png instead of .ico.  Thanks to NV for suggesting this, and to Google.
	//img.src = '//g.etfv.co/http://" + host; // As suggested by decembre
	img.width = '16';
	img.height = '16';
	img.className = 'favicon';
	img.border = 0;
	img.style.display = 'none';
	img.addEventListener('load',function(){ img.style.display = ''; },false);
	return img;
}

function getGoogleResultsLinks () {
	// var links = document.evaluate("//a[@class='l']",document,null,6,null);
	// var links = filterListBy(document.links, function(x){ return x.className=='l'; } );
	// var links = document.links.filter( function(x){ return x.className=='l'; } );

	/*
	return filterListBy(document.getElementsByTagName('a'), function (x) {
		// Most pages show links with class 'l'
		// But on pages where the first result has an indented block of sub-pages,
		// the indented links have class 'l' but all the other links have no class but parent class 'r'
		return x.className === 'l' || x.parentNode.className === 'r';
	});
	*/

	// For Google search
	// a.l
	// a.fl are small one-line sub-results.  Search "squeak" and see the Wikipedia result.
	// a._vQb._mnc are images which appear next to articles on the News tab
	var links = document.querySelectorAll('.g a:not(.fl):not(._vQb):not(._mnc)');

	// For news.google.com
	if (links.length === 0) {
		links = document.querySelectorAll('a.article:not(.esc-thumbnail-link)');
	}

	// For startpage.com
	if (links.length === 0) {
		links = document.querySelectorAll('.clk > a');
	}

	return links;
}

function getElementsByClassName (cN) {
	return getElementsByTagNameAndClassName("*",cN);
}

var style = document.createElement('STYLE');
var padSide = (placeFaviconAfter?'left':'right');
style.innerHTML = ".favicon { padding-"+padSide+": 4px; vertical-align: middle; width: 1em; height: 1em; padding-bottom: 0.2em; }";
document.getElementsByTagName('head')[0].appendChild(style);

function updateFavicons () {
	var links = getGoogleResultsLinks();

	// Allows it to work on any sites:
	if (links.length === 0) {
		links = document.getElementsByTagName("A");
	}
	// console.log("Got links = "+links.snapshotLength);

	// for (var i=0;i<links.snapshotLength;i++) {
		// var link = links.snapshotItem(i);
	for (var i = 0; i < links.length; i++) {
		var link = links[i];
		// if (link.href.match('^javascript:') || link.href.match('^#')) {
			// continue;
		// }
		var targetUrl = link.getAttribute('data-href') || link.href;
		//// Skip relative and same-host links:
		if (targetUrl.match(/^[/]/) || targetUrl.match("://"+document.location.host)) {
			continue;
		}
		//console.log("[faviconizegoogle.user.js] link.getAttribute(data-faviconized):" ,link.getAttribute("data-faviconized"));
		if (link.getAttribute("data-faviconized")) {
			// Already faviconized
			console.log("[faviconizegoogle.user.js] Skipping");
			continue;
		}
		link.setAttribute("data-faviconized", "yes");
		var img = createFaviconFor(targetUrl);
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
}

// TODO: Use MutationObserver instead?

var last_srg = null;

function checkForUpdate () {
	// #ires was needed for the News tab, which doesn't have a .srg.  Perhaps we could use ires for all tabs.
	var new_srg = document.getElementsByClassName("srg")[0] || document.getElementById("ires") || document.querySelector('.web_regular_results');
	//console.log("[FaviconizeGoogle.user.js] last_srg:" ,last_srg);
	//console.log("[faviconizegoogle.user.js] new_srg:" ,new_srg);
	if (new_srg !== last_srg) {
		//console.log("Page change detected!");
		updateFavicons();
		last_srg = new_srg;
	} else {
		//console.log("Pages are the same:", last_srg, new_srg);
	}
	setTimeout(checkForUpdate, 1000);
}

setTimeout(checkForUpdate, 100);

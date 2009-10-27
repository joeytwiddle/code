// ==UserScript==
// @name           Get Page Meta
// @namespace      GPM
// @description    Shows Delicious info for target page in a tooltip when you hover over a link.
// @include        *
// ==/UserScript==
// old require        http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.js

// Code adapted from Hover Links
//// Thanks to:
// http://delicious.com/help/feeds
//// The delicious JSON url we use is:
// http://feeds.delicious.com/v2/json/urlinfo?url=http://www.google.co.uk/
//// Since bookmarklets can't make CD-XHR, we could make a JSONP proxy...
// http://hwi.ath.cx/json_plus_p?url=http://feeds.delicious.com/... ?

// TESTING: onclick should cancel the lookup (user has followed link, no need to query)
//
// BUG: If the XHR request calls callback *after* we have done mouseout, link still
// appears.  Related problem if two requests are being made and neither has run
// callback yet.  Recommend setting/unsetting/checking var targettingLink =
// null or elem;  Or maybe we can use stillFocused.

// TODO: We still need to implement something to prevent a second XHR being
// made if one is already running for a given link.
// OK that is partially implemented now but has bugs.  To test, mouse off and
// straight back on as soon as the XHR starts.

// Give all tags an automatic color by hash, but not too vivid.
// May help us to track common tags.

// == Config ==

var bg_color = "#EAEAEA";
var warn_bg_color = "#EADADA";
var warn_color = "#FF8888";
var border_color = "#AAAAAA";
var font_color = "#000000";
var font_face = "tahoma";
var font_size = "11px";
var max_width = 300;



// == Library functions ==

function getCanonicalUrl(url) {
	if (url.substring(0,1)=="/") {
		url = document.location.protocol + "://" + document.location.domain + "/" + url;
	}
	if (!url.match("://")) {
		// Surely wrong: url = document.location.path + "/" + url;
		// Also fail imo: url = document.location.protocol + "://" + document.location.domain + document.location.pathname + "/" + url;
		url = document.location.href.match("^[^?]*/") + url;
	}
	return url;
}

function getHostnameOfUrl(url) {
	return url.split('/')[2];
	// return url.match(/[^:]*:[/][/][^/]*[/]/)[0];
}

function addCommasToNumber(numString) {
	numString = ""+numString;
	x = numString.split('.');
	x1 = x[0];
	x2 = x.length > 1 ? '.' + x[1] : '';
	var rgx = /(\d+)(\d{3})/;
	while (rgx.test(x1)) {
		x1 = x1.replace(rgx, '$1' + ',' + '$2');
	}
	return x1 + x2;
}

function boldTextElement(txt) {
	var span = document.createElement("SPAN");
	span.appendChild(document.createTextNode(txt));
	span.style.fontWeight = 'bold';
	span.style.fontSize = '1.1em';
	return span;
}



// == Main ==

var dataCache = ({});

var timer;
var stillFocused;
var tooltipDiv;

function positionTooltip(event) {
	if (tooltipDiv) {
		var posx, posy;
		posx = event.clientX + window.pageXOffset;
		posy = event.clientY + window.pageYOffset;
		tooltipDiv.style.left = (posx + 15) + "px";
		tooltipDiv.style.top = (posy + 7) + "px";
		if (tooltipDiv.clientWidth > max_width) {
			tooltipDiv.style.width = max_width;
		}
		var scrollbarWidth = 20;
		// If the cursor is so low in the window that the tooltip would appear
		// off the bottom, then we move it to above the cursor
		if (parseInt(tooltipDiv.style.top) + tooltipDiv.clientHeight + scrollbarWidth > window.innerHeight + window.pageYOffset) {
			tooltipDiv.style.top = (posy - 7 - tooltipDiv.clientHeight) + "px";
		}
		// Similar for the right-hand-side, but beware the browser might have
		// reduced clientWidth in an attempt to make everything fit, so we ignore
		// it and check against clientWidth.  (Better would be to check against
		// its desiredWidth.)
		var divWidth = ( tooltipDiv.clientWidth > max_width ? tooltipDiv.clientWidth : max_width ) + 20;
		if (parseInt(tooltipDiv.style.left) + divWidth + scrollbarWidth > window.innerWidth + window.pageXOffset) {
			tooltipDiv.style.left = (posx - 15 - divWidth) + "px";
			tooltipDiv.style.width = divWidth;
		}
		// The +scrollbarWidth deals with the case when there is a scrollbar
		// which we don't want to be hidden behind!
		// TODO: To prevent the flashing of scrollbar (visible in Konq, invisible
		// but slow in FF), we should use desiredWidth/Height to check the
		// situation before setting bad left/top in the first place.
	}
}

function displayResults(resultObj,subjectUrl,event) {
	// GM_log("Displaying results for "+subjectUrl+": "+uneval(resultObj));
	hideTooltip();

	tooltipDiv = document.createElement("div");
	tooltipDiv.id = "link_tt";
	tooltipDiv.setAttribute("style", "background:" + bg_color + ";border:1px solid " + border_color + ";padding:2px;color:" + font_color + ";font-family:" + font_face + ";font-size:" + font_size + ";position:absolute;z-index:1000;")
	tooltipDiv.style.padding = '6px';

	if (resultObj) {

		if (resultObj.url != subjectUrl) {
			tooltipDiv.style.backgroundColor = warn_bg_color;
			var warningSpan = document.createElement("SPAN");
			if (resultObj.url == getHostnameOfUrl(subjectUrl)) {
				warningSpan.appendChild(document.createTextNode("Results for website"));
			} else {
				warningSpan.appendChild(document.createTextNode("Results for: "+resultObj.url));
			}
			warningSpan.style.backgroundColor = warn_color;
			tooltipDiv.appendChild(warningSpan);
			tooltipDiv.appendChild(document.createElement('BR'));
		}

		// tooltipDiv.appendChild(document.createTextNode("Title: "));
		tooltipDiv.appendChild(boldTextElement(resultObj.title));
		tooltipDiv.appendChild(document.createElement('BR'));

		// tooltipDiv.appendChild(document.createTextNode("Popularity: "));
		// tooltipDiv.appendChild(boldTextElement(""+resultObj.total_posts));
		// var popWidth = Math.log(3 + parseInt(resultObj.total_posts))*30;
		var popWidth = Math.log(parseInt(resultObj.total_posts)/40)*max_width/8;
		if (!popWidth || popWidth<=10) popWidth = 10;
		if (popWidth>max_width) popWidth = max_width;
		var popBar = document.createElement('DIV');
		var thru = popWidth/max_width;
		popBar.style.backgroundColor = 'rgb(128,'+parseInt(127+128*thru)+','+parseInt(255-128*thru)+')';
		popBar.style.width = popWidth+'px';
		popBar.style.padding = '1px 6px 1px 6px';
		popBar.style.margin = '2px 0px 2px 0px';
		// popBar.appendChild(document.createTextNode(" "));
		popBar.appendChild(boldTextElement(addCommasToNumber(resultObj.total_posts)));
		tooltipDiv.appendChild(popBar);

		if (resultObj.top_tags) {
			var tags = "";
			for (var tag in resultObj.top_tags) {
				tags += (tags==""?"":", ") + tag;
			}
			tooltipDiv.appendChild(document.createTextNode("Tags: "+tags+""));
		}

	} else {
		tooltipDiv.appendChild(document.createTextNode("No info for "+subjectUrl));
	}
	document.body.appendChild(tooltipDiv);
	positionTooltip(event);
}

function createTooltip(event) {
	hideTooltip();

	var link = event.currentTarget;

	stillFocused = link;

	if (!link.href.match(/^javascript:/)) {

		var subjectUrl = getCanonicalUrl(link.href);

		if (dataCache[subjectUrl] != null) {
			displayResults(dataCache[subjectUrl],subjectUrl,event);
		} else {

			function tryLookup(lookupURL,onFailFn) {
				var jsonUrl = 'http://feeds.delicious.com/v2/json/urlinfo?url=' + encodeURIComponent(lookupURL);
				GM_xmlhttpRequest({
					method: "GET",
					url: jsonUrl,
					headers: {
						"Accept":"text/json"
					},
					onload: function(response) {
						// GM_log("Delicious responded: "+response.responseText);
						// var resultObj = JSON.parse(response.responseText); // TODO: @require JSON!
						var resultObj = eval(response.responseText); // INSECURE!
						if (resultObj)
							resultObj = resultObj[0];
						if (!resultObj && onFailFn) {
							onFailFn();
						} else {
							dataCache[subjectUrl] = resultObj;
							// GM_log("Got data for "+subjectUrl+" meanwhile link="+link);
							if (stillFocused == link) {
								displayResults(resultObj,subjectUrl,event);
							}
						}
					}
				});
			}

			timer = setTimeout(function() {
				if (stillFocused == link) {
					dataCache[subjectUrl] = "working...";
					tryLookup(subjectUrl,function(){ tryLookup(getHostnameOfUrl(subjectUrl)); });
				}
			},500);

		}

	}

}

function hideTooltip() {
	stillFocused = null;
	if (timer) {
		clearTimeout(timer);
		timer = null;
	}
	if (tooltipDiv) {
		if (tooltipDiv.parentNode) {
			tooltipDiv.parentNode.removeChild(tooltipDiv);
		}
		tooltipDiv = null;
	}
}

for (var i=0; i<document.links.length; i++) {
	var link = document.links[i];
	link.addEventListener("mouseover",createTooltip,false);
	link.addEventListener("mouseout",hideTooltip,false);
	link.addEventListener("mousemove",positionTooltip,true);
	// If user clicks either button on the link, then we hide it
	link.addEventListener("mousedown",hideTooltip,true);
}


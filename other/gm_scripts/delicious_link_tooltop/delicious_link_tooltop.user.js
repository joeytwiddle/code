// ==UserScript==
// @name           Get Page Meta
// @namespace      GPM
// @description    Shows Delicious info for target page in a tooltip when you hover over a link.
// @include        *
// ==/UserScript==
// old require        http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.js



// == Notes ==

// TODO: Rename script "Get Link Meta" or "Get Delicious Info for Links" or
// "Delicious Link Tooltip"

// Some code adapted from Hover Links userscript.
// Thanks to: http://delicious.com/help/feeds
// The delicious JSON url we use is:
//   http://feeds.delicious.com/v2/json/urlinfo?url=http://www.google.co.uk/

// TODO: Since bookmarklets can't make CD-XHR, we could use a JSONP proxy like:
//   http://hwi.ath.cx/json_plus_p?url=http://feeds.delicious.com/... ?

// DONE: onclick should cancel the lookup (user has followed link, no need to query)

// DONE: If the XHR request calls callback *after* we have done mouseout, link still
// appears.  Related problem if two requests are being made and neither has run
// callback yet.  Recommend setting/unsetting/checking var targettingLink =
// null or elem;  Or maybe we can use stillFocused.

// DONE: We prevent a second XHR being made if one is already running for a
// given link, by sacing the "working..." string in the dataCache.

// DONE: Give all tags an automatic color by hash, but not too vivid.  May help
// us to track common tags.

// TODO: Many Mediawiki sites are configured to show a tooltip for external
// links, and this can conflict with our tooltip!
// We could try to stop this in all cases, by doing evt.preventDefault() on all
// mouseovers/outs, but that is a bit strong.
// At least a fix for the Mediawiki problem would make us (me) happy.
// Tried 100x larger zIndex but no success.

// TODO: Does not activate for links added to the document later.  Should add a
// DOMNodeInserted event listener.  Or maybe neater, add listeners to the top
// of the doc, but have them only activate on A elements.



// == Config == //

var max_width = 300;
var getWebsiteInfoOnly = false;   // Shows info for target website, not target page.
var bg_color = "#EAEAEA";
var warn_bg_color = "#EADADA";
var warn_color = "#FF4444";
var border_color = "#AAAAAA";
var font_color = "#000000";
var font_face = "tahoma";
var font_size = "11px";



// == Library functions == //

function hsv2rgbString(h,s,v) {
	// hsv input values range 0 - 1, rgb output values range 0 - 255
	// Adapted from http://www.easyrgb.com/math.html
	var red, green, blue;
	if(s == 0) {
		red = green = blue = Math.round(v*255);
	} else {
		// h should be < 1
		var var_h = h * 6;
		if (var_h == 6) var_h = 0; // TODO: get offset if h<0 or h>1
		var var_i = Math.floor( var_h );
		var var_1 = v*(1-s);
		var var_2 = v*(1-s*(var_h-var_i));
		var var_3 = v*(1-s*(1-(var_h-var_i)));
		if (var_i==0) {
			red = v; 
			green = var_3; 
			blue = var_1;
		} else if(var_i==1) {
			red = var_2;
			green = v;
			blue = var_1;
		} else if(var_i==2) {
			red = var_1;
			green = v;
			blue = var_3
		} else if(var_i==3) {
			red = var_1;
			green = var_2;
			blue = v;
		} else if (var_i==4) {
			red = var_3;
			green = var_1;
			blue = v;
		} else {
			red = v;
			green = var_1;
			blue = var_2
		}
		red = Math.round(red * 255);
		green = Math.round(green * 255);
		blue = Math.round(blue * 255);
	}
	return "rgb("+red+","+green+","+blue+")";
}

function getHash(str) {
	var sum = 0;
	for (i=0;i<str.length;i++) {
		sum = sum + (253*i)*(str.charCodeAt(i));
		sum = sum % 100000;
	}
	return sum;
}

function unescapeHTML(htmlString) {
	var tmpDiv = document.createElement("DIV");
	tmpDiv.innerHTML = htmlString;
	return tmpDiv.textContent;
}

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



// == Main == //

var dataCache = ({});

var timer;
var stillFocused;
var tooltipDiv;

function positionTooltip(event) {
	if (tooltipDiv) {
		var posx = event.clientX + window.pageXOffset;
		var posy = event.clientY + window.pageYOffset;

		tooltipDiv.style.left = (posx + 15) + "px";
		tooltipDiv.style.top = (posy + 7) + "px";

		if (tooltipDiv.clientWidth > max_width) {
			tooltipDiv.style.width = max_width + "px";
		}

		var scrollbarWidth = 20;
		// If the cursor is so low in the window that the tooltip would appear
		// off the bottom, then we move it to above the cursor
		if (parseInt(tooltipDiv.style.top) + tooltipDiv.clientHeight + scrollbarWidth > window.innerHeight + window.pageYOffset) {
			tooltipDiv.style.top = (posy - 7 - tooltipDiv.clientHeight) + "px";
		}
		// Similar for the right-hand-side, but beware the browser might have
		// reduced clientWidth in an attempt to make everything fit, so instead
		// we use divWidth.
		var divWidth = ( tooltipDiv.clientWidth > max_width ? tooltipDiv.clientWidth : max_width ) + 20;
		if (parseInt(tooltipDiv.style.left) + divWidth + scrollbarWidth > window.innerWidth + window.pageXOffset) {
			tooltipDiv.style.left = (posx - 15 - divWidth) + "px";
			// tooltipDiv.style.width = divWidth;
			// TODO: Can move the tooltip too far left, if it is naturally narrow.
		}
		// The +scrollbarWidth deals with the case when there is a scrollbar
		// which we don't want to be hidden behind!
		// TODO: To prevent the flashing of scrollbar (visible in Konq, invisible
		// but slow in FF), we should use desiredWidth/Height to check the
		// situation before setting bad left/top in the first place.
		// This does still occasionally occur, but seems to go away after the
		// first move-to-left.
	}
}

function displayResults(resultObj,subjectUrl,event) {
	// GM_log("Displaying results for "+subjectUrl+": "+uneval(resultObj));
	hideTooltip();

	tooltipDiv = document.createElement("div");
	tooltipDiv.id = "link_tt";
	// tooltipDiv.setAttribute("style", "background:" + bg_color + ";border:1px solid " + border_color + ";padding:2px;color:" + font_color + ";font-family:" + font_face + ";font-size:" + font_size + ";position:absolute;z-index:100000;")
	with (tooltipDiv.style) {
		backgroundColor = bg_color;
		border = "1px solid "+border_color;
		padding = "2px";
		color = font_color;
		fontFamily = font_face;
		fontSize = font_size;
		position = "absolute";
		zIndex = 100000;
	}
	tooltipDiv.style.padding = '6px';

	// Sometimes Delicious returns a result with nothing but the hash and
	// total_posts=1.  These days I am getting this more often than
	// resultObj==null, but it is about as informative, so:
	if (resultObj && resultObj.url=="" && resultObj.title=="" &&
			resultObj.total_posts==1 && resultObj.top_tags.length==0) {
		resultObj = null;
	}
	// However I think this may be due to the fact that there are *private*
	// bookmarks for the URL.  If so, Delicious is a little bit naughty in
	// admitting that they even exist in its database!
	// TODO: Test this theory!

	if (resultObj) {

		if (unescapeHTML(resultObj.url) != subjectUrl) {
			tooltipDiv.style.backgroundColor = warn_bg_color;
			var warningSpan = document.createElement("SPAN");
			if (unescapeHTML(resultObj.url) == getHostnameOfUrl(subjectUrl)) {
				warningSpan.appendChild(document.createTextNode("Results for website"));
			} else {
				// Sometimes Delicious returns a different URL from the one requested.
				// So far this has always seemed fine to me, either adding or
				// removing index.html, or changing a few irrelevant CGI params.
				// TODO/CONSIDER: So maybe the warning colors/message are not needed?
				// Hmm no there is an example where it's not fine.  The "Shopping"
				// link along the top of Google's search results page.  It's
				// actually just a bunch of CGI parameters.  But Delicious drops
				// all the parameters and gives me the results for the top domain
				// page (http://www.google.com/).  This is not the link I wanted
				// info for, so the warnings are needed!
				warningSpan.appendChild(document.createTextNode("Results for: "+resultObj.url));
			}
			warningSpan.style.color = warn_color; // TODO: Oddly this does not get applied in Konqueror!
			tooltipDiv.appendChild(warningSpan);
			tooltipDiv.appendChild(document.createElement('BR'));
		}

		// tooltipDiv.appendChild(document.createTextNode("Title: "));
		// tooltipDiv.appendChild(boldTextElement(resultObj.title));
		// tooltipDiv.appendChild(document.createElement('BR'));
		//// The title can contain HTML-encoded chars, so we must decode/present accordingly
		tooltipDiv.innerHTML += "<B style='font-size:1.1em;'>" + resultObj.title + "</B><BR>";

		// tooltipDiv.appendChild(document.createTextNode("Popularity: "));
		// tooltipDiv.appendChild(boldTextElement(""+resultObj.total_posts));
		// var popWidth = Math.log(3 + parseInt(resultObj.total_posts))*30;
		var popWidth = Math.log(parseInt(resultObj.total_posts)/40)*max_width/8;
		if (!popWidth || popWidth<=10) popWidth = 10;
		if (popWidth>max_width) popWidth = max_width;
		var popBar = document.createElement('DIV');
		var thru = popWidth/max_width;
		// popBar.style.backgroundColor = 'rgb(128,'+parseInt(127+128*thru)+','+parseInt(255-128*thru)+')';
		var hue = 2/3 - 1/3*thru;   // blue -> cyan -> green
		// var hue = thru/3;        // red -> yellow -> green
		popBar.style.backgroundColor = hsv2rgbString(hue, 0.4, 0.9);
		popBar.style.width = popWidth+'px';
		popBar.style.padding = '1px 6px 1px 6px';
		popBar.style.margin = '2px 0px 2px 0px';
		// popBar.appendChild(document.createTextNode(" "));
		popBar.appendChild(boldTextElement(addCommasToNumber(resultObj.total_posts)));
		tooltipDiv.appendChild(popBar);

		if (resultObj.top_tags) {
			/*
			//// Simple list
			var tags = "";
			for (var tag in resultObj.top_tags) {
				tags += (tags==""?"":", ") + tag;
			}
			tooltipDiv.appendChild(document.createTextNode("Tags: "+tags+""));
			*/
			//// List with colored tags
			var first = true;
			for (var tag in resultObj.top_tags) {
				if (!first)
					tooltipDiv.appendChild(document.createTextNode(", "));
				first = false;
				var tagSpan = document.createElement("SPAN");
				tagSpan.appendChild(document.createTextNode(tag));
				tagSpan.style.color = hsv2rgbString( (getHash(tag)%256)/256, 1.0, 0.5 );
				tooltipDiv.appendChild(tagSpan);
			}
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

		if (dataCache[subjectUrl] == "working...") {
			return;  // We can't do anything useful here.  We must wait for the XHR to respond.
		}

		if (dataCache[subjectUrl] != null) {
			displayResults(dataCache[subjectUrl],subjectUrl,event);
		} else {

			// Closure around link and subjectUrl.
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
						if (!resultObj && onFailFn && stillFocused==link) {
							onFailFn();
						} else { // Got resultObj, or no onFailFn, or no longer focused
							// Overwrite "working..." with the good or failed result
							dataCache[subjectUrl] = resultObj;
							// GM_log("Got data for "+subjectUrl+" meanwhile link="+link);
							if (stillFocused == link) {
								displayResults(resultObj,subjectUrl,event);
							}
						}
					}
				});
				// TODO: The GM_xmlhttpRequest should also include an
				// onreadystatechange function to catch any failed requests.
			}

			timer = setTimeout(function() {
				if (stillFocused == link) {
					dataCache[subjectUrl] = "working...";
					var hostUrl = "http://"+getHostnameOfUrl(subjectUrl)+"/";
					if (getWebsiteInfoOnly) {
						subjectUrl = hostUrl;
						tryLookup(subjectUrl,null);
					} else {
						tryLookup(subjectUrl,function(){ tryLookup(hostUrl); });
					}
				}
			},1000);

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

// Initialise
for (var i=0; i<document.links.length; i++) {
	var link = document.links[i];
	link.addEventListener("mouseover",createTooltip,false);
	link.addEventListener("mouseout",hideTooltip,false);
	link.addEventListener("mousemove",positionTooltip,true);
	// If user clicks either button on the link, then we hide it
	link.addEventListener("mousedown",hideTooltip,true);
}


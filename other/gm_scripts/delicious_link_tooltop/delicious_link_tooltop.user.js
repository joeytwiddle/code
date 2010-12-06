// ==UserScript==
// @name           Delicious Link Tooltop
// @namespace      GPM
// @description    Shows Delicious info for the target page in a tooltip when you hover over a link.
// @include        *
// ==/UserScript==
// don't require   http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.js
// @require        http://json.org/json2.js

var showTooltips = true;
var lookupCurrentPage = true;
var annotateAllLinks = true;



//// NOTES for Delicious Network Add:
// GET http://delicious.com/settings/networkadd?networkadd=EHKNIGHT&.crumb=7FsAGAcj8szIc7Pt_37ua1qsjMM-
// GET http://delicious.com/network?add=EHKNIGHT
// GET http://delicious.com/EHKNIGHT?networkaction=1
//// When user hits OK:
// POST http://delicious.com/settings/networkadd?.crumb=7FsAGAcj8szIc7Pt_37ua1qsjMM-&jump=%2FEHKNIGHT%3Fnetworkaction%3D1&network-action-ok=&networkadd=EHKNIGHT
// redirects to: http://delicious.com/EHKNIGHT?networkaddconfirm=EHKNIGHT

// TODO: Don't annotate images, duplicates of last, or anything on delicious.com




// log = (unsafeWindow.console && unsafeWindow.console.log) ? unsafeWindow.console.log : GM_log;
// log = GM_log;
function log(x) {
	if (this.GM_log) {
		GM_log(x);
	} else if (this.console && console.log) {
		console.log(x);
	}
	window.status = ""+x;
};


/*
// == Silly scrolling logger == //
var scrollText = "";

function showScroller() {
	window.status = scrollText.substring(0,240).replace(/\s/,' ','g');
}

function animateScroller() {
	showScroller();
	if (scrollText.length > 0)
		setTimeout(animateScroller,200);
	scrollText = scrollText.substring(10);
}

var oldLog = log;
log = function(x) {
	oldLog(x);
	if (scrollText == "") // if not, there is probably already a timeout running
		setTimeout(animateScroller,2000);
	scrollText = scrollText + "     " + x;
	showScroller();
};
*/



// == Notes == //

// DONE: Rename script "Get Link Meta" or "Get Delicious Info for Links" or
// "Delicious Link Tooltip" with an accidental typo then upload it.

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
// given link, by saving the "working..." string in the dataCache.

// DONE: Give all tags an automatic color by hash, but not too vivid.  May help
// us to track common tags.

// TODO: Many Mediawiki sites are configured to show a tooltip for external
// links, and this can conflict with our tooltip!
// We could try to stop this in all cases, by doing evt.preventDefault() on all
// mouseovers/outs, but that is a bit strong.
// At least a fix for the Mediawiki problem would make us (me) happy.
// Tried 100x larger zIndex but no success.
// Maybe it's the browser's display of the alt or title property.

// DONE: Activate for links added to the document later.  Should add a
// DOMNodeInserted event listener.  Or maybe neater, add listeners to the top
// of the doc, but have them only activate on A elements.

// I made some of the info in the tooltip into links, but to access them we
// have to allow mouseover the tooltip without it disappearing.  I tried to
// implement that but things haven't been 100% smooth since...

// DONE: Sometimes if we move quickly over to the link, no tooltip will appear.
// DONE: The tooltip should appear wherever we last mousemoved-ed over the
// link, (current cursor pos) rather than where we mouseover-ed (entered it).
// HMMM it seems to me that this is happening when we mouseover the <EM> part
// of a Google result but works fine on the non-EM parts.  So maybe we should
// be using capture?

// DONE: The right-floating popBar/Cont is working for Chrome and Konq, but not FF!
// Forget CSS float, I used a TABLE and all works fine.  :)
// TODO: Tidy up the aftermath.
// Unfortunately the table appears to override our popWidth, but maybe we don't
// need/want that anymore anyway.

// TODO: Automatic annotation's decision about which links to scan could be improved.
// e.g. some search ? links could be checked if they are short.  Some same-host links
// we may also want to checked.  But some URLs we definitely won't want to, e.g. if
// they appear to contain a (large) session ID.



// == Config == //

var max_width = 300;
var getWebsiteInfoOnly = false;   // Shows info for target website, not target page.
var bg_color = "#EAEAEA";
var warn_bg_color = "#EADADA";
var warn_color = "#FF4444";
var border_color = "#AAAAAA";
var font_color = "#000000";
var font_face = ""; // "tahoma";
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

function startsWith(bigStr,smallStr) {
	return (smallStr == (""+bigStr).substring(0,smallStr.length));
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

function addCommasToNumber(num) {
	var str = (num+"").split(".");
	dec=str[1]||"";
	num=str[0].replace(/(\d)(?=(\d{3})+\b)/g,"$1,");
	return (dec) ? num+'.'+dec : num;
}

function boldTextElement(txt) {
	var span = document.createElement("SPAN");
	span.appendChild(document.createTextNode(txt));
	span.style.fontWeight = 'bold';
	span.style.fontSize = '1.1em';
	return span;
}



// == Delicious lookup service, and cache == //

var dataCache = ({});

function doLookup(lookupURL,onSuccess,onFailFn) {
	// lookupSpeed = 3000 + 5000*Math.random(); // Do not poll Delicious again for 3-8 seconds
	lookupSpeed = 3000;
	log("Requesting info for: "+lookupURL);
	// We can use https here, but it is slower.
	var jsonUrl = 'http://feeds.delicious.com/v2/json/urlinfo?url=' + encodeURIComponent(lookupURL);
	GM_xmlhttpRequest({
		method: "GET",
		url: jsonUrl,
		headers: {
			"Accept":"text/json"
		},
		onload: function(response) {
			log("Delicious responded: "+response.responseText.substring(0,80));
			// var resultObj = JSON.parse(response.responseText); // TODO: @require JSON!
			var resultObj = eval(response.responseText); // INSECURE!
			// TODO: Sometimes Delicious return "[]" but this gets evaled as an Object.
			// This is hard for later code to detect, e.g. ==[] does not trigger.
			if (resultObj && resultObj[0] && resultObj[0].total_posts)
				resultObj = resultObj[0];
			if (!resultObj && onFailFn) { // TODO: put back? && stillFocused==link) {
				// Done: Shouldn't we put something non-null in the dataCache, to prevent repeat requests?
				// Oh "Working..." is there.
				GM_log("Failed to get result, may leave \"Working...\" in the DB but won't write it to storage.");
				onFailFn();
			} else { // Got resultObj, or no onFailFn, or no longer focused
				// Overwrite "working..." with the good or failed result
				dataCache[lookupURL] = resultObj;
				dataCache[lookupURL].lastUsed = new Date().getTime();
				GM_setValue("CachedResponse"+lookupURL,uneval(resultObj));
				// log("Got data for "+lookupURL+" meanwhile link="+link);
				// if (stillFocused == link) {
					// showResultsTooltip(resultObj,lookupURL,lastMoveEvent || evt);
				// }
				onSuccess(resultObj,lookupURL);
			}
		}
	});
	// TODO: The GM_xmlhttpRequest should also include an
	// onreadystatechange function to catch any failed requests.
}

function tryLookup(subjectUrl,onSuccess,onFailure) {
	if (dataCache[subjectUrl] == null) {
		dataCache[subjectUrl] = eval(GM_getValue("CachedResponse"+subjectUrl,"null"));
	}
	if (dataCache[subjectUrl] != null && dataCache[subjectUrl]!=[]) {
		// log("Got data for: "+subjectUrl);
		// log("Had it cached: "+uneval(dataCache[subjectUrl])+" (Request:"+subjectUrl+")");
		dataCache[subjectUrl].cacheCount++;
		dataCache[subjectUrl].lastUsed = new Date().getTime();
		// I'm paranoid that setValue takes time to complete!  I guess we could just put it after.
		setTimeout(function(){
			GM_setValue("CachedResponse"+subjectUrl,uneval(dataCache[subjectUrl]));
		},20);
		onSuccess(dataCache[subjectUrl],subjectUrl);
	} else {
		dataCache[subjectUrl] = "working...";
		doLookup(subjectUrl,onSuccess,null);
	}
}

function age(cachedRecord) {
	if (!cachedRecord || cachedRecord.lastUsed == null) {
		// log("Error: record has no date! "+uneval(cachedRecord));
		return 30;
	} else {
		var ageInMilliseconds = (new Date().getTime() - cachedRecord.lastUsed);
		// var ageInMonths = ageInMilliseconds / 86400 / 30;
		var ageInDays = ageInMilliseconds / 86400;
		// Avoid division by zero on current time, and future times
		if (ageInDays <= 0.01)
			ageInDays = 0.01;
		return ageInDays;
	}
}

function cleanupCache() {
	if (typeof GM_listValues === 'undefined') {
		log("Cannot cleanupCache - GM_listValues() is unavaiable.");
	} else {

		var cacheList = GM_listValues();

		// Rather casual method: Keep deleting records at random until we meet
		// our max cache size.
		/*
		while (cacheList.length > 512) {
			var i = parseInt(Math.random() * cacheList.length);
			// log("Deleting "+cacheList[i]+" length is currently "+cacheList.length);
			GM_deleteValue(cacheList[i]);
			// delete cacheList[i];
			cacheList[i] = cacheList[cacheList.length-1];
			cacheList.length--;
		}
		*/

		// Delete the oldest and least-used, keep new/popular entries
		while (cacheList.length > 790) {
			var poorestScore = 99999;
			var poorestScorer = null;
			for (var i=0;i<cacheList.length;i++) {
				if (Math.random() < 0.25) // Only really scan quarter the record set
					continue; // so we sometimes keep new (cacheCount=0) records
				var crURL = cacheList[i]; // e.g. "CachedResponsehttp://..."
				var url = crURL.replace(/^CachedResponse/,'');
				if (dataCache[url] == null) {
					dataCache[url] = eval(GM_getValue(crURL,"null"));
				}
				// All cached urls have a minimum score of 2.
				var thisScore = (dataCache[url].cacheCount+2) / age(dataCache[url]);
				if (thisScore == NaN) // e.g. if dataCache[url] == null
					thisScore = 0.00001;
				if (!poorestScorer || thisScore < poorestScore) {
					poorestScore = thisScore;
					poorestScorer = crURL;
					poorestScorerIndex = i;
				}
			}
			if (poorestScorer == null)
				break;
			log("Cleaning up "+poorestScorer+" with "+ ( dataCache[poorestScorer] ? "age="+age(dataCache[poorestScorer])+" and count="+dataCache[poorestScorer].cacheCount : "score="+poorestScore+" and uneval="+uneval(dataCache[poorestScorer]) ) );
			// log("  Data: " + uneval(dataCache[poorestScorer]) );
			GM_deleteValue(poorestScorer);
			// cacheList = GM_listValues();
			cacheList[poorestScorerIndex] = cacheList[cacheList.length-1];
			cacheList.length--;
		}

	}
}





// == Link tooltip (popup on rollover) == //

var timer;
var stillFocused;
var tooltipDiv;

var rolledOverTooltip = false;

// A different version of tryLookup, which will try to lookup the hostname if the initial URL failed.
function initiateDoubleLookup(lookupURL,onSuccess) {
	function onFailure() {
		var hostUrl = "http://"+getHostnameOfUrl(lookupURL)+"/";
		if (hostUrl != lookupURL) {
			lookupURL = hostUrl;
			tryLookup(lookupURL,onSuccess,function(){log("both url and host lookup failed.");});
		}
	}
	tryLookup(lookupURL,onSuccess,onFailure);
}

function positionTooltip(evt) {
	if (tooltipDiv) {
		var posx = evt.clientX + window.pageXOffset;
		var posy = evt.clientY + window.pageYOffset;

		tooltipDiv.style.right = '';
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
			// tooltipDiv.style.left = (posx - 15 - divWidth) + "px";
			tooltipDiv.style.left = '';
			tooltipDiv.style.right = (window.innerWidth - posx + 15) + "px";
			// tooltipDiv.style.width = divWidth;
			if (tooltipDiv.clientWidth > max_width) {
				tooltipDiv.style.width = max_width + "px";
			}
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

function showResultsTooltip(resultObj,subjectUrl,evt) {
	// if (stillFocused == link) {
	// log("Displaying results for "+subjectUrl+": "+uneval(resultObj));
	hideTooltip();

	tooltipDiv = document.createElement("div");
	tooltipDiv.id = "DLTtooltip";
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
		textAlign = 'left';
	}
	tooltipDiv.style.padding = '6px';

	tooltipDiv.addEventListener('mouseover',function(evt){
			rolledOverTooltip = true;
	},false);

	tooltipDiv.addEventListener('mouseout',function(evt){
			hideTooltipMomentarily();
	},false);

	// Sometimes Delicious returns a result with nothing but the hash and
	// total_posts=1.  These days I am getting this more often than
	// resultObj==null, but it is about as informative, so:
	if (resultObj && resultObj.total_posts==1 /*&& resultObj.hash*/ &&
			resultObj.url=="" && resultObj.title=="" &&
			resultObj.top_tags.length==0) {
		GM_log("Got boring response: "+uneval(resultObj));
		resultObj = null;
	}
	// However I think this may be due to the fact that there are *private*
	// bookmarks for the URL.  If so, Delicious is a little bit naughty in
	// admitting that they even exist in its database!
	// TODO: Test this theory!

	if (resultObj && resultObj.total_posts) {

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

		var titleCont = document.createElement("TD");

		// titleCont.appendChild(document.createTextNode("Title: "));
		// titleCont.appendChild(boldTextElement(resultObj.title));
		// titleCont.appendChild(document.createElement('BR'));
		//// The title can contain HTML-encoded chars, so we must decode/present accordingly
		// titleCont.innerHTML += "<B style='font-size:1.1em;'>" + resultObj.title + "</B><BR>";

		var titleElem = boldTextElement(unescapeHTML(resultObj.title));

		// Make it a link?
		var link = document.createElement("A");
		link.href = 'http://delicious.com/url/view?url='+encodeURIComponent(resultObj.url)+'&show=notes_only';
		link.target = "_blank";
		link.style.paddingRight = '8px';
		link.appendChild(titleElem);
		titleElem = link;

		titleCont.appendChild(titleElem);
		//// For some reason Firefox refuses to notice the addition of this
		//// style, so we do it with a CSS class instead.
		// titleCont.style.float = 'left';
		// titleCont.className = 'dlttLeft';

		// titleCont.appendChild(document.createTextNode("Popularity: "));
		// titleCont.appendChild(boldTextElement(""+resultObj.total_posts));
		// var popWidth = Math.log(3 + parseInt(resultObj.total_posts))*30;
		var popWidth = Math.log(parseInt(resultObj.total_posts)/40)*max_width/8;
		if (!popWidth || popWidth<=10) popWidth = 10;
		if (popWidth>max_width) popWidth = max_width;
		var popBar = document.createElement('SPAN');
		var thru = popWidth/max_width;
		// popBar.style.backgroundColor = 'rgb(128,'+parseInt(127+128*thru)+','+parseInt(255-128*thru)+')';
		// var hue = 2/3 - 1/3*thru;   // blue -> cyan -> green
		// var hue = thru/3;        // red -> yellow -> green
		// var saturation = 0.4;
		// var variance = 0.9;
		var hue = 1.8/3;
		var saturation = 0.6+0.3*thru;
		var variance = 0.9-0.4*thru;
		popBar.style.backgroundColor = hsv2rgbString(hue, saturation, variance);
		popBar.style.color = 'white';
		popBar.style.width = popWidth+'px';
		popBar.style.margin = '2px 0px 2px 0px';
		popBar.style.padding = '3px 8px 2px 8px';
		popBar.style.textAlign = 'right';
		// popBar.appendChild(document.createTextNode(" "));
		// popBar.style.float = 'right';
		popBar.appendChild(boldTextElement(addCommasToNumber(resultObj.total_posts)));

		var popBarCont = document.createElement("TD");
		popBarCont.appendChild(popBar);
		// popBarCont.style.float = 'right';
		// popBarCont.className = 'dlttRight';
		// popBarCont.style.position = 'fixed';
		// popBarCont.style.right = '0px';
		popBarCont.style.textAlign = 'right';
		// popBarCont.align = 'right';
		popBarCont.align = 'right';

		// titleCont.appendChild(popBarCont);

		var topTable = document.createElement("TABLE");
		topTable.width = '100%';
		var topRow = document.createElement("TR");
		topTable.appendChild(topRow);
		topRow.appendChild(titleCont);
		topRow.appendChild(popBarCont);
		tooltipDiv.appendChild(topTable);

		/* top_tags is a hashtable, it has no .length */
		if (resultObj.top_tags /* && resultObj.top_tags.length>0 */ ) {

			// tooltipDiv.appendChild(document.createElement("BR"));

			var tagsCont = document.createElement("P");
			tagsCont.style.marginTop = '4px';
			// tagsCont.style.float = 'right';
			var tagsDiv = document.createElement("DIV");
			tagsDiv.style.textAlign = 'right';

			/*
			//// Simple list
			var tags = "";
			for (var tag in resultObj.top_tags) {
				tags += (tags==""?"":", ") + tag;
			}
			tagsDiv.appendChild(document.createTextNode("Tags: "+tags+""));
			*/

			//// List with colored tags
			var first = true;
			for (var tag in resultObj.top_tags) {
				if (!first)
					tagsDiv.appendChild(document.createTextNode(", "));
				first = false;

				var tagSpan = document.createElement("SPAN");

				tagSpan.appendChild(document.createTextNode(tag));
				tagSpan.style.color = hsv2rgbString( (getHash(tag)%256)/256, 1.0, 0.5 );

				// Make it a link?
				var link = document.createElement("A");
				link.href = "http://delicious.com/tag/"+tag;
				link.target = "_blank";
				link.title = resultObj.top_tags[tag];
				link.appendChild(tagSpan);
				tagSpan = link;

				tagsCont.appendChild(tagsDiv);

				tagsDiv.appendChild(tagSpan);

			}

			tooltipDiv.appendChild(tagsCont);

		}

	} else {
		tooltipDiv.appendChild(document.createTextNode("No info for "+subjectUrl));
	}
	document.body.appendChild(tooltipDiv);
	positionTooltip(evt);
	// log("tooltipDiv.innerHTML = "+tooltipDiv.innerHTML);
}

// Old: These functions make use of the scope vars link and subjectUrl.
// Note: These functions make use of the scope var dataCache.

function createTooltip(evt) {

	hideTooltip();

	var link = evt.target;

	lastMoveEvent = evt;

	stillFocused = link;

	// Links we do not want to act on:
	if (link.href.match(/^javascript:/))
		return;
	if (document.location.hostname == link.hostname
			&& document.location.pathname == link.pathname)
		return;   // Either a link to self, or a # link to an anchor in self

	var subjectUrl = getCanonicalUrl(link.href);
	// Remove any #anchor from the url
	subjectUrl = ""+subjectUrl.match(/^[^#]*/);

	if (dataCache[subjectUrl] == "working...") {
		return;  // We can't do anything useful here.  We must wait for the XHR to respond.
	}

	var waitTime = ( dataCache[subjectUrl] != null ? 300 : 1000 );

	timer = setTimeout(
			function(){
				if (stillFocused==link) {
					initiateDoubleLookup(subjectUrl,function(foundResults,foundUrl) {
						showResultsTooltip(dataCache[foundUrl],foundUrl,lastMoveEvent || evt);
					});
				}
			},waitTime);


}

function hideTooltipMomentarily() {
	rolledOverTooltip = false;
	stillFocused = null;
	setTimeout(function(){
			if (stillFocused == null && !rolledOverTooltip)
				hideTooltip();
	},200);
}

function hideTooltip() {
	stillFocused = null;
	if (timer) {
		clearTimeout(timer);
		timer = null;
	}
	// TODO: iff we are mousingoff the link, we should delay before closing, to see if the user is mousing onto the tooltip, and if so not hide.
	if (tooltipDiv) {
		if (tooltipDiv.parentNode) {
			tooltipDiv.parentNode.removeChild(tooltipDiv);
		}
		tooltipDiv = null;
	}
	rolledOverTooltip = false;
}

function initialiseTooltipListeners() {

	// GM_addStyle(".dlttLeft { float: left; }");
	// GM_addStyle(".dlttRight { float: right; }");

	/*
	for (var i=0; i<document.links.length; i++) {
		var link = document.links[i];
		link.addEventListener("mouseover",createTooltip,false);
		link.addEventListener("mouseout",hideTooltip,false);
		// TODO: We should only really enable the mousemove/out/down events when we have done a mouseover!
		link.addEventListener("mousemove",positionTooltip,false);
		// If user clicks either button on the link, then we hide it
		link.addEventListener("mousedown",hideTooltip,true);
	}
	*/

	// A better event listener, which will respond to links added to the DOM later.
	// DONE: One problem with this method.  If we mouseover a //A/EM then the event
	// doesn't fire!
	// DONE: I guess we better look up the tree at our parentNodes for the A, and maybe
	// even change/fake the evt.target to point to the A!

	// DONE: New bug with the new global event listener.  Now we can't mouseover
	// the tooltip any more.  :s
	// Maybe the problem here is the As inside the tooltip.  But why wasn't that a
	// problem before? :o
	// Solved with checkParentsForId().

	// TODO: One bug with this method is that when moving in or out of the SPAN
	// inside the A, a mouseout then a mouseover get fired on the A.  This is
	// slightly hard to fix, how to we know whether the mouseout should be fired or
	// not?

	// var linksOnly = function(evt) { return (evt && evt.target && evt.target.tagName == "A"); };

	function checkParentsForId(node,id) {
		while (node) {
			if (node.id == id)
				return true;
			node = node.parentNode;
		}
		return false;
	}

	var linksOnly = function(evt) {
		var node = evt.target;
		// We don't want to act on links inside the tooltip
		if (checkParentsForId(node,"DLTtooltip"))
			return null;
		while (node) {
			if (node.tagName == "A")
				return node;
			node = node.parentNode;
		}
		return node;
	};

	addGlobalConditionalEventListener("mouseover",createTooltip,linksOnly);
	addGlobalConditionalEventListener("mouseout",hideTooltipMomentarily,linksOnly);
	// TODO: We should only really enable the mousemove/out/down events when we have done a mouseover!
	// addGlobalConditionalEventListener("mousemove",positionTooltip,linksOnly);
	addGlobalConditionalEventListener("mousemove",function(evt){if (evt.target == stillFocused) { lastMoveEvent=evt; } },linksOnly);
	// If user clicks either button on the link, then we hide it
	addGlobalConditionalEventListener("mousedown",hideTooltip,linksOnly);

	function addGlobalConditionalEventListener(evType,handlerFn,whereToFireFn) {
		document.body.addEventListener(evType,function(evt){
				// if (conditionFn(evt)) {
				var finalTarget = whereToFireFn(evt);
				if (finalTarget) {
					var fakeEvt = ({});
					// Maybe better to do a for (var prop in evt) to construct fakeEvt?
					// Hmm no that acts weird, only showing properties for evt which I
					// have already read!
					// OK then let's just set the ones we know we need:
					fakeEvt.target = finalTarget;
					fakeEvt.clientX = evt.clientX;
					fakeEvt.clientY = evt.clientY;
					/* if (evType != "mousemove") {
						log("Performing "+evType+" on "+fakeEvt.target);
					} */
					return handlerFn(fakeEvt);
				}
		},true);
	}

}



// == Initialise Tooltips == //

if (showTooltips) {
	initialiseTooltipListeners();
}



// == Initialise current-page auto-lookup == //

function createScoreSpan(resultObj) {
	var scoreSpan = document.createElement("span");
	if (resultObj.total_posts) {
		var text = resultObj.total_posts;
		scoreSpan.appendChild(boldTextElement(text));
		var greatness = 80 - 30 * Math.log(resultObj.total_posts) / Math.log(10000);
		// scoreSpan.style.backgroundColor = hsv2rgbString(2/3,greatness,0.8);
		scoreSpan.style.backgroundColor = "hsl(240,70%,"+greatness+"%)";
	}
	scoreSpan.style.color = 'white';
	scoreSpan.style.fontWeight = 'bold';
	if (resultObj.top_tags) {
		var tagArray = [];
		for (var tag in resultObj.top_tags) {
			tagArray.push(tag);
		}
		// Not needed - they are already sorted
		tagArray.sort( function(a,b) {
				return resultObj.top_tags[a] < resultObj.top_tags[b];
		});
		scoreSpan.title = tagArray.join(", ");
		// scoreSpan.title = "Popularity: "+addCommasToNumber(resultObj.total_posts) + " Tags: "+tagArray.join(", ");
		// scoreSpan.title = addCommasToNumber(resultObj.total_posts)+" "+tagArray.join(", ");
	}
	return scoreSpan;
}

if (lookupCurrentPage) {
	try {
		tryLookup(document.location.href,function(resultObj,subjectUrl,evt){
			if (resultObj.total_posts) {
				var lc_div = createScoreSpan(resultObj);
				lc_div.style.position = 'fixed';
				lc_div.style.top = '20px';
				lc_div.style.right = '20px';
				lc_div.style.padding = '4px';
				lc_div.style.fontSize = '2.0em';
				document.body.appendChild(lc_div);
			}
		});
	} catch (e) {
		log("Caught exception: "+e);
	}
}



// == Initialise all-links auto-lookup == //

var lookupSpeed = 50;

function addLabel(link) {
	var sameHost = (link.host==document.location.host);
	var badHost = (link.host=="webcache.googleusercontent.com") || (link.host.match(".google.com"));
	var goodUrl = startsWith(link,"http://") || startsWith(link,"https://") || (""+link).indexOf(":")==-1; // skip any "about:config" or "javascript:blah" links
	var isSearch = link.href.indexOf("?")>-1;
	if (link.href && !sameHost && !badHost && goodUrl && !isSearch) {

		function addAnnotationLabel(resultObj,subjectUrl,evt){
			// log("Adding annotation for "+link.href+" with result "+uneval(resultObj));
			if (resultObj && resultObj.total_posts) {
				var newDiv = createScoreSpan(resultObj);
				var text = " " + (resultObj && addCommasToNumber(resultObj.total_posts)) + " ";
				// BUG TODO: Sometimes the left or right space gets ignored, because we are in a span, and it merges with outer text
				newDiv.textContent = text;
				newDiv.style.marginLeft = '0.2em';
				newDiv.style.marginRight = '0.4em';
				newDiv.style.padding = '0.1em 0.2em 0.1em 0.2em';
				// newDiv.style.padding = '0';
				newDiv.style.verticalAlign = "middle";
				// newDiv.style.verticalAlign = "super";
				// newDiv.style.marginTop = '-1.5em'; // For "super", sometimes works.
				// newDiv.style.marginBottom = '-1.5em'; // Try to not make our parent line grow taller
				newDiv.style.opacity = 0.7;
				// newDiv.style.fontSize = parseInt((greatness+0.1)*10)/10 + 'em';
				newDiv.style.fontSize = '0.6em';
				// newDiv.style.paddingBottom = "0.2em";
				// newDiv.firstChild.style.backgroundColor = "hsv("+2/3+","+greatness+",0.8)";
				link.parentNode.insertBefore(newDiv,link.nextSibling);
				// link.parentNode.appendChild(newDiv);
				// newDiv.addEventListener('mouseover',function(evt){ showResultsTooltip(resultObj,subjectUrl,evt); },false);
				// newDiv.addEventListener('mouseout',hideTooltipMomentarily,false);
			}
		}

		tryLookup(link.href,addAnnotationLabel);
	}
}

if (annotateAllLinks) {
	(function(){
		var links = document.getElementsByTagName("A");
		var i = 0;
		function doOne() {
			if (i < links.length) {
				addLabel(links[i]);
				i++;
				if (lookupSpeed <= 50 && Math.random()<0.8) {
					doOne();
				} else {
					setTimeout(doOne,lookupSpeed);
					lookupSpeed = 20;
				}
			} else {
				log("Considered "+i+" links on "+document.location+" for annotation with Delicious labels.");
			}
		}
		doOne();
	})();
}



// == Cleanup old cached data == //

if (Math.random() < 10.1) { // TODO 0.1
	setTimeout(cleanupCache,15000);
}


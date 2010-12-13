// ==UserScript==
// @name           track_history
// @namespace      http://userscripts.org/users/89794   (joeytwiddle)
// @description    Presents a list of the group of links including the one that took you to the current page.  Allows you to go to a neighbouring link without having to go Back.  THIS IS BETA PREVIEW OF THE FINAL SCRIPT.  I recommend you grab the Userscripts Updater, so that you know when I release the 1.0 version.
// @include        *
// ==/UserScript==

//// Config ////

var maxPageHistory = 16;
var goForwards = false; // Whether to navigate forward in browser when following a generated link, otherwise we replace the current page.  Disadvantage with the replace method, is that your browser won't colour the visited links differently.
var useReferrerOnly = false; // (!goForwards); // Argh document.location.replace() also updates referrer.  Leave this false or the script will only work on the first result!
var verboseAlternatives = false; // Whether to show info when we had multiple options for the parentPage.
var maxLinks = 100;
var dontBeDaft = false; // Hides info when it's not useful.  Only disable this for debug/development.
var showFavicons = true;


/**

//// INTRO ////

Original idea which has since changed:
Present a list/tree of the browsing path you took to reach the current page,
for easy navigation back (or sideways), and to keep track of how the hell
you got to this tab.

How the idea developed:
As well as displaying the tree of other pages, we could display *all* the
links from the previous page, to allow fast switching.  (Or better: all the
links with the same xpath?).



//// CONSIDER ////

Curious: How does http://stories.swik.net/jaunty_jackalope_ubuntu_springs_into_beta
add images next to the links *after* I have generated them?!  stylez?

We could provide the feature that the history thingy will switch between
the offered links using hidden/shown IFrames, a bit like tags.

An alternative (more powerful, maybe lighter, but more invasive) approach might
be to detect onclick events, and at that moment store the relevant link data
for this page (maybe even associate with current tab somehow?!).



//// TODOs ////

Let's reduce data to only that we need.  ATM it's too much!
Do this by waiting for onclik on an <A href>.  When caught, scan the document but only for same-group links as the one clicked.  Only these need go to the data.
This will mean slowdown at page-load is gone, but we get some slowdown on link-click instead.

Other sorts of history we want:
Three icons providing drop-down lists:
  - Recent Google Searches (tree of descendents)
  - Recent history
  - Recent history on this website

We keep getting renderered according to the page styles.  We need to disable
that, so that we look the same all the time!
Provide <Previous and Next> buttons.

When we have multiple options for parentNode, we could choose the one with
the most links in group (although group is not calculated yet).
Or we could choose the oldest referring page (probably more stable for user
to stick with the first one he used).  (This would require new date property
firstSeen.)

TODO: We have made data an Object rather then an Array, so that it serializes.
But this is creating some inefficiencies.
Maybe better to make data an Array, but convert it to/from Object during serialization.

BUG: The script loses track if the link we follow actually redirects to
another address.  (Probably not fixable.)
Ofc it also loses track if we get new page by filling and submitting form.

TODO: Make the popup draggable, in case user would prefer it in a different
position.  (And remember position?)

Goes slow on big pages with many links (was invoking "Unresponsive Script"
window before maxLinks.) This may be due to the slow XPath generator for
each link, and the recursive nature of that function.  This huge list of
links will also slow down the script for other pages, because they are there
in the data and will be searched.
HELPED: Refuse to store more than maxLinks links from each page!

Actually the slowdown is probably due to the searching?
One solution for faster system would be to store the data indexed by link
URL, rather than by page URL.  But we would still need to be able to
retrieve all the links for one page.
The search goes in 2 stages:
  1) Find all pages which have this page as a link
  2) Find all links on that page, then do XPath grouping.
So in fact using either indexing method, we will still end up searching ALL
the links in our DB, either in stage 1 or 2.
Unless ... we use double indexing!
That would double the size of our DB, but greatly improve speed.

Also, the cleanup method is a little slow.  Maybe better to build and sort
list of (age,page) pairs, then select oldest few for cleanup.



//// DONE ////

Now tracking secondary (followed) links.
Now cleaning up old data.
Made the links use location.replace(newURL), so user's Back button will now
skip those intermediary pages.

WENT AWAY: We were getting 'undefined' parentPage and links, but I think that
bug is gone now.

**/



var html = ""; // Output report - may be written to by various sub-functions.



//// General functions ////

function getXPath(node) {
	var parent = node.parentNode;
	if (!parent) {
		return '';
	}
	var siblings = parent.childNodes;
	var totalCount = 0;
	var thisCount = -1;
	for (var i=0;i<siblings.length;i++) {
		var sibling = siblings[i];
		if (sibling.nodeType == node.nodeType) {
			totalCount++;
		}
		if (sibling == node) {
			thisCount = totalCount;
			break;
		}
	}
	return getXPath(parent) + '/' + node.nodeName.toLowerCase() + (totalCount>1 ? '[' + thisCount + ']' : '' );
}

function escapeHTML(s) {
	// return s.split("&").join("&amp;").split( "<").join("&lt;").split(">").join("&gt;")
	return s.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/'/g,"&apos;").replace(/"/g,"&quot;");
}

// NOTE: this drops the keys from the Object-map entirely, keeping only the values.
function objectToArray(o) {
	var ret = new Array();
	for (var key in o) {
		var value = o[key];
		// GM_log("objectToArray: '"+key+"' -> '"+value+"'");
		ret[ret.length] = value;
	}
	return ret;
}

function getItemsMatching(l,c) {
	if (!l.item && !l.length) {
		// GM_log("Could not process (non-list) "+l+" with condition "+c);
		// return;
		l = objectToArray(l);
	}
	if (l.item) {
		GM_log("getItemsMatching() has actually never been testing with .item()");
	}
	var ret = new Array();
	for (var i=0;i<l.length;i++) {
	// for (var it in l) { // working ok? yes but not for real arrays :P
		var it = ( l.item ? l.item(i) : l[i] );
		// if (it && it.url && Math.random()<0.1)
			// GM_log("  Running condition on "+it.url);
		if (!it) {
			//// TODO: Why is this happening?
			// GM_log("  That is odd - it["+i+"] = "+it);
			continue;
		}
		if (c(it)) {
			ret[ret.length] = it;
		}
	}
	return ret;
}

function getIndexOf(o,l) {
	for (var i=0;i<l.length;i++) {
		if (l[i] == o)
			return i;
	}
	return -1;
}

function makeLinkAbsolute(url) {
	if (url.indexOf("://") >= 0) {
		return url;
	} else {
		var docTop = "" + document.location;
		var i = docTop.lastIndexOf('/');
		if (i>=0) {
			docTop = docTop.substring(0,i+1);
		}
		GM_log("Absolutized "+url+" to "+docTop+url);
		return docTop + url;
	}
}

function stripAnchor(url) {
	return url.replace(/#[^#]*$/,'');
}

function arrayToJSON(a) {
	var out = "[{";
	var first = true;
	for (var key in a) {
		if (first)
			first=false;
		else
			out += ", ";
		out += uneval(key) + ":" + uneval(a[key]);
	}
	out += "}]";
	return out;
}



//// Data collection and persistence ///

function saveData() {
	// GM_setValue('pageHistoryData',uneval(data));
	GM_log(new Date()+" Preparing data with uneval...");
	var unevaled = uneval(data);
	GM_log(new Date()+" Calling GM_setValue...");
	GM_setValue('pageHistoryData',unevaled);
	GM_log(new Date()+" Done.");
}

function loadData() {
	var dataStr = GM_getValue('pageHistoryData');
	try {
		data = eval(dataStr);
	} catch (e) {
		GM_log("loadData() failed to parse dataStr: "+e);
		GM_log("  dataStr = "+dataStr);
		// I really want to warn but gotta stay GM compatible.
		// document.writeln("<BR/>track_history.loadData() failed - sorry your data was corrupt and destroyed.  Check the log.");
		html += "Sorry Data Corrupted. uneval failed with:<BR/>\n"+escapeHTML(""+e)+"<BR/>\n";
	}
}

function addDataForThisPage() {
	var pageData = new Object();
	pageData.url = ""+document.location; // stripAnchor?
	pageData.title = document.title;
	pageData.referrer = document.referrer;
	pageData.lastUsed = new Date().getTime();
	pageData.groups = new Object();
	var links = document.links;
	for (var i=0;i<links.length;i++) {
		// var link = document.links[i];
		var link = links[i];
		if (link.href.match('^\s*javascript:'))
			continue;
		// TODO: other protocols we should skip: "news:rec.arts.sf.written", "mailto:...", ...
		// var j = pageData.links.length;
		if (i >= maxLinks) {
			GM_log("Too many links ("+links.length+") - skipping "+(links.length-maxLinks)+".");
			break;
		}
		var xpath = getXPath(link).replace(/\[[0-9]*\]/g,'');
		if (!pageData.groups[xpath])
			pageData.groups[xpath] = new Array();
		var k = pageData.groups[xpath].length;
		// GM_log("k="+k);
		// GM_log("pageData.groups[xpath]="+typeof(pageData.groups[xpath]));
		pageData.groups[xpath][k] = new Object();
		pageData.groups[xpath][k].url = makeLinkAbsolute(link.href);
		pageData.groups[xpath][k].title = link.textContent;
		// pageData.groups[xpath].xpath = getXPath(link).replace(/\[[0-9]*\]/g,'');
		if (!data.revLinks[link.href]) // keep oldest copy
			data.revLinks[link.href] = ""+document.location;
	}
	data[document.location] = pageData;
	// GM_log(data[document.location]+" should = "+pageData);
	// if (links.length > maxLinks) {
		// GM_log("Finished parsing at "+new Date());
	// }
	// GM_log("pageData.groups = "+pageData.groups);
}

function cleanupData() {
	var dataArray = objectToArray(data);
	var cnt = 0; // failsafe
	while (dataArray.length > maxPageHistory) {
		// Find oldest piece of data
		var oldest;
		var oldestLastUsed = new Date().getTime();
		// for (var i=0;i<dataArray.length;i++) {
			// var pageData = dataArray[i];
		for (var key in data) {
			var pageData = data[key];
			if (pageData.lastUsed && pageData.lastUsed<oldestLastUsed) {
				// oldest = pageData.url;
				oldest = key;
				oldestLastUsed = pageData.lastUsed;
			}
		}
		GM_log("Cleaning up old record: "+oldest);
		// data.remove(oldest);
		// data[oldest] = undefined;
		delete data[oldest];
		cnt++; if (cnt>10) break; // failsafe
		dataArray = objectToArray(data);
	}
	// TODO: revLinks are never cleaned up
}

function clearHistoryData() {
	data = new Object();
	saveData();
}



//// Update data for this page ////

var data;

loadData();
if (!data) {
	// var data = new Array(); // Fails uneval().  (TOTEST: right?!)
	data = new Object(); // Survives uneval().
}
if (!data.revLinks) {
	data.revLinks = new Object();
}

GM_log(new Date()+" Parsing current page");

// Add data for this page
addDataForThisPage();

GM_log(new Date()+" Done");

cleanupData();

// saveData();
// saveData() might be called again later (e.g. to update the lastUsed of the
// parentPageData), but we do it here in case 1) the code below breaks, or 2)
// we don't actually display the parentPage history.
// TODO: The best solution would be to put the rest of the code in a try-catch,
// and call saveData() just once, at the end.




//// Presenting the data ////

function pageContainsLinkTo(pageData,url) {
	// GM_log("Checking "+pageData.links.length+" links...");
	return getItemsMatching(pageData.links, function(link){ /*if (Math.random()<0.01) { GM_log("link="+link+" link.url="+link.url); }*/ return link!=undefined && urlsMatch(link.url,url); } ).length > 0;
}

function findLinkGroup(pageData,url) {
	// GM_log("pageData.groups = "+typeof(pageData.groups));
	// GM_log("pageData.groups = "+pageData.groups);
	for (var xpath in pageData.groups) {
	// for (var i=0;i<pageData.groups.length;i++) {
		// var xpath = pageData.groups[i];
		var links = pageData.groups[xpath];
		// GM_log("For xpath="+xpath+" Got links = "+typeof(links)+" = "+links);
		// GM_log("length = "+pageData.groups[xpath].length);
		if (getItemsMatching(links, function(link){ return link!=undefined && urlsMatch(link.url,url); } ).length > 0) {
			return links;
		}
	}
	return false;
}

function urlsMatch(x,y) {
	if (x==undefined || y==undefined) {
		GM_log("There are still UNDEFINED links being stored/used!");
		return false;
	}
	if (typeof(x)!='string' || typeof(y)!='string') {
		GM_log("There are NON-STRINGS links being stored/used! "+typeof(x)+" "+typeof(y));
		return false;
	}
	// return x==y || x+'/'==y || x==y+'/';
	return x.replace(/\/$/,'') == y.replace(/\/$/,'');
}

// function findPagesContainingLinkTo(url) {
	// return getItemsMatching(data, function(pageData){ return pageContainsLinkTo(pageData,url); } );
// }

function drawHistoryTree() {

	// historyBlock.style = 'position: fixed; top: 4px; left: 4px; z-index: 10000; border: solid 1px black; background-color: white; padding: 6px;';

	GM_log(new Date()+" Calling showNeighbours");
	html = showNeighbours();
	GM_log(new Date()+" Done");

	if (html == "") {
		return;
	}

	// // var clearButton = " <A target='_self' href='javascript:(function(){ clearHistoryData(); })();'>Clear</A>";
	// var clearButton = "";
	// html += "<DIV style='text-align: right'>(We have "+objectToArray(data).length+" pages of history."+clearButton+")</DIV>\n";

	html =
		"<FONT size='+1'>"
		+ "<DIV style='float: right; padding-left: 12px;'>"
		+"[<A target='_self' href='javascript:(function(){ var h = document.getElementById(&quot;historyResults&quot;); h.style.display = (h.style && h.style.display?&quot;&quot;:&quot;none&quot;); })();'>--</A>] "
		+"[<A target='_self' href='javascript:(function(){ var h = document.getElementById(&quot;historyFloat&quot;); h.parentNode.removeChild(h); })();'>X</A>]"
		+ "</DIV>\n"
		+ "</FONT>\n"
		+ "<DIV id='historyResults'>\n"
		+ "<FONT size='-1'>"
		+ html
		+ "</FONT>"
		+ "</DIV>\n";

	var historyBlock = document.createElement("DIV");

	historyBlock.id = 'historyFloat';
	historyBlock.style.position = 'fixed';
	historyBlock.style.left = "" + parseInt(document.width*0.75) + "px"; // Appears further right in Konqueror.
	// GM_log("parseInt gave: "+parseInt(document.width*0.85) + "px");
	// historyBlock.style.left = '600px';
	// historyBlock.style.right = '4px';
	historyBlock.style.top = '60px';
	historyBlock.style.zIndex = '50000';
	// historyBlock.style.setProperty('z-index','50000');
	historyBlock.style.border = 'solid 2px #442200';
	// historyBlock.style.backgroundColor = 'white';
	historyBlock.style.backgroundColor = '#dddddd';
	historyBlock.style.padding = '6px';

	historyBlock.innerHTML = html;

	document.body.appendChild(historyBlock);

	// FAIL (should be a function, not javascript url):
	// historyBlock.onclick = 'javascript:alert(self);self.parentNode.removeChild(self);';

}

function showNeighbours() {

	// var html = "";

	if (showFavicons)
		html += "<STYLE type='text/css'> .favicon { padding-right: 4px; vertical-align: middle; } </STYLE>\n";

	var parentPageData;

	// Find the page which took us to this page:
	if (useReferrerOnly) {
		if (!document.referrer) {
			html += "This page has no referrer.<BR/>\n";
			return html;
		}
		parentPageData = data[document.referrer];
		if (!parentPageData) {
			html += "No data for <A href='"+document.referrer+"'>referrer</A>.<BR/>\n";
			return html;
		}
	} else {
		/*
		var parentPages = findPagesContainingLinkTo(""+document.location);
		GM_log("Got parentPages = "+parentPages);
		GM_log("With length = "+parentPages.length);
		// Remove self - There may be links from self to self.
		parentPages = getItemsMatching(parentPages, function(pageData){ return (!pageData.url) || stripAnchor(pageData.url) != stripAnchor(""+document.location); } );
		if ((!parentPages) || (parentPages.length < 1)) {
			if (dontBeDaft) {
				return "";
			}
			html += "I do not know how we got to this page.<BR/>\n";
			if (document.referrer)
				html += "Referrer was <A href='"+document.referrer+"'>here</A>.<BR/>\n";
			return html;
		}
		if (parentPages.length > 1) {
			if (verboseAlternatives) {
				// html += "(One of "+parentPages.length+" options)<BR/>\n";
				html += "Multiple pages link to this: ";
				for (var i=0;i<parentPages.length;i++) {
					var pageData = parentPages[i];
					html += "<A href='"+pageData.url+"'>"+escapeHTML(pageData.title)+"</A>" + " ";
				}
				html += "<BR/><BR/>\n";
			}
			// html += parentPages.length + " pages link to this one.<BR/><BR/>\n";
		}
		parentPageData = parentPages[0];
		*/
		var parentPageURL = data.revLinks[""+document.location];
		if (!parentPageURL) { // Try without trailing /
			parentPageURL = data.revLinks[(""+document.location).replace(/\/$/,'')];
		}
		if (!parentPageURL) { // Try with trailing /
			parentPageURL = data[document.location+'/'];
		}
		GM_log('Got parentPageURL='+parentPageURL);
		if (parentPageURL) {
			parentPageData = data[parentPageURL];
		}
		if (!parentPageData) {
			html += "I do not know how we got to this page.<BR/>\n";
			if (document.referrer)
				html += "Referrer was <A href='"+document.referrer+"'>here</A>.<BR/>\n";
			return html;
		}
	}

	parentPageData.lastUsed = new Date().getTime();

	// Find all links in that page with the same xpath as our link:
	// var linksToMe = getItemsMatching(parentPageData.links, function(link){ return link.url == ""+document.location });
	// var linksToMe = getItemsMatching(parentPageData.links, function(link){ return urlsMatch(link.url,""+document.location) });
	// if (linksToMe.length < 1) {
		// html += "Could not find myself in " + parentPageData.url + "!<BR/>\n";
		// return html;
	// }
	// var myLink = linksToMe[0];
	// var myXPathWas = myLink.xpath;
	// var group = getItemsMatching(parentPageData.links, function(link){ return link.xpath == myXPathWas });
	// GM_log("parentPageData = "+parentPageData);
	// GM_log("parentPageData.groups = "+parentPageData.groups);
	var group = findLinkGroup(parentPageData,""+document.location);
	// GM_log("Got group = "+group);
	var myLink;
	// This should not really fail, but just in case.
	try {
		myLink = getItemsMatching(group, function(link){ return urlsMatch(link.url,""+document.location); } )[0];
	} catch (e) {
		GM_log("Failed to find myLink."+e);
	}
	var myIndex = getIndexOf(myLink,group);
	// var myIndex = 1337;
	// html += "Got group: "+group+"<BR/>\n";
	html += "<FONT size='+0'>";
	html += (myIndex+1)+" of "+group.length+" from ";
	// TODO: We could make this link do Back for the user, only if the url == referrer.
	html += "<A href='"+parentPageData.url+"'>"+escapeHTML(parentPageData.title)+"</A>";
	html += "</FONT><BR/>\n";
	html += "<FONT size='-1'>\n";
	// html += "<BLOCKQUOTE>\n";
	html += "<P style='padding-left: 16px;'>\n";
	for (var i=0;i<group.length;i++) {
		var link = group[i];
		if (!link.title) {
			link.title = "_"+i+"_";
		}
		if (showFavicons)
			html += createFaviconHTMLFor(link.url);
		if (urlsMatch(link.url,""+document.location)) {
			html += "<FONT color='#7f7f7f'><B>"+escapeHTML(link.title)+"</B></FONT>";
		} else {
			// DONE: We should escape these link titles, but not with CGI escape()!
			if (goForwards) {
				html += "<A href='"+link.url+"'>"+escapeHTML(link.title)+"</A>";
			} else {
				html += "<A href='"+link.url+"' onclick='document.location.replace(\""+link.url+"\");'>"+escapeHTML(link.title)+"</A>";
			}
		}
		html += "<BR/>\n";
	}
	html += "</FONT>\n";
	// html += "</BLOCKQUOTE>\n";
	html += "</P>\n";

	return html;
}

function createFaviconHTMLFor(url) {
	var host = makeLinkAbsolute(url).replace(/^[^\/]*:\/\//,'').replace(/\/.*$/,'');
	// if (host == document.location.host) {
		// return null;
	// }
	return "<IMG src='http://"+host+"/favicon.ico' width='16' height='16' class='favicon' border='0'/>";
}


GM_log(new Date()+" data.revLinks = "+data.revLinks);

try {
	drawHistoryTree();
} catch (e) {
	GM_log("drawHistoryTree() broke with "+e);
}

unsafeWindow.document.data = data; // a reference for debugging (which I can't seem to access anyway :P )

if (navigator.appName == 'Konqueror') {
	// Fine in Konqueror but not Firefox (even with unsafeWindow):
	document.write('<SCRIPT type="text/javascript" src="http://hwi.ath.cx/javascript/make_elements_draggable.js"></SCRIPT>');
	document.write('<SCRIPT type="text/javascript"> makeDraggable(document.getElementById("historyFloat")); </SCRIPT>');

} else if (navigator.appName == 'Mozilla' || true) {

	// This works in Firefox:
	var scr = document.createElement('SCRIPT');
	scr.type = 'text/javascript';
	scr.src = 'http://hwi.ath.cx/javascript/make_elements_draggable.js';
	document.body.appendChild(scr);

	scr = document.createElement('SCRIPT');
	scr.type = 'text/javascript';
	scr.textContent = ' makeDraggable(document.getElementById("historyFloat")); ';
	document.body.appendChild(scr);

}


saveData();
// if (testReadWrite)
// 	var checkData = eval(GM_getValue("pageHistoryData"));


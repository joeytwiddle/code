// ==UserScript==
// @name           track_history
// @namespace      noggieb
// @description    Presents a list/tree of the browsing path you took to reach the current page.  An advanced in-page Back button.
// @include        *
// ==/UserScript==

// As well as displaying the tree of other pages, we could display *all* the
// links from the previous page, to allow fast switching.  (Or better: all the
// links with the same xpath?).

// I guess our database could be:
// All documents this session (URL), with a list of all their links (and their
// names+levels!), to which we may add extra links to if we later find an
// unknown link with this as referer.
// That doesn't yet contain tree information, although enough information to
// build it.
// Oh we should really include backlinks - the document.referrer of each URL.

// We could provide the feature that the history thingy will switch between
// the offered links using hidden/shown IFrames, a bit like tags.

// TODO:
// Track secondary (followed) links.


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
		}
	}
	return getXPath(parent) + '/' + node.nodeName.toLowerCase() + (totalCount>1 ? '[' + thisCount + ']' : '' );
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

// function deserialize(name, def) {
	// return eval(GM_getValue(name, (def || '({})')));
// }

function saveData() {
	GM_setValue('g_track_history_data',uneval(data));
	// GM_setValue('g_track_history_data',arrayToJSON(data));
}

function loadData() {
	data = eval(GM_getValue('g_track_history_data'));
	// if (data)
		// data = data[0];
}



var html = "";



var data;
loadData();
GM_log("I am here.");
// html += "Loaded data: "+data+"<BR/>\n";
// html += "Loaded data: "+data.length+"<BR/>\n";
// html += "Loaded data: "+uneval(data)+"<BR/>\n";
if (!data) {
	// var data = new Array(); // Fails uneval().
	data = new Object(); // Survives uneval().
}
// html += "Starting data: "+uneval(data)+"<BR/>\n";
// html += "Starting data: "+data.length+"<BR/>\n";

// Add data for this page
var pageData = new Object();
pageData.title = document.title;
pageData.links = new Array();
for (var i=0;i<document.links.length;i++) {
	var link = document.links[i];
	pageData.links[i] = new Object();
	pageData.links[i].url = link.href;
	pageData.links[i].title = link.textContent;
	pageData.links[i].xpath = getXPath(link).replace(/\[[0-9]*\]/g,'');
}
data[document.location] = pageData;

// alert('data.length = '+data.length);

GM_log("Saving...");
saveData();
// html += "Saved data: "+data+"<BR/>\n";
// html += "Saved data: "+uneval(data)+"<BR/>\n";
// html += "Saved data: "+eval(uneval(data)).length+"<BR/>\n";
loadData(); // Testing
// html += "Reloaded data: "+data+"<BR/>\n";
// html += "Reloaded data: "+uneval(data)+"<BR/>\n";
// html += "Reloaded data: "+eval(uneval(data)).length+"<BR/>\n";
GM_log("Saved.");



function drawHistoryTree() {

}

function getItemsMatching(l,c) {
	if (!l.item && !l.length) {
		GM_log("Could not process (non-list) "+l+" with condition "+c);
		return;
	}
	var ret = new Array();
	for (var i=0;i<l.length;i++) {
		var it = ( l.item ? l.item(i) : l[i] );
		if (c(it)) {
			ret[ret.length] = it;
		}
	}
	return ret;
}

function showNeighbours() {
	var parentPageData = data[document.referrer];
	if (!parentPageData) {
		html += "Unknown referrer: " + document.referrer + "<BR/>\n";
		return;
	}
	// Find all links in that page with the same xpath as our link.
	var myXPathWas = getItemsMatching(parentPageData.links, function(x){ return x.url == document.location })[0].xpath;
	if (!myXPathWas) {
		html += "Could not find myself in " + document.referrer + "<BR/>!\n";
		return;
	}
	var group = getItemsMatching(parentPageData.links, function(x){ return x.xpath == myXPathWas });
	// html += "Got group: "+group+"<BR/>\n";
	html += "<FONT size='+1'><A href='"+document.referrer+"'>"+parentPageData.title+"</A></FONT><BR/>\n";
	html += "<BLOCKQUOTE>\n";
	for (var i=0;i<group.length;i++) {
		var link = group[i];
		if (link.url == document.location)
			html += "<B>"+link.title+"</B>";
		else
			html += "<A href='"+link.url+"'>"+link.title+"</A>";
		html += "<BR/>\n";
	}
	html += "</BLOCKQUOTE>\n";
}

var historyBlock = document.createElement("DIV");

historyBlock.id = 'historyFloat';
historyBlock.style.position = 'fixed';
historyBlock.style.left = '4px';
historyBlock.style.top = '4px';
historyBlock.style.zIndex = '1000';
// historyBlock.style.setProperty('z-index','1000');
historyBlock.style.border = 'solid 1px black';
historyBlock.style.backgroundColor = 'white';
historyBlock.style.padding = '6px';

showNeighbours();
drawHistoryTree();

// data = eval(GM_getValue("g_track_history_data"));

html = "<P align='right'>["
	+"<A target='_self' href='javascript:(function(){ var h = document.getElementById(&quot;historyFloat&quot;); h.parentNode.removeChild(h);})();'>X</A>"
	+ "]</P>\n"
	+ html;

historyBlock.innerHTML = html;

document.body.appendChild(historyBlock);
GM_log("I am NOT HERE.");

// historyBlock.style = 'position: fixed; top: 4px; left: 4px; z-index: 10000; border: solid 1px black; background-color: white; padding: 6px;';

// FAIL
// historyBlock.onclick = 'javascript:alert(self);self.parentNode.removeChild(self);';

top.data = data; // a reference for debugging


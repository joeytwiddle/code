// ==UserScript==
// @name           Auto Highlight Text on All Results Pages
// @namespace      search
// @description    Highlights the words you used in your search on the search result page and on the page itself, by checking Referrer.
// @include        *
// ==/UserScript==

/*
// TODO:
// Scroll down to (just above) first occurrence (in case our search terms do not appear until later in the document).
// Provide at least minimal compatibility with common search term styles.
// e.g. BUG: If q="may+not+load+or+link+to" then the 'may' and 'to' words will
// not be highlighted due to the quotes.  (In fact strictly we should only
// highlight if it's the whole phrase.)
// Highlight the different words in different colors?
// Highlight excluded terms (e.g. 'wheat' in q=food+-wheat) in red?
*/

function findSearchTerm(url) {
	url = "" + url;
	if (url.indexOf("?")>=0) {
		url = url.substring(url.indexOf("?")+1);
		if (url.indexOf("search=")==0 || url.indexOf("&search=")>=0) {
			// log(url.replace(/.*search=/,''));
			// log(url.replace(/.*search=/,'').replace(/&.*/,''));
			return url.replace(/.*search=/,'').replace(/&.*/,'');
		} else if (url.indexOf("q=")==0 || url.indexOf("&q=")>=0) {
			// log(url.replace(/.*q=/,''));
			// log(url.replace(/.*q=/,'').replace(/&.*/,''));
			return url.replace(/.*q=/,'').replace(/&.*/,'');
		}
	}
	return null;
}

var words;

words = findSearchTerm(window.document.location);
if (!words)
	words = findSearchTerm(window.document.referrer);
// I was prioritising referrer before, but that's messy if you perform a different search on Google! :P

if (words) {

	GM_log("Searching for '"+words+"'...");
	/* window.status="Searching for '"+words+"'..."; */

	/* This function taken from the "Highlight..." bookmarklet on SquareFree. */
	var count = 0, text, dv;
	var dv = window.document.defaultView;
	function searchWithinNode(node, te, len){
		var pos, skip, spannode, middlebit, endbit, middleclone;
		skip = 0;
		if(node.nodeType == 3) {
			pos = node.data.toUpperCase().indexOf(te);
			if(pos >= 0) {
				spannode = window.document.createElement("SPAN");
				spannode.className = node.className;
				/* spannode.style.backgroundColor = "#ccffcc";
				spannode.style.setProperty('opacity','0.5');
				spannode.style.setProperty('-moz-opacity','0.5');
				spannode.style.setProperty('filter','alpha(opacity=50)'); */
				spannode.setAttribute('style','background-color: #bbffbb; opacity: 0.7;');
				middlebit = node.splitText(pos);
				endbit = middlebit.splitText(len);
				middleclone = middlebit.cloneNode(true);
				spannode.appendChild(middleclone);
				middlebit.parentNode.replaceChild(spannode,middlebit);
				++count;
				skip = 1;
			}
		} else if (node.nodeType==1&& node.childNodes && node.tagName.toUpperCase()!="SCRIPT" && node.tagName.toUpperCase!="STYLE") {
			for (var child=0; child < node.childNodes.length; ++child){
				child=child+searchWithinNode(node.childNodes[child], te, len);
			}
		}
		return skip;
	}
	/*
	searchWithinNode(window.document.body, text.toUpperCase(), text.length);
	window.status="Found "+count+" occurrence"+(count==1?"":"s")+" of '"+text+"'.";
	*/

	words = unescape(words.replace(/\+/g,' '));
	// window.status = "Got Words! \""+words+"\"";
	/*
	var splitWords = words.split(" ");
	log("splitWords="+splitWords);
	for (var text in splitWords) {
	// Hmm we *can* loop it like an array but not with for..in
	}
	*/
	while (words.indexOf(" ")>=0) {
		var firstWord = words.substring(0,words.indexOf(" "));
		words = words.substring(words.indexOf(" ")+1);
		if (firstWord.length>0) {
			searchWithinNode(window.document.body, firstWord.toUpperCase(), firstWord.length);
		}
	}
	searchWithinNode(window.document.body, words.toUpperCase(), words.length);

} else {
	// window.status = "No search terms found in "+window.document.referrer+" or "+window.document.location;
}


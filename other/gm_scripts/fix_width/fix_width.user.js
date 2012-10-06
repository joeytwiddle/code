// ==UserScript==
// @name           Fix_Width
// @namespace      FW
// @include        http://*/
// @description    ...
// ==/UserScript==

var mainWidth = 800 - 32;

// GM_log("document.location.host="+document.location.host);
// GM_log("this.GM_addStyle="+this.GM_addStyle);

/*
if (document.location.host == "stackoverflow.com") {
	// GM_addStyle("#content { width: "+mainWidth+"px }");
	GM_addStyle(".post-text { width: "+mainWidth*2/3+"px }");
	GM_addStyle("#answers-header { width: "+mainWidth*2/3+"px }");
	GM_addStyle("#answers { width: "+mainWidth*2/3+"px }");
	GM_addStyle(".answer { width: "+mainWidth*2/3+"px }");
	GM_addStyle(".comments { width: "+mainWidth*2/3+"px }");
	GM_addStyle("#sidebar { width: "+mainWidth*1/5+"px }");
}
*/

function traverse(node,fn) {
	fn(node);
	var children = node.childNodes;
	for (var i=0;i<children.length;i++) {
		traverse(children[i],fn);
	}
}

try {
	traverse(document.body,checkNode);
} catch (e) {
	GM_log("[FixWidth] Error"+e);
}

function checkNode(node) {
	var rules = getComputedStyle(node);
	if (rules) {
		// for (var i=0;i<rules.length;i++) {
			// if (rule[i] == "width") {
			// }
		// }
		if (rules["width"].slice(-2) == "px") {
			GM_log("Replacing node "+node+"'s width "+rules["width"]+" with ''.");
			// rules["width"] = '';
			node.style["width"] = 'auto'; // '100%';   // '' did not work!
		}
	}
}


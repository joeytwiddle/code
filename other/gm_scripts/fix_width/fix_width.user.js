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

	var words = [ "width", "maxWidth" ]; // Chrome offers both but "max-width" cannot be written (DOM Exception 7 NO_MODIFICATION_ALLOWED_ERR)!
	if (node.tagName == "IMG") {
		return;
	}
	var rules = getComputedStyle(node);
	if (rules) {
		// for (var i=0;i<rules.length;i++) {
		// 	if (rule[i] == "width") {
		// 	}
		// }
		for (var i=0;i<words.length;i++) {
			var word = words[i];
			if (rules[word] && rules[word].slice(-2) == "px") {
				var intValue = parseInt(rules[word],10);
				// We don't want to affect small boxes
				if (intValue >= 250) {
					GM_log("Replacing node "+node+"'s "+word+" rule '"+rules[word]+"' with 'none'.");
					// '' 'clear' and 'auto' and 'reset' and 'default' did not work!
					// node.style[word] = '100%';   // enlarges some things that are naturally small
					node.style[word] = 'none';
				}
			}
		}
	}

}

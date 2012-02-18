// ==UserScript==
// @name           Table of Contents Everywhere
// @namespace      TOCE
// @description    On pages which do not have a Table of Contents, but should do, create one!
// @include        http://*/*
// ==/UserScript==

var minimumItems = 4;   // Don't display a TOC for fewer than this number of entries.
var delayBeforeRunning = 1600;

// 2012-01-30  Implemented GM_log and GM_addStyle so this script can be included on any web page.

// TODO: derbyjs.com is an example of a site with a <div id=toc> that has no
// hide or close buttons.  Perhaps we should add close and rollup buttons if we
// cannot find any recognisable buttons.  (Medawiki tocs for example, do have a
// show/hide button, so we don't want to add to them!)

setTimeout(function(){

// Implementing these two means we can run as a stand-alone script on any page.
if (typeof GM_log == "undefined") {
	GM_log = function() {
		// Firefox's console.log does not have apply or call functions!
		var txt = Array.prototype.join.call(arguments," ");
		console.log(txt);
	};
}
if (typeof GM_addStyle == "undefined") {
	this.GM_addStyle = function(css) {
		var s = document.createElement("style");
		s.type = 'text/css';
		s.innerHTML = css;
		document.getElementsByTagName("head")[0].appendChild(s);
	};
}

// Implementing these allows us to remember toggled state.  (Chrome's set/getValue don't work.)
if (typeof GM_setValue == 'undefined' || window.navigator.vendor.match(/Google/)) {
	GM_log("TOCE: Adding fallback implementation of GM_set/getValue");

	if (typeof localStorage == 'undefined') {

		GM_getValue = function(name, defaultValue) {
			return defaultValue;
		};

	} else {

		GM_setValue = function(name, value) {
			value = (typeof value)[0] + value;
			localStorage.setItem(name, value);
		};

		GM_getValue = function(name, defaultValue) {
			var value = localStorage.getItem(name);
			if (!value)
				return defaultValue;
			var type = value[0];
			value = value.substring(1);
			switch (type) {
				case 'b':
					return value == 'true';
				case 'n':
					return Number(value);
				default:
					return value;
			}
		};

	}

}

// Modified for this script's needs.
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
		if (true /*sibling.nodeType == node.nodeType*/) {
			totalCount++;
		}
		if (sibling == node) {
			thisCount = totalCount;
			break;
		}
	}
	return getXPath(parent) + '/' + '*' /*node.nodeName.toLowerCase()*/ + (totalCount>1 ? '[' + thisCount + ']' : '' );
}

try {

	function newNode(tag,data) {
		var elem = document.createElement(tag);
		if (data) {
			for (var prop in data) {
				elem[prop] = data[prop];
			}
		}
		return elem;
	}

	function newSpan(text) {
		return newNode("span",{textContent:text});
	}

	var tocFound = document.getElementById("toc");
	//// Konqueror 3.5 does NOT have document.getElementsByClassName()!
	// tocFound = tocFound || document.getElementsByClassName("toc")[0];

	var toc = tocFound;

	if (!toc) {

		// Can we make a TOC?
		var headers = "//h1 | //h2 | //h3 | //h4 | //h5 | //h6 | //h7 | //h8";
		var anchors = "//a[@name]";
		// For coffeescript.org:
		var elementsMarkedAsHeader = "//*[@class='header']";
		var nodeSnapshot = document.evaluate(headers+"|"+anchors+"|"+elementsMarkedAsHeader,document,null,6,null);
		//// Chrome needs lower-case 'h', Firefox needs upper-case 'H'!
		// var nodeSnapshot = document.evaluate("//*[starts-with(name(.),'h') and substring(name(.),2) = string(number(substring(name(.),2)))]",document,null,6,null);
		// var nodeSnapshot = document.evaluate("//*[starts-with(name(.),'H') and substring(name(.),2) = string(number(substring(name(.),2)))]",document,null,6,null);
		if (nodeSnapshot.snapshotLength > minimumItems) {

			GM_log("Making TOC with "+nodeSnapshot.snapshotLength+" nodes.");

			toc = newNode("div");
			toc.id = 'toc';

			var heading = newSpan("Table of Contents");
			heading.style.fontWeight = "bold";
			heading.style.fontSize = "100%";
			toc.appendChild(heading);

			var closeButton = newSpan("[X]");
			closeButton.style.float = 'right';
			closeButton.style.cursor = 'pointer';
			closeButton.style.paddingLeft = '5px';
			closeButton.onclick = function() { toc.parentNode.removeChild(toc); };
			toc.appendChild(closeButton);

			var table = newNode("div");

			var rollupButton = newSpan("[-]");
			rollupButton.style.float = 'right';
			rollupButton.style.cursor = 'pointer';
			rollupButton.style.paddingLeft = '10px';
			function toggleRollUp() {
				if (table.style.display == 'none') {
					table.style.display = '';
					rollupButton.textContent = "[-]";
				} else {
					table.style.display = 'none';
					rollupButton.textContent = "[+]";
				}
				setTimeout(function(){
					GM_setValue("TOCE_rolledUp", table.style.display=='none');
				},5);
			}
			rollupButton.onclick = toggleRollUp;
			toc.appendChild(rollupButton);
			if (GM_getValue("TOCE_rolledUp",false)) {
				toggleRollUp();
			}

			// The xpath query did not give the elements back in page-order.
			// Sort them back into the order they appear in the document
			var nodeArray = [];
			for (var i=0;i<nodeSnapshot.snapshotLength;i++) {
				var node = nodeSnapshot.snapshotItem(i);
				nodeArray.push(node);
				// node.xpath = getXPath(node);
				node.magicPath = getXPath(node).substring(3).slice(0,-1).split("]/*[").map(Number);
			}
			nodeArray.sort(function(a,b){
				// return getXPath(a) > getXPath(b) ? +1 : -1;
				// return a.xpath > b.xpath ? +1 : -1;
				GM_log("Comparing "+a.magicPath+" against "+b.magicPath);
				for (var i=0;i<a.magicPath.length;i++) {
					if (i >= b.magicPath.length) {
						return +1; // b wins
					}
					if (a.magicPath[i] > b.magicPath[i]) {
						return +1; // b wins
					}
					if (a.magicPath[i] < b.magicPath[i]) {
						return -1; // a wins
					}
				}
				return -1; // assume b is longer, or they are equal
			});
			// Yep it's goofy code, but it works.

			for (var i=0;i<nodeArray.length;i++) {
				var node = nodeArray[i];

				var level = (node.tagName.substring(1) | 0) - 1;
				if (level < 0) {
					level = 0;
				}

				var linkText = node.textContent && node.textContent.trim() || node.name;
				if (!linkText) {
					continue;   // skip things we cannot name
				}

				var link = newNode("A");
				if (linkText.length > 40)
					linkText = linkText.substring(0,32)+"...";
				link.textContent = linkText;
				/* Dirty hack for Wikimedia: */
				if (link.textContent.substring(0,7) == "[edit] ") {
					link.textContent = link.textContent.substring(7);
				}
				if (node.tagName == "A") {
					link.href = '#'+node.name;
				} else {
					(function(node){
						link.onclick = function(evt){
							node.scrollIntoView();

							// Optional: CSS animation
							// NOT WORKING!
							/*
							node.id = "toc_current_hilight";
							["","-moz-","-webkit-"].forEach(function(insMode){
								GM_addStyle("#toc_current_hilight { "+insMode+"animation: 'fadeHighlight 4s ease-in 1s alternate infinite'; }@"+insMode+"keyframes fadeHighlight { 0%: { background-color: yellow; } 100% { background-color: rgba(255,255,0,0); } }");
							});
							*/

							evt.preventDefault();
							return false;
						};
					})(node);
					link.href = '#';
				}
				table.appendChild(link);
				liType = "li";
				if (node.tagName == "A") {
					liType = "div";
				}
				var li = newNode(liType);
				link.parentNode.replaceChild(li,link);
				if (node.tagName == "A") {
					li.appendChild(document.createTextNode("\u2693 "));
				}
				li.appendChild(link);
				li.style.paddingLeft = (1.5*level)+"em";
				li.style.fontSize = (100-6*(level+1))+"%";
				li.style.size = li.style.fontSize;
				// Debugging:
				/*
				li.title = node.tagName;
				if (node.name)
					li.title += " (#"+node.name+")";
				li.title = getXPath(node);
				*/
			}
			toc.appendChild(table);

			document.body.appendChild(toc);

			// TODO scrollIntoView if newly matching 1.hash exists

		}

	}

	// We make the TOC float regardless whether we created it or it already existed.
	// Interestingly, the overflow settings seems to apply to all sub-elements.
	// E.g.: http://mewiki.project357.com/wiki/X264_Settings#Input.2FOutput
	// FIXED: Some of the sub-trees are so long that they also get scrollbars, which is a bit messy!
	// FIXED : max-width does not do what I want!  To see, find a TOC with really wide section titles (long lines).
	GM_addStyle("#toc { position: fixed; top: 10%; right: 4%; background-color: white; color: black; font-weight: normal; padding: 5px; border: 1px solid grey; z-index: 5555; max-height: 80%; max-width: 32%; overflow: auto; }");
	GM_addStyle("#toc       { opacity: 0.2; }");
	GM_addStyle("#toc:hover { opacity: 1.0; }");

} catch (e) {
	GM_log("[TOCE] Error! "+e);
}

},delayBeforeRunning);
// We want it to run fairly soon but it can be quite heavy on large pages - big XPath search.


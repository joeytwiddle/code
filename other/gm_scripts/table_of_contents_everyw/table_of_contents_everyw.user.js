// ==UserScript==
// @name           Table of Contents Everywhere
// @namespace      TOCE
// @description    On pages which do not have a Table of Contents, but should do, create one!
// @include        http://*/*
// ==/UserScript==

(function(){

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

	if (true || !toc) {

		// Can we make a TOC?
		var nodes = document.evaluate("//h1 | //h2 | //h3 | //h4 | //h5 | //h6 | //h7 | //h8 | //a[@name]",document,null,6,null);
		//// Chrome needs lower-case 'h', Firefox needs upper-case 'H'!
		// var nodes = document.evaluate("//*[starts-with(name(.),'h') and substring(name(.),2) = string(number(substring(name(.),2)))]",document,null,6,null);
		// var nodes = document.evaluate("//*[starts-with(name(.),'H') and substring(name(.),2) = string(number(substring(name(.),2)))]",document,null,6,null);
		GM_log("[TOCE] found "+nodes.snapshotLength+" nodes.");
		if (nodes.snapshotLength > 1) {

			GM_log("Making TOC with "+nodes.snapshotLength+" nodes!");

			toc = newNode("div");
			toc.id = 'toc';
			// Interestingly, the overflow settings seems to apply to all sub-elements.
			// E.g.: http://mewiki.project357.com/wiki/X264_Settings#Input.2FOutput
			// Some of the sub-trees are so long that they also get scrollbars, which is a bit messy!
			GM_addStyle("#toc { position: fixed; top: 8%; right: 4%; background-color: white; color: black; font-weight: normal; padding: 5px; border: 1px solid grey; z-index: 5555; max-height: 80%; /*max-width: 40%;*/ overflow: auto; }");
			// BUG TODO: max-width does not do what I want!  To see, find a TOC with really wide section titles (long lines).
			//           WikiIndent's solution is to set a max-width of all sub-elements of the TOC in pixels.
			GM_addStyle("#toc       { opacity: 0.2; }");
			GM_addStyle("#toc:hover { opacity: 1.0; }");

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
			rollupButton.onclick = function() {
				if (table.style.display == 'none') {
					table.style.display = '';
					rollupButton.textContent = "[-]";
				} else {
					table.style.display = 'none';
					rollupButton.textContent = "[+]";
				}
			};
			toc.appendChild(rollupButton);

			for (var i=0;i<nodes.snapshotLength;i++) {
				var node = nodes.snapshotItem(i);
				var level = (node.tagName.substring(1) | 0) - 1;
				var link = newNode("A");
				link.textContent = node.textContent;
				/* Dirty hack for Wikimedia: */
				if (link.textContent.substring(0,7) == "[edit] ") {
					link.textContent = link.textContent.substring(7);
				}
				(function(node){
					link.onclick = function(evt){
						node.scrollIntoView();
						evt.preventDefault();
						return false;
					};
				})(node);
				link.href = '#';
				table.appendChild(link);
				var li = newNode("li");
				link.parentNode.replaceChild(li,link);
				li.appendChild(link);
				li.style.paddingLeft = (1.5*level)+"em";
				li.style.fontSize = (100-6*(level+1))+"%";
				li.style.size = li.style.fontSize;
			}
			toc.appendChild(table);

			document.body.appendChild(toc);

			// TODO scrollIntoView if newly matching 1.hash exists

		}

	}

} catch (e) {
	GM_log("[TOCE] Error! "+e);
}

})();


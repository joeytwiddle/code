// ==UserScript==
// @name           Table of Contents Everywhere
// @namespace      TOCE
// @description    On pages which do not have a Table of Contents, but should do, create one!
// @include        http://*/*
// ==/UserScript==

var tocFound = document.getElementById("toc");
tocFound = tocFound || document.getElementsByClassName("toc");

var toc = tocFound;

if (true /*!toc*/) {

	// Can we make a TOC?
	// var nodes = document.evaluate("//h1 | //h2 | //h3 | //h4 | //h5 | //h6 | //h7 | //h8",document,null,6,null);
	var nodes = document.evaluate("//*[starts-with(name(.),'h') and substring(name(.),2) = string(number(substring(name(.),2)))]",document,null,6,null);
	if (nodes.snapshotLength > 1) {

		GM_log("Making TOC with "+nodes.snapshotLength+" nodes!");

		toc = document.createElement("div");
		toc.id = 'toc';
		GM_addStyle("#toc { position: fixed; top: 8%; right: 4%; background-color: white; color: black; font-weight: normal; padding: 5px; border: 1px solid grey; z-index: 5555; max-height: 80%; overflow: auto; }");
		GM_addStyle("#toc       { opacity: 0.2; }");
		GM_addStyle("#toc:hover { opacity: 1.0; }");

		function create(tag) {
			return document.createElement(tag);
		}
		function textSpan(text) {
			var span = create("span");
			span.textContent = text;
			return span;
		}

		var heading = textSpan("Table of Contents");
		heading.style.fontWeight = "bold";
		heading.style.fontSize = "100%";
		toc.appendChild(heading);

		var closeButton = textSpan("[X]");
		closeButton.style.float = 'right';
		closeButton.style.cursor = 'pointer';
		closeButton.style.paddingLeft = '5px';
		closeButton.onclick = function() { toc.parentNode.removeChild(toc); };
		toc.appendChild(closeButton);

		var table = document.createElement("div");

		var rollupButton = textSpan("[-]");
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

		var targets = {};
		for (var i=0;i<nodes.snapshotLength;i++) {
			var node = nodes.snapshotItem(i);
			var level = (node.tagName.substring(1) | 0) - 1;
			var targetName = node.textContent.replace(/ /g,'_').replace(/[^A-Za-z0-9._()]/g,'');
			if (targets[targetName] != null) {
				targetName = "toc"+i+"_"+targetName;
			}
			targets[targetName] = true;
			var targetLink = document.createElement("A");
			targetLink.name = targetName;
			node.parentNode.replaceChild(targetLink,node);
			targetLink.appendChild(node);
			var link = document.createElement("A");
			link.href = "#"+targetName;
			link.textContent = node.textContent;
			table.appendChild(link);
			var li = document.createElement("li");
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



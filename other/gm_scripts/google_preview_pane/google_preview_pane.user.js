// ==UserScript==
// @name           Google Preview Pane
// @namespace      http://userscripts.org/users/89794
// @description    Loads Google results in a Preview Pane on mouse hover
// @include        http://google.*/search?*
// @include        http://www.google.*/search?*
// ==/UserScript==

var highlightFocusedResult = true;

var resultsBlock = window.document.getElementById("res");

var table = window.document.createElement("TABLE");
var tbody = window.document.createElement("TBODY");
var row = window.document.createElement("TR");
var leftCell = window.document.createElement("TD");
var rightCell = window.document.createElement("TD");

leftCell.width = '50%';
// leftCell.width = (window.innerWidth/2) +'px';
rightCell.width = '50%';
// leftCell.height = window.innerHeight * 0.70;
// rightCell.height = window.innerHeight * 0.75;
// resultsBlock.style.width = (window.innerWidth/2) + 'px';
resultsBlock.style.height = (window.innerHeight * 0.70) + 'px';

// leftCell.scrollable = true;
// rightCell.scrollable = true;
// leftCell.style.overflow = 'auto';
resultsBlock.style.overflow = 'auto';

tbody.appendChild(row);
table.appendChild(tbody);
row.appendChild(leftCell);
row.appendChild(rightCell);

resultsBlock.parentNode.insertBefore(table,resultsBlock);
leftCell.appendChild(resultsBlock);

// leftCell.style.backgroundColor = '#eeeeee';

var iframe = window.document.createElement('IFRAME');
iframe.width = '100%';
iframe.height = window.innerHeight * 0.70;
rightCell.appendChild(iframe);

iframe.style.backgroundColor = '#eeeeee';

(function(){

	var lastHover = null;
	var lastPreview = null;

	function checkFocus() {
		if (lastHover) {
			GM_log("Previewing "+lastHover.href);
			if (highlightFocusedResult) {
				if (lastPreview)
					lastPreview.parentNode.style.backgroundColor = "";
				lastHover.parentNode.style.backgroundColor = "#ffccff";
			}
			iframe.src = lastHover.href;
			lastPreview = lastHover;
		}
	}

	function helloMouse(evt) {
		var node = evt.target;
		// window.status = "Over "+node;
		if (node.tagName=="A" /*&& node.className=="l"*/) {
			lastHover = node;
			setTimeout(checkFocus,1000);
			// setTimeout('checkFocus();',1000);
		}
	}

	function goodbyeMouse(evt) {
		var node = evt.target;
		// window.status = "Out "+node;
		if (node.tagName=="A" /*&& node.className=="l"*/) {
			lastHover = null;
		}
	}

	resultsBlock.addEventListener('mouseover',helloMouse,false);
	resultsBlock.addEventListener('mouseout',goodbyeMouse,false);

	unsafeWindow.checkFocus = checkFocus;

})();

/*
for (var i=0;i<document.links.length;i++) {
	if (document.links[i].className == "l") {
		document.links[i].addEventListener('mouseover',helloMouse,false);
		document.links[i].addEventListener('mouseout',goodbyeMouse,false);
	}
}
*/


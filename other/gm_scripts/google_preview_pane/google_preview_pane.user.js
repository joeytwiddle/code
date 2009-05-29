// ==UserScript==
// @name           Google Preview Pane
// @namespace      http://userscripts.org/users/89794
// @description    Loads Google results in a Preview Pane on mouse hover
// @include        http://google.*/search?*q=*
// @include        http://www.google.*/search?*q=*
// @include        http://google.*/webhp?*q=*
// @include        http://www.google.*/webhp?*q=*
// @version        0.9.5
// ==/UserScript==

// Settings:

var highlightFocusedResult = true;
var previewWidth = 0.6;
var pageHeightUsed = 0.7;



if (window != top)
	return;

// On webhp pages, I think the content is loaded by Javascript.  We must wait
// for the page to finish loading before we can find the resultsBlock.

// Dagnammit Konqueror needs document.onload and Firefox needs window.onload.
var browsersSuck = ( navigator.appName.match(/Konqueror/i) ? document : window );

browsersSuck.addEventListener('load',function(){

	var resultsWidth = 1.0 - previewWidth;

	var resultsBlock = document.getElementById("res");

	GM_log("resultsBlock = " + resultsBlock);

	var table = document.createElement("TABLE");
	var tbody = document.createElement("TBODY");
	var row = document.createElement("TR");
	var leftCell = document.createElement("TD");
	var rightCell = document.createElement("TD");

	leftCell.style.width = (window.innerWidth * resultsWidth) +'px';
	rightCell.style.width = (window.innerWidth * previewWidth) +'px';
	resultsBlock.style.width = (window.innerWidth * resultsWidth) + 'px';
	resultsBlock.style.height = (window.innerHeight * pageHeightUsed) + 'px';
	resultsBlock.style.overflow = 'auto';

	var iframe = document.createElement('IFRAME');
	iframe.width = '100%';
	iframe.height = window.innerHeight * 0.70;
	iframe.style.backgroundColor = '#eeeeee';
	rightCell.appendChild(iframe);

	tbody.appendChild(row);
	table.appendChild(tbody);
	row.appendChild(leftCell);
	row.appendChild(rightCell);

	resultsBlock.parentNode.insertBefore(table,resultsBlock);
	leftCell.appendChild(resultsBlock);

	// Listeners:

	var lastHover = null;
	var lastPreview = null;

	function checkFocus() {
		if (lastHover) {
			// GM_log("Previewing "+lastHover.href);
			if (highlightFocusedResult) {
				if (lastPreview)
					lastPreview.style.backgroundColor = "";
				lastHover.style.backgroundColor = "#ccccff"; // "#ffccff";
			}
			iframe.src = lastHover.href;
			lastPreview = lastHover;
		}
	}

	function helloMouse(evt) {
		var node = evt.target;
		while (node && node.tagName!="A") {
			node = node.parentNode;
		}
		if (node /*&& node.className=="l"*/) {
			lastHover = node;
			setTimeout(checkFocus,1000);
		}
	}

	function goodbyeMouse(evt) {
		var node = evt.target;
		while (node && node.tagName!="A") {
			node = node.parentNode;
		}
		if (node /*&& node.className=="l"*/) {
			lastHover = null;
		}
	}

	resultsBlock.addEventListener('mouseover',helloMouse,true);
	resultsBlock.addEventListener('mouseout',goodbyeMouse,true);

	// The "Sponsored Links" block gets in the way.
	var toKill = document.getElementById("mbEnd");
	if (toKill) {
		toKill.parentNode.removeChild(toKill);
	}

},false);


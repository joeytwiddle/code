// ==UserScript==
// @name           Google Preview Pane
// @namespace      http://userscripts.org/users/89794
// @description    Loads Google results in a Preview Pane on mouse hover
// @include        http://google.*/search?*q=*
// @include        http://www.google.*/search?*q=*
// @include        http://google.*/webhp?*q=*
// @include        http://www.google.*/webhp?*q=*
// @version        0.9.8
// ==/UserScript==

// Settings:

var fillWholeWindow = true;
var keepHeaderAbove = true;
var removeLogo = false;
var previewWidth = 0.6;
var pageHeightUsed = 0.7;
var hoverTime = 800;
var highlightFocusedResult = true;
var noPanelBorder = false;



if (window != top)
	return;

// On webhp pages, I think the content is loaded by Javascript.  We must wait
// for the page to finish loading before we can find the resultsBlock.

// Dagnammit Konqueror needs document.onload and Firefox needs window.onload.
var browsersSuck = ( navigator.appName.match(/Konqueror/i) ? document : window );

browsersSuck.addEventListener('load',function(){

	var resultsWidth = 1.0 - previewWidth;

	var resultsBlock = document.getElementById("res");

	if (fillWholeWindow) {
		pageHeightUsed = 0.97;
		resultsBlock = document.createElement("DIV");

		// Copy wanted parts of window into our left-pane block:
		if (keepHeaderAbove) {
			pageHeightUsed = 0.75;
			var curNode = document.getElementById("ssb").nextSibling;
			while (curNode) {
				var nextNode = curNode.nextSibling;
				resultsBlock.appendChild(curNode);
				curNode = nextNode;
			}
		} else {
			while (document.body.childNodes.length > 0) {
				resultsBlock.appendChild(document.body.childNodes[0]);
			}
		}

		document.body.appendChild(resultsBlock);

		try {
			var annoyingLine = document.getElementsByClassName("gbh")[0];
			annoyingLine.parentNode.removeChild(annoyingLine);
			annoyingLine = document.getElementsByClassName("gbh")[0];
			annoyingLine.parentNode.removeChild(annoyingLine);
		} catch (e) { }

		// The header is too wide, and some parts are nowrap.
		// We must reduce the width to avoid getting a horizontal scrollbar.

		if (!keepHeaderAbove) {

			if (removeLogo) {
				var logo = document.getElementsByTagName("IMG")[0];
				logo = logo.parentNode.parentNode.parentNode; // TD
				// logo = logo.parentNode.parentNode; // A
				var pNode = logo.parentNode;
				pNode.removeChild(logo);
				document.getElementById('sff').getElementsByTagName('table')[0].style.marginTop = '5px'
			}
			/*
			var newImg = document.createElement("IMG");
			newImg.src = "/favicon.ico";
			pNode.appendChild(newImg);
			*/

			document.getElementsByTagName('form')[0].getElementsByTagName("input")[1].size = 20;
			// This avoids the white-space: wrap but kills the blue header background.
			document.getElementById("ssb").id = 'not_ssb';
			document.getElementById("not_ssb").style.backgroundColor = '#F0F7F9';
			document.getElementById("not_ssb").style.borderTop = '1px solid #6890DA';
			var resText = document.getElementById('prs').nextSibling;
			resText.style.textAlign = 'right';

		}

		document.getElementById("res").style.width = (window.innerWidth * resultsWidth - 48) +'px';

	}

	// GM_log("resultsBlock = " + resultsBlock);

	var table = document.createElement("TABLE");
	var tbody = document.createElement("TBODY");
	var row = document.createElement("TR");
	var leftCell = document.createElement("TD");
	var rightCell = document.createElement("TD");

	leftCell.style.width = (window.innerWidth * resultsWidth) +'px';
	rightCell.style.width = (window.innerWidth * previewWidth) +'px';
	// If we leave room for vertical scrollbar, we won't need horizontal one. :)
	resultsBlock.style.width = (window.innerWidth * resultsWidth) + 'px';
	resultsBlock.style.height = (window.innerHeight * pageHeightUsed) + 'px';
	resultsBlock.style.overflow = 'auto';

	var iframe = document.createElement('IFRAME');
	iframe.width = '100%';
	iframe.height = (window.innerHeight * pageHeightUsed) + 'px';
	iframe.style.backgroundColor = '#eeeeee';
	if (noPanelBorder)
		iframe.style.border = '0px solid white';
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
			if (highlightFocusedResult && lastHover!=lastPreview) {
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
			setTimeout(checkFocus,hoverTime);
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


/*
// Yikes!  Causes blank page if done after page load.
// wtf is that image from?!
var logo = document.getElementsByTagName("IMG")[0];
logo.width = logo.width / 2;
logo.height = logo.height / 2;
logo.src = log.src;
logo = logo.parentNode;
logo.style.width = logo.style.width / 2;
logo.style.height = logo.style.height / 2;
*/


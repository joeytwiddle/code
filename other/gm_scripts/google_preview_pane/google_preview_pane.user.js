// ==UserScript==
// @name           Google Preview Pane
// @namespace      http://userscripts.org/users/89794
// @description    Loads Google results in a Preview Pane on mouse hover
// @include        http://google.*/search?*q=*
// @include        http://www.google.*/search?*q=*
// @include        http://google.*/webhp?*q=*
// @include        http://www.google.*/webhp?*q=*
// @version        0.9.9
// ==/UserScript==

// Settings:

var fillWholeWindow = true;    // Bring more of the page into the left pane.
var keepHeaderAbove = true;    // Avoid bringing the top of the page in.
var miniLogo = true;           // miniLogo or removeLogo may help to reduce
var removeLogo = false;        // width, especially when keepHeaderAbove==false.
var reduceWidth = false;       // You may need to reduceWidth if keepHeaderAbove==false.
var previewWidth = 0.7;        // Size of the preview pane.
var pageHeightUsed = 0.7;      // Will be overridden later if keepHeaderAbove==true.
var noPanelBorder = false;     // I like the preview pane to have depth.
var hoverTime = 800;           // Milliseconds of mouse hover before load.
var highlightFocusedResult = true;   // Who wouldn't want this?



// Don't run if we are in a sub-frame:
if (window != top)
	return;

// On webhp pages, I think the content is loaded by Javascript.  We must wait
// for the page to finish loading before we can find the resultsBlock.

// FIXED in GrimeApe:  Dagnammit Konqueror needs document.onload and Firefox needs window.onload.
// var browsersSuck = ( navigator.appName.match(/Konqueror/i) ? document : window );

window.addEventListener('load',function(){

	var resultsWidth = 1.0 - previewWidth;

	var resultsBlock = document.getElementById("res");

	if (fillWholeWindow) {
		pageHeightUsed = 0.97;
		resultsBlock = document.createElement("DIV");

		// Copy wanted parts of window into our left-pane block:
		if (keepHeaderAbove) {
			pageHeightUsed = ( miniLogo||removeLogo ? 0.80 : 0.75 );
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

		if (reduceWidth) {

			// If we pulled more stuff than just the results into the left pane,
			// then we may need to reduce the width of the contents, or we will
			// get an unpleasant horizontal scrollbar.
			// Just about everything except the results can become too wide at
			// some point, so this is quite a fiddle...

			var nobr = document.getElementsByTagName('nobr')[0];
			nobr.parentNode.innerHTML = nobr.innerHTML;
			// document.getElementsByTagName('form')[0].getElementsByTagName("input")[1].size = 20;
			var inputs = document.getElementsByTagName("INPUT");
			for (var i=0;i<inputs.length;i++) {
				if (inputs[i].name == 'q')
					inputs[i].size = 20;
			}

			// This avoids the white-space: wrap but kills the blue header background.
			document.getElementById("ssb").id = 'not_ssb';
			// OK we restore the blue background:
			document.getElementById("not_ssb").style.backgroundColor = '#F0F7F9';
			document.getElementById("not_ssb").style.borderTop = '1px solid #6890DA';
			// TODO: But vertical alignment of text is still wrong.
			// document.getElementById("not_ssb").style.verticalAlign = 'middle';
			var resText = document.getElementById('prs').nextSibling;
			resText.style.textAlign = 'right';

			document.getElementsByTagName("bsf").padding = '0px';

			// TODO:
			// Google gave me a lot of "Searches related to: resize window iframe" at the bottom
			// which were too wide (5 column table, one with long word "addeventlistener").
			// Users with very narrow pane may not be able to fit the 10 next page images.

		}

		if (removeLogo || miniLogo) {
			var logo = document.getElementsByTagName("IMG")[0];
			logo = logo.parentNode.parentNode; // A
			if (!miniLogo)
				logo = logo.parentNode; // TD
			var pNode = logo.parentNode;
			pNode.removeChild(logo);
			document.getElementById('sff').getElementsByTagName('table')[0].style.marginTop = '5px'
			if (miniLogo) {
				var newImg = document.createElement("IMG");
				// newImg.src = "/favicon.ico";
				newImg.src = "http://www.google.com/intl/en_ALL/images/logo.gif";
				newImg.width = 276/3;
				newImg.height = 110/3;
				pNode.appendChild(newImg);
			}
		}

		document.getElementById("res").style.width = (window.innerWidth * resultsWidth - 48) +'px';

	}

	// GM_log("resultsBlock = " + resultsBlock);

	var table = document.createElement("TABLE");
	var tbody = document.createElement("TBODY");
	var row = document.createElement("TR");
	var leftCell = document.createElement("TD");
	var rightCell = document.createElement("TD");

	leftCell.width = resultsWidth*100+"%";
	rightCell.width = previewWidth*100+"%";
	// rightCell.style.width = (window.innerWidth * previewWidth) +'px';
	// leftCell.style.width = (window.innerWidth * resultsWidth) +'px';
	// rightCell.style.width = (window.innerWidth * previewWidth) +'px';
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
	var currentTimerID = null;

	function highlightNode(node,col) {
		node = isSelectable(node);
		node.style.backgroundColor = col;
	}

	function checkFocus() {
		if (lastHover && lastHover != lastPreview) {
			// GM_log("Previewing "+lastHover.href);
			lastHover = isSelectable(lastHover);
			if (highlightFocusedResult) {
				// if (lastPreview)
					// lastPreview.style.backgroundColor = "";
				// lastHover.style.backgroundColor = "#ccccff"; // "#ffccff";
				if (lastPreview)
					highlightNode(lastPreview,'');
				highlightNode(lastHover,'#eeeeff');
			}
			var link = lastHover;
			if (link.tagName != "A") {
				link = lastHover.getElementsByTagName('A')[0];
			}
			iframe.src = link.href;
			lastPreview = lastHover;
		}
	}

	function isSelectable(node) {
		while (node) {
			if (node.tagName == "A") {
				if (node.className == 'l' && node.onmousedown) {
					// This is a Google result.  We skip it, to select the parent 'g' DIV
					// but will use this link (let's hope it is the first in the DIV!).
				} else {
					// A link but not a normal result.  Use it directly:
					return node;
				}
			}
			if (node.className == 'g' || node.className == 'g w0' || node.tagName=='LI') {
				return node;
			}
			node = node.parentNode;
		}
		return undefined;
	}

	function helloMouse(evt) {
		var node = evt.target;
		if (isSelectable(node) && isSelectable(node)!=lastPreview) {
			lastHover = node;
			if (currentTimerID)
				clearTimeout(currentTimerID);
			currentTimerID = setTimeout(checkFocus,hoverTime);
			if (highlightFocusedResult)
				highlightNode(node,'#eeffee');
		}
	}

	function goodbyeMouse(evt) {
		var node = evt.target;
		if (isSelectable(node) && isSelectable(node)!=lastPreview) {
			lastHover = null;
			if (highlightFocusedResult)
				highlightNode(node,'');
		}
	}

	resultsBlock.addEventListener('mouseover',helloMouse,false);
	resultsBlock.addEventListener('mouseout',goodbyeMouse,false);

	// The "Sponsored Links" block gets in the way.
	var toKill = document.getElementById("mbEnd");
	if (toKill) {
		toKill.parentNode.removeChild(toKill);
	}

},false);


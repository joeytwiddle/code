// ==UserScript==
// @name           Google Preview Pane
// @namespace      http://userscripts.org/users/89794
// @description    Loads Google results in a Preview Pane on mouse hover
// @include        http://www.google.*/search?*q=*
// @include        http://www.google.*/webhp?*q=*
// @include        http://google.*/search?*q=*
// @include        http://google.*/webhp?*q=*
// @version        0.9.9.2
// ==/UserScript==

// Settings:

var fillWholeWindow = true;    // Bring more of the page into the left pane.
var keepHeaderAbove = true;   // Avoid bringing the top of the page in.
var miniLogo = true;           // miniLogo or removeLogo help to reduce the
var removeLogo = false;        // width and the height of header.
var reduceWidth = !keepHeaderAbove;  // Shrink the width of the header and footer.
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
window.addEventListener('load',function(){



	//// Set up the Layout ////

	var resultsWidth = 1.0 - previewWidth;

	var resultsBlock = document.getElementById("res");

	// Reformatting which might break
	try {

		if (fillWholeWindow) {

			pageHeightUsed = 0.97;
			resultsBlock = document.createElement("DIV");

			// Copy wanted parts of window into our left-pane block:
			if (keepHeaderAbove) {
				pageHeightUsed = ( miniLogo||removeLogo ? 0.80 : 0.72 );
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

			// I think only really needed if keepHeaderAbove==false.
			try {
				var annoyingLine = document.getElementsByClassName("gbh")[0];
				annoyingLine.parentNode.removeChild(annoyingLine);
				annoyingLine = document.getElementsByClassName("gbh")[0];
				annoyingLine.parentNode.removeChild(annoyingLine);
			} catch (e) { }

			if (reduceWidth) {

				// Makes a few pieces of text smaller, but most have custom style font sizes.
				document.body.style.fontSize='10px';

				// If we pulled more stuff than just the results into the left pane,
				// then we may need to reduce the width of the contents, or we will
				// get an unpleasant horizontal scrollbar.
				// Just about everything except the results can become too wide at
				// some point, so this is quite a fiddle...

				// The first nobr was a lone child when I wrote this.
				// We break it.
				var nobr = document.getElementsByTagName('NOBR')[0];
				nobr.parentNode.innerHTML = nobr.innerHTML;

				// document.getElementsByTagName('form')[0].getElementsByTagName("input")[1].size = 20;
				var inputs = document.getElementsByTagName("INPUT");
				for (var i=0;i<inputs.length;i++) {
					if (inputs[i].name == 'q')
						inputs[i].size = 10;
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

				// TODO:
				// Google gave me a lot of "Searches related to: resize window iframe" at the bottom
				// which were too wide (5 column table, one with long word "addeventlistener").
				// Users with very narrow pane may not be able to fit the 10 next page images.

				// Footer
				document.getElementById("bsf").padding = '0px';

			}

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
				newImg.width = 276/5;
				newImg.height = 110/5;
				pNode.appendChild(newImg);
				pNode.style.paddingTop = '8px';
				pNode.style.paddingRight = '0px';
				document.getElementById('sft').style.marginTop = '0px';
				document.getElementById('sft').style.marginBottom = '0px';
				document.getElementById('sff').style.paddingBottom = '0px';
			}
		}

	} catch (e) {
		GM_log("Error during layout: "+e);
	}

	var table     = document.createElement("TABLE");
	var tbody     = document.createElement("TBODY");
	var row       = document.createElement("TR");
	var leftCell  = document.createElement("TD");
	var rightCell = document.createElement("TD");

	leftCell.width = resultsWidth*100+"%";
	rightCell.width = previewWidth*100+"%";

	// If we leave room for vertical scrollbar, we won't need horizontal one. :)
	document.getElementById("res").style.width = (window.innerWidth * resultsWidth - 48) +'px';
	// resultsBlock.style.width = (window.innerWidth * resultsWidth) + 'px';
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

	resultsBlock.parentNode.insertBefore(table,resultsBlock.nextSibling);
	leftCell.appendChild(resultsBlock);

	// The "Sponsored Links" block gets in the way.
	var toKill = document.getElementById("mbEnd");
	if (toKill) {
		toKill.parentNode.removeChild(toKill);
	}



	//// Listeners ////

	var lastHover = null;
	var lastPreview = null;
	var currentTimerID = null;

	function highlightNode(node,col) {
		if (highlightFocusedResult) {
			node = getContainer(node);
			node.style.backgroundColor = col;
		}
	}

	function checkFocus() {
		if (lastHover && getContainer(lastHover) != lastPreview) {
			if (lastPreview)
				highlightNode(lastPreview,'');
			// We don't really seem to use lastHover much - we could have set it
			// to container from the start.
			highlightNode(lastHover,'#eeeeff');
			// GM_log("Got lastHover = "+lastHover);
			var container = getContainer(lastHover);
			// GM_log("Got container = "+container);
			var link = ( container.tagName == "A" ? container : container.getElementsByTagName('A')[0] );
			// GM_log("Got link = "+link);
			iframe.src = link.href;
			// lastPreview = lastHover;
			lastPreview = container; // normalises - two different nodes might both hit the same container
		}
	}

	function getContainer(startNode) {
		GM_log("Got startNode = "+startNode);
		var node = startNode;
		var link = null; // node.getElementsByTagName('A')[0];
		// To make it easier to select links, they can select results by hovering over the non-link areas.
		// We check this by going up parent nodes until we find a link, or hit the top of a results block.
		while (node) {
			if (!link) {
				if (node.tagName == "A") {
					link = node;
				}
			}
			var goUp = true;
			if (link) {
				// If we have found a link, only go up if that link was a main result.
				// Otherwise the Cache selects the div above it, which looks weird.
				if (link.className == 'l') {
				} else {
					return link;
				}
			}
			// If we have a link, we must not go to the parent if the parent
			// contains any earlier links.  Unlikely given former check.
			if (link != null) {
				var parentsFirstLink = node.parentNode.getElementsByTagName('A')[0];
				if (parentsFirstLink != link) {
					goUp = false;
				}
			}
			// Or have we reached a google result block?  If so, stop here.
			if ( node.tagName=='LI' || node.className == 'g' || node.className == 'g w0' ) {
				goUp = false;
				// link will be first 'A' child.
			}
			if (!goUp) {
				// We better check we do contain a link!
				return node;
			}
			node = node.parentNode;
		}
		// Don't highlight the whole document, but highlight the link if we got one.
		return link;
	}

	function helloMouse(evt) {
		var node = evt.target;
		var container = getContainer(node);
		// Should we hover on this?
		if (container && container!=lastPreview) {
			// If we were just hovered on something, unhighlight it:
			if (lastHover && getContainer(lastHover)!=lastPreview)
				highlightNode(lastHover,'');
			// OK start hover on this.  checkFocus() will check if we are still
			// here in hoverTime ms, and if so activate.
			lastHover = node;
			if (currentTimerID)
				clearTimeout(currentTimerID);
			currentTimerID = setTimeout(checkFocus,hoverTime);
			highlightNode(node,'#eeffee');
		}
	}

	function goodbyeMouse(evt) {
		var node = evt.target;
		var container = getContainer(node);
		// Should we stop hovering on this?
		if (container && container!=lastPreview) {
			// Clear highlight:
			if (lastHover && getContainer(lastHover)!=lastPreview)
				highlightNode(lastHover,'');
			lastHover = null;
			highlightNode(node,'');
			// We don't need to clearTimeout checkFocus(), it knows we left.
		}
	}

	resultsBlock.addEventListener('mouseover',helloMouse,false);
	resultsBlock.addEventListener('mouseout',goodbyeMouse,false);



},false);



/* CONSIDER: If we were to arrange things in a two or three framed page, the
 * user would be able to resize the results/preview panes by dragging their
 * vertical bar separator.  The SplitPreview bookmarklet demonstrates this.
 */


// ==UserScript==
// @name           Google Preview Pane
// @namespace      http://userscripts.org/users/89794
// @description    Displays Google results in a Preview Pane so you don't have to leave the results page.
// @include        http://www.google.*/search?*q=*
// @include        http://www.google.*/webhp?*q=*
// @include        http://google.*/search?*q=*
// @include        http://google.*/webhp?*q=*
// @version        0.9.9.4
// ==/UserScript==

// Settings:

var previewWidth    = 0.7;     // Width of the preview pane (proportion of window width).
var gapBelow        = 16;      // Space left below panes (pixels).
var fillWholeWindow = true;    // Bring more of the page into the left pane.
var keepHeaderAbove = true;    // Do not bring the top of the page in.

var highlightHover  = true;    // Change colour of current / hovered item.
var focusWithHover  = false;   // If set, does not wait for click before loading preview.
var hoverTime       = 800;     // Milliseconds of mouse hover before preview.

var miniLogo        = true;    // miniLogo or removeLogo can help to reduce the
var removeLogo      = false;   // width and the height of the header.
var reduceWidth     = true;    // Try lots of little things to make things fit into the left pane.
var panelHasBorder  = true;    // I like the preview pane to look sunken.



/* ==================
 *
 * CHANGES in 0.9.9.4
 *
 * Frames now resize when window is resized.  Dropped setting pageHeightUsed
 * for gapBelow.
 *
 * ==================
 *
 * CHANGES in 0.9.9.3
 *
 * Made hovering optional.  Default now uses click to preview (second click to
 * follow link as usual).  Set focusWithHover=true for old behaviour.
 *
 * ================== */

// Don't run if we are in a sub-frame:
if (window != top)
	return;


function initPreview() {

	//// Set up the Layout ////

	var resultsWidth = 1.0 - previewWidth;

	var resultsBlock = document.getElementById("res");

	// Reformatting which might break
	try {

		if (fillWholeWindow) {

			resultsBlock = document.createElement("DIV");

			// Copy wanted parts of window into our left-pane block:
			if (keepHeaderAbove) {
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

			// Only really needed if keepHeaderAbove==false.
			try {
				// Without this, the blue line at the top of the light blue bar
				// stretches over the preview!
				var annoyingLine = document.getElementsByClassName("gbh")[0];
				annoyingLine.parentNode.removeChild(annoyingLine);
				annoyingLine = document.getElementsByClassName("gbh")[0];
				annoyingLine.parentNode.removeChild(annoyingLine);
			} catch (e) { }

			// If we pulled more stuff than just the results into the left pane,
			// then we may need to reduce the width of the contents, or we will
			// get an unpleasant horizontal scrollbar.
			// Just about everything except the results can become too wide at
			// some point, so this is quite a fiddle...

			// document.getElementsByTagName('form')[0].getElementsByTagName("input")[1].size = 20;
			var inputs = resultsBlock.getElementsByTagName("INPUT");
			for (var i=0;i<inputs.length;i++) {
				if (inputs[i].name == 'q')
					inputs[i].size = 10;
			}

			if (reduceWidth) {

				// TODO: Sometimes it is the row of Next/Previous Goooooooogle results.
				// Google gave me a lot of "Searches related to: resize window iframe" at the bottom
				// which were too wide (5 column table, one with long word "addeventlistener").

				// TODO: Sometimes it is the <cite> URL of the result that it too wide!  (They wrap in Moz but not in Konq.))
				// Or the result summary text may contain a really long word which does not wrap.

				// Makes a few pieces of text smaller, but most have custom style font sizes.
				document.body.style.fontSize='10px';

				// The first nobr was a lone child when I wrote this.
				// We break it.
				var nobr = document.getElementsByTagName('NOBR')[0];
				nobr.parentNode.innerHTML = nobr.innerHTML;

				// This avoids the white-space: wrap but kills the blue header background.
				document.getElementById("ssb").id = 'not_ssb';
				// OK we restore the blue background:
				document.getElementById("not_ssb").style.backgroundColor = '#F0F7F9';
				document.getElementById("not_ssb").style.borderTop = '1px solid #6890DA';
				// TODO: Vertical alignment of text is still wrong.
				// document.getElementById("not_ssb").style.verticalAlign = 'middle';
				var resText = document.getElementById('prs').nextSibling;
				resText.style.textAlign = 'right';

				// Footer
				document.getElementById("bsf").padding = '0px';

			}

			// The last Goooooooogle TD has a silly huge margin
			var tds = document.getElementById('nav').getElementsByTagName('td');
			tds[tds.length-1].getElementsByTagName('span')[0].style.marginRight = '0px';

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

		// The "Sponsored Links" block gets in the way.
		var toKill = document.getElementById("mbEnd");
		if (toKill) {
			toKill.parentNode.removeChild(toKill);
		}

	} catch (e) {
		GM_log("Error during layout: "+e);
	}



	var table     = document.createElement("TABLE");
	var tbody     = document.createElement("TBODY");
	var row       = document.createElement("TR");
	var leftCell  = document.createElement("TD");
	var rightCell = document.createElement("TD");

	var previewFrame = document.createElement('IFRAME');
	previewFrame.style.backgroundColor = '#eeeeee';
	if (!panelHasBorder)
		previewFrame.style.border = '0px solid white';
	rightCell.appendChild(previewFrame);

	tbody.appendChild(row);
	table.appendChild(tbody);
	row.appendChild(leftCell);
	row.appendChild(rightCell);

	resultsBlock.parentNode.insertBefore(table,resultsBlock.nextSibling);
	leftCell.appendChild(resultsBlock);

	function setDimensions() {
		leftCell.width = resultsWidth*100+"%";
		rightCell.width = previewWidth*100+"%";
		//// If we leave room for vertical scrollbar, we won't need horizontal one. :)
		// Better without: resultsBlock.style.width = (window.innerWidth * resultsWidth - 48) +'px';
		resultsBlock.style.overflow = 'auto';
		previewFrame.width = '100%';
		// Old: resultsBlock.style.height = (window.innerHeight * pageHeightUsed) + 'px';
		// Old: previewFrame.height = (window.innerHeight * pageHeightUsed) + 'px';
		var heightFree = window.innerHeight - table.offsetTop - gapBelow;
		resultsBlock.style.height = heightFree+'px';
		previewFrame.height = heightFree+'px';
	}
	setDimensions();
	// We will call setDimensions again if the user resizes the window:
	window.addEventListener('resize',setDimensions,false);



	//// Listeners ////

	var lastHover = null;
	var lastPreview = null;
	var currentTimerID = null;

	function highlightNode(node,col) {
		if (highlightHover) {
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
			highlightNode(lastHover,'#ddeeff');
			// GM_log("Got lastHover = "+lastHover);
			var container = getContainer(lastHover);
			// GM_log("Got container = "+container);
			var link = ( container.tagName == "A" ? container : container.getElementsByTagName('A')[0] );
			// GM_log("Got link = "+link);
			previewFrame.src = link.href;
			// lastPreview = lastHover;
			lastPreview = container; // normalises - two different nodes might both hit the same container
			return true;
		}
		return false;
	}

	function checkClick(evt) {
		var node = evt.target;
		if (node == lastHover) {
			// If the user is selecting a link to another results page, they
			// probably don't want a preview!
			if (node.tagName=='A' && node.host==document.location.host
				&& node.pathname.match('/(search|webhp)')
			) {
				// We will pass the event up to click on the actual link.
			} else {
				// Let's try to Preview what the user clicked
				if (checkFocus()) {
					// OK we set focus, preview is loading.
					evt.preventDefault();
				} else {
					// Well we didn't want to focus this node.
					// Let's pass the event to other elements.
				}
			}
		}
	}

	function getContainer(startNode) {
		// GM_log("Got startNode = "+startNode);
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
			// CONSIDER: My only remaining niggle with this algorithm is that the
			// Delicious Results userscript makes 'l' class links, but there is no
			// parent LI, so the very first one highlights the whole block.
			// We could fix this by aborting ascent if we can find other 'l' links
			// in the parent (rather than just an earlier link).
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
			if (focusWithHover) {
				if (currentTimerID)
					clearTimeout(currentTimerID);
				currentTimerID = setTimeout(checkFocus,hoverTime);
			}
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
	if (!focusWithHover)
		resultsBlock.addEventListener('click',checkClick,false);

}



var earlyResultsBlock = document.getElementById('res');
if (earlyResultsBlock) {
	initPreview();
} else {
	window.addEventListener('load',initPreview,false);
	// On webhp pages, I think the content is loaded by Javascript.  We must
	// wait for the page to finish loading before we can find the resultsBlock.
}



/*

CONSIDER: If we were to arrange things in a two or three framed page, the
user would be able to resize the results/preview panes by dragging their
vertical bar separator.  The SplitPreview bookmarklet demonstrates this.

TODO: We could extra highlight the link that will be followed.  It's not
always obvious.

TODO: New double-click to load behaviour is annoying for links which point
to further Google Results pages, e.g. Next/Prev/Similar pages.

TODO: We don't need the setTimeout on non-webhp pages.  We should check to
see if resultsBlock exists first, to avoid using it unneccessarily.

*/


// ==UserScript==
// @name           Hover Preview
// @namespace      noggieb
// @description    Shows a tooltip when you hover over links, containing the target page!
// @include        *
// ==/UserScript==

// TODO:
// Don't act if the target is a file-type.  i.e. we don't want to be prompted
// to save a zip file just because we hovered on it.
// Dammit some pages break out of the iframe!

// Quite nice on apache file listings of .jpegs, but a bit slow.  Ideally pre-load hoverable images?
// Could be a bit heavy.  It depends on the page...

var focus = undefined;
var lastFocus = undefined;

var myPopup;
var myFrame;

function checkFocus() {
	if (focus) {
		// if (focus == lastFocus) {
			// User has definitely been here a while
			showPreviewWindow(focus);
		// } else {
		// }
		// lastFocus = focus;
	}
}

function eekAMouse(evt) {
	if (!focus) {
		focus = evt.currentTarget;
		// setTimeout('checkFocus();',1000);
		// Hack to bring the popup back immediately if we've gone back to the same link.
		if (myFrame && myFrame.href && myFrame.href == focus.href) {
			showPreviewWindow(focus);
		} else {
			setTimeout(checkFocus,1000);
		}
	} else {
		window.status = "Already focused on a link wtf!";
	}
}

function phewMouseGone(evt) {
	focus = undefined;
	// TODO: Don't hide the popup if mouse is currently over the popup!
	if (myPopup) {
		// myPopup.parentNode.removeChild(myPopup);
		// myPopup = undefined; // eww cache it!
		myPopup.style.display = 'none';
	}
}

// DONE: If the user clicks a link, this isn't really a hover, so we should not
// activate and just let the user's click be processed!
function aClick(evt) {
	focus = undefined;
}

function showPreviewWindow(link) {
	if (!myFrame) {
		// Create frame
		myPopup = document.createElement('DIV');
		/** Seems style does not work for Konqueror this way. **/
		myPopup.innerHTML =
			"<STYLE type='text/css'> iframe.preview { color: white; background-color: #aaaaaa; position: fixed; right: 12px; bottom: 12px; z-index: 10000; padding: 4px; border: 2px solid #000000; text-align: center; } </STYLE>"
			+
			"<IFRAME class='preview' width='"+(window.innerWidth/2)+"' height='"+(window.innerHeight/2)+"' src='about:blank'></IFRAME>";
		
		document.documentElement.appendChild(myPopup);
		myFrame = myPopup.getElementsByTagName('IFRAME')[0];
	}
	myPopup.style.display = '';
	if (!myFrame.src || myFrame.src != link.href)
		myFrame.src = link.href;
}

function init() {
	for (var i=0;i<document.links.length;i++) {
		var link = document.links[i];
		/** Apparently deprecated. **/
		// link.onmouseover = eekAMouse;
		// link.onmouseout = phewMouseGone;
		/** The new way: **/
		link.addEventListener("mouseover", function(evt) { eekAMouse(evt) }, false);
		link.addEventListener("mouseout", function(evt) { phewMouseGone(); }, false);
		link.addEventListener("click", function(evt) { aClick(); }, false);
		// link.addEventListener("mousemove", function(evt) { locate(evt); }, true);
	}
}

init();

// window.document.checkFocus = checkFocus;


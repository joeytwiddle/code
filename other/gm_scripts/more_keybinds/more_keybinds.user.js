// ==UserScript==
// @name           More Keybinds
// @namespace      MK
// @description    Adds some extra keystrokes to Firefox.
// @version        1.1.4
// @include        *
// ==/UserScript==

var SCROLL_AMOUNT = 60;

// Not all keys fire a keypress event (Chrome 2010), so we use keydown.
document.addEventListener('keydown', keypressListener, false);

function keypressListener(evt) {
	var code = evt.keyCode || evt.which;

	/*
	var modifierReport = "";
	modifierReport += ( evt.ctrlKey  ? "Ctrl "  : "" );
	modifierReport += ( evt.shiftKey ? "Shift " : "" );
	modifierReport += ( evt.altKey   ? "Alt "   : "" );
	GM_log("Caught keypress "+code+" with modifiers: "+modifierReport);
	*/

	// Actions

	// Ctrl+Delete goes Back
	if (code == 8 && evt.ctrlKey) {
		window.history.back();
	}

	// Ctrl+Enter goes Forward
	if (code == 13 && evt.ctrlKey) {
		window.history.forward();
	}

	/* These conflict with selecting words in text!

	// Ctrl+Shift+Left goes Back
	if (code == 37 && evt.ctrlKey && evt.shiftKey) {
		window.history.back();
	}

	// Ctrl+Shift+Right goes Forward
	if (code == 39 && evt.ctrlKey && evt.shiftKey) {
		window.history.forward();
	}

	*/

	// Ctrl+Shift+Up goes up in the URL path (removes the tail leaf)
	if (code == 38 && evt.ctrlKey && evt.shiftKey) {
		var newURL = document.location.href;
		if (newURL.slice(-1)=='/') {
			newURL = newURL.slice(0,-1);
		}
		document.location.href = document.location.href.replace(/[#/?][^#/?]*[/]*$/,'');
	}

	// Do not intercept any of the keys below when the user is focused on an input or textarea.
	/*
	//var focusedElement = document.activeElement;   // document.body if no input is focused
	var focusedElement = evt.target || event.srcElement;
	if (focusedElement) {
		var isInput = focusedElement.nodeName === 'INPUT' || focusedElement.nodeName === 'TEXTAREA';
		if (isInput) {
			return;
		}
	}
	*/
	// From next_imageprevious_image.user.js:
	if (evt.target.tagName && evt.target.tagName.match(/input|select|textarea/i) || event.target.getAttribute('contenteditable')==="true") {
		return;
	}

	if (!evt.ctrlKey && !evt.shiftKey && !evt.metaKey) {
		if (document.location.host !== "9gag.com") {
			if (code === 'K'.charCodeAt(0)) {
				scrollBy(-getScrollAmount());
			}

			if (code === 'J'.charCodeAt(0)) {
				scrollBy(+getScrollAmount());
			}
		}
	}

}

function scrollBy(amount) {
	// If jQuery is present, use it to perform a smooth scroll
	if (typeof $ !== "undefined") {
		$("html,body").animate({scrollTop: $(document).scrollTop() + amount}, 200);
	} else {
		// Chrome:
		document.body.scrollTop += amount;
		// Firefox:
		document.documentElement.scrollTop += amount;
	}
}

function getScrollAmount() {
	return window.innerHeight / 6;
}

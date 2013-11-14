// ==UserScript==
// @name           More Keybinds
// @namespace      MK
// @description    Adds some extra keystrokes to Firefox.
// @include        *
// ==/UserScript==

// Not all keys fire a keypress event (Chrome 2010), so we use keydown.
document.addEventListener('keydown', keypressListener, false);

function keypressListener(evt) {
	var code = evt.keyCode || evt.which;

	// TODO: We should skip most of these checks if the user is focused on an input box or textarea.

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

	// Ctrl+Shift+Up goes Up
	if (code == 38 && evt.ctrlKey && evt.shiftKey) {
		var newURL = document.location.href;
		if (newURL.slice(-1)=='/') {
			newURL = newURL.slice(0,-1);
		}
		document.location.href = document.location.href.replace(/[#/?][^#/?]*[/]*$/,'');
	}

}


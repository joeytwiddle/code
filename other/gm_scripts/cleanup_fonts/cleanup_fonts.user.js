// ==UserScript==
// @name           Cleanup Fonts
// @namespace      noggieb
// @description    Removes unwanted fonts and uses your own.
// @include        *
// ==/UserScript==

// Pick your favourite fonts:
var mySans = "FreeSans" // "sans";
var mySerif = "sans"; // "FreeSerif"; // "serif";
var myMono = "monospace"; // "FreeMono"; // "Liberation Mono";
// var myUnknown = mySans;
var myUnknown = "FreeMono"; // yuk for now ;)

// Define which ones to convert:
// The following must be lower-case!
var recognizeSans = ["arial","sans-serif","helvetica","trebuchet","verdana"];
var recognizeSerif = ["georgia","serif"]; // BUG: mmm "serif" will detect "sans-serif" :P
var recognizeMono = ["courier","andale mono","mono"];
var fontsToKeep = ["comic",myUnknown.toLowerCase(),mySans.toLowerCase(),mySerif.toLowerCase(),myMono.toLowerCase()];

var allElements = document.getElementsByTagName('*');

function convertFont(elem,fromList,toFace) {
	var elemFont = (''+getComputedStyle(elem,'').fontFamily).toLowerCase();
	// GM_log("elemFont = "+elemFont);
	// for (var x in fromList) {
	for (var x=0;x<fromList.length;x++) {
		var str = fromList[x];
		if (elemFont.indexOf(str) >= 0) {
		// if (elemFont == str) {
			if (toFace != undefined && toFace != elemFont) {
				GM_log("Changing "+elemFont+" to "+toFace);
				elem.style.fontFamily = toFace;
			}
			return true;
		}
	}
	return false;
}

// TODO: We have to do this in the end, but there might be less work to do if
// we adjust the named CSS styles first.
for (var i = 0; i < allElements.length; i++) {
	var elem = allElements[i];
	if (''+elem.style.fontFamily == '' && (''+getComputedStyle(elem,'').fontFamily) == '')
		continue; // browser default
	var changed = convertFont(elem,recognizeSans,mySans)
	 	|| convertFont(elem,recognizeSerif,mySerif)
	 	|| convertFont(elem,recognizeMono,myMono)
	 	|| convertFont(elem,fontsToKeep,undefined);
	if (!changed) {
		GM_log("Do not recognise font: "+(''+getComputedStyle(elem,'').fontFamily));
		elem.style.fontFamily = myUnknown;
	}
	 // if (/^courier($|,)/i.test(elementStyle.fontFamily)) {
		  // allElements[i].style.fontFamily = 'Courier New';
	 // }
}


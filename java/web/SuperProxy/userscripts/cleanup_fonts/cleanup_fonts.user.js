// ==UserScript==
// @name           Cleanup Fonts
// @namespace      noggieb
// @description    Removes unwanted fonts and uses your own.
// @include        *
// ==/UserScript==

// TODO: OK we've done CSS, but what about traditional <FONT face=...> or is it family?

// Pick your favourite fonts:
var mySans = "FreeSans" // "sans";
var mySerif = "FreeSerif"; // "sans"; // "serif";
var myMono = "monospace"; // "FreeMono"; // "Liberation Mono";
// var myUnknown = mySans;
var myUnknown = "FreeMono"; // yuk for now ;)

// Define which ones to convert:
// The following must be lower-case!
var recognizeSans = ["arial","sans-serif","helvetica","trebuchet","verdana"];
var recognizeSerif = ["georgia","serif"]; // BUG: mmm "serif" will detect "sans-serif" :P
var recognizeMono = ["courier","andale mono","mono"];
var fontsToKeep = ["comic",myUnknown.toLowerCase(),mySans.toLowerCase(),mySerif.toLowerCase(),myMono.toLowerCase()];

function convertFont(elemFont,elem,fromList,toFace) {
	// var elemFont = (''+getComputedStyle(elem,'').fontFamily).toLowerCase();
	// GM_log("elemFont = "+elemFont);
	// for (var x in fromList) {
	for (var x=0;x<fromList.length;x++) {
		var str = fromList[x];
		if (elemFont.indexOf(str) >= 0) {
		// if (elemFont == str) {
			if (toFace != undefined && toFace != elemFont) {
				GM_log("Changing "+elem+": "+elemFont+" -> "+toFace);
				elem.style.fontFamily = toFace;
			}
			return true;
		}
	}
	return false;
}

// DOING: We have to do this in the end, but there might be less work to do if
// we adjust the named CSS styles first.

function doThing(elem) {
	// if (''+elem.style.fontFamily == '' && (''+getComputedStyle(elem,'').fontFamily) == '')
		// return; // continue; // browser default
	// var elemFont = (''+getComputedStyle(elem,'').fontFamily).toLowerCase();
	if (!elem.style)
		return;
	// GM_log("elem="+elem+" font="+elem.style.font+" fontFamily="+elem.style.fontFamily); // +" computedFontFamily="+getComputedStyle(elem,'').fontFamily);
	var computed;
	try { computed = (""+getComputedStyle(elem,'').fontFamily).toLowerCase(); } catch (e) { GM_log("Failed to get computed."); }
	var elemFont = (""+elem.style.fontFamily).toLowerCase();
	if ((!elemFont) && computed) {
		elemFont = computed;
		GM_log("Had to use computed");
	}
	if (!elemFont)
		return;

	// if (elem.style && ""+elem.style.fontFamily) {
		GM_log("Checking: "+elem+" with "+elemFont);
		// if (!elemFont)
			// return;
		var changed = convertFont(elemFont,elem,recognizeSans,mySans)
			|| convertFont(elemFont,elem,recognizeSerif,mySerif)
			|| convertFont(elemFont,elem,recognizeMono,myMono)
			|| convertFont(elemFont,elem,fontsToKeep,undefined);
		if (!changed) {
			// GM_log("Do not recognise font: "+(''+getComputedStyle(elem,'').fontFamily));
			GM_log("Do not recognise font: "+elemFont+" so setting "+myUnknown);
			elem.style.fontFamily = myUnknown;
		}
		 // if (/^courier($|,)/i.test(elementStyle.fontFamily)) {
			  // allElements[i].style.fontFamily = 'Courier New';
		 // }
	// }
}

if (document.styleSheets) {
	for (var i=0;i<document.styleSheets.length;i++) {
		var rules = document.styleSheets[i].cssRules;
		for (var j=0;j<rules.length;j++) {
			var rule = rules[j];
			// if (rule.style && rule.style.fontFamily != '') {
				// var ruleFont = rule.style.fontFamily.toLowerCase();
				// convertFont(ruleFont,rule.style,
				if (rule.style) {
					if (rule.style.font) {
						GM_log("Rule has font = "+rule.style.font);
					}
					if (rule.style.fontFamily) {
						GM_log("Rule has fontFamily = "+rule.style.fontFamily);
					}
				}
				doThing(rule);
			// }
		}
	}
}

var allStyleElements = document.getElementsByTagName('STYLE');
for (var i = 0; i < allStyleElements.length; i++) {
	var elem = allStyleElements[i];
	doThing(elem);
}

var allElements = document.getElementsByTagName('*');
for (var i = 0; i < allElements.length; i++) {
	var elem = allElements[i];
	doThing(elem);
}


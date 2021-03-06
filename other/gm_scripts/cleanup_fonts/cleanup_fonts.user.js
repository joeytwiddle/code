// ==UserScript==
// @name           Cleanup Fonts
// @namespace      joeytwiddle
// @description    Removes unwanted fonts and applies your own.  (But I recommend my "Kill Fonts" bookmarklet instead.)
// @include        *
// ==/UserScript==

// TODO: OK we've done CSS, but what about traditional <FONT face=...> or is it family?

// DONE: There is special fontFamily "inherit" which we should not break.

// To disable debug logging:
// var GM_log = function() { };

// Pick your favourite fonts:
var mySans = "FreeSans" // "sans";
var mySerif = "FreeSerif"; // "sans"; // "serif";
var myMono = /*"monospace";*/ /*For Konq:*/ "Liberation Mono";
// var myUnknown = mySans;
var myUnknown = "FreeMono"; // yuk for now ;)

// Define which ones to convert:
// The following must be lower-case!
var recognizeSans = ["arial","sans-serif","helvetica","trebuchet","verdana","tahoma"];
var recognizeSerif = ["georgia","serif"]; // BUG: mmm "serif" will detect "sans-serif" :P
var recognizeMono = ["courier","andale mono","mono"];
var fontsToKeep = ["comic",myUnknown.toLowerCase(),mySans.toLowerCase(),mySerif.toLowerCase(),myMono.toLowerCase()];

function convertFont(elemFont,elem,fromList,toFace) {
	// var elemFont = (''+getComputedStyle(elem,'').fontFamily).toLowerCase();
	// for (var x in fromList) {
	if (elemFont == "inherit")
		return true;
	for (var x=0;x<fromList.length;x++) {
		var str = fromList[x];
		if (elemFont.indexOf(str) >= 0) {
		// if (elemFont == str) {
			if (toFace != undefined && toFace != elemFont) {
				console.log("Changing "+elem+": "+elemFont+" -> "+toFace);
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
	var computed;
	try {
		computed = (""+getComputedStyle(elem,'').fontFamily).toLowerCase();
	} catch (e) {
		//// This only seems to fail on stylesheets/blocks, which is fine.
	}
	var elemFont = (""+elem.style.fontFamily).toLowerCase();
	if ((!elemFont) && computed) {
		elemFont = computed;
	}
	if (!elemFont)
		return;

	// if (elem.style && ""+elem.style.fontFamily) {
		// if (!elemFont)
			// return;
		var changed = convertFont(elemFont,elem,recognizeSans,mySans)
			|| convertFont(elemFont,elem,recognizeSerif,mySerif)
			|| convertFont(elemFont,elem,recognizeMono,myMono)
			|| convertFont(elemFont,elem,fontsToKeep,undefined);
		if (!changed) {
			console.log("Do not recognise font: "+elemFont+" so setting "+myUnknown);
			elem.style.fontFamily = myUnknown;
		}
		 // if (/^courier($|,)/i.test(elementStyle.fontFamily)) {
			  // allElements[i].style.fontFamily = 'Courier New';
		 // }
	// }
}



// This is needed for text on the page with no style at all:
unsafeWindow.document.body.style.fontFamily = mySans; // User might want Sans - make another config? :f

if (unsafeWindow.document.styleSheets) {
	for (var i=0;i<unsafeWindow.document.styleSheets.length;i++) {
		var rules = unsafeWindow.document.styleSheets[i].cssRules;
		for (var j=0;j<rules.length;j++) {
			var rule = rules[j];
			// if (rule.style && rule.style.fontFamily != '') {
				// var ruleFont = rule.style.fontFamily.toLowerCase();
				// convertFont(ruleFont,rule.style,
				if (rule.style) {
					if (rule.style.font) {
						// Awh shucks sometimes rule.style.font exists
						// It includes the size and weight; maybe it's just a
						// combination of the other properties, and it will change
						// when we change them?  TODO: check that.
						console.log("Rule has font = "+rule.style.font);
					}
					if (rule.style.fontFamily) {
						console.log("Rule has fontFamily = "+rule.style.fontFamily);
					}
				}
				doThing(rule);
			// }
		}
	}
}

var allStyleElements = unsafeWindow.document.getElementsByTagName('STYLE');
for (var i = 0; i < allStyleElements.length; i++) {
	var elem = allStyleElements[i];
	if (!elem.style)
		console.log("Style block "+i+" has no style!");
	doThing(elem);
}

var allElements = unsafeWindow.document.getElementsByTagName('*');
for (var i = 0; i < allElements.length; i++) {
	var elem = allElements[i];
	doThing(elem);
	// TODO: As well as checking the .style.fontFamily, shouldn't we also check
	// its .font attribute?
	// I suspect that would get overriden by .style.fontFamily in most cases,
	// but what if there was font attrib but no style, is that possible?
}

// This runs but does not work.
/*
var texts = document.evaluate("//text()[normalize-space(.)!='']",document,null,6,null);
for (var i = 0; i < texts.snapshotLength; i++) {
	var elem = texts.snapshotItem(i);
	doThing(elem);
	// TODO: As well as checking the .style.fontFamily, shouldn't we also check
	// its .font attribute?
	// I suspect that would get overriden by .style.fontFamily in most cases,
	// but what if there was font attrib but no style, is that possible?
}
*/

// fontFamily strings are a little fiddle.  I think they look like this:
//   "Trebuchet MS",sans-serif,"bananas",courier
// should be possible to regexp it tho ;)


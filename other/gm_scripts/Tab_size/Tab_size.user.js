// ==UserScript==
// @name          Tab size
// @namespace     http://nv.github.com/tab-size.js/
// @include       *
// @description   Replace all tab characters with two spaces (or something else)
// @version       1.0.3
// @icon          http://userscripts.ru/js/tab-character-size/chrome/icon_48.png
// @grant         GM_addStyle
// @grant         GM_getValue
// @grant         GM_setValue
// @grant         GM_registerMenuCommand
// ==/UserScript==

// TODO: Make it so tabs display using a unicode symbol, but when selected and copied, copy as normal tabs.
// BUG: Our injected <span> HTML becomes visible on http://octodecillion.com/blog/use-groovy-to-find-path-of-a-file-in-a-directory-tree/
//      This is probably due to his syntax highlighter running later.

//var REPLACEMENT = '  ';
//var REPLACEMENT = '->';
//var REPLACEMENT = '| ';
//var REPLACEMENT = '⤚';
//var REPLACEMENT = '→';
//var REPLACEMENT = '⇒';
//var REPLACEMENT = '⇨';
//var REPLACEMENT = '➡';
var REPLACEMENT = '⟼';     // Wider than 1 char even in monospaced fonts
//var REPLACEMENT = '↦';
//var REPLACEMENT = '↠';
//var REPLACEMENT = '⤇';
//➩ ➪ ➫ ➬ ➭ ➮ ➯ ➱
//➟ ➠ 

var style_tabs = true;

if (typeof GM_getValue === 'function') {
	REPLACEMENT = GM_getValue('tab_replacement') || REPLACEMENT;
}

if (style_tabs && typeof GM_addStyle === 'function') {
	GM_addStyle('.tab-size-tab { opacity: 0.2; /* color: rgb(127,127,255); */ }');
	REPLACEMENT = '<span class="tab-size-tab">' + REPLACEMENT + '</span>';
}

var pre_elements = document.body.getElementsByTagName('pre');
replaceTabsInside(pre_elements);

// Note that on Github, we can also append ?ts=4 to any URL to change the tab spacing.
var github_compare_code_lines = document.querySelectorAll('td.blob-code');
replaceTabsInside(github_compare_code_lines);

function replaceTabsInside(elements) {
	for (var i=0; i<elements.length; i++) {
		var no_tabs = elements[i].innerHTML.replace(/\t/g, REPLACEMENT);
		if (elements[i].innerHTML != no_tabs) {
			elements[i].innerHTML = no_tabs;
		}
	}
}

if (typeof GM_registerMenuCommand === 'function') {
	GM_registerMenuCommand('Change tab size...', function setTabReplacement(value){
		GM_setValue('tab_replacement', prompt('Replace tabs with', REPLACEMENT));
	});
}

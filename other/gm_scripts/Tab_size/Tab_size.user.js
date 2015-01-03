// ==UserScript==
// @name          Tab size
// @namespace     http://nv.github.com/tab-size.js/
// @include       *
// @description   Replace all tab characters with two spaces (or something else)
// @version       1.0
// @icon          http://userscripts.ru/js/tab-character-size/chrome/icon_48.png
// @grant         GM_getValue
// @grant         GM_setValue
// @grant         GM_registerMenuCommand
// ==/UserScript==

var REPLACEMENT = '  ';
if (typeof GM_getValue === 'function') {
	REPLACEMENT = GM_getValue('tab_replacement') || REPLACEMENT;
}

var pre_elements = document.body.getElementsByTagName('pre');
replaceTabsInside(pre_elements);

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

// ==UserScript==
// @name           WikiIndent
// @namespace      joeytwiddle
// @include        *wiki*
// @require        http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.js
// ==/UserScript==

//// Features:
var indentSubBlocks = true;
var fixUnderlinesToOverlines = true;
var makeTableOfContentsFloat = true;



function doIt() {



if (indentSubBlocks) {

	function indent(tag) {
		// By targetting search we avoid indenting any blocks in left-hand-column (sidebar).
		var whereToSearch = document.getElementById('bodyContent') || document.getElementById('content') || document;
		var elems = whereToSearch.getElementsByTagName(tag);
		if (elems.length == 1)
			return;
		for (var i=0;i<elems.length;i++) {
			var elem = elems[i];
			if (elem.className == 'firstHeading')
				continue;
			if (elem.id == 'siteSub')
				continue;
			// var newChild = document.createElement('blockquote');
			//// Unfortunately blockquotes tend to indent too much!
			// var newChild = document.createElement('DIV');
			var newChild = document.createElement('UL'); // So it works with my Folding script.
			// newChild.style.paddingLeft = '1.5em';
			var toAdd = elem.nextSibling;
			while (toAdd && toAdd.tagName != tag) {
				var next = toAdd.nextSibling;
				newChild.appendChild(toAdd);
				toAdd = next;
			}
			elem.parentNode.insertBefore(newChild,elem.nextSibling);
			// GM_log("Placed "+newChild+" after "+elem);
		}
	}

	indent("H1"); indent("H2"); indent("H3"); indent("H4"); indent("H5"); indent("H6");

}


// Feature #2: Fix HRs the way I like them

if (fixUnderlinesToOverlines) {

	// @todo
	// ...
	//
	function check(tag) {
		var elems = document.body.getElementsByTagName(tag);
		for (var i=0;i<elems.length;i++) {
			var elem = elems[i];
			var style = getComputedStyle(elem,'');
			if (style['border-bottom'] && !style.borderTop) {
				elem.style.borderTop = style['border-bottom']
				elem.style.borderBottom = '';
			}
		}
	}
	check("H1"); check("H2"); check("H3"); check("H4"); check("H5"); check("H6");
	check("SPAN");

}





if (typeof $ != 'undefined')
GM_log("WikiIndent found "+$);

// document.getElementById('column-one').appendChild(document.getElementById('toc'));





if (makeTableOfContentsFloat) {

	var toc = document.getElementById('toc');
	function theRest() {
		toc.style.position = 'fixed';
		toc.style.right = '16px';
		toc.style.top = '16px';
		toc.style.zIndex = '5000';
	}
	toc.setAttribute('style','background-color: #eeeeee; opacity: 0.4;');
	theRest();
	var listenElement = toc;
	// var listenElement = toc.getElementsByTagName('TD')[0];
	listenElement.addEventListener('mouseover',function(){
		toc.setAttribute('style','background-color: #eeeeee; opacity: 1.0;');
		theRest();
	},false);
	listenElement.addEventListener('mouseout',function(){
		toc.setAttribute('style','background-color: #eeeeee; opacity: 0.2;');
		theRest();
		// toc.style.zIndex = '-10'; // If we do this, our mouseovers fail!
	},false);

	// CONSIDER TODO: If the TOC has a "Hide/Show" link ("button") then we should
	// fire this instead of changing opacity.

}



} // end doIt


// setTimeout(doIt,2000);
doIt();


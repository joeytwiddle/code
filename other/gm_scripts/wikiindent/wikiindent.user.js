// ==UserScript==
// @name           WikiIndent
// @namespace      joeytwiddle
// @include        *wiki*
// @description    Some tools for MediaWiki sites.  Indents sub-sections to make the layout clearer.  Hides the sidebar (toggle by clicking the background).  Floats the Table of Contents on the right of the page.
// ==/UserScript==

//// Features:
var hideSidebar = true;
var indentSubBlocks = true;
var fixUnderlinesToOverlines = true; // not yet working properly
var makeTableOfContentsFloat = true;


/* TODOS
As we scroll the page, light up the "current" section in the TOC.
*/


function doIt() {



if (hideSidebar) {
	// TODO: Make a toggle button for it!
	var column1 = document.getElementById("column-one");
	var content = document.getElementById("content");
	function toggleWikipediaSidebar(evt) {
		// We don't want to act on all clicked body elements (notably not the WP
		// image).  I detected two types of tag we wanted to click.
		if (!evt || evt.target.tagName == "UL" || evt.target.tagName == "DIV") {
			if (evt)
				evt.preventDefault();
			// GM_log("evt="+evt);
			// if (evt) GM_log("evt.target.tagName="+evt.target.tagName);
			if (column1.style.display == '') {
				column1.style.display = 'none';
				content.oldMarginLeft = content.style.marginLeft;
				content.style.marginLeft = 0;
				// column-one contains a lot of things, but one of them we want to preserve
				// (the row of tools across the top):
				var cac = document.getElementById("p-cactions");
				column1.parentNode.appendChild(cac);
			} else {
				column1.style.display = '';
				content.style.marginLeft = content.oldMarginLeft;
				column1.appendChild(cac); // almost back where it was :P
			}
		}
	}
	document.body.addEventListener('click',toggleWikipediaSidebar,false);
	toggleWikipediaSidebar();
}



if (indentSubBlocks) {

	function indent(tag) {
		// By targetting search we avoid indenting any blocks in left-hand-column (sidebar).
		var whereToSearch = document.getElementById('bodyContent') || document.getElementById('content') || document;
		var elems = whereToSearch.getElementsByTagName(tag);
		if (elems.length == 1)
			return;
		for (var i=0;i<elems.length;i++) {
			var elem = elems[i];
			/* Don't fiddle with main heading, siteSub, or TOC. */
			if (elem.className == 'firstHeading')
				continue;
			if (elem.id == 'siteSub')
				continue;
			if (elem.textContent == 'Contents')
				continue;
			// var newChild = document.createElement('blockquote');
			//// Unfortunately blockquotes tend to indent too much!
			// var newChild = document.createElement('DIV');
			var newChild = document.createElement('UL'); // UL works better with my Folding script, but we must not do this to the TOC!
			newChild.style.marginLeft = '1.0em';
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


// Feature #2: Change underlined headings to overlined headings.

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



if (makeTableOfContentsFloat) {

	// CONSIDER TODO: If the TOC has a "Hide/Show" link ("button") then we
	// should just fire this instead of changing opacity.

	// document.getElementById('column-one').appendChild(document.getElementById('toc'));

	var timer = null;

	// BUG: this didn't stop the two fades from conflicting when the user wiggles the mouse to start both!
	function resetTimeout(fn,ms) {
		if (timer) {
			clearTimeout(timer);
		}
		setTimeout(fn,ms);
	}

	function fadeElement(elem,start,stop,speed,current) {
		if (current == null)
			current = start;
		if (speed == null)
			speed = (stop - start) / 8;
		if (Math.abs(current+speed-stop) > Math.abs(current-stop))
			current = stop;
		else
			current = current + speed;
		elem.style.opacity = current;
		if (current != stop)
			resetTimeout(function(){fadeElement(elem,start,stop,speed,current);},50);
	}

	var toc = document.getElementById('toc');
	if (toc) {
		// toc.style.backgroundColor = '#eeeeee';
		toc.style.position = 'fixed';
		toc.style.right = '16px';
		toc.style.top = '16px';
		toc.style.zIndex = '5000';
		// fadeElement(toc,1.0,0.4);
		toc.style.opacity = 0.3;
		var listenElement = toc;
		// var listenElement = toc.getElementsByTagName('TD')[0];
		var focused = false;
		var visible = false;
		listenElement.addEventListener('mouseover',function(){
			if (!visible)
				setTimeout(function(){ if (focused) { fadeElement(toc,0.4,1.0,0.2); visible=true; } },10);
			focused = true;
		},false);
		listenElement.addEventListener('mouseout',function(){
			if (visible)
				setTimeout(function(){ if (!focused) { fadeElement(toc,1.0,0.2,-0.1); visible=false; } },10);
			focused = false;
		},false);
	}

}



} // end doIt


// setTimeout(doIt,2000);
doIt();


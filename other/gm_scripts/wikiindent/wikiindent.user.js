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





if (makeTableOfContentsFloat) {

	// CONSIDER TODO: If the TOC has a "Hide/Show" link ("button") then we
	// should just fire this instead of changing opacity.

	// document.getElementById('column-one').appendChild(document.getElementById('toc'));

	function fadeElement(elem,start,stop,speed,current) {
		if (current == null)
			current = start;
		if (speed == null)
			speed = (stop - start) / 5;
		if (Math.abs(current+speed-stop) > Math.abs(current-stop))
			current = stop;
		else
			current = current + speed;
		elem.style.opacity = current;
		// if (Math.random()<0.2)
			// GM_log("current="+current);
		if (current != stop)
			setTimeout(function(){fadeElement(elem,start,stop,speed,current);},100);
		else
			GM_log("current="+current);
	}

	var toc = document.getElementById('toc');
	if (toc) {
		// toc.style.backgroundColor = '#eeeeee';
		toc.style.position = 'fixed';
		toc.style.right = '16px';
		toc.style.top = '16px';
		toc.style.zIndex = '5000';
		fadeElement(toc,1.0,0.4);
		var listenElement = toc;
		// var listenElement = toc.getElementsByTagName('TD')[0];
		var focused = false;
		var visible = false;
		listenElement.addEventListener('mouseover',function(){
			if (!visible)
				setTimeout(function(){ if (focused) { fadeElement(toc,0.4,1.0,0.4); visible=true; } },10);
			focused = true;
		},false);
		listenElement.addEventListener('mouseout',function(){
			if (visible)
				setTimeout(function(){ if (!focused) { fadeElement(toc,1.0,0.2); visible=false; } },10);
			focused = false;
		},false);
	}

}



} // end doIt


// setTimeout(doIt,2000);
doIt();


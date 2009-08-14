// ==UserScript==
// @name           Userscripts.org Userscript
// @namespace      UOU
// @author         joeytwiddle
// @include        http://userscripts.org/*
// @include        http://*.userscripts.org/*
// ==/UserScript==

// I don't play Travian or use Orkut.  Get the hell away from me!

// Sometimes the text touches the edge of the browser, which looks naff.
document.body.style.padding = '6px';

// Userscripts.org search engine sucks.  Let's use Google.
function doSearch(evt){
	evt.preventDefault();
	var value = document.getElementById("script_q").value;
	var defaultValue = document.getElementById("script_q").defaultValue;
	if (value) {
		var target = "http://www.google.com/search?q="+encodeURIComponent(value+" site:userscripts.org/scripts/show");
		document.location = target;
	}
	return false;
}
var searchForm = document.getElementById("script_search");
var searchButton = document.getElementById("search-go");
searchForm.addEventListener("submit",doSearch,true);
searchButton.addEventListener("click",doSearch,true);

// Use color strength in the tag cloud:
var tagCloud = unsafeWindow.document.getElementById("tag-cloud");
if (tagCloud) {
	var puffs = tagCloud.getElementsByTagName("LI");
	GM_log("puffs.length = "+puffs.length);
	for (var i=0; i<puffs.length; i++) {
		var puff = puffs[i];
		if (!puff.style)
			continue;
		var fontSize = parseFloat(puff.style.fontSize,10);
		var strength = 1.7*(fontSize-0.7);
		if (strength>1.0) strength=1.0;
		if (strength<0.0) strength=0.0;
		var weakness = 1.0 - strength;
		// if (Math.random() < 0.03)
			// GM_log("puff="+puff+" firstChild="+puff.firstChild);
		var anchor = puff.getElementsByTagName("A")[0];
		if (anchor)
			puff = anchor;
		else
			continue;
		var base = 10;
		var red = parseInt( base*strength + 255*weakness );
		var green = parseInt( 100*strength + 255*weakness );
		var blue = parseInt( 190*strength + 255*weakness );
		var antisaturation = 255 - strength;
		puff.style.color = "rgb("+red+","+green+","+blue+")";
		// if (Math.random() < 0.03)
			// GM_log(""+puff+".style.color = "+puff.style.color);
	}
}
// It doesn't work so well on the front-page, since there are only two sizes of
// font atm, and they are specified in 'em' unlike the main tag cloud page
// which uses 'px'.


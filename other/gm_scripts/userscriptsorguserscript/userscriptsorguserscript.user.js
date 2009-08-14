// ==UserScript==
// @name           Userscripts.orgUserscript
// @namespace      UOU
// @include        http://userscripts.org/*
// @include        http://*.userscripts.org/*
// ==/UserScript==

document.body.style.padding = '6px';

var newSearch = document.createElement('DIV');

/*
newSearch.innerHTML = '<form id="script_search" method="get" action="http://www.google.com/search">' +
	'<input id="script_q" type="text" value="Search with Google" name="q"/>' +
	'' +
	'' +
	'' +
	'';

var submitButton = document.createElement("BUTTON");
submitButton.textContent = ">>";
submitButton.addEventListener("click",doSearch,false);
*/

function doSearch(evt){
	evt.preventDefault();
	var script_q = document.getElementById("script_q").value;
	if (script_q) {
		var target = "http://www.google.com/search?q="+encodeURIComponent(script_q+" site:userscripts.org/scripts/show");
		document.location = target;
	}
	return false;
}

var oldSearch = document.getElementById("script_search");

// oldSearch.parentNode.replaceChild(oldSearch,newSearch);

oldSearch.addEventListener("click",doSearch,true);
oldSearch.addEventListener("submit",doSearch,true);
// oldSearch.addEventListener("blur",doSearch,true);
document.getElementById("script_q").value = "Search with Google";


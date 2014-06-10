// ==UserScript==
// @name           Joeys Pikto Tweaks
// @namespace      JPT
// @description    Just some style improvements I like when using Piktochart.com
// @include        http://localhost:3000/*
// @include        https://magic.piktochart.com/*
// ==/UserScript==

if (typeof GM_addStyle == 'undefined') {
  this.GM_addStyle = function(css) {
    var s = document.createElement("style");
    s.type = 'text/css';
    s.innerHTML = css;
    document.getElementsByTagName("head")[0].appendChild(s);
  };
}

if ( document.location.pathname.match("/$|/themes|/infographics") ) {
	// Tab-style top-bar for the front pages
	var bgcol = document.location.pathname.match("/$|/themes") ? '#e3e3e3' : 'white';
	GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-top: 15px; padding-bottom: 15px; margin-bottom: -2px; border-radius: 10px 10px 0px 0px; -moz-border-radius: 10px 10px 0px 0px; -webkit-border-radius: 10px 10px 0px 0px; -ms-border-radius: 10px 10px 0px 0px; -o-border-radius: 10px 10px 0px 0px; background-color: "+bgcol+"; color: black; } ");
	// Darker modal
    GM_addStyle(" .modal-backdrop { background: #000000; } ");
}

// Centralised message boxes (instead of filling the whole width of the page)
var messageBoxes = document.getElementsByClassName("msg-container");
for (var i = messageBoxes.length; i--;) {
	var messageBox = messageBoxes[i];
	var container = document.createElement("div");
	//container.style.width = "100%";   // default anyway
	container.style.textAlign = "center";
	messageBox.style.width = 'auto';    // wasn't working
	messageBox.style.width = '30%';
	messageBox.parentNode.replaceChild(container,messageBox);
	container.appendChild(messageBox);
}

// Links to "Saved Piktocharts"
$("a").each(function(){
	if ($(this).text() == "Saved Piktocharts") {
		$(this).text("My Infographics");
	}
});

// My Infographics page ( /infographics )
$(".pikto-publish-hover-link *").filter(function(){ return $(this).text()=="open on web"; }).text("View");
$("#pikto-hover-unpublish a").filter(function(){ return $(this).text()=="Unpublish It"; }).text("Unpublish");


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

// On the front pages
if ( document.location.pathname.match("/$|/themes|/infographics|/gallery") ) {
	// Active tab is white (or light grey) and rounded
	//var bgcol = document.location.pathname.match("/$|/themes") ? '#e3e3e3' : 'white';
	//GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-top: 15px; padding-bottom: 15px; margin-bottom: -2px; border-radius: 10px 10px 0px 0px; -moz-border-radius: 10px 10px 0px 0px; -webkit-border-radius: 10px 10px 0px 0px; -ms-border-radius: 10px 10px 0px 0px; -o-border-radius: 10px 10px 0px 0px; background-color: "+bgcol+"; color: black; } ");
	// Active tab is a bit taller, and bevelled, with slightly wider borders
	GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-top: 0.5em; border: 1px solid; border-color: rgba(0,0,0,0.4) rgba(255,255,255,0.1) rgba(255,255,255,0.1) rgba(0,0,0,0.4); } ");
	GM_addStyle(" .top-menu-container ul.menu > li a.active { -webkit-border-radius: 4px; } ");
	// Grey rounded tab
	//GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-bottom: 1.0em; margin-bottom: -0.5em; -webkit-border-radius: 10px 10px 0px 0px; } ");
	// 1px inset #485456
	// Darker modal (like in the editor)
	GM_addStyle(" .modal-backdrop { background: #000000; } ");
	// My Infographics page ( /infographics )
	$(".pikto-publish-hover-link *").filter(function(){ return $(this).text()=="open on web"; }).text("View");
	$("#pikto-hover-unpublish a").filter(function(){ return $(this).text()=="Unpublish It"; }).text("Unpublish");
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

// Links to "Saved Piktocharts" (topbar on main site, menu in editor).
$("a").each(function(){
	if ($(this).text() == "Saved Piktocharts" || $(this).text() == "My Saved Piktocharts") {
		$(this).text("My Infographics");
	}
});



// ==== In editor ====

if ( document.location.pathname.match("/editor/|/editorv4/") ) {

	// Dropdowns are too damn opaque!
	GM_addStyle(".pikto-mainmenu .dropdown-menu { background: rgba(49,59,61,1.0); } .pikto-publish-dongle-popout { background-color: rgba(255,255,255,1.0); }");

	// Some menu items are missing their '...'
	$("#menu-rename").text("Rename...");
	$("#menu-save-as").text("Save As...");

}


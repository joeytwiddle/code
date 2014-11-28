// ==UserScript==
// @name           Joeys Pikto Tweaks
// @namespace      JPT
// @description    Just some style improvements I like when using Piktochart.com
// @include        http://localhost:3000/*
// @include        https://magic.piktochart.com/*
// @include        https://*.piktochart.info/*
// @version        1.1.11
// @grant          none
// ==/UserScript==

if (typeof GM_addStyle == 'undefined') {
  this.GM_addStyle = function(css) {
    var s = document.createElement("style");
    s.type = 'text/css';
    s.innerHTML = css;
    document.getElementsByTagName("head")[0].appendChild(s);
  };
}



// ==== On the front pages ====

if ( document.location.pathname.match("/$|/themes|/templates|/infographics|/gallery") ) {
	// Active tab is white (or light grey) and rounded
	//var bgcol = document.location.pathname.match("/$|/themes") ? '#e3e3e3' : 'white';
	//GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-top: 15px; padding-bottom: 15px; margin-bottom: -2px; border-radius: 10px 10px 0px 0px; -moz-border-radius: 10px 10px 0px 0px; -webkit-border-radius: 10px 10px 0px 0px; -ms-border-radius: 10px 10px 0px 0px; -o-border-radius: 10px 10px 0px 0px; background-color: "+bgcol+"; color: black; } ");
	// Active tab is a bit taller, and bevelled, with slightly wider borders
	//GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-top: 0.5em; border: 1px solid; border-color: rgba(0,0,0,0.4) rgba(255,255,255,0.1) rgba(255,255,255,0.1) rgba(0,0,0,0.4); -webkit-border-radius: 4px; } ");
	//GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-top: 0.6em; padding-bottom: 0.6em; border: 1px solid; border-color: rgba(0,0,0,0.4) rgba(255,255,255,0.1) rgba(255,255,255,0.1) rgba(0,0,0,0.4); -webkit-border-radius: 4px; } ");
	//GM_addStyle(" .top-menu-container ul.menu > li a.active { padding: 0.7em; border: 1px solid; border-color: rgba(0,0,0,0.4) rgba(255,255,255,0.1) rgba(255,255,255,0.1) rgba(0,0,0,0.4); -webkit-border-radius: 4px; } ");
	// Now it is a solid block, not a small box, I am happy with the default.
	// Grey rounded tab
	//GM_addStyle(" .top-menu-container ul.menu > li a.active { padding-bottom: 1.0em; margin-bottom: -0.5em; -webkit-border-radius: 10px 10px 0px 0px; } ");
	// 1px inset #485456
	// Darker modal (like in the editor)
	GM_addStyle(" .modal-backdrop { background: #000000; } ");
	// My Infographics page ( /infographics )
	$(".pikto-publish-hover-link *").filter(function(){ return $(this).text()=="open on web"; }).text("View");
	$("#pikto-hover-unpublish a").filter(function(){ return $(this).text()=="Unpublish It"; }).text("Unpublish");
}



// === On the theme selector page ===

// Centralise the theme search box (don't let it fill the whole width).
//GM_addStyle(" .themes-container #themes-wrapper .filter-container { /*background: #f1f4f5;*/ } .themes-container #themes-wrapper .search-container { text-align: center; } .themes-container #themes-wrapper .search-container input[type='text'] { width: 600px; } .themes-container #themes-wrapper .search-icon { position: initial; transform: translate(-35px, 0px); } ");
// Now fixed on production!  (y)


// ==== Everywhere ====

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

// Use the noun "infographic" instead of "piktochart"
// Repeating the word "piktochart" may help to make the brand name stick, but unfortunately it also indicates that a 'piktochart' is not an infographic.
// By using the word "infographic", we hint this is *the* site to create infographics.
// Links to "Saved/Featured Piktocharts" (topbar on main site, menu in editor).
$("a").each(function(){
	var currentText = $(this).text();
	if (currentText == "Saved Piktocharts") {
		$(this).text("My Infographics");
	}
	else if (currentText == "My Piktocharts") {
		$(this).text("My Infographics");
	}
	else if (currentText == "Featured Piktocharts") {
		$(this).text("Featured Infographics");
	}
	else if (currentText.indexOf("Create a new Piktochart") >= 0) {
		$(this).html( $(this).html().replace(/Piktochart/g,"Infographic") );
	}
});



// ==== In editor ====

if ( document.location.pathname.match("/editor/|/editorv4/") ) {

	// The dropdowns in the top-right are too damn opaque.
	GM_addStyle(".pikto-mainmenu .dropdown-menu { background: rgba(49,59,61,1.0); } .pikto-publish-dongle-popout { background-color: rgba(255,255,255,1.0); }");

	// Some menu items are missing their '...'s
	$("#menu-rename").text("Rename...");
	$("#menu-save-as").text("Save As...");

	// The 'Saving...' dialog should have a background
	GM_addStyle("#modal-generic .modal-dialog .modal-content { background: whitesmoke; padding: 20px 10px; } "
	          + "#modal-generic .modal-dialog .modal-content .modal-title h4 { color: black; } "
	          + "#modal-generic .modal-dialog .modal-content #general-modal-message { color: black; }"
	);

	// The File menu should have a Save option!
	var $saveAsItem = $('#menu-save-as').parent('li');
	$('<li><a href="#" id="menu-save" class="ttips" data-placement="right" title="Save this infographic">Save</a></li>').insertBefore($saveAsItem);

}



// ==== Social network ====

// Make the line touch the tabs above it, also make it a bit thicker and teal.
GM_addStyle("#v-nav hr { marginTop: -1px; height: 3px; background-color: #23b0a3; }");




// ==== On Rahsia ====

if (document.location.hostname.indexOf('rahsia') === 0) {
	// Make headers (which separate items) stand out from other text:
	GM_addStyle('.journal > div > h4 , #sidebar > h3 , .subject > div > h3, #activity > h3 { background-color: #aaddff; border-bottom: 3px solid rgba(0,0,0,0.2); }');
	// I almost want to do this to all headers.  But I didn't really want it to apply to any `h3` put *inside* a comment by a user.
}

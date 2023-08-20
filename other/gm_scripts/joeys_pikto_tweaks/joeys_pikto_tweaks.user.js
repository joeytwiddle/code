// ==UserScript==
// @name           Joeys Pikto Tweaks
// @namespace      JPT
// @description    Just some style improvements I like when using Piktochart.com
// // @include        http://localhost:3000/*
// @include        https://magic.piktochart.com/*
// @include        https://*.piktochart.info/*
// @include        https://create.piktochart.com/*
// @version        1.1.19
// @grant          none
// ==/UserScript==



// ==== Options ====

const centralizeSlideAnimations = true;



// ==== Setup ====

function $() {
    console.warn(`[PiktoTweaks] Old jQUery-based tweaks are no longer supported.`);
    return new Proxy({}, {
        get: function(target, property) {
            return function() {
                console.warn(`[PiktoTweaks] Old jQUery-based tweaks are no longer supported. prop='${property}'`);
            };
        },
    });
}



// ==== Convenience functions ====

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
	var viewInfographicLink = document.querySelectorAll(".pikto-publish-hover-link a");
	if (viewInfographicLink.length > 0) {
		var viewInfographicLinkText = viewInfographicLink[0].lastChild;
		if (viewInfographicLinkText.textContent.trim().toLowerCase() === "open on web") {
			viewInfographicLinkText.textContent = "View";
		}
	}
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
// E.g. in the links to "Saved/Featured Piktocharts" (topbar on main site, menu in editor).
// Repeating the word "piktochart" may help to make the brand name stick, but I fear it suggests that a "piktochart" is not an infographic.
// By using the word "infographic", we indicate that this is a site where you can create infographics (rather than a site where you can only create "piktocharts").
$("a").each(function(){
	var currentText = $(this).text();
	var toReplace = [
	    "Saved Piktocharts"
	,   "My Piktocharts"
	,   "My Saved Piktocharts"
	,   "Featured Piktocharts"
	,   "Create a new Piktochart"
	];
	if (toReplace.indexOf(currentText) >= 0) {
		$(this).html( $(this).html().replace(/Piktochart/g,"Infographic") );
	}
});



// ==== In editor ====

if ( document.location.pathname.match("/editor/|/editorv4/") ) {

	// The dropdowns in the top-right are too damn opaque.
	GM_addStyle(".pikto-mainmenu .dropdown-menu { background: rgba(49,59,61,1.0); } .pikto-publish-dongle-popout { background-color: rgba(255,255,255,1.0); }");

	// Some menu items are missing their '...'s
	$("#menu-rename").text("Rename...");
	//$("#menu-save-as").text("Save As...");
	// But anyway I want to re-word this one
	$("#menu-save-as").text("Create a Copy...");

	// The 'Saving...' dialog should have a background
	GM_addStyle("#modal-generic .modal-dialog .modal-content { background: whitesmoke; padding: 20px 10px; } "
	          + "#modal-generic .modal-dialog .modal-content .modal-title h4 { color: black; } "
	          + "#modal-generic .modal-dialog .modal-content #general-modal-message { color: black; }"
	);

	// The File menu should have a Save option!
	var $saveAsItem = $('#menu-save-as').parent('li');
	$('<li><a href="#" id="menu-save" class="ttips" data-placement="right" title="Save this infographic">Save</a></li>').insertBefore($saveAsItem);

	// Make the layout and naming of tabs in the charts editor more helpful
	var $settingsTab = $('#modal-charts .tab-advsettings');
	var $googleDriveTab = $settingsTab.prev();
	$settingsTab.find('a')[0].lastChild.textContent = "Appearance";
	$googleDriveTab.find('a')[0].lastChild.textContent = "Import";   // "Google Drive Import";
	$settingsTab.removeClass('tab-advsettings');
	$settingsTab.insertBefore($googleDriveTab);

	// Remove transparency from popups
	if ( $('.pikto-pbar-dongle-popout').css('background-color').match(/^rgba\(255, 255, 255, 0.9/) ) {   // Chrome yields '0.901961', Firefox '0.9'
		$('.pikto-pbar-dongle-popout').css('background-color', 'rgba(255, 255, 255, 1.0)');
	}
	if ( $('.pikto-pbar-color-picker-wrapper').css('background-color').match(/^rgba\(255, 255, 255, 0.9/) ) {
		$('.pikto-pbar-color-picker-wrapper').css('background-color', 'rgba(255, 255, 255, 1.0)');
	}

	// Advertise the transpose button
	GM_addStyle(".handsontable thead tr:first-child th:first-child { background-color: #23b0a3; }");

}



// ==== Social network ====

// Make the line touch the tabs above it, also make it a bit thicker and teal.
GM_addStyle("#v-nav hr { marginTop: -1px; height: 3px; background-color: #23b0a3; }");



// ==== On Rahsia ====

if (document.location.hostname.indexOf('rahsia') === 0) {
	// Make headers (which separate items) stand out from other text:
	GM_addStyle('.journal > div > h4 , #sidebar > h3 , .subject > div > h3, #activity > h3 { background-color: #aaddff; border-bottom: 3px solid rgba(0,0,0,0.2); }');
	// I almost want to do this to all headers.  But I didn't really want it to apply to any `h3` put *inside* a comment by a user.

	// This button claims to 'Edit' but it is also the only way to reply or comment on an issue.
	$('#content > .contextual > a.icon-edit').text('Comment/Update');
}



// ==== Slideshow ====

if (centralizeSlideAnimations) {
	// There is a 600px which keeps being set on the width and height of the .slides element.
	// This cause the animation when changing slides to rotate around an off-center origin.
	// Fortunately, we can override those two numbers with this:
	GM_addStyle(`.reveal .slides {
		width: 100% !important;
		height: 100% !important;
	`);
}

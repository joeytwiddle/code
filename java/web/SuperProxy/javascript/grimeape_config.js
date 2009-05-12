function(){

	var iconStyleString = " position: fixed; right: 4px; bottom: 4px; z-index: 1000 ";
	var iconWidth = 28; var iconHeight = 28;
	var iconURL = "/_gRiMeApE_/images/punkape.png"; // blueape.png";

	/*
	var icon = document.createElement('IMG');
	icon.src = iconURL;
	icon.width = iconWidth;
	icon.height = iconHeight;
	// javascript needs a "send to back" ;)
	document.body.appendChild(icon);
	icon.style = iconStyleString;
	*/
	document.writeln('<STYLE type="text/css">.gaicon{ '+iconStyleString+' }</STYLE>');
	document.writeln('<IMG id="gaIcon" src="'+iconURL+'" width="'+iconWidth+'" height="'+iconHeight+'" class="gaicon">');

	// Add a gap at the bottom of the page, for our 'icon-bar'.
	document.body.style.paddingBottom = iconHeight+8 + document.body.style.paddingBottom + "px";

	var icon = document.getElementByID('gaIcon');

	function showHideMenu() {
		var mmmm;
		// Menu items:
		// Show Log
		// Enable/Disable Script 1
		// Enable/Disable Script 2
		// ...
	}

	icon.onclick = showHideMenu();

}();

function(){

	// var grimeApeConfig = GM_getValue('grimeApeConfig');

	// GM_log("Got grimeApeConfig = "+grimeApeConfig);

	// if (!grimeApeConfig) {

		/*
		grimeApeConfig = {
			enabled: true,
		};
		*/

	// }

	var GrimeApeConfig = {
		enabled: true,
	};

	// var iconStyleString = " position: fixed; right: 4px; bottom: 4px; z-index: 1000 ";
	var iconWidth = 28; var iconHeight = 29;
	var iconURL = "/_gRiMeApE_/images/punkape.png"; // blueape.png";
	var iconURLDisabled = "/_gRiMeApE_/images/punkape_disabled.png";

	var icon = document.createElement('IMG');
	icon.id = 'gaIcon';
	icon.src = iconURL;
	icon.width = iconWidth;
	icon.height = iconHeight;
	// javascript needs a "send to back" ;)
	// icon.style = iconStyleString;
	// var iconHolder = document.createElement('A');
	with (icon.style) {
		position='fixed';
		right='4px';
		bottom='4px';
		zIndex='1000';
	}
	// iconHolder.appendChild(icon);
	// document.body.appendChild(iconHolder);
	document.body.appendChild(icon);
	/*
	document.writeln('<STYLE type="text/css">.gaicon{ '+iconStyleString+' }</STYLE>');
	document.writeln('<IMG id="gaIcon" src="'+iconURL+'" width="'+iconWidth+'" height="'+iconHeight+'" class="gaicon">');
	*/

	// Add a gap at the bottom of the page, for our 'icon-bar'.
	document.body.style.paddingBottom = iconHeight+8 + document.body.style.paddingBottom + "px";

	var Menu = {

		menuElement: document.createElement('DIV'),

		toggleEnabled: function() {
			GrimeApeConfig.enabled = !GrimeApeConfig.enabled;
			icon.src = ( GrimeApeConfig.enabled ? iconURL : iconURLDisabled );
			Menu.showHideMenu();
		},

		showHideLog: function() {
			alert(window.localLog);
			Menu.showHideMenu();
		},

		showHideMenu: function() {
			// var mmmm;
			// Menu items:
			// Show Log
			// Enable/Disable Script 1
			// Enable/Disable Script 2
			// ...
			/*
			if (grimeApeConfig.enabled) {
				grimeApeConfig.enabled = false;
			}
				GM_setValue('grimeApeConfig'
				yawn
				*/
			Menu.menuElement.style.display = ( Menu.menuElement.style.display ? '' : 'none' );
		},

		addMenuItem: function(txt,fn) {
			var menuItem = document.createElement('A');
			menuItem.textContent = txt;
			/*
			// bad can cause page refresh or temporary hangup in konqueror
			// menuItem.href = '#';
			// menuItem.target = '_self';
			*/
			menuItem.href = 'javascript:void(0)'; // yum the "nice" way
			menuItem.onclick = fn;
			var line = document.createElement('P');
			line.appendChild(menuItem);
			Menu.menuElement/*thisisstupid*/.appendChild(line);

			// A shorter way to create a link:
			// var menuItem = createElement('A', {href:"javascript:void(0)", style:"text-decoration:none; font-size:x-small"}, 'click '+fnName+' false', ' (options)');
		},

		init: function() {
			// fail :f
			//addMenuItem("Enable/Disable",toggleEnabled);
			//addMenuItem("Show Log",showHideLog);
			Menu.addMenuItem("Show Log",Menu.showHideLog);
			Menu.addMenuItem("Enable/Disable",Menu.toggleEnabled);
		},

	};

	Menu.init();
	document.body.appendChild(Menu.menuElement);
	with (Menu.menuElement.style) {
		display = 'none';
		position = 'fixed';
		right = '4px';
		bottom = (iconHeight+4)+'px';
		zIndex = 10000;
		border = '1px solid black';
	}

	// var icon = document.getElementById('gaIcon');
	// icon.addEventListener('onclick',Menu.showHideMenu());
	// iconHolder.href = '#'; iconHolder.target = '_self';
	icon.onclick = Menu.showHideMenu;
	// GM_log("icon.parentNode = "+icon.parentNode);

}();

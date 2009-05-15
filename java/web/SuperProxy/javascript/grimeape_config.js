
/*

CONSIDER: We could have the proxy inject the monkey at the start of the page,
so that the user can see him early, and config him while the page loads.
The loading of userscripts should still be delayed till the end of loading though.
(Either injected by proxy, or detected by onLoad or something.)

*/

(function(){



	//// Config ////

	var GrimeApeConfig = new Object();

	// Try loading config:
	try {
		GrimeApeConfig = eval(GM_getValue("GrimeApeConfig"));
		GM_log("Loaded config: "+uneval(GrimeApeConfig));
	} catch (e) {
		GM_log(e);
	}

	// If not, set default:
	if (!GrimeApeConfig || !GrimeApeConfig.scripts) {

		GrimeApeConfig = {
			enabled: true,
			// scripts: new Object(),
			// scripts: ({ "faviconizegoogle", "reclaim_cpu" }),
			scripts: ({
				"FaviconizeGoogle" : ({ enabled: true }) ,
				"Reclaim CPU" : ({ enabled: true }),
				"auto_highlight_text_on_a" : ({ enabled: true }),
				"Delicious Search Results" : ({ enabled: false }), // Requires GM_xmlhttpRequest.
				"SF ProjectHomepage" : ({ enabled: false }), // needs @include we don't want it every page!
				"Googles Old Favicon" : ({ enabled: false }),
				"Track History" : ({ enabled: false }),
				"Cleanup Fonts" : ({ enabled: false }),
			}),
		};

	}

	// This may look like a long-winded way of declaring the object, but:
	// declaring the functions this way gives them access to each other,
	// and we really should be overwriting save() because the function may have
	// been loaded from the uneval, but the versions here might be newer.

	GrimeApeConfig.save = function() {
		GM_setValue("GrimeApeConfig",uneval(GrimeApeConfig));
	};



	//// Icon ////

	var ApeIcon = {

		// var iconStyleString = " position: fixed; right: 4px; bottom: 4px; z-index: 1000 ";
		iconWidth  : 42,
		iconHeight : 43,
		iconURL         : "/_gRiMeApE_/images/punkape.png", // blueape.png",
		iconURLDisabled : "/_gRiMeApE_/images/punkape_disabled.png",

		initIcon : function() {

			var icon = document.createElement('IMG');
			icon.id = 'gaIcon';
			ApeIcon.icon = icon;
			ApeIcon.setIcon();
			icon.width = ApeIcon.iconWidth;
			icon.height = ApeIcon.iconHeight;
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
			document.body.style.paddingBottom = ApeIcon.iconHeight+8 + document.body.style.paddingBottom + "px";

		},

		setIcon : function() {
			ApeIcon.icon.src = ( GrimeApeConfig.enabled ? ApeIcon.iconURL : ApeIcon.iconURLDisabled );
		}

	};

	ApeIcon.initIcon();



	//// Menu ////

	var Menu = {

		menuElement: document.createElement('DIV'),

		showHideMenu: function() {
			Menu.menuElement.style.display = ( Menu.menuElement.style.display ? '' : 'none' );
		},

		toggleEnabled: function() {
			GrimeApeConfig.enabled = !GrimeApeConfig.enabled;
			Menu.showHideMenu();
			GrimeApeConfig.save();
			ApeIcon.setIcon();
		},

		showHideLog: function() {
			Menu.showHideMenu();
			alert(window.localLog);
		},

		clearAllData: function() {
			var url = "/_gRiMeApE_/clearAll";
			var client = new XMLHttpRequest();
			client.open('GET',url,false);
			client.send(null);
			Menu.showHideMenu();
		},

		showInstallWindow: function() {
			if (!Menu.installWindow) {
				Menu.installWindow = document.createElement('DIV');
				Menu.installWindow.innerHTML = '<FORM method="GET" action="/_gRiMeApE_/installUserscript">'
					+ ''
					+ ''
					+ '</FORM>\n';
				// TODO ...
				
				

			}
			Menu.menuElement.style.display = ( Menu.menuElement.style.display ? '' : 'none' );
		},

		addMenuItem: function(txt,fn) {
			var menuItem = document.createElement('A');
			menuItem.textContent = txt;
			if (fn) {
				menuItem.href = 'javascript:void(0)'; // yum the "nice" way
				menuItem.onclick = fn;
			}
			//var line = document.createElement('P');
			//line.appendChild(menuItem);
			//Menu.menuElement/*thisisstupid*/.appendChild(line);
			if (Menu.menuElement.childNodes.length > 0)
				// if (Menu.menuElement.lastChild.tagName!='HR')
					Menu.menuElement.appendChild(document.createElement('BR'));
			Menu.menuElement.appendChild(menuItem);

			// A shorter way to create a link:
			// var menuItem = createElement('A', {href:"javascript:void(0)", style:"text-decoration:none; font-size:x-small"}, 'click '+fnName+' false', ' (options)');
			return menuItem;
		},

		init: function() {
			Menu.addMenuItem("Install New Script",Menu.showInstallWindow);
			Menu.addMenuItem("Clear All Data",Menu.clearAllData);
			// Menu.menuElement.appendChild(document.createElement('HR')); // Too wide in Konq!
			Menu.menuElement.appendChild(document.createElement('BR'));
			Menu.menuElement.appendChild(document.createTextNode('-----------'));

			for (var script in GrimeApeConfig.scripts) {
				(function(){ // To keep scriptData and checkbox local.

					var scriptData = GrimeApeConfig.scripts[script];

					var checkbox = document.createElement('input');
					checkbox.type = 'checkbox';
					checkbox.checked = scriptData.enabled;

					var toggleScript = (function(evt) {
							// scriptData.enabled = checkbox.checked;
							scriptData.enabled = !scriptData.enabled;
							GrimeApeConfig.save();
							checkbox.checked = scriptData.enabled;
					});

					checkbox.onclick = toggleScript;
					var menuItem = Menu.addMenuItem(script,toggleScript);
					Menu.menuElement.insertBefore(checkbox,menuItem);

				})();
			}

			// Menu.menuElement.appendChild(document.createElement('HR'));
			Menu.menuElement.appendChild(document.createElement('BR'));
			Menu.menuElement.appendChild(document.createTextNode('-----------'));
			Menu.addMenuItem("Show Log",Menu.showHideLog);
			Menu.addMenuItem("Enable/Disable",Menu.toggleEnabled);
		},

		initStyle: function() {
			with (Menu.menuElement.style) {
				display = 'none';
				position = 'fixed';
				right = '4px';
				bottom = (ApeIcon.iconHeight+4)+'px';
				zIndex = 10000;
				border = '1px solid black';
				backgroundColor = '#ffffff';
				color = 'black';
				padding = '4px';
				textAlign = 'left';
			}
			// var css = " #menuEnabled.link{ color:#00ff00; } #menuDisabled.link{ color:#880000; } ";
			// document.writeln("<SCRIPT type='text/css'>"+css+"</SCRIPT>");
		}

	};

	Menu.init();
	document.body.appendChild(Menu.menuElement);
	Menu.initStyle();

	// var icon = document.getElementById('gaIcon');
	// icon.addEventListener('onclick',Menu.showHideMenu());
	// iconHolder.href = '#'; iconHolder.target = '_self';
	ApeIcon.icon.onclick = Menu.showHideMenu;
	// GM_log("icon.parentNode = "+icon.parentNode);



	//// Start ////

	function doStart() {
		var countLoaded = 0;
		if (GrimeApeConfig.enabled) {
			for (var script in GrimeApeConfig.scripts) {
				// GM_log("script = "+script);
				// GM_log("scripts[script] = " + GrimeApeConfig.scripts[script]);
				var fsName = script.toLowerCase().replace(/ /g,'_');
				var scriptData = GrimeApeConfig.scripts[script];
				if (GrimeApeConfig.scripts[script].enabled) {
					GM_log('Loading: '+script);
					// if (script.startsWith('/')) {
						var url = "/_gRiMeApE_/userscripts/" + fsName + '/' + fsName + ".user.js"
						document.writeln("<SCRIPT type='text/javascript' src='" + url + "'></SCRIPT>");
					// } else {
						// document.writeln("<SCRIPT type='text/javascript' src='" + scriptData.url + "'
								// + fsName + '/' + fsName + ".user.js'></SCRIPT>");
					// }
					countLoaded++;
				}
			}
		}
		GM_log("GrimeApe loaded "+countLoaded+" scripts.");
	}

	doStart();



})();


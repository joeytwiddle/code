
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
				"auto_highlight_text_on_a" : ({ enabled: true }),
				"Delicious Search Results On Google" : ({ enabled: false }), // Requires GM_xmlhttpRequest.
				"Twitter Search Results On Google" : ({ enabled: false }), // Requires GM_xmlhttpRequest.
				"SF ProjectHomepage" : ({ enabled: false }), // needs @include we don't want it every page!
				"Googles Old Favicon" : ({ enabled: false }),
				"Track History" : ({ enabled: false }),
				"Cleanup Fonts" : ({ enabled: false }),
				"Reclaim CPU" : ({ enabled: false }),
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

	function loadScript(scriptName) {
		var fsName = scriptName.toLowerCase().replace(/ /g,'_').substring(0,24);
		GM_log('Loading: '+scriptName);
		var url = "/_gRiMeApE_/userscripts/" + fsName + '/' + fsName + ".user.js"
		document.writeln("<SCRIPT type='text/javascript' src='" + url + "'></SCRIPT>");
	}

	function loadScriptOtherWay(scriptName) {
		var fsName = scriptName.toLowerCase().replace(/ /g,'_').substring(0,24);
		GM_log('Loading: '+scriptName);
		var url = "/_gRiMeApE_/userscripts/" + fsName + '/' + fsName + ".user.js"
		var scriptElement = document.createElement("SCRIPT");
		scriptElement.type = 'text/javascript';
		scriptElement.src = url;
		// document.body.appendChild(scriptElement); // Sometimes page change in Konq? yes
		document.getElementsByTagName("HEAD")[0].appendChild(scriptElement);
	}



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



	var ScriptEditor = {

		openNewEditor: function(scriptName) {
			var formID = "scriptEditorForm"+document.forms.length;
			var newEditor = document.createElement('DIV');
			newEditor.innerHTML = ''
				// + '<STYLE type="text/css"> .td{ text-align:right; } </STYLE>\n'
				+ '<FONT size="-1">\n'
				+ '<FORM id="'+formID+'" method="GET" action="/_gRiMeApE_/saveUserscript">\n'
				+ '<TABLE>'
				+ '<TR><TD align="right">Name:</TD><TD><INPUT type="text" name="name" value="Script Name"/></TD></TR>\n'
				+ '<TR><TD align="right">Namespace:</TD><TD><INPUT type="text" name="namespace" value="Nobody knows what this is."/></TD></TR>\n'
				+ '<TR><TD align="right">Description:</TD><TD><INPUT type="text" name="description" value="Description of the script"/></TD></TR>\n'
				// + '<TR><TD align="right">Includes:</TD><TD><TEXTAREA name="includes" cols=50>*</TEXTAREA></TD></TR>\n'
				// + '<TR><TD align="right">Excludes:</TD><TD><TEXTAREA name="excludes" cols=50>http://*google.co*/*</TEXTAREA></TD></TR>\n'
				+ '<TR><TD align="right">Script:</TD><TD><TEXTAREA name="content" cols=50 rows=20>blah(); blah(); blah(); // I am good at HMTL</TEXTAREA></TD></TR>\n'
				+ '</TABLE>\n'
				+ '<P align="right">\n'
				+ '<INPUT type="button" name="test" value="Test"/>\n'
				+ '<INPUT type="button" name="send" value="Save"/>\n'
				+ '<INPUT type="button" name="cancel" value="Cancel"/>\n'
				+ '</P>\n'
				+ '</FORM>\n'
				+ '</FONT>\n'
				+ '';
			document.body.appendChild(newEditor);
			newEditor.style.backgroundColor = '#f8f8f8';
			newEditor.style.color = 'black';
			newEditor.style.border = '2px solid black';
			// breaks: newEditor.style.setProperty('-mozBorderRadius','4px');
			newEditor.style.position = 'fixed';
			newEditor.style.zIndex = '10000';
			newEditor.style.left = parseInt(document.width * 0.15) + 'px';
			newEditor.style.top = parseInt(document.width * 0.15) + 'px';
			newEditor.style.right = '';
			newEditor.style.bottom = '';
			// newEditor.style.width = parseInt(document.width * 0.75) + 'px';
			// newEditor.style.height = parseInt(document.width * 0.75) + 'px';
			// newEditor.style.width = '400px';
			// newEditor.style.height = '400px';
			newEditor.style.padding = '12px';
			try {
				// Testing this.makeMoveable failed :o
				// But I fear in its absence, testing makeMoveable might throw an error :S
				if (makeMoveable) {
					makeMoveable(newEditor);
				}
			} catch (e) { GM_log('Trying to load makeMoveable: '+e); }
			// return document.getElementById(formID);
			var form = document.getElementById(formID);
			if (scriptName) {
				var scriptData = GrimeApeConfig.scripts[scriptName];
				if (scriptData) {
					form['name'].value = scriptName;
					form['namespace'].value = scriptData.namespace;
					form['description'].value = scriptData.description;
					// form['includes'].value = scriptData.includes;
					// form['excludes'].value = scriptData.excludes;
					form['content'].value = "TODO: Load it :P";
				}
			}
		},

	};



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

		showInstallWindow: function(scriptName) {
			Menu.showHideMenu();
			ScriptEditor.openNewEditor(scriptName);
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
			// Menu.addMenuItem("About GrimeApe",(function(){w=window.open();w.location='http://hwi.ath.cx/';}));
			Menu.addMenuItem("Install New Script",Menu.showInstallWindow);
			Menu.addMenuItem("Clear All Data",Menu.clearAllData);
			// Menu.menuElement.appendChild(document.createElement('HR')); // Too wide in Konq!
			Menu.menuElement.appendChild(document.createElement('BR'));
			Menu.menuElement.appendChild(document.createTextNode('-----------'));

			for (var script in GrimeApeConfig.scripts) {
				(function(){ // To keep scriptName, scriptData and checkbox local for toggleScript's context.
					var scriptName = script;
					var scriptData = GrimeApeConfig.scripts[script];

					var checkbox = document.createElement('input');
					checkbox.type = 'checkbox';
					checkbox.checked = scriptData.enabled;

					var editButton = document.createElement('img');
					editButton.src = "/_gRiMeApE_/images/edit16x16.png";
					editButton.title = 'Edit';
					editButton.style.textAlign = 'right';
					editButton.onclick = (function(){ Menu.showInstallWindow(scriptName); });

					var toggleScript = (function(evt) {
							// scriptData.enabled = checkbox.checked;
							scriptData.enabled = !scriptData.enabled;
							checkbox.checked = scriptData.enabled;
							GrimeApeConfig.save();
							Menu.showHideMenu();
							// Optionally, immediately start enabled script:
							if (scriptData.enabled) {
								//// TODO: Hmm this is causing page reload
								//// Mostly fixed now but still occasionally in Konq.
								//// Esp. track_history.
								// loadScript(script);
								loadScriptOtherWay(scriptName);
							}
							// evt.preventDefault();
							// return null;
					});

					checkbox.onclick = toggleScript;
					var menuItem = Menu.addMenuItem(script,toggleScript);
					Menu.menuElement.insertBefore(checkbox,menuItem);
					Menu.menuElement.insertBefore(editButton,menuItem.nextSibling);
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
				var scriptData = GrimeApeConfig.scripts[script];
				if (GrimeApeConfig.scripts[script].enabled) {
					loadScript(script);
					countLoaded++;
				}
			}
		}
		GM_log("GrimeApe loaded "+countLoaded+" scripts.");
	}

	doStart();



})();


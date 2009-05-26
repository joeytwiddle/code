
/*

CONSIDER: We could have the proxy inject the monkey at the start of the page,
so that the user can see him early, and config him while the page loads.
The loading of userscripts should still be delayed till the end of loading though.
(Either injected by proxy, or detected by onLoad or something.)

TODO: We could split the config up a bit better.  It grows rather large.
1) Common config: which scripts are enabled/disabled
      This is small so fast upload when the user changes it.
2) Common script data (includes/excludes)
      We want this every page, but we don't want to upload it too often!
2) Extra script data
      All the other data, loaded on demand.
      e.g. Userscript descriptions need only be loaded when the user is
           actually interacting with the menu.
      We could split this data into 1 request per userscript.
      But it might make sense to load the whole thing if it is needed.
Whoops I am bloating this file with TODO comments.

Consider a super-streamlined system.  It would only send what is needed.
Why send the include/exclude data to the client when the proxy side could
match them against the current URL, and do all the injection before GrimeApe
has even loaded in the client.

TODO: when uninstalling a script, GA should offer to search and destroy any
registry values set by that script's namespace.

*/

(function(){



	//// Config ////

	var GrimeApeConfig = new Object();

	// Try loading config:
	try {
		GrimeApeConfig = eval(GM_getValue("GrimeApeConfig"));
		// GM_log("Loaded config length "+uneval(GrimeApeConfig).length);
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
				"Highlight Search Result Pages" : ({ enabled: true }),
				"Delicious Search Results on Google" : ({ enabled: true }),
				"Twitter Search Results On Google" : ({ enabled: false }),
				"SF ProjectHomepage" : ({ enabled: true }),
				"Googles Old Favicon" : ({ enabled: true }),
				"Track History" : ({ enabled: false }),
				"Cleanup Fonts" : ({ enabled: false }),
				"Reclaim CPU" : ({ enabled: true }),
			}),
		};

	}

	// This may look like a long-winded way of declaring the object, but:
	// declaring the functions this way gives them access to each other,
	// and we really should be overwriting save() because the function may have
	// been loaded from the uneval, but the version here might be newer.

	GrimeApeConfig.save = function() {
		GM_setValue("GrimeApeConfig",uneval(GrimeApeConfig));
	};

	function getFsName(scriptName) {
		return scriptName.toLowerCase().replace(/ /g,'_').replace(/'/g,'').replace(/\//g,'').substring(0,24);
	}

	function loadScript(scriptName) {
		var fsName = getFsName(scriptName);
		GM_log('Loading userscript: '+scriptName);
		var namespace = GrimeApeConfig.scripts[scriptName].namespace;
		var url = "/_gRiMeApE_/userscripts/" + fsName + '/' + fsName + ".user.js?namespace=" + encodeURIComponent(namespace+"/"+scriptName);
		document.writeln("<SCRIPT type='text/javascript' src='" + url + "'></SCRIPT>");
	}

	/** This is better if the document has finished loading.  Sometimes the other method can cause
	 * problems in that situation. **/
	function loadScriptOtherWay(scriptName) {
		var fsName = getFsName(scriptName);
		GM_log('Loading: '+scriptName);
		// var url = "/_gRiMeApE_/userscripts/" + fsName + '/' + fsName + ".user.js"
		var namespace = GrimeApeConfig.scripts[scriptName].namespace;
		var url = "/_gRiMeApE_/userscripts/" + fsName + '/' + fsName + ".user.js?namespace=" + encodeURIComponent(namespace+"/"+scriptName);
		var scriptElement = document.createElement("SCRIPT");
		scriptElement.type = 'text/javascript';
		scriptElement.src = url;
		// document.body.appendChild(scriptElement); // Sometimes page change in Konq? yes
		document.getElementsByTagName("HEAD")[0].appendChild(scriptElement);
	}

	function loadScriptString(scriptString) {
		document.writeln("<SCRIPT type='text/javascript'>"+scriptString+"</SCRIPT>");
	}

	function loadScriptStringOtherWay(scriptString) {
		var scriptElement = document.createElement("SCRIPT");
		scriptElement.type = 'text/javascript';
		scriptElement.innerHTML = scriptString;
		document.getElementsByTagName("HEAD")[0].appendChild(scriptElement);
	}

	function scriptTriggers(scriptData) {
		// if (!scriptData.includes) {
			// GM_log("Odd: no includes. "+scriptData.includes);
		// }
		// GM_log("Checking triggers for "+scriptData+" "+typeof(scriptData.includes)+"/"+typeof(scriptData.excludes));
		if (scriptData.includes) {
			var pass = false;
			for (var i in scriptData.includes) {
				// GM_log("About to check include ["+i+"] = "+scriptData.includes[i]);
				if (matchesGlob(""+document.location,""+scriptData.includes[i])) {
					// GM_log("Script matched include: "+scriptData.includes[i]);
					pass = true; break;
				}
			}
			if (!pass) {
				return false;
			}
		}
		if (scriptData.excludes) {
			for (var i in scriptData.excludes) {
				// GM_log("About to check exclude ["+i+"] = "+scriptData.excludes[i]);
				if (matchesGlob(""+document.location,""+scriptData.excludes[i])) {
					// GM_log("Script matched exclude: "+scriptData.excludes[i]);
					return false;
				}
			}
		}
		return true;
	}



	// GrimeApe runs in all frames, but the icon+menu only appear in the top frame.
	// If the user really wants a monkey in a sub-frame, she could try opening the frame in her main window.
	// Sometimes this causes a "Permission denied to get property Window.document"
	// if (document == top.document) {
	if (window == top) {

		//// Icon ////

		var ApeIcon = {

			// var iconStyleString = " position: fixed; right: 4px; bottom: 4px; z-index: 1000 ";
			iconWidth  : 42,
			iconHeight : 43,
			iconURLLoading  : "/_gRiMeApE_/images/blueape_loading.png",
			iconURL         : "/_gRiMeApE_/images/blueape.png", // punkape.png",
			iconURLDisabled : "/_gRiMeApE_/images/blueape_disabled.png",

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
					bottom='2px';
					zIndex='120000';
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

				/*
				icon.style.opacity='1.0';
				// icon.style.filter='alpha(opacity=50)';
				// icon.style.mozOpacity='0.5'; // none work for Konq
				// icon.style._mozOpacity='0.5';
				icon.onmouseout = function() { icon.style.opacity='1.0'; };
				icon.onmouseover = function() { icon.style.opacity='0.8'; };
				*/

			},

			setIcon : function() {
				ApeIcon.icon.src = ( GrimeApeConfig.enabled ? ApeIcon.iconURL : ApeIcon.iconURLDisabled );
			}

		};

		ApeIcon.initIcon();



		var formID = "scriptEditorForm"+document.forms.length;
		var ScriptEditor = {

			openNewEditor: function(scriptName) {

				////// Create new editor window:
				var newEditor = document.createElement('DIV');
				newEditor.innerHTML = ''
					// + '<STYLE type="text/css"> .td{ text-align:right; } </STYLE>\n'
					// + '<FORM id="'+formID+'" method="GET" action="/_gRiMeApE_/saveUserscript">\n'
					+ '<FORM id="'+formID+'">\n'
					+ '<FONT size="-1">'
					// + '<TABLE>'
					// + '<TR><TD align="right">Name:</TD><TD><INPUT type="text" name="name" value="Script Name"/></TD></TR>\n'
					// + '<TR><TD align="right">Namespace:</TD><TD><INPUT type="text" name="namespace" value="Nobody knows what this is."/></TD></TR>\n'
					// + '<TR><TD align="right">Description:</TD><TD><INPUT type="text" name="description" value="Description of the script"/></TD></TR>\n'
					// // + '<TR><TD align="right">Includes:</TD><TD><TEXTAREA name="includes" cols=80>*</TEXTAREA></TD></TR>\n'
					// // + '<TR><TD align="right">Excludes:</TD><TD><TEXTAREA name="excludes" cols=80>http://*google.co*/*</TEXTAREA></TD></TR>\n'
					// + '<TR><TD align="right">Script:</TD><TD><TEXTAREA name="content" cols=80 rows=20>blah(); blah(); blah(); // I am good at HMTL</TEXTAREA></TD></TR>\n'
					// + '</TABLE>\n'
					+ '<TEXTAREA name="content" cols="80" rows="35">'
					+ '// ==UserScript==\n'
					+ '// @name           ....\n'
					+ '// @namespace      ....\n'
					+ '// @description    ....\n'
					+ '// @include        ....\n'
					+ '// @exclude        ....\n'
					+ '// @version        ....\n'
					+ '// @homepage       ....\n'
					+ '// @copyright      %year, %author\n'
					+ '// @license        ....\n'
					+ '// ==/UserScript==\n'
					+ '\n'
					+ '</TEXTAREA>'
					+ '<P align="right">'
					+ '<INPUT type="button" name="test" value="Test"/>\n'
					+ '<INPUT type="button" name="save" value="Save"/>\n'
					+ '<INPUT type="button" name="cancel" value="Cancel"/>'
					+ '</P>'
					+ '</FONT>'
					+ '</FORM>' // Bah this creates always an empty line after.  Should swap div and form around :P
					+ '';
				document.body.appendChild(newEditor);

				////// Style:
				newEditor.style.backgroundColor = '#f8f8f8';
				newEditor.style.color = 'black';
				newEditor.style.border = '2px solid black';
				// breaks: newEditor.style.setProperty('-mozBorderRadius','4px');
				newEditor.style.position = 'fixed';
				newEditor.style.zIndex = '12000';
				newEditor.style.left = parseInt(window.innerWidth * 0.10) + 'px';
				newEditor.style.top = parseInt(window.innerHeight * 0.05) + 'px';
				newEditor.style.right = '';
				newEditor.style.bottom = '';
				// newEditor.style.width = parseInt(document.width * 0.75) + 'px';
				// newEditor.style.height = parseInt(document.width * 0.75) + 'px';
				// newEditor.style.width = '400px';
				// newEditor.style.height = '400px';
				newEditor.style.padding = '12px';

				////// Make it moveable:
				//// TODO: bring this in for sure
				/*
				if (this.makeMoveable) {
					makeMoveable(newEditor);
				}
				*/
				// This only loads if its available
				// But recently it seems if it fails on first attempt, the form doesn't get populated.
				/*
				try {
					// Testing this.makeMoveable failed! :o
					// But I fear testing just makeMoveable in its absence, might throw an error :S
					if (makeMoveable) {
						makeMoveable(newEditor);
					}
					// if (!this.makeMoveable) {
						// GM_log("Whoa makeMoveable worked but this.makeMoveable failed!");
					// }
				} catch (e) { GM_log('Trying to load makeMoveable: '+e); }
				// return document.getElementById(formID);
				*/

				// Populate form values:
				var form = document.getElementById(formID);
				if (scriptName) {
					var scriptData = GrimeApeConfig.scripts[scriptName];
					if (scriptData) {
						// form['name'].value = scriptName;
						// form['namespace'].value = scriptData.namespace;
						// form['description'].value = scriptData.description;
						// // form['includes'].value = scriptData.includes;
						// // form['excludes'].value = scriptData.excludes;
						////// Now the big one - script content:
						//// DONE: We should be requesting CDATA wrapped response.  But Firefox still complains.  :P
						var fsName = getFsName(scriptName);
						var req = new XMLHttpRequest();
						req.open('GET','/_gRiMeApE_/userscripts/'+fsName+'/'+fsName+'.user.js?cdata=yes',false);
						req.setRequestHeader('Accept','text/javascript');
						// GM_log("Requesting script contents...");
						req.send(null);
						// GM_log("Got "+req.responseText);
						// var strippedResponse = req.responseText.replace(/^<!\[CDATA\[/,'').replace(/\]\]>$/,'');
						//// What does .replace() do if the text contains multiple newlines?
						var strippedResponse = req.responseText;
						var expectStart = "<![CDATA[";
						var expectEnd = "]]>";
						if (strippedResponse.substring(0,expectStart.length) == expectStart)
							strippedResponse = strippedResponse.substring(expectStart.length);
						// else GM_log("Expected BLAH but got \""+strippedResponse.substring(0,expectStart.length)+"\"");
						if (strippedResponse.substring(strippedResponse.length - expectEnd.length) == expectEnd)
							strippedResponse = strippedResponse.substring(0,strippedResponse.length - expectEnd.length);
						form['content'].value = strippedResponse;
						form['content'].textContent = strippedResponse; // Needed for Konq, doesn't work for Moz.
					}
				}

				////// Setup form button events:
				form['cancel'].onclick = function(evt) {
					newEditor.parentNode.removeChild(newEditor);
				};
				form['test'].onclick = function(evt) {
					eval('(function(){ ' + form['content'].value + '})();');
				};
				form['save'].onclick = ScriptEditor.saveScript;

				return form;

			},

			saveScript: function(evt) {

				GM_log("Attemping save ... ");
				var form = document.getElementById(formID);
				form['save'].disabled = true;
				try {
					// var content = form['content'].textContent; // In Konq this is not updated by user!
					var content = form['content'].value;
					function getMeta(key) {
						// key = key.replace(/\*/g,'\\*');
						// var regex = new RegExp("// *@"+key+"  *(.*)");
						var regex = new RegExp("//\\s*@"+key+"\\s\\s*(.*)");
						try {
							// return content.match(regex)[1];
							return regex.exec(content)[1];
						} catch (e) {
							// alert("Your script needs a tag: // @"+key+" ...");
							GM_log("Failed to get meta @"+key);
							return "";
						}
					}
					function getMetasArray(key) {
						// key = key.replace(/\*/g,'\\*');
						var regex = new RegExp("//\\s*@"+key+"\\s\\s*(.*)","g");
						var list = new Array();
						var match;
						while (match = regex.exec(content)) {
							GM_log("Got match for \""+key+"\": "+match[1]);
							list.push(""+match[1]);
						}
						// if (list.length == 0)
							// return undefined;
						return list;
					}
					function getMetas(key) {
						// key = key.replace(/\*/g,'\\*');
						var regex = new RegExp("//\\s*@"+key+"\\s\\s*(.*)","g");
						var obj = new Object();
						var match;
						var i = 0;
						while (match = regex.exec(content)) {
							GM_log("Got match for \""+key+"\": "+match[1]);
							obj[i++] = ""+match[1];
						}
						return obj;
					}
					function getPairedMetas(key) {
						var regex = new RegExp("//\\s*@"+key+"\\s\\s*([^\\s]*)\\s\\s*(.*)","g");
						var obj = new Object();
						var match;
						while (match = regex.exec(content)) {
							GM_log("Got pair match for \""+key+"\": "+match[1]+" -> "+match[2]);
							obj[(""+match[1])] = ""+match[2];
						}
						return obj;
					}
					// var newScriptName = content.match(/\/\/ *@name  *(.*)/)[1];
					var newScriptName = getMeta("name");
					if (!newScriptName) {
						alert("Your script needs a tag: // @name MyScriptName");
					} else {
						// Upload the file:
						var fsName = getFsName(newScriptName); // TODO: Actually should get new name from script!
						var cgi = "name="+encodeURIComponent(newScriptName) + "&content="+encodeURIComponent(content);
						var req = new XMLHttpRequest();
						req.open("POST","/_gRiMeApE_/updateScript",false);
						GM_log("Sending...");
						req.send(cgi);
						GM_log("Sent.  Got response: \""+req.responseText+"\"");
						
						// Update the config:
						if (req.responseText == "<NODATA>OK</NODATA>") {
							var scriptData = new Object();
							scriptData.enabled = true;
							scriptData.namespace = getMeta("namespace");
							scriptData.description = getMeta("description");
							scriptData.includes = getMetas("include");
							GM_log("Got includes = "+scriptData.includes);
							GM_log("Got unevaled includes = "+uneval(scriptData.includes));
							scriptData.excludes = getMetas("exclude");
							scriptData.resources = getPairedMetas("resource");
							GrimeApeConfig.scripts[newScriptName] = scriptData;
							GrimeApeConfig.save();
							Menu.rebuild();
							form['cancel'].onclick(); // close window
						} else {
							GM_log("Error uploading: "+req.responseText);
							alert("Error uploading: "+req.responseText);
						}
					}
				} catch (e) {
					alert(e);
				}
				if (form && form['save']) // May have been destroyed
					form['save'].disabled = false;

			},

		};



		//// Menu ////

		var Menu = {

			// menuElement: document.createElement('DIV'),
			menuElement: undefined,
			userscriptCommandsDiv: undefined,
			userscriptCommandsMenuItem: undefined,

			showHideMenu: function(evt) {
				Menu.menuElement.style.display = ( Menu.menuElement.style.display ? '' : 'none' );
			},

			toggleEnabled: function(evt) {
				evt.preventDefault();
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
				Menu.showHideMenu();
				var decision = confirm("Are you sure you want to reset GrimeApe to defaults?");
				if (decision) {
					var url = "/_gRiMeApE_/clearAll";
					var client = new XMLHttpRequest();
					client.open('GET',url,false);
					client.send(null);
				}
			},

			openEditorWindow: function(scriptName) {
				Menu.showHideMenu();
				ScriptEditor.openNewEditor(scriptName);
			},

			getUserscripts: function() {
				Menu.showHideMenu();
				window.open("http://www.userscripts.org/");
			},

			deleteScript: function(scriptName) {
				Menu.showHideMenu();
				var decision = confirm("Are you sure you want to delete \""+scriptName+"\"?");
				if (decision) {
					try {
						var url = "/_gRiMeApE_/deleteScript?name="+encodeURIComponent(scriptName);
						var client = new XMLHttpRequest();
						client.open("GET",url,false);
						client.send(null);
						if (client.responseText == "<NODATA>OK</NODATA>") {
							delete GrimeApeConfig.scripts[scriptName];
							GrimeApeConfig.save();
							Menu.rebuild();
						} else {
							alert("Delete did not go cleanly.");
							// Remove it from config anyway?
						}
					} catch (e) {
						GM_log(e);
						alert(e);
					}
				}
			},

			showUserscriptCommands: function(evt) {
				// alert("height="+document.height+" scroll="+window.scrollY+" mouse="+evt.clientY+" innerHeight="+window.innerHeight);
				// Menu.userscriptCommandsDiv.style.right = (document.width - window.scrollX - evt.clientX)+'px';
				// Menu.userscriptCommandsDiv.style.bottom = (document.height - window.scrollY - evt.clientY)+'px';
				// Menu.userscriptCommandsDiv.style.left = (window.scrollX + evt.clientX)+'px';
				// Menu.userscriptCommandsDiv.style.top = (window.scrollY + evt.clientY)+'px';
				Menu.userscriptCommandsDiv.style.right = (window.innerWidth - evt.clientX)+'px';
				Menu.userscriptCommandsDiv.style.bottom = (window.innerHeight - evt.clientY)+'px';
				Menu.userscriptCommandsDiv.style.display = ( Menu.userscriptCommandsDiv.style.display ? '' : 'none' );
			},

			addUserscriptCommand: function(commandName, commandFunc, accelKey, accelModifier, accessKey) {
				if (Menu.userscriptCommandsDiv.childNodes.length > 0)
					Menu.userscriptCommandsDiv.appendChild(document.createElement('BR'));
				var link = document.createElement('A');
				link.textContent = commandName;
				link.href = 'javascript:void(0)';
				// Trying onmousedown instead of onclick, maybe will reduce Konq's switch to drag mode.
				link.onmousedown = (function(evt) { commandFunc(); });
				Menu.userscriptCommandsDiv.appendChild(link);
				Menu.userscriptCommandsMenuItem.onclick = Menu.showUserscriptCommands;
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
					// menuItem.insertBefore(document.createElement('BR'),menuItem.firstChild);
				Menu.menuElement.appendChild(menuItem);

				// A shorter way to create a link:
				// var menuItem = createElement('A', {href:"javascript:void(0)", style:"text-decoration:none; font-size:x-small"}, 'click '+fnName+' false', ' (options)');
				return menuItem;
			},

			init: function() {
				Menu.menuElement = document.createElement('DIV');
				// Menu.addMenuItem("About GrimeApe",(function(){w=window.open();w.location='http://hwi.ath.cx/';}));
				// Menu.addMenuItem("Clear All Data",Menu.clearAllData);
				// Menu.menuElement.appendChild(document.createElement('HR')); // Too wide in Konq!
				// Menu.menuElement.appendChild(document.createElement('BR'));
				// Menu.menuElement.appendChild(document.createTextNode('-----------'));

				for (var script in GrimeApeConfig.scripts) {
					(function(){ // To keep scriptName, scriptData and checkbox local for toggleScript's context.
						var scriptName = script;
						var scriptData = GrimeApeConfig.scripts[script];

						var checkbox = document.createElement('input');
						checkbox.type = 'checkbox';
						checkbox.checked = scriptData.enabled;

						var editButton = document.createElement('img');
						editButton.src = "/_gRiMeApE_/images/edit16x16.png";
						editButton.title = 'Edit '+scriptName;
						// editButton.style.paddingLeft = '8px';
						// editButton.style.verticalAlign = 'middle';
						// editButton.style.textAlign = 'right';
						editButton.style.position = 'absolute';
						editButton.style.right = '22px';
						editButton.style.cursor = 'pointer';
						editButton.onclick = (function(evt){ evt.preventDefault(); Menu.openEditorWindow(scriptName); });

						var deleteButton = document.createElement('img');
						deleteButton.src = "/_gRiMeApE_/images/delete16x16.png";
						deleteButton.width = 10;
						deleteButton.height = 10;
						deleteButton.title = 'Delete '+scriptName;
						// deleteButton.style.paddingLeft = '8px';
						// deleteButton.style.verticalAlign = 'middle';
						// deleteButton.style.textAlign = 'right';
						deleteButton.style.position = 'absolute';
						deleteButton.style.right = '6px';
						deleteButton.style.cursor = 'pointer';
						deleteButton.style.marginTop = '4px';
						deleteButton.onclick = (function(evt){ evt.preventDefault(); Menu.deleteScript(scriptName); });

						var toggleScript = (function() {
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
						});

						checkbox.onclick = toggleScript;
						// var menuItem = Menu.addMenuItem(script,toggleScript);
						var menuItem = Menu.addMenuItem(script,function(evt){ evt.preventDefault(); toggleScript(); });
						menuItem.style.paddingRight = '40px';
						Menu.menuElement.insertBefore(checkbox,menuItem);
						Menu.menuElement.insertBefore(deleteButton,menuItem.nextSibling);
						Menu.menuElement.insertBefore(editButton,menuItem.nextSibling);
						if (GrimeApeConfig.scripts[script].description) {
							menuItem.title = GrimeApeConfig.scripts[script].description;
						}
						// GM_log("Checking script \""+script+"\" with includes "+GrimeApeConfig.scripts[scriptName].includes);
						if (!scriptTriggers(GrimeApeConfig.scripts[scriptName])) {
							menuItem.style.color = '#aaaaaa';
						}
					})();
				}

				// Menu.menuElement.appendChild(document.createElement('HR'));
				Menu.menuElement.appendChild(document.createElement('BR'));
				Menu.menuElement.appendChild(document.createTextNode('-----------'));
				// Menu.userscriptCommandsMenuItem = Menu.addMenuItem("Userscript Commands",Menu.showUserscriptCommands);
				Menu.userscriptCommandsMenuItem = Menu.addMenuItem("Userscript Commands");
				Menu.addMenuItem("Create New Userscript",Menu.openEditorWindow);
				// TODO: If we make Get Userscripts a link A with valid href and target, Konq won't ask confirmation to pop it up!
				Menu.addMenuItem("Get Userscripts",Menu.getUserscripts);
				Menu.addMenuItem("Show Log",Menu.showHideLog);
				Menu.addMenuItem("Enable/Disable",Menu.toggleEnabled);

				Menu.userscriptCommandsDiv = document.createElement('DIV');
				// Menu.userscriptCommandsDiv.appendChild(document.createTextNode('Userscript Commands'));
				document.body.appendChild(Menu.menuElement);
				document.body.appendChild(Menu.userscriptCommandsDiv);
				Menu.initStyle();
			},

			initStyle: function() {
				with (Menu.menuElement.style) {
					display = 'none';
					position = 'fixed';
					right = '4px';
					bottom = (ApeIcon.iconHeight+4)+'px';
					zIndex = 120000;
					border = '1px solid #444444';
					backgroundColor = '#ffffff';
					color = 'black';
					padding = '6px';
					textAlign = 'left';
				}
				// var css = " #menuEnabled.link{ color:#00ff00; } #menuDisabled.link{ color:#880000; } ";
				// document.writeln("<SCRIPT type='text/css'>"+css+"</SCRIPT>");
				Menu.userscriptCommandsDiv.style.display = 'none';
				Menu.userscriptCommandsDiv.style.position = 'fixed';
				Menu.userscriptCommandsDiv.style.zIndex = 120001;
				Menu.userscriptCommandsDiv.style.border = '1px solid #444444';
				Menu.userscriptCommandsDiv.style.backgroundColor = 'white';
				Menu.userscriptCommandsDiv.style.color = 'black';
				Menu.userscriptCommandsDiv.style.padding = '4px';
				Menu.userscriptCommandsDiv.style.textAlign = 'left';
			},

			rebuild: function() {
				if (Menu.menuElement) {
					Menu.menuElement.parentNode.removeChild(Menu.menuElement);
				}
				Menu.init();
			}

		};

		Menu.init();
		// document.body.appendChild(Menu.menuElement);
		// Menu.initStyle();

		this.GM_registerMenuCommand = Menu.addUserscriptCommand;

		// var icon = document.getElementById('gaIcon');
		// icon.addEventListener('click',Menu.showHideMenu());
		// iconHolder.href = '#'; iconHolder.target = '_self';
		// ApeIcon.icon.onclick = Menu.showHideMenu;
		ApeIcon.icon.onclick = (function(evt) { Menu.showHideMenu(); evt.preventDefault(); });
		// GM_log("icon.parentNode = "+icon.parentNode);



		//// Special - convert Userscripts.org Install button into GrimeApe Install button. ////
		if (GrimeApeConfig.enabled && document.location.href.match('http://(www.|)userscripts.org/scripts/show/')) {
			var links = document.links;
			for (var i=0;i<links.length;i++) {
				var link = links[i];
				// if (link.className == 'userjs') {
				if (link.href && link.href.match(/\.user\.js$/)) {
					(function() { // Creating a context for scriptHref
						var scriptHref = link.href;
						var doGrimeApeInstall = function(evt) {
							evt.preventDefault();
							var form = ScriptEditor.openNewEditor();
							GM_xmlhttpRequest( { method:'GET', url:scriptHref,
								onload: function(response) {
									form['content'].value = response.responseText;
									form['content'].textContent = response.responseText;
								}
							} );
						};
						link.addEventListener('click',doGrimeApeInstall,false);
						link.title = "Install in GrimeApe...";
						var miniApe = document.createElement("IMG");
						miniApe.src = "/_gRiMeApE_/images/blueape.png";
						miniApe.height = 16; miniApe.width = 16;
						miniApe.style.verticalAlign = 'middle';
						miniApe.style.paddingLeft = '6px';
						miniApe.style.paddingBottom = '2px';
						link.appendChild(miniApe);
						// link.parentNode.insertBefore(miniApe,link.nextSibling);
					})();
				}
			}
		}

	}



	//// Start ////

	function matchesGlob(str,glob) {
		try {
			if (typeof(str) != 'string') {
				GM_log("matchesGlob has been passed non-string: "+typeof(str)+"");
			}
			var globre = glob.replace(/\*/g,'.*');
			// GM_log("Checking against "+glob+" where globre="+globre);
			var re = new RegExp(globre);
			return re.exec(str);
		} catch (e) {
			// GM_log("  "+glob+"  ~~  "+re);
			GM_log("Problem matching glob \""+glob+"\": "+e);
			return false;
		}
	}

	function doStart() {
		var countLoaded = 0;
		if (GrimeApeConfig.enabled) {
			for (var script in GrimeApeConfig.scripts) {
				// GM_log("script = "+script);
				// GM_log("scripts[script] = " + GrimeApeConfig.scripts[script]);
				var scriptData = GrimeApeConfig.scripts[script];
				if (GrimeApeConfig.scripts[script].enabled && scriptTriggers(scriptData)) {
					loadScript(script);
					countLoaded++;
				} else {
					// GM_log("Skipping ("+script+")");
				}
			}
		}
		GM_log("GrimeApe loading "+countLoaded+" userscripts...");
		loadScriptString("GM_log('GrimeApe loaded "+countLoaded+" scripts.');");
	}



	// Visible to me for debugging!
	this.GrimeApeConfig = GrimeApeConfig;

	doStart();
	// document.body.addEventListener('load',doStart,false);
	// setTimeout('doStart()',1000);



})();


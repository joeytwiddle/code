// ==UserScript==
// @name           Make Bookmarklet From Javascript URL
// @namespace      MBFU
// @description    When it sees a link to a userscript or general Javascript URL, adds a Bookmarklet besides it, which you can drag to your toolbar to load the script when you next need it (running outside Greasemonkey of course).
// @include        http://hwi.ath.cx/*/gm_scripts/*
// @include        http://hwi.ath.cx/*/userscripts/*
// @include        http://*userscripts.org/*
// @exclude        http://hwi.ath.cx/code/other/gm_scripts/joeys_userscripts_and_bookmarklets_overview.html
// ==/UserScript==

// TODO: We could provide neat catches for GM_ API commands so they won't fail
// entirely.

// TODO: Provide extra feature, which allows the Bookmarks to actually trigger
// the userscript properly-running inside Greasemonkey, if this userscript is
// present to handle the request, otherwise (with warning) load outside GM.

function addBookmarklet(link,includeGMCompat,neverCache) {
	var scriptsToLoad = [];
	if (includeGMCompat) {
		scriptsToLoad.push("http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js");
	}
	scriptsToLoad.push(link.href);

	var toRun = "(function(){\n";
	for (var i=0;i<scriptsToLoad.length;i++) {
		var neverCacheStr = ( neverCache ? "+'?dummy='+new Date().getTime()" : "" );
		var script = scriptsToLoad[i];
		toRun += "  var newScript = document.createElement('script');\n";
		toRun += "  newScript.src = '" + script + "'" + neverCacheStr + ";\n";
		toRun += "  document.body.appendChild(newScript);\n";
	}
	toRun += "})();";

	// Most people will NOT want the NoCache version.  Only I do for development.
	var name = getNameFromFilename(link.href);
	/*
	if (neverCache) {
		name = name + " (NoCache)";
	}
	if (includeGMCompat) {
		name = name + " (FBAPI)";
	}
	*/
	name += ( neverCache || includeGMCompat ? neverCache && includeGMCompat ? " (NC+FB)" : neverCache ? " (NC)" : " (FB)" : "" );

	var newLink = document.createElement("A");
	newLink.href = "javascript:" + toRun;
	newLink.textContent = name;

	var extra = document.createElement("SPAN");
	extra.appendChild(document.createTextNode("(Bookmarklet: "));
	extra.appendChild(newLink);
	extra.appendChild(document.createTextNode(")"));
	extra.style.paddingLeft = '8px';
	link.parentNode.insertBefore(extra,link.nextSibling);
}

function addQuickInstall(link) {
	var br2 = document.createElement("br");
	link.parentNode.insertBefore(br2,link.nextSibling);
	var br = document.createElement("br");
	link.parentNode.insertBefore(br,link.nextSibling.nextSibling);
	var name = link.href.match(/([^\/]*)\/$/)[1];
	var newLink = document.createElement("A");
	newLink.href = link.href + name+".user.js";
	newLink.textContent = "Install"; // name+".user.js";
	var extra = document.createElement("span");
	extra.appendChild(document.createTextNode("      ["));
	extra.appendChild(newLink);
	extra.appendChild(document.createTextNode("]"));
	extra.style.paddingLeft = '8px';
	link.parentNode.insertBefore(extra,br);
	link.style.color = 'grey';
	addBookmarklet(newLink,true,true);
	addLiveUserscript(newLink);
}

function addSourceViewer(link) {
	var newLink = document.createElement("A");
	// newLink.href = '#';
	newLink.textContent = "Source";
	newLink.addEventListener('click',function(e) {
		var div = document.createElement("iframe");
		div.src = link.href;
		div.style.position = 'fixed';
		div.style.top = e.clientY+4+'px';
		div.style.left = e.clientX+4+'px';
		div.style.backgroundColor = 'white';
		div.style.color = 'black';
		div.style.padding = '8px';
		div.style.border = '2px solid #555555';
		document.body.appendChild(div);
	},true);
	// TODO: Problem with .user.js files and Chrome:
	// In Chrome, opens an empty iframe then the statusbar says it wants to install an extension.
	// For Chrome we could try: div.src = "view-source:"+...;
	var extra = document.createElement("span");
	extra.appendChild(document.createTextNode("["));
	extra.appendChild(newLink);
	extra.appendChild(document.createTextNode("]"));
	extra.style.paddingLeft = '8px';
	link.parentNode.insertBefore(extra,link.nextSibling);
}

function addLiveUserscript(link) {
	/* DISABLED
	// BUG: data:{...}.user.js does not interest my Chromium
	var name = getNameFromFilename(link.href)+" Live!";
	var name = "Install LiveLoader";
	var whatToRun = '(function(){\n'
		+ '  var ns = document.createElement("script");\n'
		+ '  ns.src = "' + encodeURI(getCanonicalUrl(link.href)) + '";\n'
		+ '  document.getElementsByTagName("head")[0].appendChild(ns);\n'
		+ '})();\n';
	var newLink = document.createElement("a");
	newLink.textContent = name;
	newLink.href = "data:text/javascript;charset=utf-8,"
		+ "// ==UserScript==%0A"
		+ "// @namespace LiveLoader%0A"
		+ "// @name " + name + " LIVE%0A"
		+ "// @description Loads " +name+ " userscript live from " + link.href + "%0A"
		+ "// ==/UserScript==%0A"
		+ "%0A"
		+ encodeURIComponent(whatToRun) + "%0A"
		+ "//.user.js";
	var extra = document.createElement("span");
	extra.appendChild(document.createTextNode("["));
	extra.appendChild(newLink);
	extra.appendChild(document.createTextNode("]"));
	extra.style.paddingLeft = '8px';
	link.parentNode.insertBefore(extra,link.nextSibling);
	*/
}

function getCanonicalUrl(url) {
	if (url.substring(0,1)=="/") {
		url = document.location.protocol + "://" + document.location.domain + "/" + url;
	}
	if (!url.match("://")) {
		url = document.location.href.match("^[^?]*/") + url;
	}
	return url;
}

function getNameFromFilename(href) {
	var isUserscript = href.match(/\.user\.js$/);
	// Remove any leading folders and trailing ".user.js"
	var name = href.match(/[^\/]*$/)[0].replace(/\.user\.js$/,'');
	if (isUserscript) {
		var words = name.split("_");
		for (var i=0;i<words.length;i++) {
			if (words[i].length) {
				var c = words[i].charCodeAt(0);
				if (c>=97 && c<=122) {
					c = 65 + (c - 97);
					words[i] = String.fromCharCode(c) + words[i].substring(1);
				}
			}
		}
		name = words.join(" ");
	} else {
		// It's just a Javascript file
		name = "Load "+name;
	}
	return name;
}

var links = document.getElementsByTagName("A");
for (var i=links.length-1;i>=0;i--) {
	var link = links[i];
	// Links to subfolders of this greasmonkey scripts folder
	if (document.location.href.match(/\/(gm_scripts|userscripts)\//)) {
		if (link.href.match(/\/$/)) {
			addQuickInstall(link);
		}
	}
	// Direct links to scripts
	if (link.href.match(/\.js$/)) { // \.user\.js
		addBookmarklet(link,true,true);
		addLiveUserscript(link);
		addSourceViewer(link);
	}
}


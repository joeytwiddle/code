// ==UserScript==
// @name           Turn Image Links into Images
// @namespace      TILI
// @description    For any links (<A> tags) on the page which point to an image, show the image!
// @author         joeytwiddle
// @license        ISC
// @grant          none
// ==/UserScript==

var imageExtensions = [ "png", "jpg", "jpeg", "gif", "bmp" ];

function isKnownExtension(ext) {
	ext = ext.toLowerCase();
	for (var i=imageExtensions.length; i-->0; ) {
		if (ext == imageExtensions[i])
			return true;
	}
	return false;
}

var links = document.getElementsByTagName("A");

for (var i = links.length; i-->0; ) {
	var link = links[i];
	var ext = link.href.replace(/.*\.([^.]*)$/,'$1');
	// var ext = link.href.match(/\.[^.]*$/)[0].substring(1);
	if (isKnownExtension(ext)) {
		link.innerHTML = '';
		var image = document.createElement("img");
		image.src = link.href;
		link.appendChild(image);
	}
}

// ==UserScript==
// @name           IMDB Inline Threads
// @namespace      IIT
// @description    Makes collapsed posts in a thread load directly into the current page, like on SlashDot, avoiding navigation to a separate page to read each post.
// @include        http://www.imdb.com/*/board/thread/*
// ==/UserScript==

function listenForLoad(newLink,href) {
	newLink.addEventListener('click',function(evt){
		loadThreadBitFor(newLink,href);
	},true);
}

var links = document.getElementsByTagName("A");
for (var i=0;i<links.length;i++) {
	var link = links[i];
	if (link.href.match(/\/board\/thread\/.*d=/)) {
		var newLink = document.createElement("A");
		newLink.textContent = "[+] " + link.textContent;
		// newLink.href = link.href;
		newLink.href = 'javascript:(void 0);';
		newLink.title = "Expand this post";
		link.parentNode.insertBefore(newLink,link);
		link.parentNode.removeChild(link);
		newLink.style.cursor = 'pointer';
		listenForLoad(newLink,link.href);
	}
}

function loadThreadBitFor(link,href) {

	function handleResponse(response) {
		var page = document.createElement("div");
		page.innerHTML = response.responseText;
		// var img = oneXPath(document.body,"//img[contains(@src,'msgup.gif')]");
		var otherPostRow = oneXPath(page,"//img[contains(@src,'msgup.gif') or contains(@src,'msgdown.gif')]/ancestor::table/ancestor::tr");
		// var newTable = document.createElement("table");
		// var tbody = document.createElement("tbody");
		// tbody.appendChild(otherPostRow);
		// newTable.appendChild(tbody);
		// link.parentNode.appendChild(newTable);
		var parentRow = link;
		while (parentRow && parentRow.tagName!="TR") {
			parentRow = parentRow.parentNode;
		}

		// Indent the post to the proper level
		var indentImg = link;
		while (indentImg) {
			if (indentImg.tagName=="IMG" && indentImg.src && indentImg.src.match("/blank.gif")) {
				break;
			}
			indentImg = indentImg.previousSibling;
		}
		if (indentImg) {
			var td = otherPostRow.firstChild;
			while (td && td.tagName!="TD") {
				td = td.nextSibling;
			}
			// console.log("indentImg.width="+indentImg.width,td);
			// td.style.paddingLeft = indentImg.getAttribute("width");
			td.style.paddingLeft = indentImg.width+'px';
		}

		parentRow.parentNode.insertBefore(otherPostRow,parentRow.nextSibling);

		// We don't really need the original "subject" row now, we have a new one.
		parentRow.parentNode.removeChild(parentRow);

		// The new subject line is actually a whole table, the first of two tables.
		var subjectTable = td.getElementsByTagName("tr")[0];
		var postTable = td.getElementsByTagName("tr")[1];
		subjectTable.onclick = function(){
			postTable.style.display = ( postTable.style.display == 'none' ? "block" : 'none' );
		};
		subjectTable.style.cursor = 'pointer';

	}

	function emitError(err) {
		console.error("Some problem loading",err);
		// document.location = ...;
	}

	getURLThen(href,handleResponse,emitError);

}

function getURLThen(url,handlerFn,onErrorFn) {
	var req = new XMLHttpRequest();
	req.open("GET", url, true);
	req.send(null);
	req.onreadystatechange = function (aEvt) {
		if (req.readyState == 4) {
			if(req.status == 200) {
				// Got it
				handlerFn(req);
			} else {
				var msg = ("XHR failed with status "+req.status+"\n");
				window.status = msg;
				onErrorFn(req);
				console.warn(msg);
			}
		}
	};
}

function oneXPath(element, xpath) {
	return document.evaluate(xpath, element, null, XPathResult.ANY_TYPE, null).iterateNext();
}


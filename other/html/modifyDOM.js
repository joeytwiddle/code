// Works for Mozilla
// Konqueror worked once I made a few things lower case.
// TODO: Mozilla should include adjacent empty paragraphs in the fold block, because ATM empty gaps are left.

// Right now this is mainly an implementation of folding,
// which works by selecting foldable elements, and attaches a handle for each by detecting the last piece of text in the document.  This should be customizable!

var uniqueIdCount = 0;
function getUniqueId() {
	uniqueIdCount++;
	return "uniqueGeneratedId" + uniqueIdCount;
}

function trim(str) {
	if (str) {
		var re = /(^[\w\n]*|[\w\n]*$)/;
		return str.replace(re,"");
	} else {
		return str;
	}
}

function findPreviousTextNode(elem) {
	while (true) {
		if (elem.previousSibling) {
			elem = elem.previousSibling;
			while (elem.lastChild) {
				elem = elem.lastChild;
			}
		} else if (elem.parentNode) {
			elem = elem.parentNode;
		} else {
			return null;
		}
		// showData("search",elem);
		var value = elem.innerHTML;
		// alert("value = " + value);
		// if ("" + elem == "[object Text]") {
			// return elem;
		// }
		if (value) {
			value = trim(value);
			if (value != "" && value != undefined ) {
				if (elem.hasChildNodes && "" + elem.firstChild == "[object Text]") {
					return elem.firstChild;
				} else {
					return elem;
				}
			}
		}
		// return findPreviousTextNode(elem);
	}
}

function strip(str) {
	if (str && str.length > 20) {
		return str.substring(0,17) + "...";
	} else {
		return str;
	}
}

// function escape(str) {
	// if (str) {
		// var re = /\n/;
		// str = str.replace(re," ");
	// }
	// return str;
// }

function escape(str) {
	if (str) {
		var i;
		while ( ( i = str.indexOf('\n') ) >= 0 ) {
			str = str.substring(0,i) + "\\n" + str.substring(i+1);
		}
	}
	return str;
}

var debugData = "";

function showData(label,elem) {
	var data = label + " = " + elem
		+ "|  .outerHTML = " + escape(strip(elem.outerHTML))
		+ "|  .innerHTML = " + escape(strip(elem.innerHTML))
		+ "|  .type      = " + elem.type
	;
	// alert(data);
	// data = data.replace(/ /,"&nbsp;");
	debugData += data + "\n";
}

function addFoldToBlockQuote(elemToFold) {
	addFoldToBlockQuoteState(elemToFold,true);
}

function addFoldToBlockQuoteState(elemToFold,startClosed) {
	// elemToFold.parentNode.insertBefore('<FONT size="-2" color="white"><INPUT TYPE="button" VALUE="X" ONCLICK="javascript:clearAndFocus(' + elemToFold + ');"></FONT>',elemToFold);
	// if (!elemToFold.id) {
	var foldId = getUniqueId();
	elemToFold.id = foldId + "Block";
	// }
	var imgElem = document.createElement("IMG");
	imgElem.setAttribute("src","http://www.cs.bris.ac.uk/~pclark/bluearrowdown.png");
	imgElem.id = foldId + "Image";
	// alert("imgElem = " + imgElem);
	var handleElem = document.createElement("SPAN");
	handleElem.id = foldId + "Handle";
	// handleElem.setAttribute("onClick",'javascript:toggleFold(this);');
	// handleElem.setAttribute("onclick",'javascript:toggleFold(document.getElementById(\"' + foldId+"Handle" + '\"));');
	handleElem.setAttribute("onclick",'javascript:toggleFoldNamed(\"' + foldId + '\");');
	handleElem.setAttribute("onmouseover",'javascript:mouseOverHandle(this);');
	handleElem.setAttribute("onmouseeout",'javascript:mouseOutHandle(this);');
	handleElem.style.cursor = "n-resize";
	// var labelElem = elemToFold.previousSibling;
	var labelElem = findPreviousTextNode(elemToFold);
	// try { labelElem = elemToFold.previousSibling.previousSibling.previousSibling.firstChild;
	// labelElem = elemToFold.previousSibling.previousSibling.previousSibling.firstChild;
	// showData("labelElem",labelElem);
	showData("parent",labelElem.parentNode);
	// } catch (e) { }
	// if (!labelElem) {
		// labelElem = elemToFold.previousSibling;
	// }
	if (labelElem) {
		labelElem.parentNode.insertBefore(handleElem,labelElem);
		// alert("handleElem = " + handleElem);
		// handleElem.appendChild(document.createTextNode("Handle" + foldId));
		handleElem.appendChild(imgElem);
		// TODO: doesn't work for Opera:
	}
	if (startClosed)
		toggleFoldNamed(foldId);
}

function addFoldsToBlockQuotes() { // TODO: also add the automatic clear on focus if default which are currently hard-wired. BUT ALSO: fork and retain link to non-javascript jumpgate
	// return;
	// plan (yuk!): re-start the list each time; and find the last one done (by id), then do the next, then loop!
	// searchDomAndDo(window.document.body,top.isATextField,top.addClearButton);
	// alert("" + nodes);
	// for (node in nodes) {
	// var lastId = null;
	// while (true) {
	// var nodes = window.document.body.getElementsByTagName("BLOCKQUOTE");
	var tagTypes = [ "ul", "blockquote" ];
	addFoldsToPage(tagTypes,true);
}

function addFoldsToPage(tagTypes,startClosed) {
	for (var t in tagTypes ) {
		var nodes = window.document.body.getElementsByTagName(tagTypes[t]);
		debugData += tagTypes[t] + ": " + nodes.length + "\n";
		for (var i=0;i<nodes.length;i++) {
			try {
			var node = nodes[i];
			// if (node.type == "ul" || node.type == "blockquote") {
				// alert("" + node);
				// try {
					addFoldToBlockQuoteState(node,startClosed);
				// } catch (e) {
					// ERROR
					// alert("" + e);
				// }
				if (i>999) { break; }
				nodes = window.document.body.getElementsByTagName(tagTypes[t]);
			} catch (e) {
				debugData += "ERROR: " + e + "\n";
			}
		}
	}
	// alert(debugData);
}

// Works for Mozilla, Konqueror, Firebird, Galeon
// BUGS: Konqueror sometimes doesn't toggle if mouse hasn't moved off and on since last click.
// var el=event.srcElement if (el.tagName=="INPUT" && el.type=="button") event.srcElement.style.backgroundColor=color
function chopOffEndIfMatched(string, possEnd) {
	var end = string.substring(string.length-possEnd.length);
	if (end==possEnd)
		string = string.substring(0,string.length-possEnd.length);
	return string;
}
function toggleFold(node) {
	var name = node.id;
	name = chopOffEndIfMatched(name,"Handle");
	// alert("got name: "+name);
	toggleFoldNamed(name);
}
function toggleFoldNamed(name) {
	var block = document.getElementById(name+"Block");
	// alert("found block for name=\""+name+"\": "+block);
	var img = document.getElementById(name+"Image");
	// alert(""+name+" -> "+block);
	if (block) {
		if (block.style.display == "none") {
			block.style.display = "";
			img.src="http://www.cs.bris.ac.uk/~pclark/bluearrowdown.png";
		} else {
			block.style.display = "none";
			img.src="http://www.cs.bris.ac.uk/~pclark/bluearrow.png";
		}
	}
}
function mouseOverHandle(node) {
	node.style.color="#ff8000";
	// node.style="text-decoration: underline;"
	// node.style.decoration="bold,underline,overline";
	// alert(node.style.textDecoration);
}
function mouseOutHandle(node) {
	node.style.color="#000080";
	// node.style.text-decoration="none";
}
// toggleFoldNamed('quote');
/* Old for IE:
function clickHandler() {
	var targetId, srcElement, targetElement;
	srcElement = window.event.srcElement;
	if (srcElement.className == "Outline") {
		targetId = srcElement.id + "Block";
		targetElement = document.all(targetId);
		if (targetElement.style.display == "none") {
			targetElement.style.display = "";
		} else {
			targetElement.style.display = "none";
		}
	}
}
document.onclick = clickHandler;
*/


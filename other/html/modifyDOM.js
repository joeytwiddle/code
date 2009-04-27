// TODO: I think we need to do new Image() or something.  In some (all?) browsers, the image gets re-requested on every mouseover!

// TODO: make all open/close "buttons" links so that text-only browsers will let user focus!

// Works for Mozilla
// Konqueror worked once I made a few things lower case.
// TODO: Mozilla should include adjacent empty paragraphs in the fold block, because ATM empty gaps are left.

// TODO: Have it check if the element has already had a fold-handle attached to it, and if so skip it.
// This means the fold generator can be run multiple times, with different conditionals, if desired.

// Right now this is mainly an implementation of folding,
// which works by selecting foldable elements, and attaches a handle for each by detecting the last piece of text in the document.  This should be customizable!



// ======== START general library functions

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
			// if (value != "" && value != undefined ) {
				// if ("" + elem.firstChild == "[object Text]") {
					// return elem.firstChild;
				// } else {
					// return elem;
				// }
			// }
			if (value != "" && value != undefined ) {
				// while (elem.firstChild && elem.firstChild.value == elem.value) {
					// elem = elem.firstChild;
				// }
				// while (elem.firstChild) { // Works but seems dodgy!
				if (elem.firstChild) {
					elem = elem.firstChild;
				}
				/* Avoid becoming trapped inside a link.  Hopefully the element above contains only the link! */
				if (elem.parentNode.tagName=='A')
					elem = elem.parentNode;
				/* I thought this might put us left of the bullets, but it put our handle above :f
				while (elem.parentNode.childNodes.length == 1) {
					elem = elem.parentNode;
				}
				*/
				return elem;
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

// ======== END general library functions



// ======== START functions from folding could be useful elsewhere

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

// ======== END functions from folding could be useful elsewhere



// ======== START functions specific to the page-folding utility.

// Note: folding and unfolding are also called rolling up and unrolling.

// var rollup_image = "http://www.cs.bris.ac.uk/~pclark/bluearrowdown.png";
// var unroll_image = "http://www.cs.bris.ac.uk/~pclark/bluearrowup.png";
var rollup_image = "http://hwi.ath.cx/images/minus.png";
var rollup_image_mouseover = "http://hwi.ath.cx/images/minus-over.png";
var unroll_image = "http://hwi.ath.cx/images/plus.png";
var unroll_image_mouseover = "http://hwi.ath.cx/images/plus-over.png";



//// Entry-level methods (call these)

function addFoldsToBlockQuotes() { // TODO: also add the automatic clear on focus if default which are currently hard-wired. BUT ALSO: fork and retain link to non-javascript jumpgate
	// return;
	// plan (yuk!): re-start the list each time; and find the last one done (by id), then do the next, then loop!
	// searchDomAndDo(window.document.body,top.isATextField,top.addClearButton);
	// alert("" + nodes);
	// for (node in nodes) {
	// var lastId = null;
	// while (true) {
	// var nodes = window.document.body.getElementsByTagName("BLOCKQUOTE");
	// var tagTypes = [ "ul", "blockquote" ];
	var tagTypes = new Array( "ul", "blockquote" );
	addFoldsToPage(tagTypes,false);
}

function addFoldsToPage(tagTypes,startClosed) {
	for (var t in tagTypes ) {
		var nodes = window.document.body.getElementsByTagName(tagTypes[t]);
		debugData += tagTypes[t] + ": " + nodes.length + "\n";
		for (var i=0;i<nodes.length;i++) {
			//// TODO: This try/catch used to exist!
			// try {
			var node = nodes[i];
			// if (node.type == "ul" || node.type == "blockquote") {
				// alert("" + node);
				try {
					addFoldToBlockQuoteState(node,startClosed);
				} catch (e) {
					// ERROR
					// alert("" + e);
				}
				if (i>999) { break; }
				nodes = window.document.body.getElementsByTagName(tagTypes[t]);
			// } catch (e) {
				// debugData += "ERROR: " + e + "\n";
			// }
		}
	}
	// alert(debugData);
}

// Unused?
function addFoldToBlockQuote(elemToFold) {
	addFoldToBlockQuoteState(elemToFold,true);
}

// We should not assume we will always have a handle.
// Some pages there will be something there but it won't be correct!
function addFoldToBlockQuoteState(elemToFold,startClosed) {
	var drawBranches = false;
	// elemToFold.parentNode.insertBefore('<FONT size="-2" color="white"><INPUT TYPE="button" VALUE="X" ONCLICK="javascript:clearAndFocus(' + elemToFold + ');"></FONT>',elemToFold);
	// if (!elemToFold.id) {
	var foldId = getUniqueId();
	elemToFold.id = foldId + "Block";
	// }

	// Occasionally we are asked to fold an empty element.  Rather than annoy
	// the user with a fold with nothing inside, we refuse it.
	if (elemToFold.innerHTML.replace(/[ 	\n\r]*/g,'') == '')
		return;
	// CONSIDER: We could also drop elements whose contents are below a certain
	// size.  mm we might not know the size of embedded things?

	// Create the image element which displays the handle/toggle state:
	var imgElem = document.createElement("IMG");
	imgElem.setAttribute("src",rollup_image);
	imgElem.id = foldId + "Image";
	// alert("imgElem = " + imgElem);
	imgElem.style.paddingLeft=2;
	imgElem.style.paddingRight=8;

	//// Create a clickable span object to wrap the image:
	var handleElem = document.createElement("SPAN");
	handleElem.id = foldId + "Handle";

	// I think these are for /old/ version of Netscape:
	if (handleElem.captureEvents) {
		handleElem.captureEvents(Event.CLICK);
		handleElem.captureEvents(Event.MOUSEOVER);
		handleElem.captureEvents(Event.MOUSEOUT);
	}
	// handleElem.addEventListener("click",handleToggleFoldEvent,false);
	// handleElem.attachEvent("click","javascript:toggleFoldEvent(\""+foldId+"\");");

	// handleElem.setAttribute("onClick",'javascript:toggleFold(this);');
	// handleElem.setAttribute("onclick",'javascript:toggleFold(document.getElementById(\"' + foldId+"Handle" + '\"));');

	//// These standards worked fine for Mozilla and Konqueror, but not for IE:
	handleElem.setAttribute("onclick",'javascript:toggleFoldNamed(\"' + foldId + '\");');
	handleElem.setAttribute("onmouseover",'javascript:rollOntoHandle(\"'+foldId+'\");');
	handleElem.setAttribute("onmouseout",'javascript:rollOffHandle(\"'+foldId+'\");');

	//// It seems IE can only attach a function (not a function call, with params):
	handleElem.onclick=handleToggleFoldEvent;
	handleElem.onmouseover=handleRollOntoEvent;
	handleElem.onmouseout=handleRollOffEvent;

	// handleElem.style.cursor = "n-resize";
	handleElem.style.cursor = "crosshair";

	//// Wait; we are going to move the node, to put the handle in a suitable place.
	//// This is a hack heuristic which served my purposes with blockQuotes:
	// var labelElem = elemToFold.previousSibling;
	var labelElem = findPreviousTextNode(elemToFold);
	// try { labelElem = elemToFold.previousSibling.previousSibling.previousSibling.firstChild;
	// labelElem = elemToFold.previousSibling.previousSibling.previousSibling.firstChild;
	// showData("labelElem",labelElem);
	// Although designed to do the opposite, this avoids folding some of the dodgy parts of the page for which a suitable label is not found.
	// if (trim("" + labelElem.innerHTML) != "") {
		// return;
	// }
	showData("parent",labelElem.parentNode);
	// } catch (e) { }
	// if (!labelElem) {
		// labelElem = elemToFold.previousSibling;
	// }

	//// OK, now we add the new elements to the DOM:
	// TODO: if labelElem is already a link, we should put the +/- image before the link (at the moment, it becomes part of it!)
	// TODO: optionally, make labelElem a clickable link for folding/unfolding
	if (labelElem) {
		labelElem.parentNode.insertBefore(handleElem,labelElem);
		// alert("handleElem = " + handleElem);
		// handleElem.appendChild(document.createTextNode("Handle" + foldId));
		// TODO: doesn't work for Opera:
		handleElem.appendChild(imgElem);
		//// This moves the label inside our fold handle.
		//// Only if it's not a link already - that would be confusing!
		// I don't know why but getElementsByTagName failed however I tried it.
		// if (labelElem.getElementsByTagName('A').length == 0)
		if (labelElem.tagName != 'A')
			handleElem.appendChild(labelElem);
	} else {
		elemToFold.parentNode.insertBefore(handleElem,elemToFold);
	}
	if (drawBranches) {
		// Move the hidable block into a table so we can give it a "branch" on the left
		var newElemToFold = document.createElement("TABLE");
		var row = document.createElement("TR");
		var cell = document.createElement("TD");
		var secondCell = document.createElement("TD");
		var innerTable = document.createElement("TABLE");
		var innerRow = document.createElement("TR");
		var innerCell = document.createElement("TD");
		innerRow.appendChild(innerCell);
		innerTable.appendChild(innerRow);
		innerTable.setAttribute("bgcolor","#ffffff");
		innerTable.setAttribute("height","50");
		innerCell.setAttribute("bgcolor","#000000");
		cell.appendChild(innerTable);
		cell.setAttribute("valign","top");
		secondCell.appendChild(elemToFold.cloneNode(true));
		secondCell.setAttribute("valign","top");
		row.appendChild(cell);
		row.appendChild(secondCell);
		newElemToFold.appendChild(row);
		elemToFold.parentNode.replaceChild(newElemToFold,elemToFold);
		newElemToFold.id = foldId + "Block";
	}

	if (startClosed)
		toggleFoldNamed(foldId);
	// Ouch! alert("" + newElemToFold.innerHTML);
}

//// END Entry-level methods (call these)



// Works for Mozilla, Konqueror, Firebird, Galeon
// BUGS: Konqueror sometimes doesn't toggle if mouse hasn't moved off and on since last click.
// var el=event.srcElement if (el.tagName=="INPUT" && el.type=="button") event.srcElement.style.backgroundColor=color
function chopOffEndIfMatched(string, possEnd) {
	var end = string.substring(string.length-possEnd.length);
	if (end==possEnd)
		string = string.substring(0,string.length-possEnd.length);
	return string;
}
/* Not used but I think it prolly works:
function toggleFold(node) {
	var name = node.id;
	name = chopOffEndIfMatched(name,"Handle");
	// alert("got name: "+name);
	toggleFoldNamed(name);
}
*/

/* These three wrappers were added for IE: */
/* Surely not!  These are the only way it works, and it works for FF. */
function handleToggleFoldEvent(evt) {
	toggleFoldNamed(chopOffEndIfMatched(this.id,"Handle"));
}
function handleRollOntoEvent(evt) {
	rollOntoHandle(chopOffEndIfMatched(this.id,"Handle"));
}
function handleRollOffEvent(evt) {
	rollOffHandle(chopOffEndIfMatched(this.id,"Handle"));
}
// evt = (evt) ? evt : ( (event) ? event : null );
// if (evt) {
// }

function toggleFoldNamed(name) {
	lastFocus = undefined;
	// alert("Toggling fold named: \""+name+"\"");
	var block = document.getElementById(name+"Block");
	// alert("found block for name=\""+name+"\": "+block);
	var img = document.getElementById(name+"Image");
	// alert(""+name+" -> "+block);
	if (block) {
		if (isFolded(block)) {
			openFold(block);
			img.src = rollup_image;
		} else {
			closeFold(block);
			img.src = unroll_image;
		}
	}
}
function isFolded(block) {
	return ( block.style.display == "none" );
}
function openFold(block) {
	block.style.display = "";
	// block.style.visibility = "visible";
}
function closeFold(block) {
	block.style.display = "none";
	// block.style.visibility = "hidden";
}
function mouseOverHandle(node) {
	node.style.color="#ff8000";
	// alert("node.nodeid = " + node.nodeid); undefined :-(
	// node.style="text-decoration: underline;"
	// node.style.decoration="bold,underline,overline";
	// alert(node.style.textDecoration);
}
function mouseOutHandle(node) {
	node.style.color="#000080";
	// node.style.text-decoration="none";
}

var lastFocus;

function rollOntoHandle(foldId) {
	// var img = document.getElementById(foldId+"Image");
	// img.src = rollup_image_mouseover;
	var block = document.getElementById(foldId+"Block");
	var img = document.getElementById(foldId+"Image");
	if (block) {
		// alert("(off) isFolded = " + isFolded(block));
		if (isFolded(block)) {
			if (unroll_image_mouseover) {
				img.src = unroll_image_mouseover;
			}
		} else {
			if (rollup_image_mouseover) {
				img.src = rollup_image_mouseover;
			}
		}
	}
	// setTimeout('handleToggleFoldEvent();',2000);
	lastFocus = foldId;
	// setTimeout('toggleFoldNamed(chopOffEndIfMatched("'+foldId+'","Handle"));',1000);
	setTimeout('processMouseHover();',2000);
	// mouseOverHandle(this);
}

function processMouseHover() {
	/** This worked but I'm not sure it's desirable!  Disabled. **/
	/** It was really only a work-around when our handle images were getting lost inside <A> elements.**/
	/*
	if (lastFocus) {
		toggleFoldNamed(chopOffEndIfMatched(lastFocus,"Handle"));
		lastFocus = undefined;
	}
	*/
}

function rollOffHandle(foldId) {
	// var img = document.getElementById(foldId+"Image");
	// img.src = rollup_image_mouseover;
	var block = document.getElementById(foldId+"Block");
	var img = document.getElementById(foldId+"Image");
	if (block) {
		// alert("isFolded = " + isFolded(block));
		if (isFolded(block)) {
			img.src = unroll_image;
		} else {
			img.src = rollup_image;
		}
	}
	// mouseOutHandle(this); 
	// setTimeout(''); // Doesn't cancel the existing timer =/
	lastFocus = undefined;
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

// ======== END functions specific to the page-folding utility.

/*alert("<P>modifyDOM.js loaded.</P>");*/


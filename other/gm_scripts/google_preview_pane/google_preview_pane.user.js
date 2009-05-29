// ==UserScript==
// @name           Google Preview Pane
// @namespace      joeytwiddle
// @description    Loads Google results in a Preview Pane on hover
// @include        http://google.*/search?*
// @include        http://www.google.*/search?*
// ==/UserScript==

var resultsBlock = unsafeWindow.document.getElementById("res");

var table = unsafeWindow.document.createElement("TABLE");
var row = unsafeWindow.document.createElement("TR");
var leftCell = unsafeWindow.document.createElement("TD");
var rightCell = unsafeWindow.document.createElement("TD");

leftCell.width = '50%';
rightCell.width = '50%';
// leftCell.height = window.innerHeight * 0.70;
// rightCell.height = window.innerHeight * 0.75;
resultsBlock.style.height = (window.innerHeight * 0.70) + 'px';

// leftCell.scrollable = true;
// rightCell.scrollable = true;
// leftCell.style.overflow = 'auto';
resultsBlock.style.overflow = 'auto';

row.appendChild(leftCell);
row.appendChild(rightCell);
table.appendChild(row);

resultsBlock.parentNode.insertBefore(table,resultsBlock);
leftCell.appendChild(resultsBlock);

// leftCell.style.backgroundColor = '#eeeeee';

var iframe = unsafeWindow.document.createElement('IFRAME');
iframe.width = '100%';
iframe.height = window.innerHeight * 0.70;
rightCell.appendChild(iframe);

iframe.style.backgroundColor = '#eeeeee';

var lastFocus = null;

function checkFocus() {
	if (lastFocus) {
		iframe.src = lastFocus.href;
	}
}

function helloMouse(evt) {
	var node = evt.target;
	window.status = "Over "+node;
	if (node.tagName=="A" && node.className=="l") {
		lastFocus = node;
		setTimeout(checkFocus,1000);
	}
}

function goodbyeMouse(evt) {
	var node = evt.target;
	window.status = "Out "+node;
	if (node.tagName=="A" && node.className=="l") {
		lastFocus = null;
	}
}

document.body.addEventListener('mouseover',helloMouse,false);
document.body.addEventListener('mouseout',goodbyeMouse,false);


// ==UserScript==
// @name           Show Link URL in Corner
// @namespace      SLUC
// @description    When hovering a link, some browsers (cough Firefox) present no information about what action clicking will perform.  This script pops up the target href/URL in the corner of the page, like Chrome does.
// @include        *
// ==/UserScript==

// TODO: Like Chrome's built-in popup, hide it when mouse is in that corner.

var timer = null;
var targetElem = null;

var urlDisplayer = null;

function actOn(elem) {
	return (elem.tagName == 'A');
}

function enteredElement(evt) {
	var elem = evt.target || evt.sourceElement;
	if (actOn(elem)) {
		if (timer) {
			clearTimeout(timer);
		}
		timer = setTimeout(hoverDetected,30);
		targetElem = elem;
	}
}

function leftElement(evt) {
	var elem = evt.target || evt.sourceElement;
	if (actOn(elem)) {
		if (timer) {
			clearTimeout(timer);
			timer = null;
			targetElem = null;
			hideDisplayer();
		}
	}
}

function hideDisplayer() {
	if (urlDisplayer) {
		urlDisplayer.style.display = 'none';
	}
}

function hoverDetected() {
	if (!urlDisplayer) {
		urlDisplayer = document.createElement('div');
		urlDisplayer.style.position = 'fixed';
		urlDisplayer.style.left = '0px';
		urlDisplayer.style.bottom = '0px';
		urlDisplayer.style.backgroundColor = '#eeeeee';
		urlDisplayer.style.color = 'black';
		urlDisplayer.style.padding = '1px 4px';
		urlDisplayer.style.border = '';
		urlDisplayer.style.borderTop = '1px solid black';
		urlDisplayer.style.borderRight = '1px solid black';
		urlDisplayer.style.fontSize = '13px';
		urlDisplayer.style.zIndex = 100;
		document.body.appendChild(urlDisplayer);
	}
	urlDisplayer.textContent = targetElem.href;
	urlDisplayer.style.display = 'block';
}

document.body.addEventListener('mouseover',enteredElement,true);
document.body.addEventListener('mouseout',leftElement,true);


// ==UserScript==
// @name           track_history
// @namespace      noggieb
// @description    Presents a list/tree of the browsing path you took to reach the current page.  An advanced in-page Back button.
// @include        *
// ==/UserScript==

// As well as displaying the tree of other pages, we could display *all* the
// links from the previous page, to allow fast switching.  (Or better: all the
// links with the same xpath?).

// I guess our database could be:
// All documents this session (URL), with a list of all their links (and their
// names+levels!), to which we may add extra links to if we later find an
// unknown link with this as referer.
// That doesn't yet contain tree information, although enough information to
// build it.
// Oh we should really include backlinks - the document.referrer of each URL.

// We could provide the feature that the history thingy will switch between
// the offered links using hidden/shown IFrames, a bit like tags.

function drawHistoryTree(html) {

}

var historyBlock = document.createElement("DIV");

historyBlock.style.position = 'fixed';
historyBlock.style.left = 4;
historyBlock.style.top = 4;
historyBlock.style.backgroundColor = 'white';
historyBlock.style.border = 'solid 1px black';
historyBlock.style.padding = '6px';

var html = "";

drawHistoryTree(html);

historyBlock.innerHTML = "Hello<BR/>This is history<BR/>";

document.body.appendChild(historyBlock);

// FAIL
// historyBlock.onclick = 'javascript:alert(self);self.parentNode.removeChild(self);';


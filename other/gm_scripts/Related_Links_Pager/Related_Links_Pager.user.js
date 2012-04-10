// ==UserScript==
// @name           Related Links Pager
// @namespace      RLP
// @description    Navigate sideways!  When you click a link, related links on the current page are carried with you.  They can be accessed from a pager on the target page, so you won't have to go back in your browser.
// @include        http://*/*
// @include        https://*/*
// ==/UserScript==

var delayBeforeRunning = 2000;
var minimumGroupSize = 5;
var maximumGroupSize = 110;       // TODO: should really be based on length of final URL, which some webservers restrict ("Bad Request")
var groupLinksByClass = true;

var enableOnCtrlClick = true;
var enableOnShiftClick = false;   // Allows you to avoid the script when needed
var enableOnRightClick = false;

var keepNavigationHistory = false;   // When off, paging is not added to browser history.  The back button will return you to the page before you started paging.
var leaveHashUrlsAlone = true;    // Many sites use # these days for their own purposes - this avoids the risk of breaking them.
var forceTravel = true;           // Attempt to fix loss of #data when clicking thumbnails on YouTube.  Failed to fix it!

var verbose = true;                     // Extra logging for debugging
var showGroupCountInLinkTitle = true;   // Updates hovered links' titles to show number of siblings.
var highlightLinkGroups = true;         // Change the background color of links in the current group on hover.
// var highlightColor         = "rgba(130,200,255,0.2)"; // very light blue
// var thisLinkHighlightColor = "rgba(130,200,255,0.1)"; // light blue
var highlightColor         = null;
var thisLinkHighlightColor = "rgba(130,230,255,0.3)";


// == CHANGELOG ==
// 2012-03-22 Added highlighting and further heuristics.
// 2012-02-07 Bugfixes and more heuristics.
// 2012-01-30 Fixed repeat rewrite bug by checking for &sib as well as #sib.
// 2012-01-28 Fixed close button positioning in Firefox.
// 2012-01-28 Restricted related links to those with the same CSS class.
//       BUG: We still group unrelated (indistinguishable) links on Wikipedia!
// 2012-01-28 Blacklisted some buggy situations.

// == NOTES ==

// You can avoid this script either by clicking a link before it runs or by
// activating the desired link with the keyboard instead of the mouse.

// TODO: The # method seems to work but could present a BUG on some sites.  It
// is sometimes needed but it's horrible.  Use alternatives where possible.  In
// Chrome that could be localStorage *if* the followed sibling is on the same
// domain.  In Firefox Greasemonkey we can use GM_set/getValue().

// TODO: Pager can only go sideways.  It should also offer ability to go "up"
// to the page that contained all the links in the current group.

// BUG: Does not work through redirects.

// CONSIDER: Would it be better to use a normal CGI '&' instead of '#' ?
// Should we compress the data to survive through more webservers?

// TODO: Do not load pager if it is redundant (i.e. if we can already see the
// sibling links on the current page!).

// DONE: removeRedirection only runs on links originally in the page, not
// those added later, e.g. by Ajax when refining a Google search.  This may be
// the cause of some issues.  (Google says "This url is too long.")



setTimeout(function(){



// We consider related links, or "siblings", to be those on the current page
// with the same DOM path as the clicked link.

function getXPath(node) {
	var parent = node.parentNode;
	if (!parent) {
		return '';
	}
	var siblings = parent.childNodes;
	var totalCount = 0;
	var thisCount = -1;
	for (var i=0;i<siblings.length;i++) {
		var sibling = siblings[i];
		if (sibling.nodeType == node.nodeType) {
			totalCount++;
		}
		if (sibling == node) {
			thisCount = totalCount;
			break;
		}
	}
	return getXPath(parent) + '/' + node.nodeName.toLowerCase() + (totalCount>1 ? '[' + thisCount + ']' : '' );
}

// What can I say?  I loooove favicons!
function addFaviconToLinkObviouslyIMeanWhyWouldntYou(link) {

  var host = link.href.replace(/^[^\/]*:\/\//,'').replace(/\/.*$/,'');
  var img = document.createElement('IMG');
  // img.src = 'http://'+host+'/favicon.ico';

  var alwaysUseGoogle = false;
  var imageExtensions = ( alwaysUseGoogle ? [] : ['gif','jpg','png','ico'] );
  function tryExtension(evt) {
    var ext = imageExtensions.pop();
    if (ext) {
      img.src = 'http://'+host+'/favicon.'+ext;
    } else {
      img.title = "Failed to find favicon for "+host;
      img.src = 'http://www.google.com/s2/favicons?domain=' + host; // Google's cache will sometimes provide a favicon we would have missed, e.g. if the site uses .png instead of .ico.  Thanks to NV for suggesting this, and to Google.
      // @consider We could also generate an md5sum and request a gravatar, which might simply allow human recognition of repeats.
      img.removeEventListener('error',tryExtension,true);
    }
  }
  img.addEventListener('error',tryExtension,true);
  tryExtension();

  img.title = ''+host;
  img.style.border = '0';
  img.style.paddingRight = '4px';
  img.style.width = '1.0em';
  img.style.height = '1.0em';
  // Favicon image elements can be hidden until they have fully loaded
  // img.style.display = 'none';
  img.addEventListener('load',function(){ img.style.display = ''; },false);

  link.parentNode.insertBefore(img,link);
}

if (!this.GM_addStyle) {
  this.GM_addStyle = function(css) {
    var s = document.createElement("style");
    s.type = 'text/css';
    s.innerHTML = css;
    document.getElementsByTagName("head")[0].appendChild(s);
  };
}



// Collect siblings when the user clicks a link, and pass them forward to the
// target page in a hash package.

function collectLinksInSameGroupAs(clickedLink) {
  // We remove the numbers from the XPath
  var seekXPath = getXPath(clickedLink).replace(/\[[0-9]*\]/g,'');
  // NOTE: We could search for matches with document.query - it might be faster.
  var links = document.getElementsByTagName("A");
  var collected = [];
  for (var i=0;i<links.length;i++) {
    var link = links[i];
    if (groupLinksByClass && link.className != clickedLink.className) {
      continue;
    }
    var xpath = getXPath(link).replace(/\[[0-9]*\]/g,'');
    if (xpath == seekXPath) {
      if (link.textContent) {   // ignore if no title
        collected.push(link);
      }
    }
  }
  return collected;
}

function isSuitable(link) {

  if (link.tagName != "A") {
    return;
  }

  if (link.href.indexOf("#siblings=")>=0 || link.href.indexOf("&siblings=")>=0) {
    // This is already a prepared link!  Probably created by the pager.
    // No need to modify it.
    return;
  }
  if (link.protocol.indexOf(/*"http") != 0)*/ "javascript:") == 0) {
    // We should not add #s to javascript: links but it seems to work ok on ftp:// (FF)
    return;
  }

  // Ignore links which are simply anchor into the current page
  // Note that .href gives the whole URL, so we check getAttribute("href")
  if (link.getAttribute("href") && link.getAttribute("href").charAt(0) == '#') {
    return;
  }

  // What about links to #s in other pages?  I decided in the end to leave them
  // alone by default (preserve the existing hash string).
  // Altering # strings can break the way some sites use # strings, and can
  // prevent the browser from scrolling to the anchor.
  // You can force appending of siblings package to hash strings using '&' if
  // desired by disabling leaveHashUrlsAlone.
  // Perhaps in these "emergency" circumstances, we should append with '&' or
  // '?' *outside* the hash.  (Which will no doubt break some sites, but
  // perhaps fewer!)
  if (link.hash && leaveHashUrlsAlone) {
    return;
  }

  //// === Some sites complain about long URLs or unexpected strings. ===

  // Some Google search pages complain about our long URLs.
  var googleWillComplain =
    link.host.indexOf("google")>=0 &&
      (    link.href.indexOf("?q=")>=0
        || link.href.indexOf("&q=")>=0
        || link.href.indexOf("url?")>=0
      );
  // TODO: There are more of these cases on Google!  (When earlier rewriting failed?)

  var isYouTubePagerLink
    link.host.indexOf("www.youtube.") == 0
    && link.href.indexOf("/all_comments?") >= -1;
  var youtubeWillComplain = isYouTubePagerLink;

  var siteWillComplain = googleWillComplain || youtubeWillComplain;

  if (siteWillComplain) {
    return;
  }

  return true;

}

function checkClick(evt) {
  var elem = evt.target || evt.sourceElement;
  // GM_log("Intercepted click event on "+getXPath(elem));

  // Do not interfere with Ctrl-click or Shift-click or right-click (usually open-in-new-window/tab)
  if ((evt.ctrlKey && !enableOnCtrlClick) || (evt.shiftKey && !enableOnShiftClick) || (evt.button>0 && !enableOnRightClick)) {
    return;
  }

  if (elem.tagName == "A") {
    var link = elem;

    if (!isSuitable(link)) {
      return;
    }

    // GM_log("User clicked on link: "+link.href);
    // Collect other links matching this one:
    var siblings = collectLinksInSameGroupAs(link);
    // Convert from links to records:
    siblings = siblings.map(function(link) {
      return [link.textContent, link.href];
    });
    if (siblings.length <= minimumGroupSize) {
      // No point.  Give the user a clean location bar for a change.  ;)
      return;
    }
    if (siblings.length > maximumGroupSize) {
      // It would be dangerous to proceed!
      return;
    }

    // GM_log("Found "+siblings.length+" siblings for the clicked link.");
    var sibsEncoded = encodeURIComponent(JSON.stringify(siblings));
    // If the link already had a #, we append our data as an & parameter, and cross our fingers.
    var appendChar = ( link.href.indexOf('#')>=0 ? '&' : '#' );
    if (appendChar == '&' || link.hash) {
      if (verbose) {
        GM_log("Appending to existing hash with "+appendChar+": "+link.hash);
      }
      // Note: If it was a normal # to an anchor then we have probably broken
      // it!  In that case we should either not append, or perhaps we can
      // append, but force movement to the correct anchor anyway (which may be
      // on the current page, or after navigation!).
    }
    var targetURL = link.href + appendChar + "siblings="+sibsEncoded;

    if (verbose) {
      GM_log("Rewriting link "+getXPath(link));
      GM_log(" url: "+link.href);
      GM_log("with: "+targetURL);
    }

    // Force travel to the new URL.  (Don't wait for the page to handle the
    // click - some sites e.g. YouTube will throw away our hash-data!)
    if (forceTravel) {
      // We only do this for normal left-clicks.
      if (!evt.ctrlKey && !evt.shiftKey && evt.button==0) {
        document.location = targetURL;
        evt.preventDefault();
        return false;
      }
    }

    // Instead of pushing the browser to the magic URL, just change the link and see what happens.
    link.href = targetURL;

    // We need this on Google search result pages, or we end up following
    // feedback/tracking redirection links, which throw away our hash data!
    link.removeAttribute('onmousedown');
    // Thanks to http://userscripts.org/scripts/review/57679

  }
}

document.body.addEventListener("click",checkClick,true);
document.body.addEventListener("mousedown",checkClick,true);
document.body.addEventListener("mouseup",checkClick,true);



// If we have been passed a hash package of siblings, present the lovely pager.

function createRelatedLinksPager(siblings) {
  var hashPart = new RegExp("#.*");
  var seekURL = document.location.href.replace(hashPart,'');
  // var currentIndex = siblings.indexOf(seekURL);   // No because the list contains records not urls!
  var currentIndex = -1;
  for (var i=0;i<siblings.length;i++) {
    var record = siblings[i];
    if (record[1].replace(hashPart,'') == seekURL) {
      currentIndex = i;
      break;
    }
  }
  // GM_log("Current index: "+currentIndex);
  if (currentIndex == -1) {
    // This should be unlikely to happen!
    GM_log("Odd, I could not find: "+seekURL+" in the siblings list.");
    // But it does happen occasionally.
    // One time by navigating to Wikipedia's main page by clicking the top-left logo.
    // Don't return.  Show the list anyway!
    // return;
  }
  var pager = document.createElement("div");
  pager.id = "linkGroupPager";
  GM_addStyle("#linkGroupPager { position: fixed; top: 5%; right: 5%; "+
    "z-index: 10; background: white; color: black; border: 1px solid black; "+
    "padding: 5px; font-size: 100%; text-align: center; "+
    "max-width: 40%; max-height: 90%; overflow: auto; } "+
    ".linkGroupPagerList { text-align: left; }"
  );

  function maybeHost(link) {
    if (link.host != document.location.host) {
      return " ("+link.host+")";
    }
  }

  function createLinkFromRecord(record, text) {
    var link = document.createElement("A");
    link.textContent = text;
    var appendChar = ( record[1].indexOf('#')>=0 ? '&' : '#' );
    link.href = record[1] + appendChar + 'siblings=' + encodedList;
    link.title = "Previous: "+record[0]+maybeHost(link);
    link.onclick = function(evt){
      if (!keepNavigationHistory) {
        document.location.replace(this.href);
        evt.preventDefault();
      }
    };
  }

  if (currentIndex > 0) {
    var leftRecord = siblings[currentIndex-1];
    var leftLink = createLinkFromRecord(leftRecord, "<<");
    leftLink.title = "Previous: "+leftLink.title;
    pager.appendChild(leftLink);
  }

  // var pagerButton = document.createTextNode(" Pager ");
  var pagerButton = document.createElement("span");
  // pagerButton.textContent = " Pager ";
  pagerButton.textContent = " Page "+(currentIndex+1)+" of "+siblings.length+" ";
  pagerButton.addEventListener("click",function(evt) {
    pageList.style.display = ( pageList.style.display == 'none' ? '' : 'none' );
  },false);
  pagerButton.style.cursor = 'pointer';
  pager.appendChild(pagerButton);

  if (currentIndex < siblings.length-1) {
    var rightRecord = siblings[currentIndex+1];
    var rightLink = createLinkFromRecord(rightRecord, ">>");
    rightLink.title = "Next: "+link.title;
    pager.appendChild(rightLink);
  }

  var closeButton = document.createElement("span");
  closeButton.textContent = "[X]";
  closeButton.style.cursor = 'pointer';
  closeButton.style.paddingLeft = '5px';
  closeButton.onclick = function() { pager.parentNode.removeChild(pager); };
  pager.appendChild(closeButton);

  // We could create this lazily, but why not immediately? :P
  var pageList = document.createElement("div");
  pageList.className = "linkGroupPagerList";
  for (var i=0;i<siblings.length;i++) {
    pageList.appendChild(document.createElement("br"));
    pageList.appendChild(document.createTextNode(""+(i+1)+". "));
    var record = siblings[i];
    var text = record[0] || record[1];   // use address if no title
    var link = createLinkFromRecord(record, text);
    if (record[1] == seekURL) {
      // Replace link with just a bold span
      var span = document.createElement("span");
      span.style.fontWeight = 'bold';
      span.textContent = link.textContent;
      link = span;
    }
    pageList.appendChild(link);
    addFaviconToLinkObviouslyIMeanWhyWouldntYou(link);
  }
  pageList.style.display = 'none';
  pager.appendChild(pageList);

  // GM_log("Created pager: "+pager);
  document.body.appendChild(pager);
}

if (document.location.hash && document.location.hash.indexOf("siblings=")>=0) {
  var hash = document.location.hash;
  // GM_log("Noticed hash: "+hash);
  var encodedList = hash.replace(/.*[#&]siblings=([^#]*)/,'$1');
  // GM_log("Got encoded: "+encodedList);
  var siblings = JSON.parse(decodeURIComponent(encodedList));
  createRelatedLinksPager(siblings);
  // document.location.hash = ".";   // Creates an unwanted history step
  document.location.replace('#');   // #. breaks google search results pages, tho we rarely page through them.
}



// Optional: Show link's siblings on hover

if (highlightLinkGroups) {

  function seekLinkInAncestry(startElem) {
    var node = startElem;
    while (node) {
      if (node.tagName == "A") {
        return node;
      }
      node = node.parentNode;
    }
    return startElem;
  }

  var list = [];

  function highlightList() {
    if (verbose) {
      GM_log("Highlighting "+list.length+" elements.");
    }
    for (var i=0;i<list.length;i++) {
      list[i].oldBackgroundColor = list[i].style.backgroundColor;
      if (highlightColor) {
        list[i].style.backgroundColor = highlightColor;
      }
      /*
      list[i].oldBorderLeft = list[i].style.borderLeft;
      list[i].oldMarginLeft = list[i].style.marginLeft;
      list[i].oldBorderRight = list[i].style.borderRight;
      list[i].oldMarginRight = list[i].style.marginRight;
      list[i].oldBorderTop = list[i].style.borderTop;
      list[i].oldMarginTop = list[i].style.marginTop;
      list[i].oldBorderBottom = list[i].style.borderBottom;
      list[i].oldMarginBottom = list[i].style.marginBottom;
      list[i].style.border = "2px dashed "+highlightColor;
      list[i].style.margin = "-2px";
      */
    }
  }

  function clearList() {
    for (var i=0;i<list.length;i++) {
      list[i].style.backgroundColor = list[i].oldBackgroundColor;
      /*
      list[i].style.marginLeft = list[i].oldMarginLeft;
      list[i].style.borderLeft = list[i].oldBorderLeft;
      list[i].style.marginRight = list[i].oldMarginRight;
      list[i].style.borderRight = list[i].oldBorderRight;
      list[i].style.marginTop = list[i].oldMarginTop;
      list[i].style.borderTop = list[i].oldBorderTop;
      list[i].style.marginBottom = list[i].oldMarginBottom;
      list[i].style.borderBottom = list[i].oldBorderBottom;
      */
    }
    list.length = 0;
  }

  document.body.addEventListener("mouseover",function(evt) {
    var link = seekLinkInAncestry(evt.target || evt.sourceElement);
    if (isSuitable(link)) {
      clearList();
      list = collectLinksInSameGroupAs(link);
      if (list.length>=minimumGroupSize && list.length<maximumGroupSize) {
        highlightList();
        if (thisLinkHighlightColor) {
          link.style.backgroundColor = thisLinkHighlightColor;
        }
        // link.style.border = "2px solid "+highlightColor;
        if (showGroupCountInLinkTitle && !link.doneAppendGroupsize) {
          link.doneAppendGroupsize = true;
          link.title += " ("+list.length+" in group)";
        }
      }
    }
  },true);
  document.body.addEventListener("mouseout",function(evt) {
    var link = seekLinkInAncestry(evt.target || evt.sourceElement);
    if (isSuitable(link)) {
      clearList();
    }
  },true);

}



},delayBeforeRunning);


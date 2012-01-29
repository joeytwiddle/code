// ==UserScript==
// @name           Related Links Pager
// @namespace      RLP
// @description    Navigate sideways!  When you click a link, related links on the current page are carried with you.  They can be accessed from a pager on the target page, so you won't have to go back in your browser.
// @include        *
// @exclude        https://*/*
// ==/UserScript==

var delayBeforeRunning = 1000;
var minimumGroupSize = 5;
var maximumGroupSize = 40;
var groupLinksByClass = true;


// CHANGELOG
// 2012-01-28 Fixed close button positioning in Firefox.
// 2012-01-28 Restricted related links to those with the same CSS class.
//       BUG: We still group unrelated (indistinguishable) links on Wikipedia!
// 2012-01-28 Blacklisted some buggy situations.

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



setTimeout(function(){

// This is http://userscripts.org/scripts/review/57679
// We need it on Google search result pages, or we end up following Google
// feedback/tracking links, which then throw away our hash package!
function removeRedirection() {
  handle(document);

  function handle(doc) {
    var links = document.evaluate('descendant::a', doc, null, 7, null);
    for (var i = 0; i < links.snapshotLength; i++){
      links.snapshotItem(i).removeAttribute('onmousedown');
    }
  }

  function registerPageHandler() {
    window.AutoPagerize.addFilter(function(pages) {
      pages.forEach(function(page) {
        handle(page);
      });
    });
  }
  if (window.AutoPagerize) {
    registerPageHandler();
  } else {
    window.addEventListener('GM_AutoPagerizeLoaded', registerPageHandler, false);
  }
}
removeRedirection();



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
  var seekXPath = getXPath(clickedLink).replace(/\[[0-9]*\]/g,'');
  // NOTE: We could do this with document.query - it might be faster.
  var links = document.getElementsByTagName("A");
  var collected = [];
  for (var i=0;i<links.length;i++) {
    var link = links[i];
    var xpath = getXPath(link).replace(/\[[0-9]*\]/g,'');
    if (groupLinksByClass && link.className != clickedLink.className) {
      continue;
    }
    if (xpath == seekXPath) {
      if (link.textContent) {   // ignore if no title
        var record = [link.textContent, link.href];
        collected.push(record);
      }
    }
  }
  return collected;
}

function checkClick(evt) {
  var elem = evt.target;
  // GM_log("Intercepted click event on "+getXPath(elem));
  if (elem.tagName == "A") {
    var link = elem;

    if (link.href.indexOf("#siblings=") >= 0) {
      // This is already a prepared link!  Probably created by the pager.
      // No need to modify it.
      return;
    }
    if (link.protocol.indexOf(/*"http") != 0)*/ "javascript:") == 0) {
      // We should not add #s to javascript: links but it seems to work ok on ftp:// (FF)
      return;
    }
    // Some Google search pages complain about our long URLs.
    var siteWillComplain = link.host.indexOf("google")>=0 &&
      link.href.indexOf("?q=")>=0 || link.href.indexOf("&q=")>=0;
    if (siteWillComplain) {
      return;
    }

    // GM_log("User clicked on link: "+link.href);
    // Collect other links matching this one:
    var siblings = collectLinksInSameGroupAs(link);
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
      GM_log("Appending to existing hash with "+appendChar+": "+link.hash);
    }
    var targetURL = link.href + appendChar + "siblings="+sibsEncoded;

    // A bit aggressive: overrides AJAX or other JS friendliness.
    /*
    document.location = targetURL;
    evt.preventDefault();
    return false;
    */
    // Instead of pushing the browser to the magic URL, change the link and see what happens.
    GM_log("Rewriting link "+getXPath(link));
    GM_log(" url: "+link.href);
    GM_log("with: "+targetURL);
    // link.hash = "siblings="+sibsEncoded;
    link.href = targetURL;

  }
}

document.body.addEventListener("click",checkClick,true);
document.body.addEventListener("mousedown",checkClick,true);
document.body.addEventListener("mouseup",checkClick,true);



// If we have been passed a hash package of siblings, present the lovely pager.

function createRelatedLinksPager(siblings) {
  var seekURL = document.location.href.replace(new RegExp("#.*"),'');
  // var currentIndex = siblings.indexOf(seekURL);   // No because the list contains records not urls!
  var currentIndex = -1;
  for (var i=0;i<siblings.length;i++) {
    var record = siblings[i];
    if (record[1] == seekURL) {
      currentIndex = i;
      break;
    }
  }
  // GM_log("Current index: "+currentIndex);
  if (currentIndex == -1) {
    // This should be unlikely to happen!
    GM_log("Could not find: "+seekURL);
    GM_log("in list: "+siblings);
    return;
  }
  var pager = document.createElement("div");
  pager.id = "linkGroupPager";
  GM_addStyle("#linkGroupPager { position: fixed; top: 5%; right: 5%; "+
    "z-index: 10; background: white; color: black; border: 1px solid black; "+
    "padding: 5px; font-size: 100%; text-align: center; } "+
    ".linkGroupPagerList { text-align: left; }"
  );

  function maybeTitle(link) {
    if (link.host != document.location.host) {
      return " ("+link.host+")";
    }
  }

  if (currentIndex > 0) {
    var leftLink = document.createElement("A");
    var leftRecord = siblings[currentIndex-1];
    leftLink.textContent = "<<";
    leftLink.href = leftRecord[1] + '#siblings=' + encodedList;
    leftLink.title = "Previous: "+leftRecord[0]+maybeTitle(leftLink);
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
    var rightLink = document.createElement("A");
    var rightRecord = siblings[currentIndex+1];
    rightLink.textContent = ">>";
    rightLink.href = rightRecord[1] + '#siblings=' + encodedList;
    rightLink.title = "Next: "+rightRecord[0]+maybeTitle(rightLink);
    pager.appendChild(rightLink);
  }

  var closeButton = document.createElement("span");
  closeButton.textContent = "[X]";
  closeButton.style.cursor = 'pointer';
  // closeButton.style.paddingLeft = '5px';
  closeButton.onclick = function() { pager.parentNode.removeChild(pager); };
  pager.appendChild(closeButton);

  // We could create this lazily, but why not immediately? :P
  var pageList = document.createElement("div");
  pageList.className = "linkGroupPagerList";
  for (var i=0;i<siblings.length;i++) {
    pageList.appendChild(document.createElement("br"));
    pageList.appendChild(document.createTextNode(""+(i+1)+". "));
    var record = siblings[i];
    var link = document.createElement("A");
    if (record[1] == seekURL) {
      link = document.createElement("span");
      link.style.fontWeight = 'bold';
    }
    link.textContent = record[0] || record[1];   // use address if no title
    link.href = record[1] + '#siblings=' + encodedList;
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
}


},delayBeforeRunning);


// ==UserScript==
// @name           Related Links Pager
// @namespace      RLP
// @description    Navigate sideways!  When you click a link, similar links on the current page are carried with you.  They can be accessed from a pager on the target page.
// @include        *
// @exclude        https://*/*
// ==/UserScript==



// TODO: The # method seems to work but could present a BUG on some sites.  It
// is sometimes needed but it's horrible.  Use alternatives where possible.  In
// Chrome that could be localStorage *if* the followed sibling is on the same
// domain.  In Firefox Greasemonkey we can use GM_set/getValue().

// TODO: Pager can only go sideways.  It should also offer ability to go "up"
// to the page that contained all the siblings.

// BUG: Does not work through redirects.



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



// Collect siblings when the user clicks a link, and pass them forward to the
// target page in a hash package.

function collectLinksMatchingXPath(seekXPath) {
  var links = document.getElementsByTagName("A");
  var collected = [];
  for (var i=0;i<links.length;i++) {
    var link = links[i];
    var xpath = getXPath(link).replace(/\[[0-9]*\]/g,'');
    if (xpath == seekXPath) {
      var record = [link.textContent, link.href];
      collected.push(record);
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
    // GM_log("User clicked on link: "+link.href);
    var xpath = getXPath(link).replace(/\[[0-9]*\]/g,'');
    // Collect other links matching this one:
    var siblings = collectLinksMatchingXPath(xpath);
    if (siblings.length < 2) {
      // No point.  Give the user a clean location bar for a change.  ;)
      return;
    }
    // GM_log("Found "+siblings.length+" siblings for the clicked link.");
    var sibsEncoded = encodeURIComponent(JSON.stringify(siblings));
    var targetURL = link.href + "#siblings="+sibsEncoded;
    // GM_log("Redirecting to: "+targetURL);
    document.location = targetURL;
    evt.preventDefault();
    return false;
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
  if (true || !this.GM_addStyle) {
    this.GM_addStyle = function(css) {
      var s = document.createElement("style");
      s.type = 'text/css';
      s.innerHTML = css;
      document.body.appendChild(s);
    };
  }
  GM_addStyle("#linkGroupPager { position: fixed; top: 5%; right: 5%; "+
    "z-index: 10; background: white; color: black; border: 1px solid black; "+
    "padding: 5px; font-size: 100%; text-align: center; } "+
    ".linkGroupPagerList { text-align: left; }"
  );

  var closeButton = document.createElement("span");
  closeButton.textContent = "[X]";
  closeButton.style.float = 'right';
  closeButton.style.cursor = 'pointer';
  closeButton.style.paddingLeft = '5px';
  closeButton.onclick = function() { pager.parentNode.removeChild(pager); };
  pager.appendChild(closeButton);

  if (currentIndex > 0) {
    var leftLink = document.createElement("A");
    var leftRecord = siblings[currentIndex-1];
    leftLink.textContent = "<";
    leftLink.title = "Previous: "+leftRecord[0];
    leftLink.href = leftRecord[1] + '#siblings=' + encodedList;
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
    rightLink.textContent = ">";
    rightLink.title = "Next: "+rightRecord[0];
    rightLink.href = rightRecord[1] + '#siblings=' + encodedList;
    pager.appendChild(rightLink);
  }

  // We could create this lazily, but why not immediately :P
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
    link.textContent = record[0];
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
  var encodedList = hash.replace(/.*siblings=([^#]*)/,'$1');
  // GM_log("Got encoded: "+encodedList);
  var siblings = JSON.parse(decodeURIComponent(encodedList));
  createRelatedLinksPager(siblings);
}




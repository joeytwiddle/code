// ==UserScript==
// @name          Google Link Preview
// @namespace     http://loucypher.wordpress.com/
// @include       http://www.google.tld/search?*
// @include       http://www.google.tld/custom?*
// @include       http://www.google.tld/search%3F*
// @include       http://www.google.tld/custom%3F*
// @include       http://www.google.com/blogsearch*
// @include       http://*/*
// @include       https://*/*
// @description   Adds Clusty.com-like magnifiers on web and news search results to preview a link in a frame
// ==/UserScript==

// Last updated: 2007-08-03

window.setTimeout(function() {

  function getXPathSnapshot(XPath) {
    return document.evaluate(XPath, document, null, 6, null);
  }

  var thisSite = document.location.protocol+"//"+document.location.host+"/";

  var relativeExpr = "starts-with(@href,'/')";
  var localExpr = "starts-with(@href,'"+ thisSite +"')";
  var remoteExpr = "(not(relativeExpr) and not(localExpr))";
  var localCondition = "[" + relativeExpr + " or " + localExpr + "]";
  var remoteCondition = "[" + remoteExpr + "]";

  var XPath;
  // This script has special support for:some sites
  // TODO: Rather than have strict URL matching, try all XPaths, and then
  // choose a set which has an appropriate number?
  if (location.href.match(/google/)) {
    if (location.href.match(/\/blogsearch\?/)) {
      XPath = "//a[starts-with(@id, 'p-') and count(img)=0]";
    } else {
      XPath = "//a[@class='l']";
    }
  } else {
    XPath = "//a[not( . = '' )]";
  }

  var links = getXPathSnapshot(XPath);
  //if (!links.snapshotLength) return;
  if (links.snapshotLength<=0 || links.snapshotLength>=50) {
    links = getXPathSnapshot(XPath + localCondition);
  }
  if (links.snapshotLength<=0 || links.snapshotLength>=50) {
    links = getXPathSnapshot(XPath + remoteCondition);
  }
  if (links.snapshotLength<=0 || links.snapshotLength>=50) {
    links = getXPath("(//a)[1]");
  }

  window.document.title = "+"+links.snapshotLength+" "+window.document.title;

  for (var i = 0; i < links.snapshotLength; i++) {
    var link = links.snapshotItem(i);
    if(link.hasAttribute("onmousedown")) link.removeAttribute("onmousedown");

    var pLink = link.parentNode.insertBefore(document.createElement("a"),
                                             link.nextSibling)
    pLink.href = link.href;
    pLink.title = "preview";
    pLink.style.marginLeft = "0.2em";
    pLink.style.marginRight = "0.2em";
    pLink.style.width = "0.1em";
    pLink.style.height = "0.1em";

    // start closure for iframe and thereby pLink
    (function(){

      var iframe = null;

      pLink.addEventListener("click", function(e) {
        e.preventDefault();

        if (iframe == null) {
          iframe = link.parentNode.insertBefore(document.createElement("iframe"), pLink.nextSibling);
          iframe.style.display = "none";
          iframe.width = "90%";
          iframe.height = "250";
          iframe.appendChild(document.createTextNode("")); // Initial empty body
        }

        var pOpen = "data:image/gif;base64,\
  R0lGODlhDAAMAMIGAKZZWatkYcqfjsyikM2mk9KumfLsyPLsyCH5BAEKAAcALAAAAAAM\
  AAwAAAMneEcRpFCFokqIi8Ly4MWfhB1hFnGgZgkj4wjAMEZDPEO1fB%2F5zg8JADs%3D";
        var pClosed = "data:image/gif;base64,\
  R0lGODlhDAAMAMIGAMwAAKtkYc2Tk8yikM2mk9KumQAAAAAAACH5BAEKAAcALAAAAAAM\
  AAwAAAMyeEcRpFCFokqIix5xytvHtQHcJZDiKAQnR2gqCU1VizEsKWPtYEM%2F307Bgf\
  gGGMxgkAAAOw%3D%3D";
        this.firstChild.src = this.firstChild.src == pOpen ? pClosed : pOpen;
        this.title = this.title == "preview"?"close preview":"preview";
        this.nextSibling.style.display = this.nextSibling
                                             .style.display == "none" ? "" : "none";
        if (!this.nextSibling.hasAttribute("src"))
          this.nextSibling.src = this.previousSibling.href;
      }, false);

      var img = pLink.appendChild(document.createElement("img"));
      img.alt = "preview";
      img.border = "none";
      img.align = "absmiddle";
      img.src = "data:image/gif;base64,\
  R0lGODlhDAAMAMIGAKZZWatkYcqfjsyikM2mk9KumfLsyPLsyCH5BAEKAAcALAAAAAAM\
  AAwAAAMneEcRpFCFokqIi8Ly4MWfhB1hFnGgZgkj4wjAMEZDPEO1fB%2F5zg8JADs%3D";

    })(); // end closure for iframe, pLink

  } // end for i < links.snapshotLength

  window.document.title = window.document.title.substring(window.document.title.indexOf(' '));

});


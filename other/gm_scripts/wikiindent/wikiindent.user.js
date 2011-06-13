// ==UserScript==
// @name           WikiIndent
// @namespace      joeytwiddle
// @include        *wiki*
// @include        http://www.buzztard.com/*
// @include        http://encyclopediadramatica.com/*
// @include        http://www.wormus.com/leakytap/*
// @description    Four tools for MediaWiki sites: hide the sidebar (with toggle), floats the Table of Contents in the corner for quick navigation, indents sub-sections to make the layout clearer, overline all headings instead of underlining.
// ==/UserScript==

//// TODO: It would be better to perform some things before the content loads
//// (e.g. style rules like indentation, floating toc and hidden sidebar).
//// Chrome could do this using:
// @run-at         document-start

//// Features:
var toggleSidebar = true;
var makeTableOfContentsFloat = true;
var indentSubBlocks = true;
var fixUnderlinesToOverlines = true;


/* TODO: As we scroll the page, light up the "current" section in the TOC.
 *
 * BUG: One occasional problem with the TOC is when it is taller than the
 *      window!  (I usually work around this by zooming out (reducing font
 *      size), but perhaps we can use CSS overflow to solve it properly.)
*/

/* Changelog
 * 23/3/2011 - Added Chrome compatibility.
*/

function log(x) {
	if (this.GM_log) {
		this.GM_log(x);
	} else if (this.console && console.log) {
		console.log(x);
	} else {
		window.status = ""+x;
		// alert(x);
	}
}

// For bookmarklets:
if (typeof GM_addStyle == "undefined") {
	GM_addStyle = function(css) {
		var doc = document;
		var head, style;
		head = doc.getElementsByTagName("head")[0];
		if (!head) { return; }
		style = doc.createElement("style");
		style.type = "text/css";
		style.innerHTML = css;
		head.appendChild(style);
	};
}

if (typeof GM_getValue == 'undefined') {
	GM_log("WikiIndent: Adding fallback implementation of GM_getValue");
	GM_getValue = function(key,def) {
		return def;
	};
}

function doIt() {



	//// Feature #1 : Hide the sidebar.  Fullsize the content.

	// Toggle the sidebar by clicking the "page background" (empty space outside
	// the main content).  Sometimes clicking the content background is enough.

	if (toggleSidebar) {

		var content = document.getElementById("content") || document.getElementById("column-content");
		var column1 = document.getElementById("column-one") || document.getElementById("panel") || document.getElementById("mw-panel") || document.getElementById("jq-interiorNavigation");
		var toToggle = [ document.getElementById("page-base"), document.getElementById("siteNotice"), document.getElementById("head") ];
		var cac = document.getElementById("p-cactions");
		var cacOldHome = ( cac ? cac.parentNode : null );

		function toggleWikipediaSidebar(evt) {
			// We don't want to act on all clicked body elements (notably not the WP
			// image).  I detected two types of tag we wanted to click.
			if (!evt || evt.target.tagName == "UL" || evt.target.tagName == "DIV") {
				if (evt)
					evt.preventDefault();
				// GM_log("evt="+evt);
				// if (evt) GM_log("evt.target.tagName="+evt.target.tagName);
				/* We put the GM_setValue calls on timers, so they won't slow down the rendering. */
				if (column1) {
					if (column1.style.display == '') {
						// column-one contains a lot of things we want to hide
						column1.style.display = 'none';
						content.oldMarginLeft = content.style.marginLeft;
						content.style.marginLeft = 0;
						for (var i in toToggle) {
							if (toToggle[i]) { toToggle[i].style.display = 'none'; }
						}
						// but one of them we want to preserve
						// (the row of tools across the top):
						if (cac)
							column1.parentNode.insertBefore(cac,column1.nextSibling);
						setTimeout(function(){
							GM_setValue("sidebarVisible",false);
						},300);
					} else {
						column1.style.display = '';
						content.style.marginLeft = content.oldMarginLeft;
						for (var i in toToggle) {
							if (toToggle[i]) { toToggle[i].style.display = ''; }
						}
						if (cac && cacOldHome)
							cacOldHome.appendChild(cac); // almost back where it was :P
						setTimeout(function(){
							GM_setValue("sidebarVisible",true);
						},300);
					}
				}
			}
		}

		// log("column1="+column1+" and content="+content);
		if (column1 && content) {
			document.body.addEventListener('click',toggleWikipediaSidebar,false);
		} else {
			log("Did not have column1 "+column1+" or content "+content); // @todo Better to warn or error?
		}

		if (!GM_getValue("sidebarVisible",true)) {
			toggleWikipediaSidebar();
		}

		// TODO: Make a toggle button for it!

		// Fix for docs.jquery.com:
		/*
		var j = document.getElementById("jq-primaryContent");
		if (j) {
			j.style.setAttribute('display', 'block');
			j.style.setAttribute('float', 'none');
			j.style.setAttribute('width', '100%');
		}
		*/
		GM_addStyle("#jq-primaryContent { display: block; float: none; width: 100%; }");

	}



	//// Feature #2: Make Table of Contents float

	if (makeTableOfContentsFloat) {

		/* @consider If the TOC has a "Hide/Show" link ("button") then we could
		 * fire that instead of changing opacity.
		 */

		// document.getElementById('column-one').appendChild(document.getElementById('toc'));

		function createFader(toc) {

			var timer = null;

			// BUG: this didn't stop the two fades from conflicting when the user wiggles the mouse to start both!
			function resetTimeout(fn,ms) {
				if (timer) {
					clearTimeout(timer);
				}
				setTimeout(fn,ms);
			}

			function fadeElement(elem,start,stop,speed,current) {
				if (current == null)
					current = start;
				if (speed == null)
					speed = (stop - start) / 8;
				if (Math.abs(current+speed-stop) > Math.abs(current-stop))
					current = stop;
				else
					current = current + speed;
				elem.style.opacity = current;
				if (current != stop)
					resetTimeout(function(){fadeElement(elem,start,stop,speed,current);},50);
			}

			var listenElement = toc;
			// var listenElement = toc.getElementsByTagName('TD')[0];
			var focused = false;
			var visible = false;
			listenElement.addEventListener('mouseover',function(){
				if (!visible)
					setTimeout(function(){ if (focused) { visible=true; fadeElement(toc,0.4,1.0,0.2); } },10);
				focused = true;
			},false);
			listenElement.addEventListener('mouseout',function(){
				if (visible)
					setTimeout(function(){ if (!focused) { visible=false; fadeElement(toc,1.0,0.2,-0.1); } },10);
				focused = false;
			},false);

		}


		function tryTOC() {

			// Find the table of contents element:
			var toc = document.getElementById("toc")   /* MediaWiki */
					 || document.getElementsByClassName("table-of-contents")[0]   /* BashFAQ */
					 || document.getElementsByClassName("toc")[0]   /* LeakyTap */
					 || document.getElementsByClassName("wt-toc")[0];   /* Wikitravel */

			if (toc) {

				// toc.style.backgroundColor = '#eeeeee';
				// alert("doing it!");
				toc.style.position = 'fixed';
				toc.style.right = '16px';
				// toc.style.top = '16px';
				// A healthy gap from the top allows the user to access things fixed in the top right of the page, if they can scroll finely enough.
				toc.style.top = '24px';
				toc.style.left = '';
				toc.style.bottom = '';
				toc.style.zIndex = '5000';
				// fadeElement(toc,1.0,0.4);
				toc.style.opacity = 0.3;

				createFader(toc);

				return true;

			}

			return false;

		}

		// We try to act before # anchor position occurs, but if not we
		// fire later in case the toc loads slowly.
		if (!tryTOC()) {
			setTimeout(tryTOC,400);
		}

	}



	// In case you have * in your includes, only continue for pages which have
	// "wiki" before "?" in the URL, or who have both toc and content elements.
	var isWikiPage = document.location.href.split("?")[0].match("wiki")
		|| ( document.getElementById("toc") && document.getElementById("content") );

	if (!isWikiPage)
		return;



	//// Feature #3 : Indent the blocks so their tree-like structure is visible

	if (indentSubBlocks) {

		function indent(tag) {
			// By targetting search we avoid indenting any blocks in left-hand-column (sidebar).
			var whereToSearch = document.getElementById('bodyContent') || document.getElementById('content') || document;
			var elems = whereToSearch.getElementsByTagName(tag);
			if (elems.length == 1)
				return;
			for (var i=0;i<elems.length;i++) {
				var elem = elems[i];
				/* Don't fiddle with main heading, siteSub, or TOC. */
				if (elem.className == 'firstHeading')
					continue;
				if (elem.id == 'siteSub')
					continue;
				if (elem.textContent == 'Contents')
					continue;
				// var newChild = document.createElement('blockquote');
				//// Unfortunately blockquotes tend to indent too much!
				// var newChild = document.createElement('DIV');
				var newChild = document.createElement('UL'); // UL works better with my Folding script, but we must not do this to the TOC!
				newChild.style.marginLeft = '1.0em';
				var toAdd = elem.nextSibling;
				while (toAdd && toAdd.tagName != tag) {
					var next = toAdd.nextSibling;
					newChild.appendChild(toAdd);
					toAdd = next;
				}
				elem.parentNode.insertBefore(newChild,elem.nextSibling);
				// GM_log("Placed "+newChild+" after "+elem);
			}
		}

		setTimeout(function(){
			indent("H1"); indent("H2"); indent("H3"); indent("H4"); indent("H5"); indent("H6");
		},200);

	}



	//// Feature #4: Change underlined headings to overlined headings.

	if (fixUnderlinesToOverlines) {

		GM_addStyle("h1, h2, h3, h4, h5, h6 { border-bottom: 0px solid #AAAAAA; }");
		GM_addStyle("h1, h2, h3, h4, h5, h6 { border-top: 1px solid #AAAAAA; }");
		// Do not use "text-decoration: underline;" - it makes text look like links.

	}




} // end doIt


// setTimeout(doIt,2000);
doIt();


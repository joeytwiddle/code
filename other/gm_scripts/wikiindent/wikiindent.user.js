// ==UserScript==
// @name           WikiIndent
// @namespace      joeytwiddle
// @include        *wiki*
// @include        http://www.buzztard.com/*
// @include        http://encyclopediadramatica.com/*
// @description    Four tools for MediaWiki sites.  Indents sub-sections to make the layout clearer.  Hides the sidebar (toggle by clicking the background).  Changes (sub)heading underlines to overlines.  Floats the Table of Contents on the right of the page for quick navigation.
// ==/UserScript==

//// Features:
var hideSidebar = true;
var indentSubBlocks = true;
var fixUnderlinesToOverlines = true;
var makeTableOfContentsFloat = true;


/* TODOS
As we scroll the page, light up the "current" section in the TOC.
*/

function log(x) {
	if (this.GM_log) {
		this.GM_log(x);
	} else if (this.console && console.log) {
		console.log(x);
	} else {
		// alert(x);
	}
};

function doIt() {



	//// Feature #0 : Hide the sidebar.  Fullsize the content.

	if (hideSidebar) {

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
					} else {
						column1.style.display = '';
						content.style.marginLeft = content.oldMarginLeft;
						for (var i in toToggle) {
							if (toToggle[i]) { toToggle[i].style.display = ''; }
						}
						if (cac && cacOldHome)
							cacOldHome.appendChild(cac); // almost back where it was :P
					}
				}
			}
		}

		log("column1="+column1+" and content="+content);
		if (column1 && content) {
			document.body.addEventListener('click',toggleWikipediaSidebar,false);
		} else {
			log("Did not have column1 "+column1+" or content "+content); // @todo Better to warn or error?
		}

		toggleWikipediaSidebar();

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



	//// Feature #1 : Indent the blocks so their tree-like structure is visible

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

		indent("H1"); indent("H2"); indent("H3"); indent("H4"); indent("H5"); indent("H6");

	}


	// Feature #2: Change underlined headings to overlined headings.

	if (fixUnderlinesToOverlines) {

		// For bookmarklets:
		if (typeof GM_addStyle == "undefined") {
			function GM_addStyle(css) {
				var doc = document;
				var head, style;
				head = doc.getElementsByTagName("head")[0];
				if (!head) { return; }
				style = doc.createElement("style");
				style.type = "text/css";
				style.innerHTML = css;
				head.appendChild(style);
			}
		}

		GM_addStyle("h1, h2, h3, h4, h5, h6 { border-bottom: 0px solid #AAAAAA; }");
		GM_addStyle("h1, h2, h3, h4, h5, h6 { border-top: 1px solid #AAAAAA; }");
		// Do not use "text-decoration: underline;" - it makes text look like links.

	}



	//// Feature #3: Make Table of Contents float

	if (makeTableOfContentsFloat) {

		// CONSIDER TODO: If the TOC has a "Hide/Show" link ("button") then we
		// should just fire this instead of changing opacity.

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
					setTimeout(function(){ if (focused) { fadeElement(toc,0.4,1.0,0.2); visible=true; } },10);
				focused = true;
			},false);
			listenElement.addEventListener('mouseout',function(){
				if (visible)
					setTimeout(function(){ if (!focused) { fadeElement(toc,1.0,0.2,-0.1); visible=false; } },10);
				focused = false;
			},false);

		}

		setTimeout(function(){

			// Find the table of contents element:
			var toc = document.getElementById("toc")   /* MediaWiki */
					 || document.getElementsByClassName("table-of-contents")[0];   /* BashFAQ */

			if (toc) {
				// toc.style.backgroundColor = '#eeeeee';
				// alert("doing it!");
				toc.style.position = 'fixed';
				toc.style.right = '16px';
				toc.style.top = '16px';
				toc.style.left = '';
				toc.style.bottom = '';
				toc.style.zIndex = '5000';
				// fadeElement(toc,1.0,0.4);
				toc.style.opacity = 0.3;
				createFader(toc);
			}

		},2000);

	}



} // end doIt


// setTimeout(doIt,2000);
doIt();


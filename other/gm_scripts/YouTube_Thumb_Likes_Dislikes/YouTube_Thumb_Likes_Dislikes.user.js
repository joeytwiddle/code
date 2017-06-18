// ==UserScript==
// @name           YouTube Thumb Likes Dislikes
// @namespace      YTTLD
// @description    Adds the likes/dislikes light-saber to YouTube thumbnails, so you can avoid watching crap videos.  Activates when mouse passes over a thumbnail.
// @version        1.0.8
// @downstreamURL  http://userscripts.org/scripts/source/126705.user.js
// @include        http://youtube.com/*
// @include        https://youtube.com/*
// @include        http://*.youtube.com/*
// @include        https://*.youtube.com/*
//// For thumbnails when google search presents video results - but probably fails from XSS in Chrome
// @include        http://www.google.co.uk/search?q=*&tbm=vid&*
// @include        https://www.google.co.uk/search?q=*&tbm=vid&*
// @grant          GM_log
// @grant          GM_addStyle
// @grant          GM_xmlhttpRequest
// ==/UserScript==

// Some YouTube videos get lots of views even though they are rubbish, maybe
// even Rick Rolls.  This may be because they have some enticing screenshot
// and title that was part of an older or existing trend.

// However, it is fairly easy to filter these out, if we can see the video has
// significantly more dislikes than likes!



// == Configuration == //

var addCountsToTooltip = false;           // Shows likes/dislikes when
                                          // (re-)hovering a thumbail/link.

var addCountsToThumbnail = true;          // Display likes/dislikes next to the
                                          // thumbnail.

var addLightSaberBarToThumbnail = true;   // The lightsaber is the green/red
                                          // bar, easier than reading numbers!

var addVideoDescriptionToTooltip = true;  // The "tooltip" is actually the
                                          // alt/title of a link, which
                                          // browsers display on hover.

var spamYouTube = false;   // Automatically looks up data for ALL the thumbnails on the page.
                           // Normally it waits for a mouseover before doing a lookup.



// BUG: Hover detection is very poor.  It appears to only trigger when we move the mouse over the description text or uploader's name, but not when we hover over the actual link title!
//      The easiest thing for the user is just to pass their mouse slowly up/down the text next to a thumbnail, to trigger the data lookup.



// Some other scripts like this:
// http://userscripts.org/scripts/search?q=youtube+likes+dislikes&submit=Search

function findLinkAbove(elem) {
	while (elem && typeof elem.tagName === 'string' && elem.tagName.toUpperCase() !== "A") {
		elem = elem.parentNode;
	}
	return elem;
}

function isSuitableLink(elem) {
	var link = findLinkAbove(elem);
	return (
		link
		&& typeof link.tagName === 'string'
		&& link.tagName.toUpperCase() === "A"
		&& link.pathname.indexOf("/watch") >= 0
		// But not if it's the same page:
		&& link.href.replace(/#.*/,'') !== document.location.href.replace(/#.*/,'')
	);
}

function fakeGetElementById(root, id) {
	var allElems = root.getElementsByTagName("*");
	for (var i=0;i<allElems.length;i++) {
		if (allElems[i].id == id) {
			return allElems[i];
		}
	}
	return null;
}

// Sometimes the element hovered is not the one we want to modify to show extra info.  We need to find a nearby element with a title attribute we can add to.
function findClosestLinkElem(orig) {
	var startFrom = orig;
	while (startFrom != null) {
		if (startFrom.tagName == "A") {
			return startFrom;
		}
		var descendents = startFrom.getElementsByTagName("A");
		for (var i=0;i<descendents.length;i++) {
			if (true) {
				return descendents[i];
			}
		}
		startFrom = startFrom.parentNode;
	}
	return orig;
}

// Display likes/dislikes on links to other videos
function lookupLikesDislikes(link) {
	if (link && !link.doneLikesDislikes) {
		link.doneLikesDislikes = true;

		function gotTargetPage(response) {
			var content = response.responseText;
			//GM_log("GOT CONTENT LENGTH: "+content.length);
			if (content) {
				var lePage = document.createElement("div");
				lePage.innerHTML = content;
				//console.log("GOT lePage:", lePage);
				//unsafeWindow.lePage = lePage;
				var infoElem = lePage.getElementsByClassName("watch-likes-dislikes")[0];
				infoElem = infoElem || lePage.getElementsByClassName("video-extras-likes-dislikes")[0]; // Oct 2012
				infoElem = infoElem || lePage.getElementsByClassName("like-button-renderer")[0]; // Oct 2015
				//console.log("GOT INFOELEM: "+infoElem);

				// Find suitable element for adding tooltip info.
				var elemWithTitle = findClosestLinkElem(link);
				// On sidebar thumbnails this tends to be the containing <A> rather
				// than the <span> holding the video title, which is what I usually
				// mouseover!  However on YT's front page it does find the title.

				// CONSIDER: We might want to add our extra elements to .stat.view-count or .yt-lockup-meta-info (on channel pages) or perhaps a clone of that element.

				if (infoElem) {

					// Old:
					var infoText = infoElem.textContent.trim();

					// New:
					var newLikedButton = infoElem.querySelector(".like-button-renderer-like-button-unclicked");
					if (newLikedButton) {
						var newDislikedButton = infoElem.querySelector(".like-button-renderer-dislike-button-unclicked");
						infoText = newLikedButton.textContent + " likes, " + newDislikedButton.textContent + " dislikes.";
					}

					if (addCountsToTooltip) {
						elemWithTitle.title += " ("+infoText+")";
					}

					if (addCountsToThumbnail) {
						var span = document.createElement("div");
						span.className = "stat";
						// On channel pages, the stat rule does not apply, so we apply it ourselves:
						span.style.fontSize = '11px';
						span.appendChild(document.createTextNode(infoText));
						link.appendChild(span);
					}

				}

				if (addLightSaberBarToThumbnail) {

					// Pictures are easier to read than words:
					var lightSaber = lePage.getElementsByClassName("watch-sparkbars")[0];
					lightSaber = lightSaber || lePage.getElementsByClassName("video-extras-sparkbars")[0]; // Oct 2012
					if (lightSaber) {
						link.appendChild(lightSaber);
						if (document.location.pathname === "/watch") { // not on search results
							// It often falls on the line below the thumbnail, aligned left
							// Here is a dirty fix to align it right, with all the other info.
							//lightSaber.style.marginLeft = '124px';
							// Well none of the other info is aligned right for me now, it's all aligned left.
							// Just give it a small gap on the right
							lightSaber.style.marginRight = '10px';
						}
						// Bars are unneccessarily wide on search results pages, so:
						if (lightSaber.clientWidth > 150) {
							lightSaber.style.maxWidth = '120px';
						}
					} else {
						elemWithTitle.title += " [No likes/dislikes available]";
					}

				}

				if (addCountsToTooltip || addLightSaberBarToThumbnail) {
					// On channel pages, the extra elements we add are hidden by this rule, so we grow it larger
					GM_addStyle('.yt-ui-ellipsis-2 { max-height: 5.5em; } .webkit .yt-ui-ellipsis-2 { -webkit-line-clamp: 5; }');
				}

				if (addVideoDescriptionToTooltip) {
					// Uncaught TypeError: Object #<HTMLDivElement> has no method 'getElementById'
					// var descrElem = lePage.getElementById("watch-description-text");
					var descrElem = fakeGetElementById(lePage, "watch-description-text");
					if (descrElem) {
						elemWithTitle.title += " ::: " + descrElem.textContent.trim();
					}

					// I also want to add the date to the thumbnail stats
					var uploadedDateSrcElem = lePage.getElementsByClassName('watch-time-text')[0];
					var uploadedDateText = uploadedDateSrcElem.textContent;
					uploadedDateText = uploadedDateText.replace(/^(Uploaded|Published) on /, 'on ');
					var uploadedDateNewElem = document.createElement("span");
					uploadedDateNewElem.textContent = " " + uploadedDateText;
					var targetElem = link.getElementsByClassName("attribution")[0];
					targetElem.appendChild(uploadedDateNewElem);
				}

			}
		}

		//console.log("Requesting: "+link.href);
		GM_xmlhttpRequest({
			method: "GET",
			url: link.href,
			headers: {
				// "Reason": "I want to display watch-likes-dislikes, watch-sparkbars and watch-description-text by this thumbnail"
			},
			onload: gotTargetPage,
			onerror: function(err){
				// Cannot ""+err.  Chrome once gave me: Uncaught TypeError: Function.prototype.toString is not generic
				GM_log("Got error requesting YT page: "+link.href);
				GM_log(err);
			}
		});

	}
}

// Hover detection is a fiddle, because a mouseout event is fired on the A element immediately after the mouseover, presumably due to all the things inside it.
// DONE: We don't cancel hover if we are firing on a child of the hoveredElem.

var hoveredElem = null;
var hoverTimer = null;

// Including self
function hasAncestor(node, seekNode) {
	while (node != null) {
		if (node == seekNode) {
			return true;
		}
		node = node.parentNode;
	}
	return false;
}

function watchForHover(evt) {
	var target = evt.target || evt.srcElement;
	var link = findLinkAbove(target);
	// Do nothing (don't start a new timeout or cancel existing timeout) if we are now on a child of the hoveredElem.
	if (hoveredElem && hasAncestor(target, hoveredElem)) {
		return;
	}
	if (isSuitableLink(link)) {
		clearTimeout(hoverTimer);
		hoveredElem = link;
		hoverTimer = setTimeout(function(){
			lookupLikesDislikes(hoveredElem);
			hoveredElem = null;
			hoverTimer = null;
		}, 1000);
	} else {
		//GM_log("Cancelling hover on "+hoveredElem+" because of mousemove on "+target.outerHTML);
		clearTimeout(hoverTimer);
		hoveredElem = null;
		hoverTimer = null;
	}
}

document.body.addEventListener("mousemove", watchForHover, false);


if (spamYouTube) {
	function queueLink(link, when) {
		GM_log("In "+(when/1000|0)+" seconds I will do "+link);
		setTimeout(function(){
			lookupLikesDislikes(link);
		}, when);
	}
	var ls = document.getElementsByTagName("A");
	var lastUrlDone = "";
	var num = 0;
	for (var i=0;i<ls.length;i++) {
		var link = ls[i];
		if (link.href != lastUrlDone && isSuitableLink(link)) {
			num++;
			queueLink(link, 1000 * Math.pow(1.2 + 1.2*num, 1.7) );
			lastUrlDone = link.href;
		}
	}
}

// TODO:
// On search results pages, why does feature not active/work when we hover over
// thumbs, only when we hover over the text/title links?

// BUG TODO: Not all information is appearing on Feed pages, guess it's
// appending to the wrong place.


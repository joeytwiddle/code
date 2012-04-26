// ==UserScript==
// @name           YouTube Thumb Likes Dislikes
// @namespace      YTTLD
// @description    Adds the likes/dislikes light-saber to YouTube thumbnails, so you can avoid watching crap videos.  Activates when mouse passes over a thumbnail.
// @include        http://youtube.com/*
// @include        https://youtube.com/*
// @include        http://*.youtube.com/*
// @include        https://*.youtube.com/*
// For thumbnails when google is presenting video search - may not work!
// @include        https://www.google.co.uk/search?q=*&tbm=vid&*
// ==/UserScript==

// Some YouTube videos get lots of views even though they are rubbish, maybe
// even Rick Rolls.  This is probably because they have some faked screenshot
// and title that was part of an older or existing trend.

// However, it is fairly easy to filter these out, if we see the video has
// significantly more dislikes than likes!

var spamYouTube = false;   // Automatically looks up all thumbnails on the page.
                           // Otherwise, waits for mouseovers before doing lookup.

// Some other scripts like this:
// http://userscripts.org/scripts/search?q=youtube+likes+dislikes&submit=Search

function suitableLink(link) {
	return (link.tagName == "A" && link.pathname.indexOf("/watch")==0);
}

// Display likes/dislikes on links to other videos
function checkLikesDislikes(evt) {
	var target = evt.target || evt.srcElement;
	if (suitableLink(target) && !target.doneLikesDislikes) {
		target.doneLikesDislikes = true;

		function gotTargetPage(response) {
			var content = response.responseText;
			// GM_log("GOT CONTENT: "+content);
			if (content) {
				var lePage = document.createElement("div");
				lePage.innerHTML = content;
				var infoElem = lePage.getElementsByClassName("watch-likes-dislikes")[0];
				// GM_log("GOT INFOELEM: "+infoElem);
				if (infoElem) {
					var infoText = infoElem.textContent.trim();

					var elemWithTitle = null;

					// Now, target is not actually the element with the title attribute.
					// It was actually a <span dir="ltr" class="title"> which held
					// the full name in its title attribute...

					var descendents = target.getElementsByTagName("*");
					for (var i=0;i<descendents.length;i++) {
						if (descendents[i].title) {
							elemWithTitle = descendents[i];
							break;
						}
					}

					if (elemWithTitle) {
						elemWithTitle.title += " ("+infoText+")";
						// Optional: effectively gives a title to the thumbnail image too:
						//target.title = elemWithTitle.title;
						// Not convinced we want that.  :P
					} else {
						// Not what we expected, but lets give it a bash anyway!
						target.title += " ("+infoText+")";
					}

					// OK adding to the title sucks balls anyway - the browser
					// (Chrome) does not update the displayed title until
					// mouseoff/over again. :f

					var span = document.createElement("div");
					span.className = "stat";
					span.appendChild(document.createTextNode(infoText));
					target.appendChild(span);

					var lightSaber = lePage.getElementsByClassName("watch-sparkbars")[0];
					if (lightSaber) {
						// Pictures are easier to read than words:
						target.appendChild(lightSaber);
						// It often falls on the line below the thumbnail, aligned left
						// Here is a dirty fix to align it right, with all the other info.
						if (document.pathname === "/watch") { // not on search results
							lightSaber.style.marginLeft = '124px';
						}
						// Bars are unneccessarily wide on search results pages, so:
						if (lightSaber.clientWidth > 150) {
							lightSaber.style.maxWidth = '120px';
						}
					}

				}
			}
		}

		GM_xmlhttpRequest({
			method: "GET",
			url: target.href,
			onload: gotTargetPage
		});

	}
}
document.body.addEventListener("mouseover",checkLikesDislikes,true);

if (spamYouTube) {
	function queueLink(link,when) {
		GM_log("In "+(when/1000|0)+" seconds will do "+link);
		setTimeout(function(){
			checkLikesDislikes({target:link});
		},when);
	}
	var ls = document.getElementsByTagName("A");
	var lastUrlDone = "";
	var num = 0;
	for (var i=0;i<ls.length;i++) {
		var link = ls[i];
		if (link.href != lastUrlDone && suitableLink(link)) {
			num++;
			queueLink(link, 1000 * Math.pow(1.2 + 1.2*num,1.7) );
			lastUrlDone = link.href;
		}
	}
}

// TODO:
// On search results pages, why does feature not active/work when we hover over
// thumbs, only when we hover over the text/title links?


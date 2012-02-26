// ==UserScript==
// @name           YouTube Thumb Likes Dislikes
// @namespace      YTTLD
// @description    Adds the likes/dislikes light-saber to YouTube thumbnails, so you can avoid watching crap videos.  Activates when mouse passes over a thumbnail.
// @include        http://youtube.com/*
// @include        https://youtube.com/*
// @include        http://*.youtube.com/*
// @include        https://*.youtube.com/*
// ==/UserScript==

// THANKS A LOT YOUTUBE FOR LEAVING THIS FOR ME TO DO.  WE ASKED FOR THIS
// FEATURE FOR AGES BUT YOU DID NOTHING SO YOUR USERS ARE STILL GETTING
// REGULARLY RICK-ROLLED.  LETS HOPE YOU ENJOY THE EXTRA SPAM YOUR SERVERS WILL
// NOW RECEIVE!  (Yeah, I stopped smoking this week, so I'm a bit on edge.)

// Some other scripts like this:
// http://userscripts.org/scripts/search?q=youtube+likes+dislikes&submit=Search

// Display likes/dislikes on links to other videos
function checkLikesDislikes(evt) {
	var target = evt.target || evt.srcElement;
	if (target.tagName == "A" && target.pathname.indexOf("/watch")==0 && !target.doneLikesDislikes) {
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
					var info = infoElem.textContent.trim();

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
						elemWithTitle.title += " ("+info+")";
						// Optional: effectively gives a title to the thumbnail image too:
						//target.title = elemWithTitle.title;
						// Not convinced we want that.  :P
					} else {
						// Not what we expected, but lets give it a bash anyway!
						target.title += " ("+info+")";
					}

					// OK adding to the title sucks balls anyway - the browser
					// (Chrome) does not update the displayed title until
					// mouseoff/over again. :f

					var lightSaber = lePage.getElementsByClassName("watch-sparkbars")[0];
					if (lightSaber) {
						// Pictures are easier to read than words:
						target.appendChild(lightSaber);
						// DONE: Bars are unneccessarily wide on search results pages.
						if (lightSaber.clientWidth > 150) {
							lightSaber.style.maxWidth = '120px';
						}
					} else {
						var span = document.createElement("span");
						span.textContent = info;
						span.className = "stat";
						target.appendChild(span);
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

// TODO:
// On search results pages, why does feature not active/work when we hover over
// thumbs, only when we hover over the text/title links?


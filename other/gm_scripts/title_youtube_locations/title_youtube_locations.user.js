// ==UserScript==
// @name           Title Youtube Locations
// @namespace      TYTLs
// @description    Forces all YouTube pages to put the video title in the Location Bar
// @include        http://*.youtube.*/*
// @include        http://youtube.*/*
// For thumbnails when google is presenting video search:
// @include        https://www.google.co.uk/search?q=*&tbm=vid&*
// ==/UserScript==

// TODO: In case for some reason YouTube or another script redirects us to
// another URL which has lost the title param we added, we will re-run forever.
// To avoid this we should have a fallback.  E.g. if the title we want to set
// is the same as the last one we did set (via GM_set/getValue) then do not try
// again.
// This has never happened so far.  :)

setTimeout(function(){
	if (document.location.pathname == "/watch") {

		var title = document.getElementsByTagName("h1")[0].textContent
			|| document.title.replace(/^YouTube - /,'')
			|| null;

		if (title)
			title = title.replace(/ /g,'_').replace(/^[\r\n_]*/,'').replace(/[\r\n_]*$/,''); // "_"s paste better into IRC, since " "s become "%20"s which are hard to read.  The second and third parts trim "_"s and newlines from the start and end of the string.

		if (title) {
			if (!document.location.hash) {
				document.location.replace(document.location.href + '#' + title); // Does not alter browser history
				// document.location.hash = title; // Crashes Chrome less often
			}
		}
	}
},5000); // This is what really stops the crashing!



// == Reduce font size of thumbnail titles ==
GM_addStyle(".yt-tile-default.video-list-item a .title, #watch-sidebar .video-list-item .title { font-size: 11px; line-height: 10px; }");
// Defaults are font-size: 13px; and line-height: 15px; which show only two lines in my browser.



// == Scrollbars on comments and related vids, to keep the video in view. ==
setTimeout(function(){
	// We could alternatively act on watch-panel but that includes the video navigation buttons!
	// BUG: Interferes with YouTube's lazy-loading of thumbnails for related video.
	if (document.location.href.indexOf("/all_comments?") >= 0) {
		return;   // Leave the full comments page alone.
	}
	var watchDiscussion = document.getElementById("watch-discussion");
	if (watchDiscussion) {
		watchDiscussion.style.overflow = "auto";
		watchDiscussion.style.maxHeight = (window.innerHeight - 574)+"px"; /* For a video height 360p */
	}
	var watchSidebar = document.getElementById("watch-sidebar");
	if (watchSidebar) {
		watchSidebar.style.overflow = "auto";
		watchSidebar.style.maxHeight = (window.innerHeight - 26)+"px";
		// May 2012 - fixes below no longer needed.
		// Now the text wraps because of the scrollbar, so we widen the element:
		// watchSidebar.style.width = (320+24)+"px";
		// watchSidebar.style.width = '300px';
		// And we must widen its container also:
		// TODO BUG: Why does this work in the console, but not from the userscript?
		// document.getElementById("watch-main").style.width = (960+24)+"px";
	}
	// Title text
	//document.getElementById("eow-title").scrollIntoView();
	// Uploader info and videolist popdown.
	//document.getElementById("watch-headline-user-info").scrollIntoView();
	// The author's video list (was supposed to be a small gap above the video when collapsed, but it's not)
	document.getElementById("watch-more-from-user").scrollIntoView();
	// The video
	//document.getElementsByTagName("embed")[0].scrollIntoView();
	// The video
	//document.getElementById("watch-video").scrollIntoView();
},1000);



// == Thumbnail animation ==
// TODO: This is working fine on "related videos" thumbnails, but not on queue
// thumbnails, even if I have the queue open when I load the page.
// Perhaps we are responding to a mouseout event from a child element, because
// we are not checking the event target like we should do.
function initThumbnailAnimator() {
	// function createThumbnailAnimatorEvent(thumbImage) {
	var thumbImage    = null;
	var evt    = null;
	var timer  = null;
	var frames = ["1.jpg","2.jpg","3.jpg"];   // "default.jpg",
	var frameI = 0;
	function changeFrame() {
		frameI = (frameI + 1) % frames.length;
		thumbImage.src = thumbImage.src.replace(/\/[^/]*$/,'') + '/' + frames[frameI];
	}
	function startAnimation() {
		// We make this check quite late, due to lazy loading
		if (thumbImage.src.match(/default\.jpg$/)) {
			timer = setInterval(changeFrame,600);
		}
	}
	function stopAnimation() {
		if (timer) {
			// logElem("Stopping elem",thumbImage);
			clearInterval(timer);
			// This isn't really neccessary, except to ensure the check for default\.jpg above works next time!
			thumbImage.src = thumbImage.src.replace(/\/[^/]*$/,'') + '/' + "default.jpg";
		}
	}
	function logElem(name,elem) {
		report = "<"+elem.tagName+" id="+elem.id+" class="+elem.className+" src="+elem.src+" />";
		GM_log(name+" = "+report);
	}
	function check(fn) {
		return function(evt) {
			// logElem("["+evt.type+"] evt.target",evt.target);
			var elemToCheck = evt.target || evt.srcElement;
			if (elemToCheck.tagName == "IMG") {
				thumbImage = elemToCheck;
				return fn();
			} else if (elemToCheck.className=='screen') {
				var seekImg = elemToCheck.parentNode.getElementsByTagName("img")[0];
				if (seekImg) {
					thumbImage = seekImg;
					fn();
				}
			// } else {
				// var imgCount = elemToCheck.getElementsByTagName("img").length;
				// if (imgCount == 1) {
					// thumbImage = elemToCheck.getElementsByTagName("img")[0];
					// // logElem("["+evt.type+"] checking sub-image",thumbImage);
					// logElem("Whilst checking",elemToCheck);
					// logElem("  Animating elem",thumbImage);
					// logElem("  with parent",thumbImage.parentNode);
					// logElem("  whilst currentTarget",evt.currentTarget);
					// logElem("  and srcElement",evt.srcElement);
					// return fn();
				// }
			}
		};
	}
	//// Unfortunately these do not fire on any HTMLImageElements when browsing the queue.
	document.body.addEventListener("mouseover",check(startAnimation),false);
	document.body.addEventListener("mouseout",check(stopAnimation),false);
	// var videoList = document.getElementById("watch-sidebar"); // or watch-module or watch-module-body or watch-related or watch-more-related
	// var videoList = document.getElementsByClassName("video-list")[0]; // can be 4 of these!
	// var thumbs = document.getElementsByTagName("img");
	// for (var i=0;i<thumbs.length;i++) {
		// createThumbnailAnimatorEvent(thumbs[i]);
	// }
}
setTimeout(initThumbnailAnimator,1000);


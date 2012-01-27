// ==UserScript==
// @name           Auto scroll keys
// @namespace      http://userscripts.org/users/44573
// @description    Auto scroll page on Ctrl+Down. Escape to stop.
// @include        *
// ==/UserScript==

var initialSpeed = 10;

/* BUG: Does not work well when zoomed in Chrome.
 * It does scroll, if you hold down the key long enough.
 * But when the threshold is passed, movement is quite fast.  Perhaps we can
 * overcome this by holding our own analogue (non-integer) scroll position, and
 * setting the page scroll from this.
 * Perhaps this should catch up if the real value differs significantly from
 * the stored value, indicating perhaps that the user jumped the page manually.
 */

/* BUG:
 * I think at times we are moving 1.2pixels each timeout, then accelerating to
 * say 1.4pixels per timeout, but in reality the browser is always doing 1
 * pixel!
 */

var u44573_go = false;
var scrollSpeed = 0;

var DOM_VK_DOWN = 40;
var DOM_VK_UP = 38;
var DOM_VK_ESCAPE = 27;
var DOM_VK_SPACE = 32;
var DOM_VK_ENTER = 13;

window.addEventListener('keydown', u44573_handler, true);

function sgn(x) { return ( x>0 ? 1 : x<0 ? -1 : 0 ); }
function u44573_handler(e) {
	var change = 0.60;   // Probably could be lowered a bit if we make scrollSpeed truly analogue.
	var upDelta, downDelta; // The acceleration of each key
	if (scrollSpeed === 0 || !u44573_go) {
		upDelta = initialSpeed;
		downDelta = initialSpeed;
	} else {
		if (Math.abs(scrollSpeed) < 0.1) {
			scrollSpeed = 0.1 * sgn(scrollSpeed);
		}
		downDelta = Math.abs(scrollSpeed)*change;
		upDelta = Math.abs(scrollSpeed)*change/(1+change);
		if (scrollSpeed < 0) {
			var tmpDelta = downDelta;
			downDelta = upDelta;
			upDelta = tmpDelta;
		}
	}
	// if (Math.random()<0.001) { document.title = ""+upDelta+" : "+downDelta; }
	if(e.ctrlKey && e.keyCode == DOM_VK_DOWN) { // Scroll downwards with CTRL-Down_Arrow
		scrollSpeed += downDelta;
		e.preventDefault(); // Most
	}
	if(e.ctrlKey && e.keyCode == DOM_VK_UP) { // Scroll upwards with CTRL-Up_Arrow
		scrollSpeed -= upDelta;
		e.preventDefault(); // Most
	}
	if(!u44573_go && scrollSpeed != 0) {
		u44573_go = true;
		u44573_goScroll();
	}
	if (e.keyCode == DOM_VK_ESCAPE || e.keyCode == DOM_VK_ENTER || e.keyCode == DOM_VK_SPACE) { // Stop (ESCAPE or ENTER or SPACE)
		if (u44573_go) {
			u44573_go = false;
			scrollSpeed = 0;
			// Do not pass keydown event to page:
			e.preventDefault(); // Most
			return false; // IE
		}
	}
}

function sgn(x) {
	return (x>0 ? +1 : x<0 ? -1 : 0);
}

var abs = Math.abs;

var maxPerSecond = 15;

function u44573_goScroll() {
	if (u44573_go) {
		var s = u44573_getScrollPosition();
		var jumpPixels = abs(scrollSpeed) / maxPerSecond;
		var timeToNext = 1000/maxPerSecond;
		// The browser can only jump a whole number of pixels, and it rounds down.
		// We had to do the following anyway for jumpPixels<1 but by doing it for
		// small numbers (<5) we workaround the analogue/digital bug.  (5*1.2=6)
		if (jumpPixels < 3) {
			timeToNext /= jumpPixels;
			// jumpPixels /= jumpPixels;
			jumpPixels = 1;
		}
		/*
			var timeToNext = 1000/abs(scrollSpeed);
			if (timeToNext < 1000/maxPerSecond) {
				jumpPixels = abs(scrollSpeed)/maxPerSecond;
				timeToNext = 1000/maxPerSecond;
			} else {
				jumpPixels = 1;
			}
		*/
		unsafeWindow.scroll(s[0], s[1] + jumpPixels*sgn(scrollSpeed));
		if (scrollSpeed == 0) {
			u44573_go = false;
		} else {
			setTimeout(u44573_goScroll, timeToNext);
		}
	}
}

function u44573_getScrollPosition() {
    return Array((document.documentElement && document.documentElement.scrollLeft) || window.pageXOffset || self.pageXOffset || document.body.scrollLeft,(document.documentElement && document.documentElement.scrollTop) || window.pageYOffset || self.pageYOffset || document.body.scrollTop);
}

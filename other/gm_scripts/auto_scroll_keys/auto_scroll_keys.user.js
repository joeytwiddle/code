// ==UserScript==
// @name           Auto scroll keys
// @namespace      http://userscripts.org/users/44573
// @description    Auto scroll page on Ctrl+Down. Escape to stop.
// @include        *
// ==/UserScript==

/* BUG: Does not work well in Chromium when zoomed in.  Sometimes moves 1 pixel
 * down, or scrolls left, but does not scroll down, although scrolling upward
 * works ok.
 * Oh it does scroll, if you hold down the key long enough to pass the
 * threshold needed when zooming.
 * But when the threshold is exceeded, movement is quite fast.  Perhaps we can
 * overcome this by holding our own analogue (non-integer) scroll position, and
 * setting the page scroll from this.
 * Perhaps this should catch up if the read value differs significantly from
 * the stored value, indicating perhaps that the user jumped the page manually.
 * */

var u44573_go = false;
var scrollSpeed = 0;

var DOM_VK_DOWN = 40;
var DOM_VK_UP = 38;
var DOM_VK_ESCAPE = 27;
var DOM_VK_SPACE = 32;

window.addEventListener('keydown', u44573_handler, true);

function u44573_handler(e) {
	if(e.ctrlKey && e.keyCode == DOM_VK_DOWN) { // Scroll downwards with CTRL-Keypad_Down_Arrow
		scrollSpeed += 1;
	}
	if(e.ctrlKey && e.keyCode == DOM_VK_UP) { // Scroll upwards with CTRL-Keypad_Up_Arrow
		scrollSpeed -= 1;
	}
	if(!u44573_go && scrollSpeed != 0) {
		u44573_go = true;
		u44573_goScroll();
	}
	if (e.keyCode == DOM_VK_ESCAPE || e.keyCode == DOM_VK_SPACE) { // Stop (ESCAPE or SPACE)
		u44573_go = false;
		scrollSpeed = 0;
	}
}

function sgn(x) {
	return (x>0 ? +1 : x<0 ? -1 : 0);
}

var abs = Math.abs;

function u44573_goScroll() {
	if (u44573_go) {
		var s = u44573_getScrollPosition();
			var timeToNext = 1000/abs(scrollSpeed);
			if (timeToNext < 1000/20) {
				jumpPixels = abs(scrollSpeed)/20
				timeToNext = 1000/20;
			} else {
				jumpPixels = 1
			}
		unsafeWindow.scroll(s[0], s[1] + jumpPixels*sgn(scrollSpeed));
		if (scrollSpeed == 0) {
			u44573_go = false;
		} else {
			setTimeout(u44573_goScroll, timeToNext);
		}
	}
}

function u44573_getScrollPosition()
{
    return Array((document.documentElement && document.documentElement.scrollLeft) || window.pageXOffset || self.pageXOffset || document.body.scrollLeft,(document.documentElement && document.documentElement.scrollTop) || window.pageYOffset || self.pageYOffset || document.body.scrollTop);
}

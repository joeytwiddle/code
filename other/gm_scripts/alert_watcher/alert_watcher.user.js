// ==UserScript==
// @name           Alert Watcher
// @namespace      alertwatcher
// @description    Protects your browser from infinite alert loops.  Overrides the default alert/confirm/prompt dialogs with versions that allow further messages to be cancelled.  You need never be Rick-Rolled again!  This script is actually a combination of some other greasemonkey anti-alert-loop scripts.
// @include        *
// ==/UserScript==

// BUG TODO: We stop further dialogs if the user hit Cancel, which is fine when we replace alert, but for the others, the user might have hit Cancel for a valid reason.  Solution: If they hit Cancel on a dialog which would have had a Cancel button anyway, then we ask "Cancel All Alerts?".  Mmm that's hassle, so we should revert to original system, which detected when there were multiple popups.
// TODO: Work off the non-GM version.

var resetTime = 60; // This many seconds after user Cancels alerts, re-enable them.  60 gives you a whole minute to leave the page.  I prefer 10 when I am developing.

// This version of the script:
//   http://hwi.ath.cx/javascript/alert_watcher.user.js
// can be embedded into any web-page, to protect users who are not using
// GreaseMonkey or Mozilla.

var init = function()
{
	var nexttime = 0;
	var oldconfirm = unsafeWindow.confirm;

	var remap = function(func) {
		var oldfunc = unsafeWindow[func];
		unsafeWindow[func] = function() {
			var d = new Date();
			var now = d.valueOf();
			if (nexttime <= now) {
				if (!oldconfirm.apply(this,arguments)) {
					nexttime = now + 1000*resetTime;
				}
			}
		}
	}

	remap('alert');
	remap('prompt');
	remap('confirm');
}();


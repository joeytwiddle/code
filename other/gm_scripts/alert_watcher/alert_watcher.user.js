// ==UserScript==
// @name           Alert Watcher
// @namespace      alertwatcher
// @description    Protects your browser from infinite alert loops.  Overrides the default alert/confirm/prompt commands with a version that allows further messages to be cancelled.
// @include        *
// This script is actually a combination of some other greasemonkey anti-alert-loop scripts.
// ==/UserScript==

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
					nexttime = now + 10000;
				}
			}
		}
	}

	remap('alert');
	remap('prompt');
	remap('confirm');
}();


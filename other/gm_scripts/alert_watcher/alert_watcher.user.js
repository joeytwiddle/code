// ==UserScript==
// @name           Alert Watcher
// @namespace      alertwatcher
// @description    Watch for possible malicious alert/confirm/prompt loops and allow the user to break out.
// @include        *
// ==/UserScript==

var init = function()
{
	var lasttime = 0; 
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

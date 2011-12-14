// ==UserScript==
// @name           FastJSLogger
// @namespace      FastJSLogger
// @description    Attempts to redirect console.log to a div floating on the page.
// @include        *
// ==/UserScript==
// Dropped: @require        http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js

// This fails to intercept GM_log calls from other userscripts.
// However it intercepts everything when we run bookmarklets.

// Partly DONE: We may be able to capture errors by overriding setTimeout,
// setInterval, XMLHttpRequest and any other functions which use callbacks,
// with our own versions which attempt the callback within a try-catch which
// logs and throws any intercepted exceptions.
// Unfortunately wrapping a try-catch around the main scripts in the document
// (which have alread run) is a bit harder from here.  ;)

// TODO: No thanks to Wikipedia's pre styling(?) I can't set a good default, so we need font zoom buttons!
// TODO: Add ability to minimize but popup again on new log message.
// TODO: Report file/line-numbers where possible.  (Split output into table? :E)
// TODO: Option to watch for new globals.
// TODO: Options to toggle logging of any intercepted setTimeouts, XMLHttpRequest's etc, in case the reader is interested.

(function(){



var autoHide = true;
var autoHideTimer = null;



function newNode(tag,data) {
	var elem = document.createElement(tag);
	if (data) {
		for (var prop in data) {
			elem[prop] = data[prop];
		}
	}
	return elem;
}

function newSpan(text) {
	return newNode("span",{textContent:text});
}

function addStyle(css) {
	// Konqueror 3.5 does not act on textValue, it requires textContent.
	// innerHTML doesn't work for selectors containing '>'
	var st = newNode("style", { type: "text/css", innerHTML: css } );
	document.getElementsByTagName('head')[0].appendChild(st);
}

function showLogger() {
	logDiv.style.display = '';
	//// BUG: Transition is not working!  Perhaps it only works when switching between CSS classes.
	// logDiv.style._webkit_transition_property = 'opacity';
	// logDiv.style._webkit_transition_duration = '2s';
	logDiv.style.opacity = 1.0;
}

function hideLogger() {
	logDiv.style.display = 'none';
	// logDiv.style._webkit_transition_property = 'opacity';
	// logDiv.style._webkit_transition_duration = '2s';
	// logDiv.style.opacity = 0.0;
}

var css = "";
css += " .fastJSLogger { position: fixed; right: 8px; top: 8px; width: 45%; /*max-height: 90%; height: 320px;*/ background-color: #ffffcc; color: black; border: 1px solid black; z-index: 10000; } ";
css += " .fastJSLogger > span { max-height: 10%; }";
// css += " .fastJSLogger > pre  { max-height: 90%; overflow: auto; }";
//// On the pre, max-height: 90% is not working, but specifying px does.
var maxHeight = window.innerHeight * 0.8 | 0;
css += " .fastJSLogger > pre  { max-height: "+maxHeight+"px; overflow: auto; }";
css += " .fastJSLogger > pre  { font-size: 80%; padding: 0.4em; }";
css += " .fastJSLogger        { opacity: 0.8; } ";
css += " .fastJSLogger:hover  { opacity: 1.0; } ";
if (document.location.host.match(/wikipedia/))
	css += " .fastJSLogger > pre  { font-size: 60%; }";
addStyle(css);

var logDiv = newNode("div",{id:'fastJSLogger',className:'fastJSLogger'});
hideLogger();
document.body.appendChild(logDiv);

logDiv.style.position = 'fixed';
logDiv.style.top = '20px';
logDiv.style.right = '20px';

		// @todo refactor
		// I/O: logDiv, logContainer

		var heading = newSpan("FastJSLogger");
		logDiv.appendChild(heading);

		var closeButton = newSpan("[X]");
		closeButton.style.float = 'right';
		closeButton.style.cursor = 'pointer';
		closeButton.style.paddingLeft = '5px';
		closeButton.onclick = function() { logDiv.parentNode.removeChild(logDiv); };
		logDiv.appendChild(closeButton);

		var logContainer = newNode("pre");

		var rollupButton = newSpan("[-]");
		rollupButton.style.float = 'right';
		rollupButton.style.cursor = 'pointer';
		rollupButton.style.paddingLeft = '10px';
		rollupButton.onclick = function() {
			if (logContainer.style.display == 'none') {
				logContainer.style.display = '';
				rollupButton.textContent = "[-]";
			} else {
				logContainer.style.display = 'none';
				rollupButton.textContent = "[+]";
			}
		};
		logDiv.appendChild(rollupButton);

		logDiv.appendChild(logContainer);

function addCloseButtonTo(elem) {

	var b = document.createElement("button");
	b.textContent = "X";
	b.style.zIndex = 2000;

	b.onclick = function() {
		GM_log("[",b,"] Destroying:",elem);
		elem.style.display = 'none';
		elem.parentNode.removeChild(elem);
	};

	b.style.float = 'right';   // BUG: not working in FF4!
	elem.appendChild(b);

	// b.style.position = 'absolute';
	// b.style.right = '4px';
	// b.style.top = '4px';
	// document.body.appendChild(b);
	//// BUG: b is not removed from DOM on click!

}
// addCloseButtonTo(logDiv);

var oldConsole = this.console; // When running as a userscript in Chrome, cannot see this.console!
var oldGM_log = this.GM_log;

// Any reason why we're using this.unsafeWindow instead of just unsafeWindow?  Probably not...
var target = ( this.unsafeWindow ? this.unsafeWindow : window );

var preventInfLoop = null;

target.console = {};
target.console.log = function(a,b,c) {

	// Make visible if hidden
	showLogger();

	if (a+b+c === preventInfLoop) {
		return;
	}
	preventInfLoop = a+b+c;

	// Replicate to the old loggers we intercepted (overrode)

	if (oldConsole && oldConsole.log) {
		// Some browsers dislike use of .call and .apply here, e.g. GM in FF4.
		// So to avoid "oldConsole.log is not a function":
		try {
			oldConsole.log.apply(oldConsole,arguments);
		} catch (e) {
			oldConsole.log("oldConsole.log.apply() failed!",a,b,c);
		}
	}

	if (oldGM_log) {
		oldGM_log(a,b,c);
	}

	// We cannot do arguments.join (FF4)
	var out = "";
	for (var i=0;i<arguments.length;i++) {
		if (i>0) {
			out += " ";
		}
		out += (""+arguments[i]);
	}
	// logContainer.appendChild(document.createElement("br"));
	// logContainer.appendChild(document.createTextNode(out));
	// logContainer.appendChild(document.createTextNode("div")).textContent = out;
	// var d = document.createElement("div");
	// d.style.fontStyle = 'Monospaced';
	var d = document.createElement("div");
	d.textContent = out;
	logContainer.appendChild(d);

	// Scroll to bottom
	// TODO: This is undesirable if the scrollbar was not already at the bottom, i.e. the user has scrolled up manually and is trying to read earlier log entries!
	logContainer.scrollTop = logContainer.scrollHeight;

	if (autoHide) {
		if (autoHideTimer !== null) {
			clearTimeout(autoHideTimer);
			autoHideTimer = null;
		}
		autoHideTimer = setTimeout(hideLogger,15 * 1000);
	}

};

/*
target.console.error = function(e) {
	// target.console.log("[ERROR]",e,e.stack);
	var newArgs = [];
	newArgs.push("[ERROR]");
	for (var i=0;i<arguments.length;i++) {
		newArgs.push(arguments[i]);
	}
	try {
		newArgs.push("(reported from function "+arguments.callee.caller.name+")");
		// console.error("caller = "+arguments.callee.caller);
		// console.error("stack = "+e.stack);
	} catch (e) {
	}
	target.console.log.apply(target.console,newArgs);
	target.console.log.apply(target.console,["Stacktrace:\n",e.stack]);
};
*/

target.console.error = function(a,b,c) {
	oldConsole.log("[ERROR]",a,b,c);
	if (oldConsole.error) {
		oldConsole.error(a,b,c);
	} else {
		throw new Error("Nowhere to send console.error() call!",a,b,c);
	}
};

target.console.log("FastJSLogger loaded this="+this+" GM_log="+typeof this.GM_log);

//// Not really needed
// this.GM_log = target.console.log;



// Try to intercept errors
// Strategy: Whenever a callback is placed, we should wrap it!

var oldSetTimeout = window.setTimeout;
window.setTimeout = function(fn,ms) {
	var wrappedFn = function(){

		// We can catch the error here, but if we do, even if we throw it again, the Chrome debugger shows this function as the source.
		// It is preferable to let the error fall up to Chrome, so we can easily jump to the line number.
		// Unfortunately, if we don't catch the error, we can't report it to FJSL!

		/*
		try {
		*/

			// console.log("Timeout happening after "+ms+"ms: "+fn)
			if (typeof fn == 'function') {
				fn();
			} else if (typeof fn == 'string') {
				eval(fn);
			} else  {
				throw new Error("[FJSL] setTimeout.wrappedFn(): Unsure how to execute fn of type \""+typeof fn+"\": "+fn);
			}

		/*
		} catch (e) {
			// Actually hard to read!
			// var prettyFn = fn; // (""+fn) .replace(/\n/g,'\\n');
			var fnName = fn && fn.name;
			if (!fnName) {
				fnName = "<anonymous>";
			}
			// console.log("[ERR]",e,prettyFn);
			console.log("[Exception]",e,"from "+fnName+"()");
			console.log(""+e.stack);
			throw e;
			// Unfortunately even Chrome dev shows the throw as coming from here!
			// So it is better if we leave it alone.
		}
		*/

	};

	return oldSetTimeout(wrappedFn,ms);
};

// TODO: all DOM events!  XHR.
// We could even put intercepts on generic functions, in case an error occurs inside them, in a context which we had otherwise failed to intercept.


/*
// TESTING
for (var i=0;i<45;i++) {
	var s = "";
	for (var j=Math.random()*5;j>=0;j--) {
		s += Math.random();
	}
	console.log(s);
}
*/


})();

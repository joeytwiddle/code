// ==UserScript==
// @name           FastJSLogger
// @namespace      FastJSLogger
// @description    Attempts to redirect console.log to a div floating on the page.
// @include        *
// ==/UserScript==
// Dropped: @require        http://hwi.ath.cx/code/other/gm_scripts/fallbackgmapi/fallbackgmapi.user.js



var autoHide = false;

var watchWindowForErrors = true;
var interceptTimeouts = true;
var interceptEvents = true;   // Wraps around any listeners registered later, so errors can be caught and reported.
var logEvents = true;   // Log activity for the listeners.

var bringBackTheStatusBar = true;   // TODO: watch window.status for change

if (this.localStorage) {
	// TODO: Recall and store FJSL preferences in localStorage.

	// UNWANTED: During testing, I am toggling this setting.
	interceptTimeouts = !Boolean(localStorage['fastjslogger_interceptTimeouts']);
	localStorage['fastjslogger_interceptTimeouts'] = interceptTimeouts;
}



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

// TESTING: all DOM events!  TODO: XHR.
// TESTING: We could even put intercepts on generic functions, in case an error occurs inside them, in a context which we had otherwise failed to intercept.

// TODO: If the FJSL is wanted for custom logging, not normal console.log
// logging, we will need to expose a function (FJSL.log()?), and *not*
// intercept console.log.

(function(){



// I did have two running happily in parallel (Chrome userscript and page
// script) but it is rarely useful.
if (document.getElementById("fastJSLogger") != null) {
	return;
}



// GUI creation library functions

function addStyle(css) {
	// Konqueror 3.5 does not act on textValue, it requires textContent.
	// innerHTML doesn't work for selectors containing '>'
	var st = newNode("style", { type: "text/css", innerHTML: css } );
	document.getElementsByTagName('head')[0].appendChild(st);
}

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

}



// == Main Feature - Present floating popup for log messages ==

var logDiv = null;
var logContainer = null;

var autoHideTimer = null;

function createGUI() {

	var css = "";
	css += " .fastJSLogger { position: fixed; right: 8px; top: 8px; width: 45%; /*max-height: 90%; height: 320px;*/ background-color: #ffffcc; color: black; border: 1px solid black; z-index: 10000; } ";
	css += " .fastJSLogger > span { max-height: 10%; }";
	// css += " .fastJSLogger > pre  { max-height: 90%; overflow: auto; }";
	//// On the pre, max-height: 90% is not working, but specifying px does.
	var maxHeight = window.innerHeight * 0.8 | 0;
	css += " .fastJSLogger > pre  { max-height: "+maxHeight+"px; overflow: auto; word-wrap: break-word; }";
	css += " .fastJSLogger > pre  { font-size: 80%; padding: 0.4em; }";
	// css += " .fastJSLogger > pre > input { width: 100%, background-color: #888888; }";
	css += " .fastJSLogger        { opacity: 0.8; } ";
	css += " .fastJSLogger:hover  { opacity: 1.0; } ";
	if (document.location.host.match(/wikipedia/))
		css += " .fastJSLogger > pre  { font-size: 60%; }";
	addStyle(css);

	var logDiv = newNode("div",{ id: 'fastJSLogger', className: 'fastJSLogger' });
	hideLogger();
	document.body.appendChild(logDiv);

	logDiv.style.position = 'fixed';
	logDiv.style.top = '20px';
	logDiv.style.right = '20px';

	// @todo refactor
	// I/O: logDiv, logContainer

	var heading = newSpan("FastJSLogger");
	logDiv.appendChild(heading);

	// addCloseButtonTo(logDiv);

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

	var displaySearchFilter = true;
	if (displaySearchFilter) {
		function createSearchFilter(logDiv,logContainer) {
			var searchFilter = document.createElement("input");
			searchFilter.type = 'text';
			searchFilter.style.float = 'right';
			searchFilter.style.paddingLeft = '5px';
			searchFilter.onchange = function(evt) {
				var searchText = this.value;
				// console.log("Searching for "+searchText);
				var logLines = logContainer.childNodes;
				for (var i=0;i<logLines.length;i++) {
					if (logLines[i].textContent.indexOf(searchText) >= 0) {
						logLines[i].style.display = '';
					} else {
						logLines[i].style.display = 'none';
					}
				}
			};
			return searchFilter;
		}
		var searchFilter = createSearchFilter(logDiv,logContainer);
		// logDiv.appendChild(document.createElement("br"));
		logDiv.appendChild(searchFilter);
	}

	logDiv.appendChild(logContainer);

	return [logDiv,logContainer];

}



function addToFastJSLog(a,b,c) {

	if (logContainer) {

		// We cannot do arguments.join (FF4)
		var out = "";
		for (var i=0;i<arguments.length;i++) {
			var obj = arguments[i];
			var str = ""+obj;
			// Non-standard: inform type if toString() is dull.
			if (str === "[object Object]" && typeof obj === 'object' /*&& obj!==null*/) {
				if (obj.constructor) {
					str = "[object "+obj.constructor.name+"]";
				} else {
					// str += " [no type]";
				}
			}
			var gap = (i>0?' ':'');
			out += gap + str;
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

	}

	if (autoHide) {
		if (autoHideTimer !== null) {
			clearTimeout(autoHideTimer);
			autoHideTimer = null;
		}
		autoHideTimer = setTimeout(hideLogger,15 * 1000);
	}

}

function showLogger() {
	if (logDiv) {
		logDiv.style.display = '';
		//// BUG: Transition is not working!  Perhaps it only works when switching between CSS classes.
		// logDiv.style._webkit_transition_property = 'opacity';
		// logDiv.style._webkit_transition_duration = '2s';
		logDiv.style.opacity = 1.0;
	}
}

function hideLogger() {
	if (logDiv) {
		logDiv.style.display = 'none';
		// logDiv.style._webkit_transition_property = 'opacity';
		// logDiv.style._webkit_transition_duration = '2s';
		// logDiv.style.opacity = 0.0;
	}
}

var k = createGUI();
logDiv = k[0];
logContainer = k[1];

// target.console.log("FastJSLogger loaded this="+this+" GM_log="+typeof this.GM_log);
// console.log("interceptTimeouts is "+(interceptTimeouts?"ENABLED":"DISABLED"));



// Intercept messages for console.log if it exists.
// Create console.log if it does not exist.

var oldConsole = this.console; // When running as a userscript in Chrome, cannot see this.console!
var oldGM_log = this.GM_log;

var target = ( this.unsafeWindow ? this.unsafeWindow : window );

// Replace the old console
target.console = {};

var preventInfLoop = null;

target.console.log = function(a,b,c) {

	// Make FJSL visible if hidden
	showLogger();

	// I tried disabling this and regretted it!
	if (a+b+c === preventInfLoop) {
		return;
	}
	preventInfLoop = a+b+c;

	addToFastJSLog.apply(this,arguments);

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

};

//// Not really needed.  Why not?
// this.GM_log = target.console.log;



// == MAIN SCRIPT ENDS ==
// The following are optional and can be stripped for minimifiation.



/* Provide/intercept console.info/warn/error(). */

target.console.error = function(a,b,c) {
	if (a instanceof Error) {
		a = ""+a.stack;   // Far more informative
	}
	addToFastJSLog("[ERROR]",a,b,c);
	if (oldConsole) {
		if (oldConsole.error) {
			oldConsole.error(a,b,c);
		} else {
			oldConsole.log("[ERROR]",a,b,c);
		}
	}
};

// Could generalise the two functions below:
//interceptLogLevel("warn");
//interceptLogLevel("info");

target.console.warn = function(a,b,c) {
	addToFastJSLog("[WARN]",a,b,c);
	if (oldConsole) {
		if (oldConsole.warn) {
			oldConsole.warn(a,b,c);
		} else {
			oldConsole.log("[WARN]",a,b,c);
		}
	}
};

target.console.info = function(a,b,c) {
	addToFastJSLog("[INFO]",a,b,c);
	if (oldConsole) {
		if (oldConsole.info) {
			oldConsole.info(a,b,c);
		} else {
			oldConsole.log("[INFO]",a,b,c);
		}
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



// Some more library functions:

function tryToDo(fn,target,args) {

	try {

		fn.apply(target,args);
		return;

	} catch (e) {
		// Actually hard to read!
		// var prettyFn = fn; // (""+fn) .replace(/\n/g,'\\n');
		var fnName = fn && fn.name;
		if (!fnName) {
			fnName = "<anonymous>";
		}
		// console.log("[ERR]",e,prettyFn);
		// console.log("[Exception]",e,"from "+fnName+"()");
		console.error(""+e.stack);
		var prettyFn = (""+fn).replace(/\n/,/ /,'g');
		console.error("occurred when calling: "+prettyFn);
		// throw e;
		// Unfortunately even Chrome dev shows the throw as coming from here!
		// So it is better if we leave it alone.
	}

}

function showObject(obj) {
	return "{ " + Object.keys(obj).map(function(prop) {
		return prop+": "+obj[prop];
	}).join(", ") + " }";
}

function getXPath(node) {
	var parent = node.parentNode;
	if (!parent) {
		return '';
	}
	var siblings = parent.childNodes;
	var totalCount = 0;
	var thisCount = -1;
	for (var i=0;i<siblings.length;i++) {
		var sibling = siblings[i];
		if (sibling.nodeType == node.nodeType) {
			totalCount++;
		}
		if (sibling == node) {
			thisCount = totalCount;
			break;
		}
	}
	return getXPath(parent) + '/' + node.nodeName.toLowerCase() + (totalCount>1 ? '[' + thisCount + ']' : '' );
}

function getStack(drop,max) {
	var stack;
	try {
		throw new Error("Dummy for getStack");
	} catch (e) {
		stack = e.stack.split('\n').slice(drop).slice(0,max).join('\n');
	}
	return stack;
}



// == Error Interceptors ==

// Whenever a callback is placed, we should wrap it!
// DONE:
//   event listeners (added after we run)
//   setTimeout
// TODO:
//   setInterval
//   XMLHttpRequest

// We could even wrap any standard functions which are common sources of
// Errors, in case we fail to catch them any other way.

if (watchWindowForErrors) {
	function handleError(evt) {
		// console.log("Error caught by FJSL:");
		// console.log(evt);
		// console.log(Object.keys(evt));
		// target.console.error(evt.filename+":"+evt.lineno+" "+evt.message+" ["+evt.srcElement+"]",evt);
		console.error(evt.message," ",evt.filename+":"+evt.lineno);
	}
	window.addEventListener("error",handleError,true);
	// document.body.addEventListener("error",handleError,true);
}

if (interceptTimeouts) {

	var oldSetTimeout = window.setTimeout;
	window.setTimeout = function(fn,ms) {
		var wrappedFn = function(){

			// We can catch the error here, but if we do, even if we throw it again, the Chrome debugger shows this function as the source.
			// It is preferable to let the error fall up to Chrome, so we can easily jump to the line number.
			// Unfortunately, if we don't catch the error, we can't report it to FJSL!

			if (typeof fn === 'string') {
				var str = fn;
				fn = function(){ eval(str); };
			}
			if (!typeof fn === 'function') {
				throw new Error("[FJSL] setTimeout was not given a function!",fn);
			}

			tryToDo(fn);

			console.log("Re-running to reproduce stack-trace (may fail)");
			fn();
			throw new Error("Re-run failed to produce any error!",fn);

		};

		return oldSetTimeout(wrappedFn,ms);
	};

}

if (interceptEvents) {

	var realAddEventListener = HTMLElement.prototype.addEventListener;
	// HTMLElement.prototype.oldAddEventListener = realAddEventListener;
	HTMLElement.prototype.addEventListener = function(type,handler,capture,other){
		if (logEvents) {
			var niceFunc = handler.name || (""+handler).substring(0,40).replace(/\n/," ",'g')+"...";
			console.info("[EVENTS] We will log "+type+" events on "+getXPath(this)+" when they fire "+niceFunc);
			console.info(getStack(3,3));
		}
		var newHandler = function(evt) {
			if (logEvents) {
				if (evt.target.parentNode == logContainer) {
					// Do not log events in the console's log area, such as DOMNodeInserted!
				} else {
					// console.log("("+type+") on "+evt.target);
					// console.log("[EVENT] "+type+" on "+getXPath(evt.target)+" evt="+showObject(evt));
					console.info("[EVENT] "+type+" on "+getXPath(evt.target)+" being handled by "+niceFunc);
				}
			}
			// handler.call(this,evt);
			tryToDo(handler,this,[evt]);
		};
		// tryToDo(realAddEventListener,this,type,handler,capture,other);
		/*
		var that = this;
		tryToDo(function(){
			realAddEventListener.call(that,type,newHandler,capture,other);
			//realAddEventListener.apply(that,[type,handler,capture,other]);
			//that.oldAddEventListener(type,handler,capture,other);
		});
		*/
		/*
		tryToDo(realAddEventListener,this,[type,newHandler,capture,other]);
		*/
		realAddEventListener.call(this,type,newHandler,capture,other);
	};

}



})();

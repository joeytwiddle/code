/*\ grimeape_greasemonkey_compat.js
)\( Provides emulation of GreaseMonkey library functions implemented to work through GrimeApe.
\*/

/*

CONSIDER: Will we need to make XMLHttpRequests targetted at the page host, to
prevent security from blocking us, but still have them caught by the Proxy?

CONSIDER: If we really don't want to make XMLHttpRequests, we could use this
filthy hack instead:  Add a <SCRIPT> element to the page, which would return
results by writing them invisibly into the document somewhere for retrieval.

TODO:
document.evaluate (XPath tool available in Mozilla, often used in userscripts)
GM_xmlhttprequest or whatever it's called.

*/

// In case you were wondering, self=window and this=window.
// GM_log("Self = "+self+" This = "+this+" so self==window? "+(self==window)); // true
// I believe if we are in a sub-context, and code tries to use a function which
// is not defined there, JS will check up the contexts eventually to the top
// window and find our introduced function there.

// When do we want to check before overriding an object, and when do we want to
// always override?  Bear in mind we are inside an insecure environment - a
// malicious webpage may have already set variables in an attempt to confuse us.
// OTOH, testing for the existence of a function before overriding it, is a good
// way to use browser-internals when they exist, or our alternatives otherwise.

// TODO: These should be hidden out of the main context in some sub-context,
// but visible to GM_log() (which is at top/main context).
var logToProxy = false;
var localLog = "";

function GM_log(obj) {
	localLog += ""+obj+"\n";
	window.status = ""+obj;
	if (logToProxy) {
		//// Send log line to the proxy, by requesting a special URL.
		//// This works, although it does cause the DOM to grow.
		//// Of course, we could do it with a hidden IFrame, or an image, or
		//// anything which will cause an HTTP request without messing up too much.
		var logUrl = "/_gRiMeApE_/log/?data=" + escape(""+obj);
		/*
		var tmpScript = document.createElement('SCRIPT');
		tmpScript.src = logUrl;
		document.body.appendChild(tmpScript);
		// document.writeln(''); // Ends up separate from the tmpScript element!
		//// TODO: We should make a version of this which removes the SCRIPT from
		//// the DOM afterwards, to prevent continual growth.
		*/
		document.writeln('<SCRIPT src="'+logUrl+'"/>'); // Easier to read in LiveDOMHTML.
		// I'm not sure which is faster.  Apparently setting innerHTML is
		// recommended over createElement and appendChild, because the "parser is
		// faster".  That would suggest document.writeln() should be faster.
	}
}

GM_log("GM_log() works!");

function GM_setValue(name,value) {
	// TODO
	GM_log('setting 1');
	if (value==undefined || value==null)
		value = "";
	GM_log('setting 2');
	document.writeln('<SCRIPT type="text/javascript" src="/_gRiMeApE_/setValue?name='+escape(name)+'&value='+escape(value)+'"/>');
	GM_log('setting 3');
}

function GM_getValue(name,defaultValue) {
	document.writeln('<SCRIPT src="/_gRiMeApE_/getValue?name='+escape(name)+'"/>');
	// In Konqueror I think we must wait before we can get result.
	// Awh shucks how are we gonna return? :f
	if (window.GM_getValueResult)
		return unescape(window.GM_getValueResult);
	else
		return defaultValue;
}

// var unsafeWindow = window;
// if (!this.unsafeWindow) {
	// this.unsafeWindow = window;
// }
unsafeWindow = window;

if (!document.evaluate) {
	document.evaluate = function (args) {
		GM_log("TODO: "+this);
	}
}

// Awhh hell Konqueror doesn't even have uneval().

// if (!uneval) { /* Testing existence this way fails with "No such var". */
if (!this.uneval) {
	this.uneval = function (obj) {
		if (obj==undefined) { return "undefined"; }
		// Still this==window: GM_log("TODO: uneval() (this="+this+" self="+self+")");
		// GM_log("Trying to uneval: "+obj+" (type "+typeof(obj)+")");
		if (typeof(obj)=='string') {
			return '"' + obj.replace(/"/g,'\\"') + '"';
		} else if (typeof(obj)=='number') {
			return ""+obj;
		} else if (typeof(obj)=='object') {
			// ({x:"fart", 1:"pants"})
			var arrayString = "";
			for (var key in obj) {
				var val = obj[key];
				// GM_log("Got key "+key+" and val "+val);
				if (arrayString!="") arrayString += ", ";
				arrayString += uneval(key)+":"+uneval(val);
			}
			arrayString = "({" + arrayString + "})";
			// GM_log("Returning arrayString="+arrayString);
			return arrayString;
		} else {
			GM_log("<WARNING!> uneval does not know type "+typeof(obj));
			return "DUNNO_TYPE_"+typeof(obj);
		}
	}
}

// TODO: Remove all the TODOs from this file.  We know the whole file is TODO.


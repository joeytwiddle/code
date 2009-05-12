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
// I believe if we are in a sub-context, and code tries to use a function which
// is not defined there, JS will check up the contexts eventually to the top
// window and find our introduced function there.

// TODO: These should be hidden out of the main context in some sub-context,
// but visible to GM_log() (which is at top/main context).
var logToProxy = true;
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
}

function GM_getValue(name,defaultValue) {
	// TODO
	return "";
}

var unsafewindow = window;

if (!document.evaluate) {
	document.evaluate = function (args) {
		GM_log("TODO: "+this);
	}
}

// Awhh hell Konqueror doesn't even have uneval().

// if (!uneval) { /* Testing existence this way fails with "No such var". */
if (!this.uneval) {
	this.uneval = function (obj) {
		GM_log("TODO: uneval() (this="+this+" self="+self+")");
	}
}

GM_log("Self = "+self+" This = "+this+" so self==window? "+(self==window));

// TODO: Remove all the TODOs from this file.  We know the whole file is TODO.


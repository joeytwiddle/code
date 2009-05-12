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

function GM_log(obj) {
	window.status = ""+obj;
	/*
	//// This works, although it does cause the DOM to grow.
	var tmpScript = document.createElement('SCRIPT');
	tmpScript.src = "/gReAsEmOnKeY/log/?data=" + escape(""+obj);
	top.document.body.appendChild(tmpScript);
	//// Of course, we could do it with a hidden IFrame, or an image, or
	//// anything which will cause and HTTP request without messing up too much.
	*/
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

// Awhh hell.
if (!this.uneval) {
	this.uneval = function (obj) {
		GM_log("TODO: uneval() (this="+this+" self="+self+")");
	}
}

GM_log("Self = "+self+" This = "+this);

// TODO: Remove all the TODOs from this file.  We know the whole file is TODO.


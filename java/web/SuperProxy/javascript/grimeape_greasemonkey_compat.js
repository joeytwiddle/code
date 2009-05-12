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
	// TODO
}

function GM_setValue(name,value) {
	// TODO
}

function GM_getValue(name,defaultValue) {
	// TODO
	return "";
}

var unsafewindow = window;

// document.evaluate = function (args) {
	// TODO
// }


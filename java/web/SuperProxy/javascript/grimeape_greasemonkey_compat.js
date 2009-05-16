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
GM_xmlhttpRequest or whatever it's called.

*/

// OLD IE compat:
if(typeof XMLHttpRequest == "undefined") {
	XMLHttpRequest = function() {
		try { return new ActiveXObject("Msxml2.XMLHTTP.6.0") } catch(e) {}
		try { return new ActiveXObject("Msxml2.XMLHTTP.3.0") } catch(e) {}
		try { return new ActiveXObject("Msxml2.XMLHTTP") } catch(e) {}
		try { return new ActiveXObject("Microsoft.XMLHTTP") } catch(e) {}
		throw new Error( "This browser does not support XMLHttpRequest." )
	};
}

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
		var logUrl = "/_gRiMeApE_/log/?data=" + cgiEscape(""+obj);
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

// escape(val) is not enough for CGI.  It does nothing to real +s, but webserver reads real +s from CGI as spaces!
function cgiEscape(val) {
	return escape(val).replace(/\+/g,'%2b');
}
function cgiUnescape(val) {
	return unescape(val.replace(/\+/g,' '));
}

// CONSIDER: We could queue up GM_setValue requests, and flush them in a
// separate thread.  This might speed up some scripts (no stalling for return),
// and by sending batches we might reduce HTTP traffic.  We must ensure not to
// break order, and we must stall GM_getValue requests until queue is flushed.
// Having our own setValue/getValue queue might actually improve synchronization
// over multiple JS threads.
function GM_setValue(name,value) {
	// TODO
	// Konqueror appears to fail (or take forever) if params.length exceeds 30,000.  Needs batching!  :f
	// Hmm Firefox was going very slow about 40k also.  =/  Stream timed out when it completed.  Are we blocking in a buffer somewhere?
	if (value==undefined || value==null)
		value = "";
	var url = '/_gRiMeApE_/setValue';
	var params = 'name='+cgiEscape(name)+'&value='+cgiEscape(value);
	// document.writeln('<SCRIPT type="text/javascript" src="'+url+'"/>');
	//// Doing it the above way in Konqueror, means the request does not actuall happen until *after* this thread has finished.
	//// This way will set immediately:
	var request = new XMLHttpRequest();
	// request.open('GET',url+'?'+params,false);
	request.open('POST',url,false);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	request.setRequestHeader("Content-length", params.length);
	request.setRequestHeader("Connection", "close");
	request.send(params);
	// request.send('\n');
	// request.send('\n');
	// GM_log(new Date()+' GM_setValue("'+name+'","'+value+'") responded with "'+request.responseText+'".');
	GM_log(new Date()+' GM_setValue("'+name+'",{"'+value.length+'"}) responded with "'+request.responseText+'".');
	return null;
}

function GM_getValue(name,defaultValue) {
	var url = '/_gRiMeApE_/getValue?name='+cgiEscape(name);
	/*
	url += '&type=js';
	window.GM_getValueResult = undefined;
	document.writeln('<SCRIPT src="'+url+'"/>');
	*/
	// In Konqueror I think we must wait before we can get result.
	// Awh shucks how are we gonna return? :f
	// We can do a long pause: for (var i=0;i<1000000;i++) { }
	// But the request doesn't get made until after it.  :P
	/*
	if (window.GM_getValueResult)
		return unescape(window.GM_getValueResult);
	else
		return defaultValue;
	*/
	var request = new XMLHttpRequest();
	request.open('GET',url,false);
	request.send(null);
	// GM_log("GM_getValue(\""+name+"\") returned: "+request.responseText);
	// return request.responseText; // this worked fine for konqueror ;p
	return cgiUnescape(unescape(request.responseText.replace(/^<RESPONSE>/,'').replace(/<\/RESPONSE>$/,''))); // mozilla
	/*
	var waitUntil = new Date().getTime() + 5*1000;
	for (var i=0;i<100000;i++) {
		if (request.responseText!=undefined && request.responseText!=null) {
			GM_log("GM_getValue() Client responded at loop "+i+" with "+request.responseText);
			return request.responseText;
		}
		// if (window.GM_getValueResult) {
			// GM_log("GM_getValue() Client responded at loop "+i+" with "+window.GM_getValueResult);
			// return window.GM_getValueResult;
		// }
		var now = new Date().getTime();
		if (now > waitUntil) {
			GM_log("GM_getValue() Time expired - no response.");
			return undefined;
		}
	}
	// GM_log("Got response: "+request.responseText);
	// return request.responseText;
	GM_log("GM_getValue() WARNING! Loop gave up before time did!");
	return undefined;
	*/
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

this.ga_uneval = function (obj) {
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
	} else if (typeof(obj)=='function') {
		// return (""+obj).replace(/\n/g,''); // This is quite similar to what FF produces
		return '"NO_FUNKTIONS"';
	} else if (typeof(obj)=='boolean') {
		return ""+obj;
	} else {
		GM_log("<WARNING!> uneval does not know type "+typeof(obj));
		return ""+obj+" <DUNNO_TYPE_"+typeof(obj)+">";
	}
}

// if (!uneval) { /* Testing existence this way fails with "No such var". */
if (!this.uneval) {
	this.uneval = this.ga_uneval;
}

// TODO
// ga_xmlhttpRequest = function (method,url,headers,data,onload,onerror,onreadystatechange) {
ga_xmlhttpRequest = function (req) {
	GM_log("xmlhttpRequest: "+req);
	var responseState;
	try {
		// Security:
		/*
		if (req.url.startsWith("http://") || req.url.startsWith("https://") || req.url.startsWith("ftp://")) {
		} else {
			GM_log("Making illegal request scheme: "+req.url);
			return;
		}
		// Er lol that's GM security.  I doubt our proxy can access Mozilla's chrome:// scheme :P
		*/
		var url = '/_gRiMeApE_/xmlhttpRequest?url='+cgiEscape(req.url); // +'&data='+cgiEscape(req.data);
		var request = new XMLHttpRequest();
		// TODO: data(?),onload,onerror,onreadystatechange
		GM_log("Doing: open("+req.method+","+url+",false)...");
		request.open(req.method,url,false); // user,passwd
		for (var header in req.headers) {
			GM_log("Header ["+header+"] = "+req.headers[header]);
			request.setRequestHeader(header,req.headers[header]);
		}
		GM_log("Done");
		request.send(req.data);

		GM_log(new Date()+' GM_xmlhttpRequest("'+url+'") responded with "'+request.responseText+'".');

		responseState = {
			// can't support responseXML because security won't
			// let the browser call properties on it
			responseText:request.responseText,
			readyState:request.readyState,
			responseHeaders:(request.readyState == 4 ? request.getAllResponseHeaders() : ""),
			status:(request.readyState == 4 ? request.status : 0),
			statusText:(request.readyState == 4 ? request.statusText : ""),
			// finalUrl:(request.readyState == 4 ? request.channel.URI.spec : "")
			// finalUrl:req.url,
		}
		// The callback function for onreadystatechange is called repeatedly while
		// the request is in progress. It takes a single parameter, responseDetails.
		if (req.onreadystatechange) { req.onreadystatechange(responseState); }
		if (req.onload) { req.onload(responseState); }

	} catch (e) {
		if (req.onerror) { req.onerror(responseState); }
		throw e;
	}
	return null;
}

if (!this.GM_xmlhttpRequest) {
	GM_xmlhttpRequest = ga_xmlhttpRequest;
}

// TODO: Remove all the TODOs from this file.  We know the whole file is TODO.


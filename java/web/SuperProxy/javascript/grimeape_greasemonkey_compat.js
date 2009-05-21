/*\ grimeape_greasemonkey_compat.js
)\( Provides emulation of GreaseMonkey library functions implemented to work through GrimeApe.
\*/

/*

CONSIDER: Will we need to make XMLHttpRequests targetted at the page host, to
prevent security from blocking us, but still have them caught by the Proxy?

CONSIDER: If we really don't want to make XMLHttpRequests, we could use this
filthy hack instead:  Add a <SCRIPT> element to the page, which would return
results by writing them invisibly into the document somewhere for retrieval.

// TODO: Complete API: http://wiki.greasespot.net/Greasemonkey_Manual:API
// GM_listValues, GM_getResourceURL, GM_getResourceText,
// GM_addStyle, XPathResult, GM_openinTab.
// Also: @resource @require @unwrap
// TODO: namespace magic, and maybe some jail for scripts?

TODO:
document.evaluate (XPath tool available in Mozilla, often used in userscripts)
GM_xmlhttpRequest or whatever it's called.

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

function GA_padLeft(str,num,padChar) {
	str = ""+str;
	while (str.length < num) {
		str = padChar + str;
	}
	return str;
}

function GA_getSimpleTime() {
	var now = new Date();
	return GA_padLeft(now.getHours(),2,'0')
		+":"+ GA_padLeft(now.getMinutes(),2,'0')
		+":"+ GA_padLeft(now.getSeconds(),2,'0')
		+"."+ GA_padLeft(now.getMilliseconds(),4,'0').substring(0,2);
}

function GA_log(namespace,obj) {
	localLog += "["+GA_getSimpleTime()+"] "+namespace+": "+obj+ "\n";
	window.status = ""+obj;
	if (logToProxy) {
		//// Send log line to the proxy, by requesting a special URL.
		//// This works, although it does cause the DOM to grow.
		//// Of course, we could do it with a hidden IFrame, or an image, or
		//// anything which will cause an HTTP request without messing up too much.
		var logUrl = "/_gRiMeApE_/log/?data=" + encodeURIComponent(""+obj);
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

function GM_log(obj) {
	GA_log("NONE!",obj);
}

GM_log("GM_log() works!");

//// @deprecated - I found encodeURIComponent!
// escape(val) is not enough for real CGI escaping.  It does nothing to real
// '+'s, but the webserver will read real '+'s from CGI as spaces!
//// Deprecated/removed
// function cgiEscape(val) {
	// return encodeURIComponent(val);
	// // return escape(val).replace(/\+/g,'%2b');
// }
//// Do we still need this, or is there unencodeURIComponent()?
function cgiUnescape(val) {
	return unescape(val.replace(/\+/g,' '));
}

// CONSIDER: We could queue up GM_setValue requests, and flush them in a
// separate thread.  This might speed up some scripts (no stalling for return),
// and by sending batches we might reduce HTTP traffic.  We must ensure not to
// break order, and we must stall GM_getValue requests until queue is flushed.
// Having our own setValue/getValue queue might actually improve synchronization
// over multiple JS threads.
function GA_setValue(namespace,name,value) {
	// Konqueror appears to fail (or take forever) if params.length exceeds 30,000.  Needs batching!  :f
	// Hmm Firefox was going very slow about 40k also.  =/  Stream timed out when it completed.  Are we blocking in a buffer somewhere?
	if (value==undefined || value==null)
		value = "";
	var url = "/_gRiMeApE_/setValue";
	var params = "name="+encodeURIComponent(name)+"&namespace="+encodeURIComponent(namespace)+"&value="+encodeURIComponent(value);
	// document.writeln('<SCRIPT type="text/javascript" src="'+url+'"/>');
	//// Doing it the above way in Konqueror, means the request does not actuall happen until *after* this thread has finished.
	//// This way will set immediately:
	var request = new XMLHttpRequest();
	// request.open("GET",url+"?"+params,false);
	request.open("POST",url,false);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	request.setRequestHeader("Content-length", params.length);
	request.setRequestHeader("Connection", "close");
	request.send(params);
	// request.send("\n");
	// request.send("\n");
	// GM_log('GM_setValue("'+name+'","'+value+'") responded with "'+request.responseText+'".');
	GA_log(namespace,'GM_setValue("'+name+'","'+namespace+'",{'+value.length+'}) responded with "'+request.responseText+'".');
	return null;
}

function GM_setValue(name,value) {
	GA_setValue("_",name,value);
}

function GA_getValue(namespace,name,defaultValue) {
	var url = '/_gRiMeApE_/getValue?name='+encodeURIComponent(name)+"&namespace="+encodeURIComponent(namespace);
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

function GM_getValue(name,defaultValue) {
	return GA_getValue("_",name,defaultValue);
}

function GA_deleteValue(namespace,name) {
	var url = '/_gRiMeApE_/deleteValue?name='+encodeURIComponent(name)+"&namespace="+encodeURIComponent(namespace);
	var request = new XMLHttpRequest();
	request.open('GET',url,false);
	request.send(null);
	if (request.responseText != "<NODATA>OK</NODATA>") {
		GM_log("GM_deleteValue(name) failed!  "+request.responseText);
	}
	return;
}

function GM_deleteValue(name) {
	GA_deleteValue("_",name);
}

function GM_listValues() {
	GM_log("WARNING: GM_listValues() will not be implemented until we have a namespace.");
}

// var unsafeWindow = window;
// if (!this.unsafeWindow) {
	// this.unsafeWindow = window;
// }
unsafeWindow = window;

// document.evaluate and XPathResult are implemented in xpath.js
// TODO: If you load an XML document via XMLHttpRequest or another mechanism,
// the evaluate() method is also available.

// Awhh hell Konqueror doesn't even have uneval().

this.ga_uneval = function (obj) {
	if (obj==undefined) { return "undefined"; }
	// GM_log("Trying to uneval: "+obj+" (type "+typeof(obj)+")");
	if (typeof(obj)=='string') {
		// TODO: ok we have done some basic chars, but what about non-ascii chars?!
		return '"' + obj.replace(/\\/g,'\\\\').replace(/"/g,'\\"').replace(/\n/g,'\\n').replace(/\r/g,'\\r') + '"';
	} else if (typeof(obj)=='number') {
		return ""+obj;
	} else if (typeof(obj)=='object') {
		// if (obj.length == 0) { // It's actually an array
			// return "[]"; // return "({})"; // If we do the stuff below to an empty array, we list all the fns?
		// }
		// ({x:"fart", 1:"pants"})
		var arrayString = "";
		for (var key in obj) {
			// If obj is a custom object, any functions in it are iterated
			// If obj is an array, in Moz only the items (keys) are iterated, but
			// in Konq, the default array fns are also iterated :f
			// We could sort-of detect arrays badly by detecting the length property :f
			// But in general I found it preferable to use objects for storage.
			var val = obj[key];
			// GM_log("Got key "+key+" and val "+val);
			if (arrayString!="") arrayString += ", ";
			arrayString += uneval(key)+":"+uneval(val);
			// If key here is a string, Moz does not quote it if not needed, or surrounds
			// it with single apostrophes if it contains a space or special chars.
			// I believe our current method is equivalent anyway, although not identical.
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

// ga_xmlhttpRequest = function (method,url,headers,data,onload,onerror,onreadystatechange) {
ga_xmlhttpRequest = function (req) {
	if (this.namespace) { // Long shot, hoping to see context of calling function, but I think it failed due to closure.
		GM_log(namespace,"xmlhttpRequest: "+req);
	} else {
		GM_log("NO_NAMESPACE","xmlhttpRequest: "+req);
	}
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
		var url = '/_gRiMeApE_/xmlhttpRequest?url='+encodeURIComponent(req.url); // +'&data='+encodeURIComponent(req.data);
		var request = new XMLHttpRequest();
		// TODO: data(?),onload,onerror,onreadystatechange
		GM_log("Doing: open("+req.method+","+url+",false)...");
		request.open(req.method,url,false); // user,passwd
		if (req.headers) {
			for (var header in req.headers) {
				GM_log("Header ["+header+"] = "+req.headers[header]);
				request.setRequestHeader(header,req.headers[header]);
			}
		}
		GM_log("Done");
		request.send(req.data);

		GM_log('GM_xmlhttpRequest("'+url+'") responded with "'+request.responseText+'".');

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

// GM_registerMenuCommand is implemented in grimeape_config.js

function GM_addStyle(css) {
	var doc = document;
	var head, style;
	head = doc.getElementsByTagName("head")[0];
	if (!head) { return; }
	style = doc.createElement("style");
	style.type = "text/css";
	style.innerHTML = css;
	head.appendChild(style);
}

// OLD IE compat:
if(typeof XMLHttpRequest == "undefined") {
	XMLHttpRequest = function() {
		try { return new ActiveXObject("Msxml2.XMLHTTP.6.0") } catch(e) {}
		try { return new ActiveXObject("Msxml2.XMLHTTP.3.0") } catch(e) {}
		try { return new ActiveXObject("Msxml2.XMLHTTP") } catch(e) {}
		try { return new ActiveXObject("Microsoft.XMLHTTP") } catch(e) {}
		GM_log("This browser does not support XMLHttpRequest.");
	};
}


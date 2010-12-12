#!/usr/local/node-v0.3.1/bin/node
// @name           JSON callback proxy for cross-site XHR
// @description    Run this service in nodejs to serve cross-site XMLHttpRequests via a JSONP callback.
// @copyright      2010 Paul Clark
// @license        cc-by-3.0; http://creativecommons.org/licenses/by/3.0/

// TODO: Fill out some of the standard headers
// TODO: Correct handling for failure cases.

var http = require('http');
var net = require('net');

http.createServer(function (request, response) {
	try {
		var url = require("url").parse(request.url,true);
		if (url.pathname === "/xhrasjson") {
			handleRequestForXhrOverJson(request,response,url);
		} else {
			response.writeHead(501,"text/plain");
			response.end("We don't serve that kind of thing here, sir.\n");
		}
	} catch (e) {
		console.log("Error handling request: "+e);
		response.writeHead(502,"text/plain");
		response.end("Error handling request: "+e+"\n"); // body allowed in 502?
	}
}).listen(8124);
console.log('Server running at http://127.0.0.1:8124/');

function handleRequestForXhrOverJson(incomingRequest, response, url) {
	// We expect the XHR request object to be provided in the "request" parameter.
	var xhrReqEscaped = url.query["request"];
	var xhrReqStr = require("querystring").unescape(xhrReqEscaped); // or decodeURIComponent()?
	console.log(">> Received request: "+xhrReqStr);
	var xhrReq = JSON.parse(xhrReqStr);
	var targetUrl = xhrReq.url;
	var targetHost = targetUrl.match(/:\/\/([^/]*)/);
	targetHost = ( targetHost ? targetHost[1] : "ERROR_NO_HOSTNAME" );
	var httpClient = http.createClient(80,targetHost);
	var outgoingHeaders = {};
	// TODO: copy or generate some common headers
	/*
	// We *could* copy over headers from the input request.
	// But this copies over the "accept-encoding" header which returns a
	// confusing gzip to us!
	for (var h in incomingRequest.headers) {
		outgoingHeaders[h] = incomingRequest.headers[h];
	}
	*/
	// We *should* copy over headers from the XHRequest.
	if (xhrReq.headers) {
		for (var header in xhrReq.headers) {
			outgoingHeaders[header] = xhrReq.headers[header];
		}
	}
	outgoingHeaders.host = targetHost;
	console.log("|| >> Making request: "+JSON.stringify(outgoingHeaders));
	var outgoingRequest = httpClient.request(xhrReq.method, targetUrl, outgoingHeaders);
	outgoingRequest.end();
	outgoingRequest.on('response',function (incomingResponse) {
		// We taint the given headers to create our response headers.
		console.log("|| << Got response: "+JSON.stringify(incomingResponse.headers));
		var httpResponseHeaders = incomingResponse.headers;
		httpResponseHeaders["content-type"] = "text/javascript";
		response.writeHead(incomingResponse.statusCode, httpResponseHeaders);
		var xhrResponse = {};
		xhrResponse.responseText = "";
		incomingResponse.on('data',function(data) {
			xhrResponse.responseText += data;
		});
		var callbackName = url.query["callback"];
		incomingResponse.on('end',function() {
			var xhrResponseStrung = JSON.stringify(xhrResponse);
			// console.log("unescaped xhrResponseStrung = "+xhrResponseStrung);
			var script =
				  '(function(){\n'
				+ '	var xhrResponseStrung = decodeURIComponent("'+encodeURIComponent(xhrResponseStrung)+'");\n'
				+ '	var xhrResponse = JSON.parse(xhrResponseStrung);\n'
				+ '	window["'+callbackName+'"](xhrResponse);\n'
				+ '})();\n';
			console.log("<< Returning callback script: "+script); // TODO: replace nonprinting chars
			// console.log("<< Responding with script length "+script.length+".");
			response.end(script);
		});
	});
}


#!/usr/local/node-v0.3.1/bin/node
// @name           JSON callback proxy for cross-site XHR
// @description    Run this service in nodejs to serve cross-site XMLHttpRequests via a JSONP callback.
// @copyright      2010 Paul Clark
// @license        cc-by-3.0; http://creativecommons.org/licenses/by/3.0/

// TODO: Fill out some of the standard headers
// DONE: Correct handling for failure cases.
// TODO: Handle ports other than 80.

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
		throw e;
	}
}).listen(8124);
console.log('Server running at http://127.0.0.1:8124/');

function handleRequestForXhrOverJson(incomingRequest, response, url) {
	// We expect the XHR details object to be provided in the "details" parameter.
	var xhrReqEscaped = url.query["details"];
	var xhrReqStr = require("querystring").unescape(xhrReqEscaped); // same as decodeURIComponent?
	var xhrReq = JSON.parse(xhrReqStr);
	var targetUrl = xhrReq.url;
	console.log(">> Received request for "+targetUrl);
	var targetHost = targetUrl.match(/:\/\/([^/]*)/);
	targetHost = ( targetHost ? targetHost[1] : "ERROR_NO_HOSTNAME" );
	console.log("|| >> Opening connection to "+targetHost);
	var httpClient = http.createClient(80,targetHost);
	var outgoingHeaders = {};
	// Copy over the headers requested by the XHRequest.
	if (xhrReq.headers) {
		for (var header in xhrReq.headers) {
			outgoingHeaders[header] = xhrReq.headers[header];
		}
	}
	// TODO: We should copy or generate some common headers
	// But we don't want the "accept-encoding" header which might return a
	// confusing gzip to us!
	// for (var h in incomingRequest.headers) {
		// outgoingHeaders[h] = incomingRequest.headers[h];
	// }
	outgoingHeaders.host = targetHost;
	// console.log("|| >> Making request: "+JSON.stringify(outgoingHeaders));
	var outgoingRequest = httpClient.request(xhrReq.method, targetUrl, outgoingHeaders);
	outgoingRequest.end();
	outgoingRequest.on('response',function (incomingResponse) {
		// console.log("|| << Got response: "+JSON.stringify(incomingResponse.headers));
		console.log("|| << Got response with "+incomingResponse.headers.length+" headers.");
		var httpResponseHeaders = {};
		httpResponseHeaders["content-type"] = "text/javascript";
		response.writeHead(incomingResponse.statusCode, httpResponseHeaders);
		var xhrResponse = {};
		xhrResponse.status = incomingResponse.statusCode;
		xhrResponse.statusText = "HTTP "+incomingResponse.statusCode+" Unlabeled";
		xhrResponse.responseHeaders = incomingResponse.headers;
		xhrResponse.responseText = "";
		incomingResponse.on('data',function(data) {
			xhrResponse.responseText += data;
		});
		var callbackName = url.query["callback"];
		incomingResponse.on('end',function() {
			var xhrResponseStrung = JSON.stringify(xhrResponse);
			var escapedXhrResponse = encodeURIComponent(xhrResponseStrung);
			console.log("|| unescaped xhrResponseStrung = "+xhrResponseStrung);
			var script =
				  '(function(){\n'
				+ '	window["'+callbackName+'"](JSON.parse(decodeURIComponent("'+escapedXhrResponse+'")));\n'
				+ '})();\n';
			// console.log("<< Returning callback script: "+script);
			console.log("<< Returning script length "+script.length+".");
			response.end(script);
		});
	});
}


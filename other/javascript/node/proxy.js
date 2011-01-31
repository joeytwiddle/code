var http = require('http');
// var net = require('net');

/* Warning: This causes infloop if you try to access it like a normal webserver. */

function logObject(indent, obj) {
	for (var prop in obj) {
		console.log(indent+prop+": "+obj[prop]);
	}
}

http.createServer(function (request, response) {
	console.log(">> Received request:", JSON.stringify(request.headers));
	console.log("   -> "+request.method+" "+request.url);
	logObject("   ->   ",request.headers);
	var hostname = request.headers['host'];
	var splitHost = hostname.split(":");
	var port = 80;
	if (splitHost.length > 1) {
		hostname = splitHost[0];
		port = parseInt(splitHost[1],10);
	}
	// Try to avoid repeatedly connecting to self!
	// Konqueror does not set the "proxy-connection" header
	if (!request.headers['proxy-connection'] && (hostname=="127.0.0.1" || hostname=="localhost")) {
		failWithError("I'm a proxy not a webserver!");
		return;
	}
	// We should not put this header in our outbound request
	// Especially if our client set the host to this proxy, that might cause an infinite loop!
	delete request.headers["proxy-connection"];
	var httpClient = http.createClient(port,hostname);
	var outgoingRequest = httpClient.request(request.method, request.url, request.headers);
	//// Sticking with 0.3.1 form since 0.3.7 failed
	// var outgoingRequest = http.request(request,function(incomingResponse){
		outgoingRequest.on('error',function (err) {
			failWithError(err);
		});
		outgoingRequest.on('response',function (incomingResponse) {
			console.log("<< Responding:", JSON.stringify(incomingResponse.headers));
			console.log("   <- HTTP "+incomingResponse.statusCode);
			logObject("   <-   ",incomingResponse.headers);
			response.writeHead(incomingResponse.statusCode, incomingResponse.headers);
			incomingResponse.on('data',function(data) {
				response.write(data);
			});
			incomingResponse.on('end',function() {
				response.end();
			});
		});
	// });
	outgoingRequest.end();
	function failWithError(err) {
		response.writeHead(200, { 'Content-Type': 'text/html' } );
		response.end("<html><body>"+err+"</body></html>");
	}
}).listen(8080);

console.log('Proxy listening at http://127.0.0.1:8080/');


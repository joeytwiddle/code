var http = require('http');
var net = require('net');

var serverDir = '.';

function inspect(obj) {
	var outStr = "";
	for (var prop in obj) {
		if (typeof obj[prop]=='function') {
			outStr += "fn:"+prop+" ";
		} else {
			outStr += prop+"="+obj[prop]+" ";
		}
	}
	return outStr;
}

function log() {
	var toLog = "";
	for (var prop in arguments) {
		if (typeof arguments[prop] == "string")
			toLog += arguments[prop]+" ";
		else
			toLog += inspect(arguments[prop])+" ";
	}
	console.log(toLog);
}

http.createServer(function (request, response) {
	log(">> Received request:",request.headers);

	var reqPath = request.url.split('?')[0];
	log("reqPath:",reqPath);
	path.exists(reqPath,function(bExists){
		if (bExists) {
			// ...
		} else {
			// ...
		}
	});
}).listen(8000);

console.log('Web-server listening at http://localhost:8080/');


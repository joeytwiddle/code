http = require 'http'

logObj = (indent,o) ->
	for key,val of o
		console.log indent + key+": "+val

handlerFn = (request,response) ->
	console.log "=> Received request:"
	console.log "   >> #{request.method} #{request.url}"
	logObj      "   >>   ", request.headers
	failWithError = (err) ->
		console.log "<< Cannot serve request:" + request.url + "because:" + err
		response.writeHeader 200, 'Content-Type': "text/html"
		escapeHTML = (str) -> str  # TODO!
		response.write "<html><body>\n" + "<p>Cannot serve "+escapeHTML(request.url)+"\n" + "<br>&nbsp;&nbsp;&nbsp;&nbsp;because: "+escapeHTML(err)+"</p>\n" + "</body></html>\n"
		response.end()
	if !(request.headers['proxy-connection']) && request.headers['host'] == '127.0.0.1'
		response.writeHeader 200, 'Content-Type': "text/html"
		response.end "<html><body>I'm a proxy not a webserver!</body></html>"
		return
	else
		delete request.headers['proxy-connection']
		## We could create the Host header if it is missing, but that has not yet been neccessary.
		hostname = request.headers['host']
		splitHost = hostname.split ":"
		port = 80
		if splitHost.length > 1
			hostname = splitHost[0]
			port = parseInt(splitHost[1],10)
		## Clients request full URL from proxy, but servers want only path part
		# request.url = request.url.replace(/^http:\/\/.*\//,'/')
		httpClient = http.createClient port, hostname
		httpClient.on 'error', failWithError
		outgoingRequest = httpClient.request request.method, request.url, request.headers
		outgoingRequest.on 'error', (err) ->
			console.log "Request error:" + err
		outgoingRequest.on 'response', (incomingResponse) ->
			console.log "<= Response (for #{request.url}):"
			console.log "   << HTTP #{incomingResponse.statusCode}"
			logObj      "   <<   ", incomingResponse.headers
			response.writeHead incomingResponse.statusCode, incomingResponse.headers
			incomingResponse.on 'data', (data) ->
				console.log "   << "+('.' for x in [1..(data.length/1024)]).join('')+" "+(data.length)+" bytes"
				response.write data
			incomingResponse.on 'end', () ->
				console.log "   << Done."
				response.end()
		outgoingRequest.end()

server = http.createServer handlerFn
server.on 'error', (err) ->
	console.log "Server error:" + err
server.listen 8080
console.log "Coffee proxy at at http://127.0.0.1:8080/"


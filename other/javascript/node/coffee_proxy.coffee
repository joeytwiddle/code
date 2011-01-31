http = require 'http'
path = require 'path'
fs   = require 'fs'
# net  = require 'net'

listenPort = 8080
serverDir = '.'

## We cannot use JSON.stringify() for general logging, because this errors if an object has a circular structure
## uneval() is not available in Chromium, most implementations still have .toSource()
## We provide simply: inspect(...)
inspect = (obj) ->
	outStr = "{"
	for prop, val of obj
		if (typeof val) == "function"
			# outStr += "fn:"+prop+" "
			outStr += prop+"() "
		else
			outStr += prop+"="+val+" "
	outStr = outStr.slice(0,-1)+"}"
	return outStr

shortDate = () -> new Date().toString().split(' ').slice(0,5).join(' ')
shortDate = () ->
	pad = (n,min) ->
		n = ""+n
		while n.length < min
			n = '0' + n
		return n
	pad2 = (n) -> pad n,2
	d = new Date()
	d.getFullYear()+"/"+pad2(d.getMonth()+1)+"/"+pad2(d.getDate()+1)+" "+
		pad2(d.getHours())+":"+pad2(d.getMinutes())+":"+pad2(d.getSeconds())

log = (args...) ->
	toLog = "["+shortDate()+"] "
	for arg in args
		do (arg) ->
			if (typeof arg == "string")
				toLog += arg + " "
			else
				toLog += inspect arg
	console.log toLog

escapeHTML = (str) -> str  # TODO!

logList = (indent,l) ->
	for x in l
		log indent + (x)

logObj = (indent,o) ->
	for key,val of o
		log indent + key+": "+val

handlerFn = (request,response) ->
	log    "=> Received request:" # , request
	log    "   >> #{request.method} #{request.url}"
	logObj "   >>   ", request.headers
	# log "   Host: "+request.headers['host']
	# log "   type: "+typeof request.headers ## object
	# logList "   ", request.headers
	# log "   headers:", request.headers
	# log "   headers:"+inspect request.headers
	failWithError = (err) ->
		log "<< Cannot serve request:", request.url, "because:", err
		response.writeHeader 200, 'Content-Type': "text/html"
		response.write "<html><body>\n"
		response.write "<p>Cannot serve "+escapeHTML(request.url)+"\n"
		response.write "<br>&nbsp;&nbsp;&nbsp;&nbsp;because: "+escapeHTML(err)+"</p>\n"
		response.write "</body></html>\n"
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
			log "Request error:", err
		outgoingRequest.on 'response', (incomingResponse) ->
			log    "<= Response (for #{request.url}):"
			log    "   << HTTP #{incomingResponse.statusCode}"
			logObj "   <<   ", incomingResponse.headers
			inspect incomingResponse.headers
			response.writeHead incomingResponse.statusCode, incomingResponse.headers
			incomingResponse.on 'data', (data) ->
				log "   << "+('.' for x in [1..(data.length/1024)]).join('')+" "+(data.length)+" bytes"
				response.write data
			incomingResponse.on 'end', () ->
				log "   << Done."
				response.end()
		outgoingRequest.end()

server = http.createServer handlerFn
server.on 'error', (err) ->
	log "Server error:", err
server.listen listenPort
log "Coffee proxy at at http://127.0.0.1:#{listenPort}/"


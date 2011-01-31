http = require 'http'
path = require 'path'
fs   = require 'fs'
# net  = require 'net'

serverDir = '.'

## We cannot use JSON.stringify() for general logging, because this errors if
## an object has a circular structure.
## uneval() is not available in Chromium, most implementations still have .toSource()
## We provide simply: inspect(...)
inspect = (args...) ->
	outStr = "{"
	for obj in args
		for prop, val of obj
			if (typeof val) == "function"
				# outStr += "fn:"+prop+" "
				outStr += prop+"() "
			else
				outStr += prop+"="+val+" "
	outStr = outStr.slice(0,-1)+"}"
	return outStr

log = (args...) ->
	toLog = ""
	for arg in args
		do (arg) ->
			if (typeof arg == "string")
				toLog += arg + " "
			else
				toLog += inspect arg
	console.log toLog

escapeHTML = (str) -> str  # TODO!

handlerFn = (request,response) ->
	log ">> Received request:", request.headers
	reqPath = (request.url.split '?')[0].replace('^/','')
	log "   Handling reqPath: "+reqPath
	path.exists reqPath, (bExists) ->
		if (bExists)
			log "<< Sending "+reqPath+" to ", request
			response.writeHeader 200, 'Content-Type': "text/html"
			fs.readFile reqPath, (err, data) ->
				(
					if (err)
						failWithError err
					if (data)
						response.write data
						response.end()
				)
			# stream = fs.createReadStream(reqPath,{'r'})
			# stream.onData = (data) ->
				# response.write data
			# stream.onError = (err) ->
			# stream.onEnd = (data) ->
				# response.end data
		else
			failWithError "Resource "+reqPath+" does not exist"
	failWithError = (err) ->
		log "<< Cannot serve request:", request.url, "because:", err
		response.writeHeader 200, 'Content-Type': "text/html"
		response.write "<html><body>\n"
		response.write "<p>Cannot serve "+escapeHTML(request.url)+"\n"
		response.write "<br>&nbsp;&nbsp;&nbsp;&nbsp;because: "+escapeHTML(err)+"</p>\n"
		response.write "</body></html>\n"
		response.end()

server = http.createServer handlerFn
server.listen 8000
console.log "Coffee web-server exposing "+fs.realpathSync(serverDir)+" at http://localhost:8000/"


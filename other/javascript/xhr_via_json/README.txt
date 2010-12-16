To get GM_xmlhttpRequest working cross-site in Google Chrome userscripts, first
run the proxy called xss_xhr_via_jsonp_server.js somewhere (requires
nodejs.org).  [Or you are welcome to use my proxy if it is running.]

Then edit the config var 'proxyHost' in xss_xhr_via_jsonp.js, and paste the
script into the top of your Userscripts.  They should now be able to perform
all manner of GM_xmlhttpRequests through the proxy!

I have not had any luck so far using @require to include the script.  I appear
to end up with Chrome's default same-domain GM_xhR.

Leave me comments on http://hwi.ath.cx/ or something.


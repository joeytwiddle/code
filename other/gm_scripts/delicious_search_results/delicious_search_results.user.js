// ==UserScript==
// @name           Delicious Search Results on Google
// @namespace      gaillard
// @description    Shows results from Delicious on Google search pages
// @include        http://www.google.*/search?*q=*
// @include        http://www.google.*/*
// @date           2009-03-07
// @version        0.1
// @GM_version     0.8.20080609.0
// 
// 99% of the code for this user script comes form markcarey's userscript 'Twitter Search Results on Google' (http://userscripts.org/scripts/show/43451)
// ==/UserScript==
// This wasn't working for me: @require        http://hwi.ath.cx/javascript/xhr_via_json/xss_xhr_via_jsonp.js

// DONE: Don't keep spamming Delicious when we go forwards and backwards to Google again.
// Cache and re-use the results if they are recent.


// We always replace Google Chrome's GM_xmlhttpRequest because it is not cross-site.
if (!this.GM_xmlhttpRequest || window.navigator.vendor.match(/Google/)) {
	GM_xmlhttpRequest = function(details) {
		var proxyHost = "hwi.ath.cx:8124";
		// We don't want to send functions to the proxy, so we remove them from the details object.
		var onloadCallback = details.onload;
		var onerrorCallback = details.onerror;
		var onreadystatechangeCallback = details.onreadystatechange;
		delete details.onload;
		delete details.onerror;
		delete details.onreadystatechange;
		// Insert a callback into the root window, anonymised by a random key.
		var callbackName = "xss_xhr_via_jsonp_callback_" + parseInt(Math.random()*987654321);
		var callbackFunction = function(responseDetails) {
			if (onreadystatechangeCallback) {
				responseDetails.readyState = 4;
				onreadystatechangeCallback(responseDetails);
			}
			if (onloadCallback) {
				onloadCallback(responseDetails);
			}
		};
		var weAreInUserscriptScope = (typeof unsafeWindow != 'undefined');
		if (!window.navigator.vendor.match(/Google/) || !weAreInUserscriptScope) {
			// This works fine in Firefox GM, or in Chrome's content scope.
			window[callbackName] = callbackFunction;
		} else {
			// But the window seen from Chrome's userscript scope is sandboxed,
			// and many updates are not shared between scopes.
			// So we must get Chrome's unsafeWindow (the real content window).
			var div = document.createElement("div");
			div.setAttribute("onclick", "return window;");
			unsafeWindow = div.onclick();
			// And place the callback in that.
			unsafeWindow[callbackName] = callbackFunction;
		}
		// Request an XHR response from the proxy, which should return some javascript to call the callback.
		var reqStrung = JSON.stringify(details);
		var params = "details="+encodeURIComponent(reqStrung)+"&callback="+callbackName;
		var script = document.createElement("script");
		script.type = "text/javascript";
		script.src = "http://" + proxyHost + "/xhrasjson?" + params;
		document.getElementsByTagName("head")[0].appendChild(script);
		// The callback should run on a successful response.  But we need to handle errors too.
		// script.onload = function(e) { GM_log("Script has loaded."); };
		script.onerror = function(e) {
			var responseDetails = {};
			responseDetails.status = 12345;
			if (onreadystatechangeCallback) {
				responseDetails.readyState = 4;
				onreadystatechangeCallback(responseDetails); // This gets called even on error, right?
			}
			if (onerrorCallback) {
				onerrorCallback(responseDetails);
			}
			throw new Error("Failed to get JSONed XHR response from "+proxyHost+" - the proxy server may be down.");
		};
	};
}


GM_DUR = {
	un : "",
	init : function()
	{
		var href = document.location.href;
		GM_DUR.un = href.match(/[&?]q=([^&]*)(?:&|$)/)[1];

		if( GM_DUR.un != "" )
		{
			// Can we re-use cached results?
			var cached_un = GM_getValue("cached_un");
			if (cached_un == GM_DUR.un) {
				GM_log("Re-using cached Delicious response.");
				var response = ({});
				response.responseText = GM_getValue("cached_responseText");
				GM_DUR.handle(response);
			} else {
				GM_log("Making a fresh request to "+GM_DUR.un);
				GM_xmlhttpRequest({
					method:"GET",
					url:"http://feeds.delicious.com/v2/json/tag/"+GM_DUR.un,
					headers:{
						"User-Agent":"Mozilla/5.0",
						"Accept":"text/json"
					},
					onload:GM_DUR.handle
				});
			}
		}
	},

	handle : function(response)
	{
		GM_log("Delicious response was "+response+" with text: "+response.responseText);
		var results = eval("("+response.responseText+")");
		GM_setValue("cached_un",GM_DUR.un);
		GM_setValue("cached_responseText",response.responseText);

		var ds = document.createElement("ol");

		var deliciousImageHTML = '<img src="data:image/png;base64,AAABAAIAEBAAAAEAGABoAwAAJgAAABAQAAABACAAaAQAAI4DAAAoAAAAEAAAACAAAAABABgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0tLT0tLT0tLT0tLT0tLT0tLT0tLT0tLT////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQy////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQy////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQy////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQy////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQy////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQy////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQy////////////////////////////////0HQy0HQy0HQy0HQy0HQy0HQy0HQy0HQyAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP/S0tP/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT/9LS0/8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP/S0tP/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT/9LS0/8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP/S0tP/0tLT/9LS0//S0tP/0tLT/9LS0//S0tP/0tLT////////////////////////////////////////////0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv///////////////////////////////////////////9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL////////////////////////////////////////////QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy////////////////////////////////////////////0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv///////////////////////////////////////////9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL////////////////////////////////////////////QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy////////////////////////////////////////////0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv///////////////////////////////////////////9B0Mv/QdDL/0HQy/9B0Mv/QdDL/0HQy/9B0Mv/QdDL/AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==" alt="" height="42" width="43">';

		if(results.length > 0) {

			//// Breaks in FF3.5:
			// ds.style = "position: absolute; right: 10px;";
			//// Works but not well with Google Preview Pane:
			// ds.style.position = 'absolute';
			// ds.style.right = '10px';

			var il, h;
			var query = unescape(GM_DUR.un).replace(/\+/g, ' ');
			h = ds.appendChild(document.createElement("li"));
			h.className = "g";
			var h3 = h.appendChild(document.createElement("h3"));
			h3.className = "r";
			h3.innerHTML = "<a href='http://delicious.com/search?p=" + GM_DUR.un +"'>Delicious results for <em>"+ query +"</em></a>";
			var t = h.appendChild(document.createElement("table"));
			t.className = "ts";
			var tb = t.appendChild(document.createElement("tbody"));
			var row = tb.appendChild(document.createElement("tr"));

			var newHTML = '<td style="padding-top: 5px; padding-right: 10px; font-size: 78%; line-height: normal; width: 43px; text-align: center;" valign="top">'+deliciousImageHTML+'</td><td style="padding-top: 3px;" valign="top">';
			for( var i=0; i < 5; i++ ) {
				if (i >= results.length) {
					continue;
				}
				// var fixedDate = results[i].dt.replace(/-/g,'/').replace(/Z$/,'').replace(/T/,' ');
				// // GM_log("fixedDate="+fixedDate);
				// var ageMonths = (new Date() - new Date(fixedDate)) / 1000 / 60 / 60 / 24 / 30;
				// // GM_log("ageMonths="+ageMonths);
				// /* My approximation of popularity, max 2400 */
				// var pop = parseInt(2400 / Math.floor(1,ageMonths));
				// // GM_log("pop="+pop);
				// ageMonths = parseInt(ageMonths*10)/10;
				// var pop = 100;
				il = "<div class='s'>"+
					// "<div style='position:relative; background-color=#00cccc; z-index:-100; height=10px; width="+pop+"px;'/>"+
					// ageMonths+"m "+
				"<a href='"+results[i].u+"' class='l'>"+
					results[i].d+"</a> "+
					' <span class="f"><FONT size="-1">'+ (""+results[i].t).replace(/,/g,', ') +'</FONT></span></div>';
				newHTML += il;
			}
			newHTML += '</td>';

			row.innerHTML = newHTML;

		} else {
			// ds.innerHTML = deliciousImageHTML;
			ds.appendChild(document.createTextNode("No Delicious results"));
		}

		var results_section = document.getElementById("res");
		results_section.insertBefore(ds, results_section.firstChild);

	},
};

//// This appears to work in Konqueror but not Firefox O_o
// document.addEventListener('DOMContentLoaded',GM_DUR.init,false);

GM_DUR.init();


// ==UserScript==
// @name           Get Page Meta
// @namespace      GPM
// @description    Shows Delicious info for target page in a tooltip when you hover over a link.
// @include        *
// ==/UserScript==
// @OLDrequire        http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.js

// Code adapted from Hover Links
//// Thanks to:
// http://delicious.com/help/feeds
//// The delicious JSON url we use is:
// http://feeds.delicious.com/v2/json/urlinfo?url=http://www.google.co.uk/
//// Since bookmarklets can't make CD-XHR, we could make a JSONP proxy...
// http://hwi.ath.cx/json_plus_p?url=http://feeds.delicious.com/... ?

// TODO: onclick should cancel the lookup (user has followed link, no need to query)

var bg_color = "#EAEAEA";
var border_color = "#D5D5D5";
var font_color = "#000000";
var font_face = "tahoma";
var font_size = "11px";

(function() {
	function boldTextElement(txt) {
		var span = document.createElement("SPAN");
		span.appendChild(document.createTextNode(txt));
		span.style.fontWeight = 'bold';
		span.style.fontSize = '1.1em';
		return span;
	}
	function locate(event)
	{
		var posx, posy;
		var d = find_window();
		posx = event.clientX + window.pageXOffset;
		posy = event.clientY + window.pageYOffset;
		d.style.top = (posy - 23) + "px";
		d.style.left = (posx + 23) + "px";
	}
	
	function find_window()
	{
		return document.getElementById("link_tt");
	}
	
	function create_window(id)
	{
		var tt_div = document.createElement("div");
		tt_div.setAttribute("id", "link_tt");
		tt_div.setAttribute("style", "background:" + bg_color + ";border:1px solid " + border_color + ";padding:2px;color:" + font_color + ";font-family:" + font_face + ";font-size:" + font_size + ";position:absolute;z-index:1000;")
		var tt_url = document.createTextNode(id.href);
		tt_div.appendChild(tt_url);
		document.body.appendChild(tt_div);

		if (!id.href.match(/^javascript:/)) {

			setTimeout(function(){

				if (!find_window())
					return;
				if (!tt_div.parentNode)
					return;

				var subjectUrl = id.href;
				if (subjectUrl.substring(0,1)=="/") {
					subjectUrl = document.location.protocol + "://" + document.location.domain + "/" + subjectUrl;
				}
				if (!subjectUrl.match("://")) {
					subjectUrl = document.location.path + "/" + subjectUrl;
				}
				GM_log("subjectUrl="+subjectUrl);
				var subjectHost = subjectUrl.match(/[^:]*:[/][/][^/]*[/]/)[0];

				function tryLookup(lookupURL,onFailFn) {
					var jsonUrl = 'http://feeds.delicious.com/v2/json/urlinfo?url=' + encodeURIComponent(lookupURL);
					GM_xmlhttpRequest({
						method: "GET",
						url: jsonUrl,
						headers: {
							"Accept":"text/json"
						},
						onload: function(response){
							GM_log("Got response="+response);
							GM_log("Got responseText="+response.responseText);
							// var obj = JSON.parse(response.responseText); // @todo @require JSON!
							var obj = eval(response.responseText);
							GM_log("Got pre obj="+obj);
							if (obj)
								obj = obj[0];
							GM_log("Got post obj="+obj);
							if (obj) {
								tt_div.appendChild(document.createElement('BR'));
								if (obj.url != subjectUrl) {
									if (obj.url == subjectHost) {
										tt_div.appendChild(document.createTextNode("Results for website"));
									} else {
										tt_div.appendChild(document.createTextNode("Results for: "+obj.url));
									}
									tt_div.appendChild(document.createElement('BR'));
								}
								tt_div.appendChild(document.createTextNode("Title: "));
								tt_div.appendChild(boldTextElement(obj.title));
								tt_div.appendChild(document.createElement('BR'));
								tt_div.appendChild(document.createTextNode("Popularity: "));
								tt_div.appendChild(boldTextElement(""+obj.total_posts));
								tt_div.appendChild(document.createElement('BR'));
								var tags = "";
								for (var tag in obj.top_tags) {
									tags += (tags==""?"":", ") + tag;
								}
								tt_div.appendChild(document.createTextNode("Tags: "+tags+""));
								tt_div.style.width = '300px';
							} else {
								if (onFailFn) {
									onFailFn();
								} else {
									tt_div.appendChild(document.createElement('BR'));
									tt_div.appendChild(document.createTextNode("No Info"));
								}
							}
						}
					});
				}

				tryLookup(subjectUrl,function(){
					setTimeout(function(){
						tryLookup(subjectHost);
					},1000);
				});

			},2000);

		}

	}
	
	function kill_window()
	{
		var elem = find_window();
		if (elem) elem.parentNode.removeChild(elem);
	}
	
	function create_event(id)
	{	
		id.addEventListener("mouseover", function() { create_window(id); }, false);
		id.addEventListener("mouseout", function() { kill_window(); }, false);
		id.addEventListener("mousemove", function(event) { locate(event); }, true);
	}
	
	var link = document.getElementsByTagName("a");
	for (i = 0; i < link.length; i++)
	{
		create_event(link[i]);
	}
})()


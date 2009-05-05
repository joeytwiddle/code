scr_meta=<><![CDATA[
// ==UserScript==
// @name          UnFuck Facebook
// @namespace     sizzlemctwizzle
// @description	  An update of the classic unfuck script. Now with a two column homepage layout!
// @version       2.2.6
// @include       http://*.facebook.com/*
// @include       https://*.facebook.com/*
// ==/UserScript==
]]></>;

// I've mananged to make most of my script a user style
// Hides
//   -People You Don't Know And Never Want to Know
//   -third party application profile boxes
//   -gifts
//   -third party wall attachment links
//   -nagging to invite users on hompage
//   -advertisements
//   -rounded image corners
//   -sponser highlight
//   -"Home" nav link
//   -Your name nav link
//   -find friends link(use the search if you wanna find someone)
//   -exapnd the filters
GM_addStyle(<><![CDATA[
#right_column { width: 77% !important; }
div[id^="box_app_"]:not(#box_app_2297529396):not(#box_app_2305272732):not(#box_app_2309869772):not(#box_app_2327158227):not(#box_app_2341989679):not(#box_app_2347471856):not(#box_app_2356318349):not(#box_app_2361831622):not(#box_app_2407511955):not(#box_app_2503140832):not(#box_app_2719290516):not(#box_app_2392950137):not(#box_app_2550392059), .gifts_received, .wall_contextual_extra, .nextstep, .app_icon_row, .invitefriends, #ssponsor, div[class$="_ad"], .divider_bar, .more_section, .fbpage_fan, .ad_capsule, .see_more_arrow, #more_apps_divider_narrow, .platform, .profile_empty_divider, .newstuff, .app_install_story, .emu_sponsor, div[id^="emu_"], .adcolumn, .social_ad, .sponsor, #attachment_buttons_list span[style*="app_"], li[id^="bookmarked_app_"] a[href^="http://apps."], li[id^="bookmarked_app_"] a[href*="gift"], #profile_tab_add, .footer_ad, .UIComposer_More_Container, .UIComposer_Attachment a[style*="gift.gif"], li[view="box_3"], li[view^="app_"], .approve_friend, #fb_menu_home, .app_story, .UIComposer_Attachment a[style*="/app_"], #fb_menu_account, .UIRoundedImage_Corners, #home_sponsor, .UIConnectWithFriends, .UIUpcoming_Info small, .UIMutableFilterList_Toggle,.UIMutableFilterList_Separator, .UIMutableFilterList_Tip, .house_sponsor, div[style*="white"], #pymk_hp_box, .UIComposer_Prompt, .comments_add_box_image .UIRoundedImage, #unfucked { display: none !important; } 
.UIFilterList { padding-bottom: 0px !important; } 
.request_link { padding: 5px !important; }
#wallpage { width: 700px !important; }
#wall_text { height: 80px !important; }
#home_filter_list { display: block !important; float: none !important; width: 100% !important; padding-bottom: 20px !important; }
#home_stream { width: 650px !important; }
.UIMutableFilterList_HiddenSection { opacity: 100 !important; display: block !important; }
.inline_comment_buttons { padding-left: 0px !important; }
.request_link { padding-left: 20px !important; }
]]></>.toString());

// Remove an element
function destroy(element) { element.parentNode.removeChild(element); }
// Get element by id
function $(element) { return document.getElementById(element); }
// Get elements by class
function $c(element) { return document.getElementsByClassName(element); }
// Get a single node with xpath
function single(x) { return document.evaluate(x, document, null,  9, null).singleNodeValue; }
// Get a list of nodes with xpath
function multi(x) { return document.evaluate(x, document, null, 6, null); }
// nextSibling whitespace skipper that I wrote
function nextSibling(startSib){
  if (!(nextSib=startSib.nextSibling)) return false;
  while (nextSib.nodeType!=1) if (!(nextSib=nextSib.nextSibling)) return false;
  return nextSib;
}

// Destroy elements from the bottom up
// Pass it a xpath expression to find element
// Pass it an attribute and value of when to stop moving up
function seekAndDestroy(start, stop) {
  var stories = multi(start), story, i=stories.snapshotLength;
  stop = stop.split('=');
  while (upNode=stories.snapshotItem(--i)) {

    // Do not remove posts on your wall
    if ( (sib=nextSibling(upNode)) && (sib.textContent == $('fb_menu_account').firstChild.firstChild.textContent) ) {
      continueDestruction = false;
    } else {
      continueDestruction = true;
    }

    while ( (!new RegExp(stop[1]).test(upNode.getAttribute(stop[0]))) && (upNode.parentNode) && (continueDestruction) ) {
	story = upNode;
	upNode = story.parentNode;
	destroy(story);
      }
    }
}

// Find out where a share link really leads
function discoverShare(link) {
  var xmlhttp =  new XMLHttpRequest();
  xmlhttp.onreadystatechange = function() {
    if (xmlhttp.readyState==4 && xmlhttp.status==200) {
      if (page=/<iframe.*src="(.*?)"/i.exec(xmlhttp.responseText)[1]) {
	link.href = page;
	link.setAttribute('unmasked', '');
      }
    }
  }
  xmlhttp.open("GET", link.href, true);
  xmlhttp.send(null);
}

// Now we can actually block the application from the Feed
function blockApp(appId) {
    var xmlhttp =  new XMLHttpRequest();
    xmlhttp.open("POST", "http://www.facebook.com/ajax/feed/filter_action.php", true);
    var params = "value="+appId+"&action=unfollow&filter_key=app_"+appId+"&post_form_id="+$('post_form_id').value+"&post_form_id_source=AsyncRequest&nectar_impid="+unsafeWindow.env_get("nectar_last_impression_id")+'&nectar_navimpid='+unsafeWindow.Env["nectar_last_impression_id"];
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xmlhttp.setRequestHeader("Content-length", params.length);
    xmlhttp.setRequestHeader("Connection", "close");
    xmlhttp.send(params);
}

// My life would be a hell of a lot easier if CSS had reverse parent selectors
function unfuckFeed(stream) {
 // Ever notice how you can't Hide application stories? wtf?
  seekAndDestroy('//div[contains(@id, "div_story_")]//a[contains(@href, "http://apps.facebook.com/") or contains(@href, "http://apps.new.facebook.com/") or contains(@href, "http://www.facebook.com/pages/") or contains(@href, "http://quiz.applatform.com/")]', 'class=UIStream');

  // Wall stories = eavesdropping. Why would I care what people say to eachother?
  seekAndDestroy('//div[contains(@id, "div_story_")]//img[contains(@class, "UIIntentionalStory_Arrow")]', 'class=UIStream');
  stream.setAttribute('unfucked', '');
}

function unfuckFB() {
  // Unfuck the feeds
  var streams = multi('//div[contains(@class, "UIStream") and not(@unfucked)]'), stream, i=streams.snapshotLength;
  while(stream=streams.snapshotItem(--i)) unfuckFeed(stream);

  if ((!$('unfucked')) && ($('content'))) {
    // Move requests, filters, events, and pokes to the top of left column so we can ignore the Highlights section easier
    if(right=$('home_sidebar')) {
      if(pokes=$c('pokes')[0]) right.insertBefore(pokes, right.firstChild);
      if(events=$c('UIUpcoming')[0]) right.insertBefore(events.parentNode.parentNode, right.firstChild);

      // Now that everything is clean we have room to make the home page two columns
      // Inspired by http://userscripts.org/scripts/show/44687
      if (left=$('home_filter_list'))  right.insertBefore(left, right.firstChild);

      // Haha! Facebook took the hint and moved requests back to the right sidebar
      // Now all I gotta do is move them back to the top
      if (requests=$c('UIRequestsBox')[0]) right.insertBefore(requests.parentNode.parentNode, right.firstChild);
    }

  // Hide the fucking application filters(Some even show up when you don't have any apps installed yourself!)
    var apps = multi('//div[contains(@class, "UIFilterList_Item")]/a[contains(@href, "?filter=app_") or contains(@href, "?filter=pp")]'), app, i=apps.snapshotLength;
    while(app=apps.snapshotItem(--i)) if(!/(2361831622|2344061033|2347471856|2392950137|2309869772|2305272732)/.test(app.href)) {
	if (app.href.indexOf('app_') != -1) blockApp(appId=app.href.split('app_')[1]); // Any application in the filters is blocked
	destroy(app.parentNode);
      }

    // App and Fan Highlights
    seekAndDestroy('//div[contains(@class, "UIHotStory")]//a[contains(@href, "http://www.facebook.com/apps/")] | //div[contains(@class, "UIHotStory")]//img[contains(@class, "sx_fbpage")]', 'class=UIHotStream');

    // Remove the thumbnail and expand the composer
    if (homeStream=$('home_stream') && (pic=$c('UIComposer_Pic')[0]) && (compose=$c('UIComposer_UIRoundedBox')[0])) {
      compose.style.marginLeft = "-59px";
      compose.style.width = "650px";
      /*breaks = 4; // This code moves the composer down to make room for status next to pic
      while(breaks--) homeStream.insertBefore(document.createElement('br'), homeStream.firstChild);
      homeStream.insertBefore(pic, homeStream.firstChild);*/
      destroy(pic); // Only remove the pic as long as status is not there
    }

    // Show older posts on home page
    if (more=$c('PagerMoreLink')[0]) {
      var checker=setInterval(function(){
	  if(unsafeWindow.__UIControllerRegistry) {
	    clearInterval(checker);
	    code = $c('PagerMoreLink')[0].getAttribute('onclick');
	    if (trySplit=code.split('{')[1]) code = trySplit;
	    document.location.href = 'javascript:(function() { void(' + code.split(';')[0] + '); })();';
	  }
	},200);
    }

    // Only unfuck the page once
    unfucked = document.createElement('div');
    unfucked.id = 'unfucked';
    $('content').appendChild(unfucked);
  }

  // Direct links http://userscripts.org/scripts/show/40691
  var links = multi('//a[contains(@href, "http://www.facebook.com/ext/share.php") and not(@unmasked)]'), link, i=links.snapshotLength;
  while(link=links.snapshotItem(--i)) discoverShare(link, link.getAttribute('href'));

  // Remove listenters
  var listeners = multi('//a[contains(@onclick, "ft(")]'), listener, i=listeners.snapshotLength;
  while(listener=listeners.snapshotItem(--i)) listener.removeAttribute('onclick');
}
// Re-run my code when the page changes, wouldn't have to do this if I could use CSS
function process() {
  document.documentElement.removeEventListener('DOMNodeInserted', process, false);
  unfuckFB();
  document.documentElement.addEventListener("DOMNodeInserted", process, false);
}

// Start the listener
process();

CheckScriptForUpdate = {
 id: '11992',
 days: 2,
 name: /\/\/\s*@name\s*(.*)\s*\n/i.exec(scr_meta)[1],
 version: /\/\/\s*@version\s*(.*)\s*\n/i.exec(scr_meta)[1],
 time: new Date().getTime() | 0,
 call: function(response) {
    GM_xmlhttpRequest({
      method: 'GET',
	  url: 'http://userscripts.org/scripts/source/'+this.id+'.meta.js',
	  headers: {
	  'User-agent': window.navigator.userAgent,
	    'Accept': 'application/atom+xml,application/xml,text/xml',
	    },
	  onload: function(xpr) {CheckScriptForUpdate.compare(xpr,response);}
      });
  },
 compare: function(xpr,response) {
    this.xversion=/\/\/\s*@version\s*(.*)\s*\n/i.exec(xpr.responseText)[1];
    this.xname=/\/\/\s*@name\s*(.*)\s*\n/i.exec(xpr.responseText)[1];
    if ( (this.xversion != this.version) && (confirm('A new version of the '+this.xname+' user script is available. Do you want to update?')) ) {
      GM_setValue('updated', this.time);
      GM_openInTab('http://userscripts.org/scripts/source/'+this.id+'.user.js');
    } else if ( (this.xversion) && (this.xversion != this.version) ) {
      if(confirm('Do you want to turn off auto updating for this script?')) {
	GM_setValue('updated', 'off');
	GM_registerMenuCommand("Auto Update "+this.name, function(){GM_setValue('updated', new Date().getTime() | 0);CheckScriptForUpdate.call('return');});
	alert('Automatic updates can be re-enabled for this script from the User Script Commands submenu.');
      } else {
	GM_setValue('updated', this.time);
      }
    } else {
      if(response) alert('No updates available for '+this.name);
      GM_setValue('updated', this.time);
    }
  },
 check: function() {
if (GM_getValue('updated', 0) == 0) GM_setValue('updated', this.time);
if ( (GM_getValue('updated', 0) != 'off') && (+this.time > (+GM_getValue('updated', 0) + (1000*60*60*24*this.days))) ) {
      this.call();
    } else if (GM_getValue('updated', 0) == 'off') {
      GM_registerMenuCommand("Enable "+this.name+" updates", function(){GM_setValue('updated', new Date().getTime() | 0);CheckScriptForUpdate.call(true);});
    } else {
      GM_registerMenuCommand("Check "+this.name+" for updates", function(){GM_setValue('updated', new Date().getTime() | 0);CheckScriptForUpdate.call(true);});
    }
    }
};
if (self.location == top.location && GM_xmlhttpRequest) CheckScriptForUpdate.check();
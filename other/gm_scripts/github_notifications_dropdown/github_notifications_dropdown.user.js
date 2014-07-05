// ==UserScript==
// @name           Github Notifications Dropdown
// @namespace      joeytwiddle
// @copyright      2014, Paul "Joey" Clark (http://neuralyte.org/~joey)
// @version        0.5.2
// @description    When clicking the notifications icon, displays notifications in a dropdown pane, without leaving the current page.
// @include        https://github.com/*
// @grant          none
// ==/UserScript==

var notificationButton = $(".header a.notification-indicator[href]");

var targetPage;

function listenForNotificationClick(){
	notificationButton.on("click", notificationButtonClicked);
	function notificationButtonClicked(evt){
		// Act normally, do nothing, if modifier key is pressed.
		if (evt.ctrlKey || evt.shiftKey || evt.metaKey) {
			return;
		}
		evt.preventDefault();
		notificationButton.css({
			"opacity": "0.3",
			"background-color": "#ececec",
			"background-image": "linear-gradient(#d9d9d9, #ececec)"
		});
		// Had to use .on and .off here because .one was firing multiple times, dunno why.  O_o
		notificationButton.off("click", notificationButtonClicked);
		targetPage = notificationButton.attr('href');
		$.get(targetPage).then(receiveNotificationsPage).fail(receiveNotificationsPage);
	}
}

function receiveNotificationsPage(data, textStatus, jqXHR){
	notificationButton.css("opacity", "");

	var notificationPage = $("<div>").append( $.parseHTML(data) );
	var notificationsList = notificationPage.find(".notifications-list");
	var notificationsDropdown = $("<div>").addClass("notifications-dropdown");
	var title = "Notifications";
	if (targetPage != "/notifications") {
		title += " for " + targetPage.replace(/^\/+|\/notifications$/g,'');
	}
	$("<h3><center>" + title + "</center></h3>").appendTo(notificationsDropdown);
	// Provide hover text for all links, so if the text is too long to display, it can at least be seen on hover.
	notificationsList.find("a").each(function(){
		$(this).attr("title", $(this).text().trim());
	});
	var minWidth = Math.min(700, window.innerWidth-32);
	if (notificationsList.children().length == 0) {
		notificationsDropdown.append("<center>No new notifications</center>");
		minWidth = 0;
	}
	notificationsDropdown.append(notificationsList);
	var linkToPage = '/notifications';
	//var linkToPage = targetPage;
	var seeAll = $("<center><b><a href='"+encodeURI(linkToPage)+"'>Notifications page</a></b></center>").css({
		"margin-top": "-12px"
	});
	notificationsDropdown.append(seeAll);

	var arrowSize = 10;

	$("<style>").html(""
	  + " .notifications-dropdown { "
	  + "   border: 1px solid #ddd; "
	  + "   background-color: #fff; "
	  + "   padding: 2px 16px; "
	  + "   box-shadow: 0px 2px 8px 1px rgba(0,0,0,0.25); "
	  + "   border-radius: 24px; "
	  //+ "   max-height: 90%; "
	  + "   margin-bottom: 20px; "   // If the body is shorter than the dropdown, the body will expand to let it fit, but only just.  This will ensure a little bit of extra space is available for the shadow and a small gap.
	  + "   z-index: 10; "           // To appear above the .bootcamp .desc on the front page.
	  + " } "
	  + " .notifications-dropdown > center { "
	  + "   padding: 8px 8px; "
	  + " } "
	  + " .notifications-dropdown .notifications-list .box { "
	  + "   margin-bottom: 16px; "
	  + " } "
	  + " .notifications-dropdown .notifications-list { "
	  + "   float: initial; "
	  + " } "
	  // There is a rule on the user profile page that applies to the notification ticks (which are usually never seen on that page).  The rule matches `body.page-profile .box-header .tooltipped`.
	  // That rule messes up the position of each tick icon relative to its containing header.  So we override to the previous values.
	  + " .notifications-dropdown .box-header .mark-all-as-read { "
	  + "   top: auto !important; "
	  + "   left: auto !important; "
	  + "   right: auto !important; "
	  + "   bottom: auto !important; "
	  + "   float: right; "
	  + " } "
	  + " .notifications-dropdown-arrow { "
	  + "   position: absolute; "
	  + "   width: 0px; "
	  + "   height: 0px; "
	  + "   border-left: "+arrowSize+"px solid transparent; "
	  + "   border-right: "+arrowSize+"px solid transparent; "
	  + "   border-bottom: "+arrowSize+"px solid white; "
	  + "   z-index: 20; "
	  + " } "
	).appendTo("body");

	notificationsDropdown.css({
		position: "absolute",   // Must be set before we can read width accurately
		"min-width": minWidth+"px",
		//overflow: "auto",
	}).appendTo("body"); // Done sooner so we can get its width
	var topOfDropdown = notificationButton.offset().top + notificationButton.height() - 2;
	var leftOfDropdown = notificationButton.offset().left + notificationButton.width()/2 - notificationsDropdown.width()/2;
	leftOfDropdown = Math.max(leftOfDropdown, 12);
	notificationsDropdown.css({
		top: topOfDropdown + "px",
		left: leftOfDropdown + "px",
		//"max-height": "calc(100% - "+(topOfDropdown+8)+"px)",
	});

	// This little white wedge should lead from the notification button to the title of the dropdown, +1 pixel lower in order to overlap the top border.  I cannot explain why we need the +2!
	var tabArrow = $("<div>").addClass("notifications-dropdown-arrow").css({
		left: (notificationButton.offset().left + notificationButton.width()/2 - arrowSize + 2) + "px",
		top: (topOfDropdown - arrowSize + 1) + "px",
	}).appendTo("body");

	makeBlocksCollapsable(notificationsDropdown);

	function listenForCloseNotificationDropdown(){
		var closeClickTargets = $("body, .header a.notification-indicator[href]");
		closeClickTargets.on("click", considerClosingNotificiationDropdown);
		function considerClosingNotificiationDropdown(evt){
			if ($(evt.target).closest(".notifications-dropdown").length){
				// A click inside the dropdown doesn't count!
			} else {
				evt.preventDefault();
				// We must use .on and .off because .one will fire once per element per event type!
				closeClickTargets.off("click", considerClosingNotificiationDropdown);
				notificationsDropdown.remove();
				tabArrow.remove();
				notificationButton.css({
					"background-color": "",
					"background-image": ""
				});
				listenForNotificationClick();
			}
		}
	}

	listenForCloseNotificationDropdown();
}

function makeBlocksCollapsable(parentElement){
	$(".box-header", parentElement).click(function(e){
		if ($(e.target).closest(".mark-all-as-read").length) {
			$(this).next(".box-body").slideUp(150);
		} else if (e.target === this) {
			$(this).next(".box-body").slideToggle(150);
		}
	}).css({ cursor: "pointer" });
}

listenForNotificationClick();

// If we are on the notifications page, add our feature there too
makeBlocksCollapsable(document.body);

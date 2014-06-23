// ==UserScript==
// @name           Github Notifications Dropdown
// @namespace      joeytwiddle
// @description    When clicking the notifications icon, displays notifications in a dropdown pane, without leaving the current page.
// @include        https://github.com/*
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
		notificationButton.css("opacity", "0.3");
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
	$("<center><b>Notifications</b></center>").appendTo(notificationsDropdown);
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
	var seeAll = $("<center><b><a href='"+encodeURI(linkToPage)+"'>See All</a></b></center>");
	notificationsDropdown.append(seeAll);
	notificationsDropdown.css({
		position: "absolute",
		border: "1px solid #ccc",
		padding: "2px 0px",
		"background-color": "#fff",
		"box-shadow": "0px 3px 8px 0px rgba(0,0,0,0.25)",
		"min-width": minWidth+"px",
		"max-height": "100%",
		overflow: "auto",
	});
	$("body").append(notificationsDropdown); // Done sooner so we can get its width
	var top = notificationButton.offset().top + notificationButton.height();
	var left = notificationButton.offset().left + notificationButton.width()/2 - notificationsDropdown.width()/2;
	left = Math.max(left, 4);
	notificationsDropdown.css({
		top: top + "px",
		left: left + "px",
		"max-height": "calc(100% - "+top+"px)",
	});
	GM_addStyle(".notifications-dropdown > center { padding: 8px 8px; } "
	          + ".notifications-dropdown .notifications-list .box { margin-bottom: 4px; } ");

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
				listenForNotificationClick();
			}
		}
	}

	listenForCloseNotificationDropdown();
}

listenForNotificationClick();

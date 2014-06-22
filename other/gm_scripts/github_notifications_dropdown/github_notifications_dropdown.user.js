// ==UserScript==
// @name           Github Notifications Dropdown
// @namespace      joeytwiddle
// @description    When clicking the notifications icon, displays notifications in a dropdown pane, without leaving the current page.
// @include        https://github.com/*
// ==/UserScript==

var notificationButton = $(".header a[href='/notifications']");

function listenForNotificationClick(){
	notificationButton.on("click", notificationButtonClicked);
	function notificationButtonClicked(evt){
		evt.preventDefault();
		notificationButton.css("opacity", "0.3");
		// Had to use .on and .off here because .one was firing multiple times, dunno why.  O_o
		notificationButton.off("click", notificationButtonClicked);
		$.get("/notifications").then(receiveNotificationsPage);
	}
}

function receiveNotificationsPage(data, textStatus, jqXHR){
	notificationButton.css("opacity", "");

	var notificationPage = $("<div>").append( $.parseHTML(data) );
	var notificationsList = notificationPage.find(".notifications-list");
	var notificationsDropdown = $("<div>").addClass("notifications-dropdown");
	notificationsDropdown.append(notificationsList);
	if (notificationsList.children().length == 0) {
		notificationsDropdown.append("<center>No new notifications</center>");
	}
	var seeAll = $("<center><b><a href='/notifications'>Notifications page</a></b></center>");
	notificationsDropdown.append(seeAll);
	notificationsDropdown.css({
		position: "absolute",
		border: "1px solid black",
		padding: "2px 6px",
		"background-color": "#dddddd",
		"box-shadow": "0px 3px 8px 0px rgba(0,0,0,0.25)",
	});
	$("body").append(notificationsDropdown); // Done sooner so we can get its width
	var top = notificationButton.offset().top + notificationButton.height();
	var left = notificationButton.offset().left + notificationButton.width()/2 - notificationsDropdown.width()/2;
	left = Math.max(left, 0);
	notificationsDropdown.css({
		top: top + "px",
		left: left + "px",
	});

	function listenForCloseNotificationDropdown(){
		var listeners = $("body, .header a[href='/notifications']");
		listeners.on("click", considerClosingNoficiationDropdown);
		function considerClosingNoficiationDropdown(evt){
			if ($(this).closest(".notifications-dropdown").length){
				// A click inside the dropdown doesn't count!
			} else {
				evt.preventDefault();
				// We must use .on and .off because .one will fire once per element per event type!
				listeners.off("click", considerClosingNoficiationDropdown);
				notificationsDropdown.remove();
				listenForNotificationClick();
			}
		}
	}

	listenForCloseNotificationDropdown();
}

listenForNotificationClick();


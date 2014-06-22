// ==UserScript==
// @name           Github Notifications Dropdown
// @namespace      joeytwiddle
// @description    When clicking the notifications icon, shows new notifications in a dropdown, rather than travelling to the /notifications page.
// @include        https://github.com/*
// ==/UserScript==

var notificationButton = $(".header a[href='/notifications']");

function listenForNotificationClick(){
	console.log("Listening for open");
	notificationButton.on("click", notificationButtonClicked);
	function notificationButtonClicked(evt){
		console.log("Open clicked.");
		evt.preventDefault();
		notificationButton.css("opacity", "0.3");
		// Had to use .on and .off here because .one was firing multiple times.  O_o
		notificationButton.off("click", notificationButtonClicked);
		$.get("/notifications").then(receiveNotificationsPage);
	}
}

function receiveNotificationsPage(data, textStatus, jqXHR){
	notificationButton.css("opacity", "");

	var notificationPage = $("<div>").append( $.parseHTML(data) );
	var notificationsList = notificationPage.find(".notifications-list");
	var notificationsDropdown = $("<div>").addClass("notifications-dropdown");
	var seeAll = $("<center><a href='/notifications'>See all</a></center>");
	notificationsDropdown.append(seeAll);
	notificationsDropdown.append(notificationsList);
	notificationsDropdown.css({
		position: "absolute",
		border: "1px solid black",
		margin: "2px solid white",
		"background-color": "#dddddd",
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


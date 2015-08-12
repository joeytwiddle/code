javascript:(function(){
	function addStyle(css) {
		var node = document.createElement("style");
		node.type = "text/css";
		node.innerHTML = css;
		document.getElementsByTagName("head")[0].appendChild(node);
	}
	addStyle("._33c { display: none; } ._33c._4af { display: block; }");
	var toggleButton = document.querySelector("[data-target=fbNotificationsFlyout]");
	if (!toggleButton.parentNode.className.match(/\bopenToggler\b/)) {
		toggleButton.click()
	}
	setTimeout(function(){
		var unreadNotifications = document.querySelectorAll("#fbNotificationsFlyout ul li._4af a");
		console.log(unreadNotifications.length+" unread notifications");
		if (unreadNotifications.length === 0) {
			/*document.location = "/notifications";*/
		} else {
			var firstUnread = unreadNotifications[0];
			var markAsReadButton = firstUnread.parentNode.querySelector("[aria-label='Mark as Read']");
			markAsReadButton.click();
			firstUnread.firstChild.click();
		}
	},1000);
}());

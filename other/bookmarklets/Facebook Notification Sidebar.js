javascript:(function(){
	function addStyle(css) {
		var node = document.createElement("style");
		node.type = "text/css";
		node.innerHTML = css;
		document.getElementsByTagName("head")[0].appendChild(node);
	}
	/* Open notifications if not already open */
	var toggleButton = document.querySelector("[data-target=fbNotificationsFlyout]");
	if (!toggleButton.parentNode.className.match(/\bopenToggler\b/)) {
		toggleButton.click()
	}
	addStyle("#fbNotificationsFlyout { position: fixed; left: 0px; }");
	/* Keep it always open */
	addStyle("#fbNotificationsFlyout { display: block; }");
	addStyle(".jewelBeeperHeader { display: none; }");
	addStyle("#fbNotificationsFlyout       { width: 66px; }");
	addStyle("#fbNotificationsFlyout:hover { width: 430px; }"); /* TODO: Pull this from CSS / clientWidth */
	addStyle("#fbNotificationsFlyout       { transition: all 200ms ease-in 500ms; }");
	addStyle("#fbNotificationsFlyout:hover { transition: all 100ms ease-in 10ms; }");
	/* When the parent shrinks (unhovered), the header wraps onto multiple lines and becomes taller */
	/* Ideally it would just be cropped, but I haven't worked out how to do that yet. */
	addStyle(".__tw .jewelHeader { display: none; }");
	/* Stronger highlight of unread notifications. */
	/* Original: rgba(237, 239, 245, 0.98) */
	/* Still the same original hue (blue) */
	/*addStyle(".__tw .jewelHeader { background: rgba(180, 187, 210, 0.98); }"); */
	/* Orange */
	/* Both of these should be a bit lighter. (In fact the second one could replace the first.) */
	addStyle("._4af ._33e { background: rgba(234, 182, 126, 0.98); }");
	/* But when hovered, it also need styling. */
	addStyle("._33c:hover ._33e { background: rgba(193, 143, 88, 0.98); }");
	addStyle(", ._33c ._33e:focus { background: rgba(193, 143, 88, 0.98); }");
	/* Make the scrollbar grip always visible when the sidebar is open */
	addStyle("#fbNotificationsFlyout:hover .uiScrollableAreaTrack { visibility: visible !important; opacity: 1 !important; }");
}());

javascript:(function(){
	var f = document.createElement('iframe');
	f.id = 'floatingFrame1';
	f.style.zIndex = 1000;
	f.style.position = 'fixed';
	f.style.width = window.innerWidth * 3/4 + 'px';
	f.style.height = window.innerHeight * 3/4 + 'px';
	f.style.bottom = 8 + 'px';
	f.style.right = 8 + 'px';
	f.style.backgroundColor = '#000088';
	var links = document.getElementsByTagName('A');
	var hoverHandler = function (evt) {
		var linkNode = evt.currentTarget;
		var originalLinkProtocol = linkNode.protocol;
		if (document.location.protocol === 'https:' && originalLinkProtocol === 'http:') {
			linkNode.protocol = 'https:';
		}
		document.getElementById('floatingFrame1').src = linkNode.href;
		linkNode.protocol = originalLinkProtocol;
	};
	for (var i = 0; i < links.length ; i++) {
		links[i].onmouseover = hoverHandler;
	}
	document.body.appendChild(f);
}());

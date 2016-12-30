javascript:(function(){
	var f = document.createElement('iframe');
	f.style.zIndex = 1000;
	f.style.position = 'absolute';
	f.style.width = window.innerWidth * 3/4;
	f.style.height = window.innerHeight * 3/4;
	f.style.top = 20;
	f.style.right = 20;
	f.id = 'floatingFrame1';
	var links = document.getElementsByTagName('A');
	for (var i = 0; i < links.length ; i++) {
		links[i].onmouseover = eventHandler;
	function eventHandler(evt) {
		var linkNode = evt.currentTarget;
		document.getElementById('floatingFrame1').src = linkNode.href;
	}
	}
	document.body.appendChild(f);
}());

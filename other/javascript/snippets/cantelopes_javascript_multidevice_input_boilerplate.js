vars.canvas.oncontextmenu = function (e) { e.preventDefault(); };
vars.canvas.addEventListener("mousemove", function(e){
	var rect = vars.canvas.getBoundingClientRect();
	vars.mx = round((e.clientX-rect.left)/(rect.right-rect.left)*vars.canvas.width);
	vars.my = round((e.clientY-rect.top)/(rect.bottom-rect.top)*vars.canvas.height);
}, true);
vars.canvas.addEventListener("mousedown", function(e){
	switch(e.which){
		case 1: vars.leftButton=1; break;
		case 3: vars.rightButton=1; break;
	}
}, true);
vars.canvas.addEventListener("mouseup", function(e){
	switch(e.which){
		case 1: vars.leftButton=0; break;
		case 3: vars.rightButton=0; break;
	}
}, true);
vars.canvas.addEventListener("mousewheel", function(e){
	var e = window.event || e; // old IE support
	vars.wheelDelta = max(-1, min(1, e.wheelDelta/120));
	vars.cursorDepth+=vars.wheelDelta*2;
	if(vars.cursorDepth<2)vars.cursorDepth=2;
	if(vars.cursorDepth>50)vars.cursorDepth=50;
}, true);
vars.canvas.addEventListener("DOMMouseScroll", function(e){
	var e = window.event || e; // old IE support
	vars.wheelDelta = max(-1, min(1, -e.detail));
	vars.cursorDepth+=vars.wheelDelta*2;
	if(vars.cursorDepth<2)vars.cursorDepth=2;
	if(vars.cursorDepth>50)vars.cursorDepth=50;
}, true);
vars.canvas.addEventListener("touchstart", function(e){
	vars.leftButton=1;
	e.preventDefault();
	var rect = vars.canvas.getBoundingClientRect();
	vars.mx = round((e.changedTouches[0].pageX-rect.left)/(rect.right-rect.left)*vars.canvas.width);
	vars.my = round((e.changedTouches[0].pageY-rect.top)/(rect.bottom-rect.top)*vars.canvas.height);
}, true);
vars.canvas.addEventListener("touchend", function(e){
	vars.leftButton=0;
}, true);
vars.canvas.addEventListener("touchmove", function(e){
	e.preventDefault();
	var rect = vars.canvas.getBoundingClientRect();
	vars.mx = round((e.changedTouches[0].pageX-rect.left)/(rect.right-rect.left)*vars.canvas.width);
	vars.my = round((e.changedTouches[0].pageY-rect.top)/(rect.bottom-rect.top)*vars.canvas.height);
}, true);

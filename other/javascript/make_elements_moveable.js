// TODO: Acts on all sub-elements, which is good if elem is say a DIV filled with an image or some text.
// But we could give be some option to skip dragging via clicking on sub-elements, maybe by depth or tag.
// Some basic heuristics could be: don't let user drag if they are on a link/button/input/textarea.
// Alternatively, only start moving element if mouse is held down, allowing normal clicks without activation.

function makeMoveable(elem) {

	var elemStartLeft,elemStartTop;
	var mouseStartX,mouseStartY;

	var defaultBorderStyle = elem.style.borderStyle;

	function startMove(evt) {
		// Certain elements we want the user to interact with normally.
		var node = evt.target;
		while (node && node!=elem) {
			if (node.tagName=='INPUT' || node.tagName=='TEXTAREA' || node.tagName=='A') {
				return;
			}
			node = node.parentNode;
		}

		evt.preventDefault(); // Otherwise we highlight the text below!

		mouseStartX = evt.clientX + window.scrollX;
		mouseStartY = evt.clientY + window.scrollY;
		elemStartLeft = parseInt(elem.style.left);
		elemStartTop = parseInt(elem.style.top);
		document.addEventListener("mousemove", doMove,   true);
		document.addEventListener("mouseup",   endMove, true);

		// elem.style.padding = (parseInt(""+elem.style.padding)-1)+'px';
		// elem.style.left = (parseInt(""+elem.style.left)+1)+'px';
		// elem.style.top = (parseInt(""+elem.style.top)+1)+'px';
		elem.style.borderStyle = 'dashed';
	}

	function doMove(evt) {
		evt.preventDefault();
		var mouseX = window.scrollX + evt.clientX;
		var mouseY = window.scrollY + evt.clientY;
		var newLeft = (elemStartLeft + mouseX-mouseStartX);
		var newTop  = (elemStartTop  + mouseY-mouseStartY);
		// if (newLeft>=0 && newTop>=0) {
			elem.style.left = newLeft +"px";
			elem.style.top = newTop + "px";
		// }
	}

	function endMove(evt) {
		document.removeEventListener("mousemove", doMove,   true);
		document.removeEventListener("mouseup",   endMove, true);

		// elem.style.padding = (parseInt(elem.style.padding)+1)+'px';
		// elem.style.left = (parseInt(""+elem.style.left)-1)+'px';
		// elem.style.top = (parseInt(""+elem.style.top)-1)+'px';
		// elem.style.borderStyle = 'solid';
		elem.style.borderStyle = defaultBorderStyle;
	}

	elem.addEventListener('mousedown', startMove, false);

	elem.style.position = 'fixed';
	if (!elem.style.left)
		elem.style.left = elem.style.offsetLeft;
	if (!elem.style.top)
		elem.style.top = elem.style.offsetTop;
	elem.style.right = '';
	elem.style.bottom = '';

	// Optional:
	elem.style.cursor = 'move';
	// elem.style.border = '1px solid black';
	// elem.style.background = '#f8f8f8';

}


// TODO: We cannot select some nodes because they are not visible, for example
// their only child node might fill its parent entirely, making it un-selectable.
// Rather than moving directly to the next node moused over, we could try to
// move from current node to target node, 1 node at a time, updating when mouse
// moves 2 pixels.  This way the user can slow their movement to select an
// intermediary node.
// Should we also think about how to highlight nodes which have no style?
// BUG: Due to the selection changing the size of the border, sometimes we can be
// stuck in a loop, with the cursor still, but two selections alternating.
// FIXED: In Konq I think, or in Moz alongside SplitPreview, a bug occurs
// where the script is re-run.  This is probably because the SCRIPT element
// added to the page gets moved into the hidden div.  But since scripts are
// invisible, this is unneccessary, so don't do it!

(function(){

	function focus(elem) {
		var showingDiv = document.createElement('DIV');
		var hiddenDiv = document.createElement('DIV');
		hiddenDiv.style.display = 'none';
		var positionMarker = document.createElement('A');
		// positionMarker.id = 'positionMarker';
		elem.parentNode.insertBefore(positionMarker,elem);
		showingDiv.appendChild(elem);
		/*while (document.body.firstChild) {
			hiddenDiv.appendChild(document.body.lastChild);
		}*/
		var kids = [];
		for (var i=0;i<document.body.childNodes.length;i++) {
			kids.push(document.body.childNodes.item(i));
		}
		for (var i=0;i<kids.length;i++) {
			var child = kids[i];
			if (child.tagName != 'SCRIPT') {
				hiddenDiv.appendChild(child);
				// i--;
			}
		}
		var undoLink = document.createElement('A');
		undoLink.textContent = 'Restore Whole Page';
		undoLink.href = 'javascript:void()';
		undoLink.onclick = function(){
			positionMarker.parentNode.insertBefore(elem,positionMarker);
			positionMarker.parentNode.removeChild(positionMarker);
			document.body.removeChild(showingDiv);
			while (hiddenDiv.firstChild) {
				document.body.appendChild(hiddenDiv.firstChild);
			}
			document.body.removeChild(hiddenDiv);
			document.body.removeChild(undoLink);
		};
		document.body.appendChild(undoLink);
		document.body.appendChild(showingDiv);
		document.body.appendChild(hiddenDiv);
	}

	function watchForFocus() {
		var focusedElement = null;
		var oldBorder = "";
		function checkMouseOver(evt) {
			if (focusedElement && focusedElement.style)
				focusedElement.style.border = oldBorder;
			focusedElement = evt.target;
			window.status = "focusedElement = "+evt.target;
			if (focusedElement.style) {
				oldBorder = ""+focusedElement.style.border;
				if (oldBorder == "")
					oldBorder = "0px solid white"; // For Konq which won't clear on "".
				focusedElement.style.border = '2px dashed blue';
			}
			evt.preventDefault();
		}
		function checkMouseOut(evt) {
			if (focusedElement==evt.target && focusedElement.style)
				focusedElement.style.border = oldBorder;
			focusedElement = null;
			evt.preventDefault();
		}
		function checkClick(evt) {
			if (focusedElement) {
				if (focusedElement.style)
					focusedElement.style.border = oldBorder;
				// FIXED: In Konqueror, using Focus bookmarklet followed by
				// Split Preview, results in the 'mouseover' event becoming active
				// again!  (Could it be base2/grimeape related?)
				document.removeEventListener('mouseover',checkMouseOver,true);
				document.removeEventListener('mouseout',checkMouseOut,true);
				document.removeEventListener('click',checkClick,true);
				focus(focusedElement);
				focusedElement = null;
			}
			evt.preventDefault();
		}
		document.addEventListener('mouseover',checkMouseOver,true);
		document.addEventListener('mouseout',checkMouseOut,true);
		document.addEventListener('click',checkClick,true);
	}

	watchForFocus();

})();


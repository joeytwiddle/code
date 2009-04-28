// Should we really make things visible that weren't already?
// We might get top-layer appearing when we don't want them.

function init() {
	var newBlock = document.createElement('P');
	newBlock.id = 'DONT_HIDE_ME';
	/*newBlock.style.padding = '4px';*/
	// newBlock.style = "backgroundColor: '#aaaaff'; padding: 2px; border-style:solid; border-width:2px; border-color: white;";
	newBlock.style.padding = '6px';
	newBlock.style.color = 'white';
	newBlock.style.backgroundColor = '#22bb22';
	newBlock.style.borderStyle = 'solid';
	newBlock.style.borderWidth = '3px';
	newBlock.style.borderColor = 'black';
	newBlock.style.margin = '0px';
	var html = "<CENTER>\n";
	html += "<B>Search filter:</B>\n";
	html += "[<A href='javascript:(function(){ document.getElementById(&quot;DONT_HIDE_ME&quot;).getElementsByTagName(&quot;INPUT&quot;)[0].value = &quot;&quot;; doUpdate(&quot;&quot;); })();'>Clear</A>]\n";
	html += "<INPUT type='text' size='30' onchange='javascript:doUpdate(value);'/>\n";
	html += "(case-sensitive)\n";
	html += "[<A href='javascript:(function(){ var e = document.getElementById(&quot;DONT_HIDE_ME&quot;); e.parentNode.removeChild(e); })();'>Close</A>]\n";
	html += "</CENTER>\n";
	newBlock.innerHTML = html;
	document.body.insertBefore(newBlock,document.body.firstChild);
	// document.location = '#';
}

var hidden,visible;

function doUpdate(searchStr) {
	hidden = 0;
	visible = 0;
	checkNode(document.body,searchStr,0);
	window.status = "Hid "+hidden+" elements, left "+visible+" visible.";
}

/** We don't actually need to hide the children of a node, if we are gonna hide
 * the parent.  But we have to check all the children, to find that out, so we
 * may as well hide them while we are there. **/
/** This is much faster in Konqueror than in Firefox. **/
function checkNode(node,searchStr,depth) {
	if (node.id == 'DONT_HIDE_ME' || depth>32)
		return true;

	/** Child-first approach: **/
	var childHit = false;
	var kids = node.childNodes;
	for (var i=0;i<kids.length;i++) {
		childHit = checkNode(kids[i],searchStr,depth+1) || childHit;
	}
	try {
		if (childHit || node.tagName=='HTML' || node.tagName=='BODY' || (""+node.textContent).indexOf(searchStr)>=0) {
			/*if (node.style.display != '')*/
				node.style.display = '';
			visible++;
			return true;
		} else {
			/*if (node.style.display != 'none')*/
				node.style.display = 'none';
			hidden++;
			return false;
		}
	} catch (e) { }

	/** Parent-first approach: **/
	/*
	if ((""+node.textContent).indexOf(searchStr)>=0) {
		var childHit = false;
		var kids = node.childNodes;
		for (var i=0;i<kids.length;i++) {
			childHit = checkNode(kids[i],searchStr,depth+1) || childHit;
		}
		if (childHit) {
			if (node.style.display != '')
				node.style.display = '';
			visible++;
			return true;
		}
	}
	if (node.style.display != 'none')
		node.style.display = 'none';
	hidden++;
	*/

	return false;
}

init();


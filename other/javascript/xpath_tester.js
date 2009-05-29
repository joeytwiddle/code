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
	newBlock.style.position = 'fixed';
	newBlock.style.top = '4px';
	newBlock.style.left = '4px';
	newBlock.style.right = '4px';
	var html = "<CENTER>\n";
	html += "<B>XPath expression:</B>\n";
	html += "[<A href='javascript:(function(){ document.getElementById(&quot;DONT_HIDE_ME&quot;).getElementsByTagName(&quot;INPUT&quot;)[0].value = &quot;&quot;; doUpdate(&quot;&quot;); })();'>Clear</A>]\n";
	html += "<INPUT type='text' size='30' onchange='javascript:doUpdate(value);'/>\n";
	html += "(case-sensitive)\n";
	html += "<A href='javascript:(function(){ document.getElementById(&quot;resultsDiv&quot;).style.display = ( document.getElementById(&quot;resultsDiv&quot;).style.display == &quot;none&quot; ? &quot;&quot; : &quot;none&quot; ); })();'>Results</A>\n"
	html += "[<A href='javascript:(function(){ var e = document.getElementById(&quot;DONT_HIDE_ME&quot;); e.parentNode.removeChild(e); })();'>Close</A>]\n";
	html += "<DIV align='left' id='resultsDiv'></DIV>";
	html += "</CENTER>\n";
	newBlock.innerHTML = html;
	document.body.insertBefore(newBlock,document.body.firstChild);
	// document.location = '#';
}

function getXPath(node) {
	var parent = node.parentNode;
	if (!parent) {
		return "";
	}
	var siblings = parent.childNodes;
	var totalCount = 0;
	var thisCount = -1;
	for (var i=0;i<siblings.length;i++) {
		var sibling = siblings[i];
		if (sibling.nodeType == node.nodeType) {
			totalCount++;
		}
		if (sibling == node) {
			thisCount = totalCount;
			break;
		}
	}
	return getXPath(parent) + "/" + node.nodeName.toLowerCase() + (totalCount>1 ? "[" + thisCount + "]" : "" );
}

var lastRes;

function doUpdate(xpathExpr) {
	if (lastRes) {
		for (var i=0;i<lastRes.snapshotLength;i++) {
			var node = lastRes.snapshotItem(i);
			if (node.style)
				node.style.backgroundColor = 'white';
		}
	}
	var resultsDiv = document.getElementById('resultsDiv');
	resultsDiv.innerHTML = '<TT>';
	var res = document.evaluate(xpathExpr, document, null, 6, null);
	for (var i=0;i<res.snapshotLength;i++) {
		var node = res.snapshotItem(i);
		if (node.style)
			node.style.backgroundColor = '#dddd00';
		resultsDiv.innerHTML += "<BR/>" + getXPath(node);
		// TODO: build results list out of elements not HTML
		// TODO: attach to each result, onmouseover/out events which will highlight the currently hovered node.
	}
	resultsDiv.innerHTML += '</TT>';
	resultsDiv.style.backgroundColor = '#ffffdd';
	resultsDiv.style.color = 'black';
	window.status = "Matched "+res.snapshotLength+" nodes.";
	lastRes = res;
}

init();


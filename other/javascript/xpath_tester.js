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
	newBlock.style.zIndex = '10000';
	document.body.style.marginTop = '44px';
	var html = "<CENTER>\n";
	html += "<B>XPath expression:</B>\n";
	html += "[<A href='javascript:(function(){ document.getElementById(&quot;DONT_HIDE_ME&quot;).getElementsByTagName(&quot;INPUT&quot;)[0].value = &quot;&quot;; doUpdate(&quot;&quot;); })();'>Clear</A>]\n";
	html += "<INPUT type='text' size='30' onchange='javascript:doUpdate(value);'/>\n";
	html += "[<A href='javascript:(function(){ document.getElementById(&quot;resultsDiv&quot;).style.display = ( document.getElementById(&quot;resultsDiv&quot;).style.display == &quot;none&quot; ? &quot;&quot; : &quot;none&quot; ); })();'>Results</A>]\n"
	html += "[<A href='javascript:(function(){ var e = document.getElementById(&quot;DONT_HIDE_ME&quot;); e.parentNode.removeChild(e); })();'>Close</A>]\n";
	html += "<DIV align='left' id='resultsDiv'></DIV>";
	html += "</CENTER>\n";
	newBlock.innerHTML = html;
	document.body.insertBefore(newBlock,document.body.firstChild);
	// document.location = '#';
	document.addEventListener("keypress",function(evt){
			if (evt.keyCode == 27) {
				newBlock.parentNode.removeChild(newBlock);
			}
	},false);
}

function getXPath(node) {
	var pNode = node.parentNode;
	if (!pNode) {
		return "";
	}
	var pPath = getXPath(pNode);
	var tagName = node.tagName;
	var siblings = pNode.childNodes;
	var totalCount = 0;
	var thisCount = -1;
	for (var i=0,len=siblings.length;i<len;i++) {
		var sibling = siblings[i];
		if (sibling.tagName == tagName) {
			totalCount++;
		}
		if (sibling == node) {
			thisCount = totalCount;
			if (thisCount > 1) {
				break;
			}
		}
	}
	return pPath + "/" + tagName.toLowerCase() + (totalCount>1 ? "[" + thisCount + "]" : "" );
}

function getXPathValue(node) {
	var xpath = getXPath(node);
	if (xpath == "") {
		// It is an attribute node, or something else
		// We show its value
		window.xpathresult = node;
		if (node.value)
			return node.value;
		else
			return node.toString();
	} else {
		// It is a node
		// We show its path
		return xpath;
	}
}

var lastRes;

// TODO: Build results list out of elements not HTML
// TODO: Attach to each result, onmouseover/out events which will highlight the hovered item.
function doUpdate(xpathExpr) {
	var resultsDiv = document.getElementById('resultsDiv');
	resultsDiv.style.backgroundColor = '#eef0ff';
	resultsDiv.style.color = 'black';
	resultsDiv.style.border = '1px solid black';

	resultsDiv.style.height = '320px';
	resultsDiv.style.overflow = 'auto';

	if (lastRes) {
		if (lastRes.resultType==XPathResult.UNORDERED_NODE_SNAPSHOT_TYPE || lastRes.resultType==XPathResult.ORDERED_NODE_SNAPSHOT_TYPE) {
			for (var i=0;i<lastRes.snapshotLength;i++) {
				var node = lastRes.snapshotItem(i);
				if (node.style)
					node.style.backgroundColor = '';
			}
		}
	}

	resultsDiv.innerHTML = '<FONT size="-1"><TT>';

	try {

		// Wow XPathResult sucks pretty hard.
		var res;
		// If we want multiple results, we must try that first:
		try {
			res = document.evaluate(xpathExpr, document, null, XPathResult.UNORDERED_NODE_SNAPSHOT_TYPE, null);
			// resultsDiv.innerHTML += "UNORDERED_NODE_SNAPSHOT_TYPE succeeded<BR/>";
			// If the result is an attribute, then it is returned as a node.
		} catch (e) {
			// If we want a string, we must try that first:
			try {
				// Number types get returned as strings ok.
				res = document.evaluate(xpathExpr, document, null, XPathResult.STRING_TYPE, null);
				// If it's multiple results, then we must try multiple before STRING (which gives us "Results") or ANY (returns only the first).
				// But if it's a string result, and we try multiple, we get a happy empty response.
				// resultsDiv.innerHTML += "STRING_TYPE succeeded<BR/>";
			} catch (e) {
				try {
					res = document.evaluate(xpathExpr, document, null, XPathResult.ANY_TYPE, null);
					// resultsDiv.innerHTML += "ANY_TYPE succeeded<BR/>";
				} catch (e) {
					resultsDiv.innerHTML += "Failed on ANY: "+e;
					res = null;
				}
			}
		}
		// resultsDiv.innerHTML = ""+res.resultType;
		window.status = "Result type "+res.resultType;
		if (res.resultType==XPathResult.NUMBER_TYPE) {
			resultsDiv.innerHTML += "" + res.numberValue;
		} else if (res.resultType==XPathResult.STRING_TYPE) {
			resultsDiv.innerHTML += "" + res.stringValue;
		} else if (res.resultType==XPathResult.BOOLEAN_VALUE) {
			resultsDiv.innerHTML += "" + res.booleanValue;
		} else if (res.resultType==XPathResult.UNORDERED_NODE_ITERATOR_TYPE || res.resultType==XPathResult.ORDERED_NODE_ITERATOR_TYPE) {
			var node = res.iterateNext();
			while (node) {
				resultsDiv.innerHTML += getXPathValue(node);
				if (node.style)
					node.style.backgroundColor = '#ffee99';
				// It's a fucking iterator, but iterateNext throws error gg.
				try { node = res.iterateNext(); }
				catch (e) { break; }
				break;
			}
		} else {
			for (var i=0;i<res.snapshotLength;i++) {
				var node = res.snapshotItem(i);
				// if (node.nodeName) { // is a node
					resultsDiv.innerHTML += getXPathValue(node);
					if (node.style)
						node.style.backgroundColor = '#ffee99';
				// }
				resultsDiv.innerHTML += "<BR/>"
			}
			window.status = "Matched "+res.snapshotLength+" nodes.";
		}
		lastRes = res;
		// The only reason I'm here is 'cos I want to test a fucking tiny xpath to select nodes by className.

	} catch (e) {
		// window.status = ""+e;
		resultsDiv.innerHTML += ""+e;
		lastRes = null;
	}

	resultsDiv.innerHTML += '</TT></FONT>';

}

init();


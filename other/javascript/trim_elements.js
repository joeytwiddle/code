var count,visible;

function init() {
	var newBlock = document.createElement('P');
	newBlock.id = 'DONT_HIDE_ME';
	newBlock.style.backgroundColor = '#bbbbff';
	/*newBlock.style.padding = '4px';*/
	newBlock.innerHTML = "<CENTER><FORM onsubmit=''>Search (case-sensitive): <INPUT type='text' length='15' onblur='javascript:doUpdate(document.body,value);'/></FORM></CENTER>";
	document.body.insertBefore(newBlock,document.body.firstChild);
}

function doUpdate(node,searchStr) {
	/*for (i=0;i<document.body.childNodes.length;i++) {
		var child = document.body.childNodes[i];
		window.status += (""+child+"<BR/>\n");
	}*/
	hidden = 0;
	visible = 0;
	checkNode(document.body,searchStr,0);
	window.status = "Hid "+hidden+" elements, left "+visible+" visible.";
}

function checkNode(node,searchStr,depth) {
	count++;
	if (node.id == 'DONT_HIDE_ME' || depth>8)
		return true;
	/*if ((""+node.textContent).indexOf(searchStr)>=0) {
		if (node.style) node.style.display = '';
		return true;
	}*/
	var childHit = false;
	var kids = node.childNodes;
	for (var i=0;i<kids.length;i++) {
		/*if (kids[i].nodeValue != null)*/
		/* alert("["+childHit+"]" + depth + node + '[' + i + '] = ' + kids[i]); // + ' = ' + kids[i].nodeValue; */
		childHit = checkNode(kids[i],searchStr,depth+1) || childHit;
	}
	/*if (node.style) {*/
	try {
		/*var str = ""+node.nodeValue;*/
		/*(node.nodeName=='#text' || node.nodeName=='P' || node.nodeName=='DIV' || node.nodeName=='LI')
		&& (""+node.nodeValue).indexOf(searchStr)>=0*/
		if (childHit || (""+node.textContent).indexOf(searchStr)>=0) {
			/*if (node.style.display != '')
				node.style.display = '';*/
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
	return false;
}

init();


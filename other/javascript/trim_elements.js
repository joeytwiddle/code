var count,visible;

function init() {
	var newBlock = document.createElement('P');
	newBlock.id = 'DONT_HIDE_ME';
	newBlock.style.backgroundColor = '#bbbbff';
	/*newBlock.style.padding = '4px';*/
	newBlock.innerHTML = "<CENTER><FORM onsubmit=''>Search (case-sensitive): <INPUT type='text' length='15' onchange='javascript:doUpdate(value);'/></FORM></CENTER>";
	document.body.insertBefore(newBlock,document.body.firstChild);
}

function doUpdate(searchStr) {
	hidden = 0;
	visible = 0;
	checkNode(document.body,searchStr,0);
	window.status = "Hid "+hidden+" elements, left "+visible+" visible.";
}

function checkNode(node,searchStr,depth) {
	count++;
	if (node.id == 'DONT_HIDE_ME' || depth>8)
		return true;
	var childHit = false;
	var kids = node.childNodes;
	for (var i=0;i<kids.length;i++) {
		childHit = checkNode(kids[i],searchStr,depth+1) || childHit;
	}
	try {
		if (childHit || (""+node.textContent).indexOf(searchStr)>=0) {
			if (node.style.display != '')
				node.style.display = '';
			visible++;
			return true;
		} else {
			if (node.style.display != 'none')
				node.style.display = 'none';
			hidden++;
			return false;
		}
	} catch (e) { }
	return false;
}

init();


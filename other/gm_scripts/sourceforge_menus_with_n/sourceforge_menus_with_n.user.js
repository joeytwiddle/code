// ==UserScript==
// @name			SourceForge Menus with No Javascript
// @namespace      	http://userscripts.org/scripts/show/41174
// @version:		0.1.1
// @description	   	This hack will eliminate the need of Javascript for seeing menus on SorceForge.net. (Handy if you have NoScript extension on)  
// @include        	http://sourceforge.net/projects/*
// @include        	http://www.sourceforge.net/projects/*
// ==/UserScript==

var $sf = {
	_html_menu_root : '//html//body//div//ul[@class="nav"]' , 
	_html_subset : '//html//body//div[@id="doc3"]//div[@id="bd"]//div[@class="subset"]',
	_html_menu_content : '//html//body//div[@id="doc3"]//div[@id="bd"]//div[@class="subset"]//div[@id="sf_menu_content"]' , 
		
	init: function(){
		this.create_menu_content();
		this.print_menu();		
	},
	
	create_menu_content : function(){
		$xpath(this._html_subset).snapshotItem(0).innerHTML ='<div id="sf_menu_content"><style>#sf_menu_content a { display: inline ; font-weight: bold ; }</style></div>'+$xpath(this._html_subset).snapshotItem(0).innerHTML ;		
		$xpath(this._html_menu_content).snapshotItem(0).setAttribute('style','background-color: #D4E5C1; -moz-border-radius: 4px; padding:10px; margin: 0px 10px;');
	},
	
	print_menu : function(){
		var txt = '';
		for(var i = 1 , k = 15 ; i < k ; i++){
			var txt1 = this.print_submenu_items(i);
			if(i != k -1 && txt1 != ''){
				txt1 += '<hr/>';
			}
			txt += txt1 ;
		}
		$xpath(this._html_menu_content).snapshotItem(0).innerHTML += txt ; 
	},
	
	print_submenu_items : function(li){
		var txt = '' ; 
		try{
			var submenu_root = $xpath(this._html_menu_root+'//li['+li+']//ul//li') ;		
			var submenu_title = $xpath(this._html_menu_root+'/li['+li+']/a').snapshotItem(0) ;
			if (submenu_title && submenu_title.innerHTML != 'Tasks' && submenu_title.innerHTML != 'More' && !(''+submenu_title.className+'').match(/disabled/) && !(''+submenu_title.parentNode.className+'').match(/disabled/) ) {
				txt += submenu_title.innerHTML + ': ' ; 
				for(var i = 0 , k = submenu_root.snapshotLength; i < k ; i++){
					var class_name = submenu_root.snapshotItem(i).className ;  
					if (!(''+class_name+'').match(/disabled/) ){
						txt += submenu_root.snapshotItem(i).innerHTML ; 
					}
				}
			}
		}
		catch(e){}
		return txt ; 
	}
}

function $xpath(q,doc) { if (!doc || doc == '') {doc = document ; } return doc.evaluate(q, doc,null,XPathResult.UNORDERED_NODE_SNAPSHOT_TYPE,null); }

(function() {
	$sf.init();
})();

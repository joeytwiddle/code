/***********************************************************************
Google Fx
Version 2.1.7
2009-05-14
Copyright 2007, Pablo Custo
Released under the GPL license
http://www.gnu.org/copyleft/gpl.html

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************
- Fixed more bugs
 **************************************************************************/
// ==UserScript==
// @name         Google Fx
// @version      2.1.7
// @description  AutoPaging + ThumbSite + Multi-lang Suggest + Easy Filters + Right Panel Feature + Images Redirect & Preview + Searchers + Static TopBar + NoAds + Nice Look&Feel + User's Settings + MultiLanguage & more... [ v 2.1.7 ]
// @date         2009-05-14
// @creator      pablocusto@gmail.com
// @namespace    http://userscripts.org/scripts/show/31950
// @updateURL    http://userscripts.org/scripts/source/31950.user.js
// @include	http://*.google.*
// @include	https://*.google.*
// @include	http://*.googleproxy.*
// @include	http://64.233.*.*/*
// @include	http://66.102.*.*/*
// @include	http://74.125.*.*/*
// @include	http://216.239.*.*/*
// @exclude	http://*.google.*/complete/*
// @exclude	http://*.google.*/ig/directory*
// @exclude	http://*analytics*

// ==/UserScript==
( function() {
//***********************************************************************************************
var SCRIPT_VER 	= '2.1.7';
var LNG_VER 	= '2.1.5';
var SCRIPT_URL = 'http://userscripts.org/scripts/show/31950';
var SCRIPT_LNGS	= 'de,en,es,fr,it,ja,nl,pl,pt,pt-br,ru,tk,zh,zh-tw';
//***********************************************************************************************
var win = (typeof(unsafeWindow) != 'undefined' ? unsafeWindow : top.window), doc = document;
var isByRequest = (doc.location.hash.indexOf('gfx-ap') == 1);
if( (doc.location != top.location && !isByRequest) || /\/[^\/]+\.[a-z0-9_]{2,4}$/i.test(doc.location.pathname) || /^\/complete\/search/.test(doc.location.pathname) || /^local/i.test(doc.location.hostname) ){return;}

// Initialization Script
function GFX_Init()
{

var doc = document;
if (doc.location == top.location && doc.isGFXLoaded){return;}
//############################################################################################
//                              U S E R     S E T T I N G S   
//############################################################################################
GFX = {_ver		: SCRIPT_VER,
		
		// Show advance search options
		bAdvSrc		: getValue('GFX.bAdvSrc',		true),
		
		//Check & Fill Google Menu (some regions don't have some menu's links)
		bMnuChk		: getValue('GFX.bMnuChk',		true),
		// Add Icons reference to the topBar's menu
		bMnuIcn		: getValue('GFX.bMnuIcn',		true),
		// Make the topBar Static
		bMnuFix		: getValue('GFX.bMnuFix',		true),
		// Convert YourName@gmail.com to only YourName
		bShtName	: getValue('GFX.bShtName',		true),

		// Add links (with query) in Searchers bar
		bSrchs		: getValue('GFX.bSrchs',		true),
		// Add icons reference in Searchers bar
		bSrchsIcn	: getValue('GFX.bSrchsIcn',		true),
		

		// Show the Script Logo in Google Image
		gFxLogo		: getValue('GFX.gFxLogo',		true),

		// Enable Google Suggest in all input's query
		gSuggest	: getValue('GFX.gSuggest',		true),
		// Enable Google Suggest Language Changer
		gSuggestLng	: getValue('GFX.gSuggestLng',	true),
		

		// Add Image Color Type to Image Bar
		iColor		: getValue('GFX.iColor',		true),
		// Add Image Color Type to Image Bar
		iColType	: getValue('GFX.iColType',		true),
		// Add Image Content Type to Image Bar
		iCntType	: getValue('GFX.iCntType',		true),
		// Add Image Exact Sizes to Image Bar
		iExactSz	: getValue('GFX.iExactSz',		true),
		// Add Image File Type to Image Bar
		iType		: getValue('GFX.iType',			true),
		// Add SafeSearch Mode to Image Bar
		iSafe		: getValue('GFX.iSafe',			true),
		// Add Sizes & image Type options to Image Bar
		iSzBar		: getValue('GFX.iSzBar',		true),
		// Enable/Disable Icon's feature in the Image Sizes Bar
		iSzBarIcn	: getValue('GFX.iSzBarIcn',		true),
		// Show Preview Original Image on MouseOver
		iPrev		: getValue('GFX.iPrev',			true),
		// Show Preview when MouseOver on ThumbShots
		iPrevNoIcn	: getValue('GFX.iPrevNoIcn',	false),
		// Reaction Time of Image Preview ( in millisecs )
		iPrevRT		: getValue('GFX.iPrevRT',		200),


		// Make Auto-Paging Streaming of the results
		rAutoPag	: getValue('GFX.rAutoPag',		true),
		// Scroll Sensitivity Detection for Auto-Streaming Pages
		//	( 1=Min, 2=Normal, 3=High, 4=Extreme )
		rAutoPagRT	: getValue('GFX.rAutoPagRT',	2.0),
		// Navigator Bar for Auto-Page
		rAutoPagNav	: getValue('GFX.rAutoPagNav',	false),

		// Show a counter number in items results
		rCounter	: getValue('GFX.rCounter',		true),

		// Remove Google Ads
		rNoAds		: getValue('GFX.rNoAds',		true),
		// Remove Google Tracks
		rNoTrack	: getValue('GFX.rNoTrack',		true),

		// Convert links of reference to intuitive icons (ex: [in Cache] - [similar page] )
		rRefIcn		: getValue('GFX.rRefIcn',		true),
		// Re-Position reference icon links
		rRefIcnRgt	: getValue('GFX.rRefIcnRgt',	true),

		// WebSearch Right Panel
		bRgtBox		: getValue('GFX.bRgtBox',		true),

		// Enable the Site Filter feature
		rSiteFlt	: getValue('GFX.rSiteFlt',		true),

		// Show site's preview ThumbShots in the results
		rThShots	: getValue('GFX.rThShots',		true),
		// ThumbShots Ratio Size  (   0.5 = 50%  -  1 = 100%  -  2 = 200%)
		rThSize		: getValue('GFX.rThSize',		1.0),
		// ThumbShots Position ( "true" for Left, "false" for right)
		rThLft		: getValue('GFX.rThLft',		true),

		// Set the links results to be open in new tab (set to "_self" to disabled)
		rTrgLnk		: getValue('GFX.rTrgLnk',		'_blank'),


		// Google Sections
		Srv	: {
			accounts	: getValue('GFX.Srv.accounts',		false),
			alerts		: getValue('GFX.Srv.alerts',		false),
			blogsearch	: getValue('GFX.Srv.blogsearch',	true),
			books		: getValue('GFX.Srv.books',			true),
			calendar	: getValue('GFX.Srv.calendar',		false),
			code		: getValue('GFX.Srv.code',			true),
			dirhp		: getValue('GFX.Srv.dirhp',			true),
			docs		: getValue('GFX.Srv.docs',			false),
			finance		: getValue('GFX.Srv.finance',		true),
			groups		: getValue('GFX.Srv.groups',		true),
			history		: getValue('GFX.Srv.history',		true),
			ig			: getValue('GFX.Srv.ig',			true),
			images		: getValue('GFX.Srv.images',		true),
			labs		: getValue('GFX.Srv.labs',			true),
			mail		: getValue('GFX.Srv.mail',			false),
			maps		: getValue('GFX.Srv.maps',			true),
			news		: getValue('GFX.Srv.news',			true),
			notebook	: getValue('GFX.Srv.notebook',		false),
			picasaweb	: getValue('GFX.Srv.picasaweb',		true),
			products	: getValue('GFX.Srv.products',		true),
			reader		: getValue('GFX.Srv.reader',		false),
			scholar		: getValue('GFX.Srv.scholar',		true),
			search		: getValue('GFX.Srv.search',		true),
			sites		: getValue('GFX.Srv.sites',			true),
			translate	: getValue('GFX.Srv.translate',		true),
			video		: getValue('GFX.Srv.video',			true)
		},


				// Mozilla Borders Radius
		mzBrd		: getValue('GFX.mzBrd',			true),
		//	Top Lef Radius (in pixels)
		mzBrdTL		: getValue('GFX.mzBrdTL',		7),
		//	Top Right Radius (in pixels)
		mzBrdTR		: getValue('GFX.mzBrdTR',		7),
		//	Bottom Lef Radius (in pixels)
		mzBrdBL		: getValue('GFX.mzBrdBL',		7),
		//	Bottom Right Radius (in pixels)
		mzBrdBR		: getValue('GFX.mzBrdBR',		7),
};

GFX.Srv.local = GFX.Srv.maps;
GFX.Srv.webhp = GFX.Srv.search;
GFX.Srv.spreadsheets = GFX.Srv.docs;

// Save Settings on Opera
if(isOpera){
	GFX = OP_getValues('GFX');
}

if(isFirstRun){

//***********************************************************************************************
//            C S S    S t y l e s      
//***********************************************************************************************
CSS = {_ver		: SCRIPT_VER,
	
	/** Commons Styles-----------------*/
	cmn:{
		/** Body------------------------------*/
		body:	getValue('CSS.cmn.body', 'background-color:#fff;'),
		/** Submit Butons-------------------*/
		btns:	getValue('CSS.cmn.btns', 'background-color:#338; border-color:#111; border-style:solid; border-width:1px; color:#fff; font-family:Arial, Hevetica, sans-serif; font-weight:bold; margin-left:1px; margin-right:1px;'),

		/** Butons on MouseOver---------*/
		btnsOv:	getValue('CSS.cmn.btnsOver', 'background-color:#36f;'),

		/** Firefox Corners Style--------*/
		mzBrd:			'-moz-border-radius-topleft		:' + GFX.mzBrdTL + 'px;'+
						'-moz-border-radius-topright	:' + GFX.mzBrdTR + 'px;'+
						'-moz-border-radius-bottomleft	:' + GFX.mzBrdBL + 'px;'+
						'-moz-border-radius-bottomright	:' + GFX.mzBrdBR + 'px;'

	},
	
	/** Content Result-------------------*/
	res:{

		/** Counter number in Results-----*/
		cnt:	getValue('CSS.res.cnt', ' color:#333; float:left; font-family:"Trebuchet MS", Georgia, serif; font-weight:normal; font-size:16pt; margin-right:5pt; margin-top: -2pt;'),

		/** Description of Results---------*/
		dsc:	getValue('CSS.res.dsc', 'color:#222; font-family:Arial, Hevetica, sans-serif; font-weight:normal; font-size:10pt;'),

		/** Highlight query  words---------*/
		hl:		getValue('CSS.res.hl', 'border-color:#d90; border-style:dashed; border-width:1px; font-style:normal;'),

		/** Content Box of Results--------*/
		box:	getValue('CSS.res.box', 'background-color:#fff; border-color:#c9d7f1; border-style:solid; border-width:1px; margin-bottom:4px; margin-left:8px; margin-right:5px; margin-top:2px; padding-bottom:5px; padding-left:8px; padding-right:5px; padding-top:5px;'),

		/** Titles of Results-----------------*/
		tit:	getValue('CSS.res.tit', 'font-family:"Trebuchet MS", Georgia, serif; font-weight:normal; font-size:14pt; color:#10c; margin-bottom:1px;'),

		/** Url Pages-------------------------*/
		url:	getValue('CSS.res.url', 'color:#070 !important; font-family:Arial, Hevetica, sans-serif; font-weight:normal; font-size:10pt;'),
		
		/** Ref Links-------------------------*/
		ref:	getValue('CSS.res.ref', 'color:#00f !important; font-family:Arial, Hevetica, sans-serif; font-weight:normal; font-size:10pt;')

	}
	
};

// Save Settings on Opera
if(isOpera){
	CSS = OP_getValues('CSS');
}

DB = {};
DB.Color = {};
var value = CSS.res.tit.match(/[^-]color\s*:\s*([^; ]+)/);
CSS.res.tit = CSS.res.tit.replace(/[^-]color\s*:\s*[^; ]+/,'');
DB.Color.link = new Color(value && value[1] ? value[1] : '#009');
value = CSS.res.box.match(/background-color\s*:\s*([^; ]+)/);
DB.Color.box = new Color(value && value[1] ? value[1] : '#fff');
DB.Color.visited = DB.Color.link.mix(DB.Color.box.invert(),'#f00',50).rgbToHex();
DB.Color.header = DB.Color.box.mix(DB.Color.box,'#fff',30).rgbToHex();
DB.Color.link = DB.Color.link.rgbToHex();


//***********************************************************************************************
//  Searchers
//***********************************************************************************************
SEARCHER = {

	// Web Searchers
	search:{
		_commons:{

	/**-------------------------------------------------------------------------------------------------
	Sintax:

	         SiteName : [ ' < Domain-Url>  '  ,   ' < Query-Path > '    , ' [ Favicon ] '  ]


	* Domain-URL       String URL of the domain site

	* Query-Path         String of Path & Query (the last value must be the query to search)
					         or
			Can be an array in case of complex search path querys
			( the query to seach is between the array elements )
			ex: 
			       [ ' /some/path/?search= ' , ' &more_options ' ]

	* Favicon         ( optional ) Can be a relative or absolute path to favicon or a data base64 image
	-----------------------------------------------------------------------------------------------------*/

			Yahoo     :['search.yahoo.com'      ,'/search?p='    ,'/'					],
			Alpha     :['au.alpha.yahoo.com'    ,'/search/web?p='						],
			Ask       :['web.ask.com'           ,'/web?q='								],
			AllTheWeb :['www.alltheweb.com'     ,'/search?q='							],
			Live      :['search.msn.com'        ,'/results.aspx?q='						],
			Lycos     :['search.lycos.com'      ,'/cgi-bin/pursuit?query='				],
			Wiki      :['en.wikipedia.org'      ,'/wiki/Special:Search?go=go&search='	],
			Altavista :['www.altavista.com'     ,'/web/results?q='						]
		},
		ar:{ Wiki:['ar.wikipedia.org'] },
		bg:{ Wiki:['bg.wikipedia.org'] },
		ca:{ Wiki:['ca.wikipedia.org'] },
		cs:{ Wiki:['cs.wikipedia.org'] },
		da:{ Wiki:['da.wikipedia.org'] },
		de:{ Wiki:['de.wikipedia.org'] },
		el:{ Wiki:['el.wikipedia.org'] },
		en:{ Technorati	:['www.technorati.com'			,'/search/'			],
			 Bloglines	:['www.bloglines.com'			,'/search?t=1&q='	],
			 A9			:['a9.com'						,'/'				],
			 Dictionary	:['dictionary.reference.com'	,'/search?q='		]
			},
		eo:{ Wiki:['eo.wikipedia.org'] },
		es:{ Yahoo		:['es.search.yahoo.com'	],
			 Lycos		:['search.lycos.es'	],
			 Wiki		:['es.wikipedia.org'	],
			 WordRef	:['www.wordreference.com'		,'/definicion/']
			},
		et:{ Wiki:['et.wikipedia.org'] },
		fi:{ Wiki:['fi.wikipedia.org'] },
		fr:{ Wiki:['fr.wikipedia.org'] },
		gl:{ Wiki:['gl.wikipedia.org'] },
		he:{ Wiki:['he.wikipedia.org'] },
		hu:{ Wiki:['hu.wikipedia.org'] },
		hr:{ Wiki:['hr.wikipedia.org'] },
		id:{ Wiki:['id.wikipedia.org'] },
		it:{ Wiki:['it.wikipedia.org'] },
		ja:{ Wiki:['ja.wikipedia.org'] },
		ko:{ Wiki:['ko.wikipedia.org'] },
		lt:{ Wiki:['lt.wikipedia.org'] },
		ms:{ Wiki:['ms.wikipedia.org'] },
		nl:{ Wiki:['nl.wikipedia.org'] },
		no:{ Wiki:['no.wikipedia.org'] },
		pl:{ Wiki		:['pl.wikipedia.org'],
			 Yahoo		:['uk.search.yahoo.com'],
			 Clusty		:['clusty.com'			,'/search?input-form=clusty-simple&v%3Asources=webplus&query='],
			 Wykop		:['www.wykop.pl'		,'/search?phrase='],
			 Elektroda	:['szukaj.elektroda.pl'	,'/search.php?mode=results&fraza=+'],
			 Ceneo		:['www.ceneo.pl'	,'/categories.aspx?search=yes&categoryID=0&inDesc=False&minPrice=0&maxPrice=99999999&searchText='],
			 Allegro	:['www.allegro.pl'		,'/search.php?string=',
						  'http://static.allegro.pl/site_images/1/0/common/favicon.ico']
			},
		pt:{ Wiki:['pt.wikipedia.org'] },
		ro:{ Wiki:['ro.wikipedia.org'] },
		ru:{ Wiki:['ru.wikipedia.org'] },
		sk:{ Wiki:['sk.wikipedia.org'] },
		sl:{ Wiki:['sl.wikipedia.org'] },
		sr:{ Wiki:['sr.wikipedia.org'] },
		sv:{ Wiki:['sv.wikipedia.org'] },
		te:{ Wiki:['te.wikipedia.org'] },
		th:{ Wiki:['th.wikipedia.org'] },
		tr:{ Wiki:['tr.wikipedia.org'] },
		uk:{ Wiki:['uk.wikipedia.org'] },
		vo:{ Wiki:['vo.wikipedia.org'] },
		zh:{ Wiki:['zh.wikipedia.org'] }
	},


	// Specials Searchers
	special:{
		_commons:{
			CracksAm		:['search.cracks.am'	,'/search.php?q='],
			CrackFind		:['crackfind.com'		,'/test.php?chto=', 'data:image/gif;base64,R0lGODlhEAAQAPQAAAAAAAkJCSMjI3x8fBERERoaGqurq1NTU8vLy6SkpMXFxXBwcDU1NdHR0evr64yMjCwsLEFBQUxMTJubm7y8vFtbW9vb2/X19ZaWlmtra7a2tgAAAAAAAAAAAAAAAAAAACwAAAAAEAAQAAAFayAgjmRZBkRglsSSDDCzAoTRSEUhPcI6XBFSAFUQCAoAQYNiChgMh0rw4HiYCIgEiWo9KbQjiEVzzQJUoolDRiIoEJnFQSRAUCBnwYGhMBiRdBgGMAMHBV8zBRACKgEIEzMnDQaRQgsVlZUhADs='],
			RapidLibrary	:['rapidlibrary.com'	,'/index.php?q='],
			RapidShareData	:['rapidsharedata.com'	,'/tag/'],
			RapidLib		:['rapid.tvphp.net'		,'/?q=','data:image/gif;base64,R0lGODdhEAAQAO4AAPr5+/nQy7rK4+qfkeHp9PrKw/zy8PjCu8DP5dHb6YSo0nCaysLT6Pq/teft9v+rg6i/3Iqq00KR0vipiPB8crLJ5SyY4OQ7AP6fgvnX0fmyqmKRxUSEw/W4sDF5vpqsziaOz/+wkRh6yQBQmjGo6iqr87Hb+Nfi78nZ65PK8EO09Ozw93XI9pbV+P61hABNmfX4+6Xc/fukm/iYjeuej/eakP63ivCbhvP2+vWaivONgeRDBfg9A/tuBvtfBf9hAPo2AOQtAP9TAMHq//EjB/qcaPqSW/STgNrz/vp0KvhnCPxcB+FIDvIpCvqmnKG82JGw1p2425u32TeW0yml64iw1o6u1abH6ZLG57HH4Tyi5rbL42+z37XD3BqS4b7R56vP7PvKvpvQ8zmx8zyAwfOxqRZhpf71867W9HXF8naezYmmzP3y7SCKzluRydfi8CaBzMPZ8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACwAAAAAEAAQAAAH34AAgoODZwaHgzArBA4OBDiCGQEBGYMOCAICDJlvAAEHBwGDBBUQXwAMEBUABRoaooInUlYCACgRUQAHTjIFgygKalkACAsRAA01M2GDDAsLCidQGwoAHUc6DYNbHBxkHh5uKwBlFBTagldwIhJxEiJYABhNRDmDaFoWaQBiXhZIGHgAmTDIhIoSLAC0KDFmyIMlQggKMkgiYQsqJGK46PEjhCA2YKaA4AIgRRsQKWwo8ZGkiJEHT8yM2ABgzYsRVUIw2XHhQpAbBBB0SQAgwYcPCQoMGEBjaQdCUKMCCAQAOw=='],
			PirateBay		:['thepiratebay.org'	,['/search/','/0/99/0']]
		}
	},


	// Blogs Searchers
	blogsearch:{
		_commons:{
			Yahoo		:['us.builder.search.yahoo.com'	,'/search?ei=UTF-8&fr=ystg-c&mobvs=0&p=', 
						  'http://search.yahoo.com/favicon.ico'],
			Ask			:['www.ask.com'					,'/blogsearch?q='],
			BlogSearcher:['www.blogsearchengine.com'	,'/blog-search/?action=Search&q='],
			Twingly		:['www.twingly.com'				,'/search?q=',
						  'http://static.twingly.com/content/images/favicon.ico'],
			IceRocket	:['www.icerocket.com'			,'/search?tab=blog&q='],
			BlogPulse	:['www.blogpulse.com'			,'/search?query=']
		},
		en:{ Twingly:[ false, '/search?lang=en&q='] },
		es:{ Twingly:[ false, '/search?lang=es&q='] },
		pl:{ Twingly:[ false, '/search?lang=pl&q='] }
	},


	// Books Searchers
	books:{
		_commons:{
			Amazon		:['www.amazon.com'				,['/exec/obidos/redirect?link_code=ur2&camp=1789&creative=9325&path=external-search%3Fsearch-type=ss%26keyword=','%26index=books']],
			NYTimes		:['topics.nytimes.com'			,'/top/features/books/bookreviews/?s=closest&amp;match=all&amp;query='],
			ReviewScout	:['www.reviewscout.com'			,['/','/']],
			BarnesNoble	:['search.barnesandnoble.com'	,'/booksearch/results.asp?WRD='],
			Blackwells	:['bookshop.blackwell.com'  	,'/bobus/scripts/home.jsp?action=search&source=3246694795&type=keywords&term=']
		}
	},


	// Image Searchers
	images:{
		_commons:{
			Picasa		:['picasaweb.google.com'		,'/lh/searchbrowse?psc=G&filter=0&q='],
			Yahoo		:['images.search.yahoo.com'		,'/search/images?p='],
			Ask			:['images.ask.com'				,'/pictures?q='],
			AllTheWeb	:['www.alltheweb.com'			,'/search?cat=img&q='],
			Live		:['search.msn.com'				,'/images/results.aspx?q='],
			PicSearch	:['www.picsearch.com'			,'/search.cgi?q='],
			Creatas		:['www.creatas.com'				,'/searchResults.aspx?searchString=', '/images/creatas/'],
			FreeFoto	:['www.freefoto.com'			,'/search.jsp?queryString='],
			WebShots	:['www.webshots.com'			,'/search?query='],
			NASA		:['www.nasaimages.org'			,'/luna/servlet/view/search?search=Search&res=0&q=', 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAYRJREFUOE+lky9IQ2EUxTf/dMOqxWS2vCA2YUEMgsEoYhCLDARndmFJkBcEDf6BGQSDDDSIyEBkiLJinAqCgiiYVExXf/dxPt4Ty3Bwebvfu+fcc+53X97Mcv/6QZCO7csnG62eWd/cgUd/qW4zW1f28f75U5at5SyASQbLx9Y7vGpdxU3LT9ase3rfg5xYObwJJNQ/x+sJAQndKKJjVDn1EIGekI/XH13NfTTiTycYi5veGRLkQyIigXvmj5wwmlpz8NdFw9V4d0mkABvI5wyQcgEBLxYXgpUcQwNAIQqQXlg68RySidKuewVIQEQjDdXlI5cDhjQwW7OdvUbGJzlAamiI3dbDW2KBK6LzS/vWC5B3d37t/4filpPhFyAhy832a0IgdiRDhnyAqKkub7gFct6hlHMaBgti1ODwh0SAzCN9rrlApKUK16ip41WDolAbCRhCyCXfLWiRGB4veerOtQOAtaVYSa90IGCIunNkp+OvVc5YYA4aSicfU7Dw+yvrJP8GhUmAY8GUJ9cAAAAASUVORK5CYII='],
			Flickr		:['www.flickr.com'				,'/photos/search/text:'],
			deviantART	:['search.deviantart.com'		,'/?section=browse&qh=boost%3Apopular+age_sigma%3A24h+age_scale%3A5&q='],
			Photobucket	:['photobucket.com'				,['/search/','/']]
		}
	},


	// News Searchers
	news:{
		_commons:{
			Yahoo		:['news.search.yahoo.com'		,'/search/news/?p='],
			Ask			:['news.ask.com'				,'/news?q='],
			AllTheWeb	:['www.alltheweb.com'			,'/search?cat=news&amp;q='],
			Live		:['search.msn.com'				,'/news/results.aspx?q='],
			Lycos		:['search.lycos.com'			,'/default.asp?tab=news&query='],
			Sky_News	:['search.sky.com'				,'/search/skynews/results/1,,,00.html?QUERY='],
			CNN			:['search.cnn.com'				,'/pages/search.jsp?type=news&sortBy=date&intl=true&query=',
						'http://edition.cnn.com/favicon.ico'],
			Bloglines	:['www.bloglines.com'			,'/search?t=1&q=']
		},
		pl:{
			Onet		:['szukaj.onet.pl'			,'/onet.html?qt=', 'http://www.onet.pl/favicon.ico'],
			WP			:['szukaj.wp.pl'			,'/szukaj.html?szukaj='],
			Interia		:['szukaj.interia.pl'		,'/szukaj?q=', 'http://www.interia.pl/interia.ico'],
			TVN24		:['www.tvn24.pl'			,'/szukaj.html?wideo=0&q='],
			Dziennik	:['www.dziennik.pl'			,'/archiwum/?to=on&se=&search.execute=true&lucyStemmed=1&lucyField=2&lucySection=2&lucyFromDateOn=false&lucyToDateOn=false&lucySort=1&lucyMaxNumberResultsSorted=500&lucyOptimized=false&lucyExpr='],
			RMF			:['www.rmf.fm'				,'/fakty/search/?hid=1&s='],
			Wykop		:['www.wykop.pl'			,'/search?phrase=']
		}
	},


	// Products Searchers
	products:{
		_commons:{
			Yahoo		:['shopping.yahoo.com'		,'/search?p='],
			eBay		:['search.ebay.com'			,'/search/search.dll?query=']
		},
		en:{
			MySimon		:['search.mysimon.com'		,'/search?qt='],
			CNET		:['shopper-search.cnet.com'	,'/search?q=',
						'http://www.cnet.com/favicon.ico'],
			NexTag		:['www.nextag.com'			,'/serv/main/buyer/OutPDir.jsp?search='],
			StreetPrices:['www.streetprices.com'	,'/x/search.cgi?query='],
			Shopping	:['www.shopping.com'		,'/xFS?KW='],
			ResellerR	:['shop.resellerratings.com','/?keyword=', 
						'http://images.resellerratings.com/images/shop/favicon.ico']
		},
		pl:{
			Ceneo		:['www.ceneo.pl'			,
			'/categories.aspx?search=yes&categoryID=0&inDesc=False&minPrice=0&maxPrice=99999999&searchText='],
			Nokaut		:['www.nokaut.pl'			,['/szukaj/','.html']],
			Bazzar		:['bazzar.interia.pl'		,'/site/szukaj/?szukaj='],
			Skapiec		:['www.skapiec.pl'			,'/szukaj/w_calym_serwisie/'],
			Eeeby		:['www.eeeby.pl'			,'/?q=',	'data:image/gif;base64,R0lGODlhEAAQAPEAAP///6oAAOq/vwAAACH5BAEAAAAALAAAAAAQABAAAAIuhI8JwerChFNQzsSy1idUD37dRgbdh4JjmVlLiiJshF3KbFKeUU2Z8XvsAD1EAQA7'],
			Kupujemy	:['www.kupujemy.pl'			,'/ceny-oferty.html?c=offer_subject&qw='],
			Cenomaniak	:['cenomaniak.pl'			,'/cenomaniakpl/cenomaniak.1.M.html?phrase=', '/skin/default/img/'],
			ZakupyWp	:['zakupy.wp.pl'			,'/szukaj.html?nazwa=', 'http://i.wp.pl/a/i/sg/favicon.ico'],
			Sklepiki	:['www.sklepiki.pl'			,'/index.php?ik1s=Wszystkie+kategorie&se=1&opiss=', 'data:image/gif;base64,R0lGODlhEAAQAPQAALOMQJ9wEKyDMMyyf6Z5IMapcOziz+XZv7+gYPn179nFn9/Pr7mWUP///wAAAKwHl5lmAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAA8ALAAAAAAQABAAAAVU4COOZCk6aKqqJ+S+sOu0cT0/TswMA/DeuZehcRj6IMAXoKFwCQgyGiSQaDQMhZ90WlhUEVHcSzCAIhpZpFRgtR4C4aASANWKa7EkHgZc+VEmgSQhADs=']
		}
	},


	// Video Searchers
	video:{
		_commons:{
			YouTube		:['www.youtube.com'			,'/results?search_query='],
			Yahoo		:['video.yahoo.com'			,'/video/search?p='],
			Metacafe	:['www.metacafe.com'		,['/tags/','/']],
			Spike		:['www.spike.com'			,'/search?query='],
			Dailymotion	:['www.dailymotion.com'		,['/visited/search/','/']],
			AOL			:['video.aol.com'			,['/video-search/query/','/']],
			Revver		:['www.revver.com'			,'/find/video/?query=' ,'http://static2.revver.com/images/favicon.ico'],
			Vimeo		:['www.vimeo.com'			,['/videos/search:','/sort:newest/format:detail']],
			Rupid		:['www.rupid.com'			,'/search_result.php?search_id='  ,'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAqtJREFUOE99kltIk2EYx98tJTpAYJdZwbCoKUS1NKPLRV5IBdmwC41kLjC8qRsvgqyboiDTDlqepZNGqaWFCTM8zANz06k7eD7NeZibOt2+6bZ/z74pOFhefDzvy8fzf3////MIADAffUKBgC05N1jx7yFwQsbCdgmZZ8PH9u4LZ3KpSMDAGOf2sj3hQrZ/dxjz9wmohz94fT6+TlnXIkQPlRDndUD8UgVxjgpRVK8UqtX66ZX3ZodbMevgDm/vCRawOUXivHac/9iLuAot4j/0QEL1yIs2VGst6J1zwDDviPcLeLyBR4MEpu0uyYmcNkhKNThb0o3YEg2i33Xh5nc95lbdaJmwQze7ItuJQMoLlGlxjkROF6kRld+JuDINZF/7kViohtnuvBcg8IYkkPkF/PjHCzqRWmPA479jiMxtR0yxGgefNqNp2LopEMLClM2piM5V4QJ5P5bfhZTKPpAtnKGXL5Jo5KsOvO2a5i1seLYReLamsLiWdfRJM06VduMQhZlUrsGEzYWTZCOG7Bx41oLX7ZN8iOvbBbbGaFnmshLLtUgh9MTPOjxSjqBcM4OEih7cqjUggf4NzDn4MQZNwbdJMEtzvlHZj7v1JmTUmZBOTSnfBpD5axBp1Xpk1g9iybXOh+fzhQjRssLJrn7S4c5PI+TUnP7DCAWdFVSTKnWo6p9H67g9egcCTnrNL0Cvy6lJXmskMROSq/pQ1G3Gn2ErGoasgRBDLRJZkAQREIWfJpl2wC9QZ1qActSW/V8Ci4MTXf/Sx2eQSt6ft07gfsMQblOoaTV6IlhE4xaBJ8QeWJZdEbFvaIGqB3CJEs9uGkWtYR5S2sTLNIkHjSMYtzv5DLyhLKxyHlZvXEBFjwXKMRtUk0swLazSfQYFajOKyYbD7dmcQoDgHyQq14azLYvsAAAAAElFTkSuQmCC'],
			JumpCut		:['jumpcut.com'				,'/explore/?type=all&keywords=' ,'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAzNJREFUOE89U0lMU1EUfdUQN8bEEHXlTpdinTBaNYipEF1IjEGiGEXYqAsrGJXWARRUMFZFmRMhxAFMXJlixNKJNlAsVi2Y1gZFaSgIpSCiBf7/x/tehZ/c/27ufefc8akURWEqlYopkNki1WLGv8CYg/WEX6X8kaJWBsaWJCxj6pVpqnWr0oRfVubY4kUJ5CInADJI4pyMjSS09OnPlLnSZq7bd+CabSuuWpNRTPpt5148+VTgHJv+rhYYmTAKEUjynACPTPWzu50HQvr29dC3bxCAWk8O6npyUeZKFzYut5xafIu+13GMRCTiNz07we53HfJfsW5BiT0Fb4KPEP07RHfi32TsJ+zfGnGrQ4urtmQiTCP/8FmeuYhu+mLMvkSRb9h3wT9uXQBGo1FQj6DIcVP/eDdKO/ZAb1HjRZ9hmkzLWfRvmBk7M3yXzEloG3iA8OAoPvk+oLXVBJPJBKPRiLq6WphaXwFE5Ao1odCsRqljN6ZmIoWsrb+KlXakwmDejF9KCJa3dpzMzcH9e/dQXl4OjUYDrVaLvLxcHDt6HIHvXtx17xP98IZNXeyp7yK7ZtuGso791NRZvH1jxvmCAlQ8qEBzczOysrKQmZkJg8GA/HP5GB4ZQr03B4VUsrm/Cux5r54V2bajzJmO6dkohsPDGBz8AbfbjZrqGmRkZKC6qhoej0f0QcIMHroPCwLbwGOwrtALxkdz2bIJwUjXQgO5otPpkJiYCJ/PJ+yyJGHoVwDX7TtxxbIZgTFnL5uVYqzyXbZgbPqoEx2XpDkEgn4wFcOatWsISsXNxQTJy8/FVL8ad1z7QdjbYozu0Ms7nKDYrsHrYIW4OBGZgrnNij6ffyErx0Cj2MpCmlj711o+3NViE2VFXv7Md2FCb1mPIvt2NHhPIzDuoGrH8VsZgX/UAfKTTwODZQPqe/J49HqxiXyn+YOakf4sbe69SKWoSZLo4kYUWTQotsXr5TaeZYP3FDV7wsIzp8DxTeQE/GGQvpRmO1DjOUEblwq+2pcJXOJIQWX3EVDDZVrM+nkwP+Pg/yQ8Ja6TrJuMjRYEI+7KLxFXC72Lm2Q7SLJiPuN5zD85FNezu7RVfAAAAABJRU5ErkJggg=='],
			ZippyVideos	:['www.zippyvideos.com'		,'/video_search.z?q=',  'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAbFJREFUOE/dkstrE1EUxo9/haLoxkWl3WgRQVEMwcdScG27UFdSXbiQBhGKGCUboRSpWEoLutC1j74QQTRFKUZcqCkJiSVt0qY6HaeZm7kzd35OJtB/YHZeOJzH5XznfB9nFyCJXgcgiSVqjrdPMj0G+PtT8WGoTHXcYuO1w+kbWWzb2QHWvhbfN3EeGsQPI9+JI1upNUT+FG2+Ta8wl/pO+VGdK7mHGI1sqapY7uoOUN35KoHRQtAWz92I/dqvUpeCWvd5c67AtlJcHHjO/OoUJ+/20n+zh0pzmezMIH2XD9A/lmLr4zjWzBCtTyNYC2OIcUNeHFtC1TWOVpw68Yqzo/txlM18aYKeO3tI5Q5Fc2Dv6G6yI/sIF4dxvgzTXJhEZs//4GWqwPtLRfL3ilzNPOXB3ACPZzNcuJ+mUHtH+kkvtyaucyZ3HNdepzE5SGP6MJuLzxCv5YluaXE326LdQPxAS/r2ET6X3mICZM2uyNFrfSivHYkYxpTd2hKVzEG2l/OdIwojRbsfxgQSRio3rWYc+4EvncbG766YXkTRRLVWNY9Tzse0kt9BUpD/YIN/ZXHJrQb5lqYAAAAASUVORK5CYII=']
		}
	}
};


//***********************************************************************************************
//                    END OF USER SETTINGS
//***********************************************************************************************
} // End isFirstRun



//############################################################################################
//                            G L O B A L    F U N C T I O N S
//############################################################################################

/***********************************/
/**  Common Global Functions	   */
/***********************************/

addStyle = function (css, target)
{
	var doc = target||document;
	var h = doc.getElementsByTagName('head')[0];
	if (!h){return;}
	var el = doc.createElement('style');
	el.type = 'text/css';
	el.innerHTML = css;
	h.appendChild(el);
};

encodeURL = function (str)
{
	return (win.encodeURIComponent ? encodeURIComponent(str) : escape(str));
};

decodeURL = function (str)
{
	return (win.decodeURIComponent ? decodeURIComponent(str) : unescape(str));
};

$nd = function (xpath, context, from)
{
	var nd = (from||doc).evaluate(xpath, (context||doc.body), null, 9, null).singleNodeValue;
	if($type($el) == 'function'){ return $el(nd); }
	return nd;
};

parseUrl = function (url) {
	var r;
	r  = "(?:([a-z0-9+-._]+)://)?"; //protocol
	r += "(?:";
	r += "(?:((?:[a-z0-9-._~!$&'()*+,;=:]|%[0-9a-f]{2})*)@)?"; //userinfo
	r += "(?:\\[((?:[a-z0-9:])*)\\])?"; // host (IPv6)
	r += "((?:[a-z0-9-._~!$&'()*+,;=]|%[0-9a-f]{2})*)"; // host
	r += "(?::(\\d*))?"; // port
	r += "(/(?:[a-z0-9-._~!$&'()*+,;=:@/]|%[0-9a-f]{2})*)?"; // path
	r += "|";
	r += "(/?";
	r += "(?:[a-z0-9-._~!$&'()*+,;=:@]|%[0-9a-f]{2})+";
	r += "(?:[a-z0-9-._~!$&'()*+,;=:@\\/]|%[0-9a-f]{2})*";
	r += ")?";
	r += ")";
	r += "(?:\\?((?:[a-z0-9-._~!$&'()*+,;=:\\/?@]|%[0-9a-f]{2})*))?"; // query
	r += "(?:#((?:[a-z0-9-._~!$&'()*+,;=:\\/?@]|%[0-9a-f]{2})*))?"; //hash
	
	match = (new RegExp(r)).exec(url);

	var o={protocol:'',userinfo:'',host:'',port:'',path:'',query:'',hash:''};
	
	switch (match.length) {
		case 10: o.hash  = match[9]||'';
		case 9:  o.query = match[8]||'';
		case 8:  o.path  = match[7]||'';
		case 7:  o.path  = match[6]||'' + o.path;
		case 6:  o.port  = match[5]||'';
		case 5:  o.host  = match[3] ? "[" + match[3] + "]" : match[4]||'';
		case 4:  o.userinfo = match[2]||'';
		case 3:  o.protocol = match[1]||'';
	}
	
	return o;
}

//############################################################################################
//                                G O O G L E    F X 
//############################################################################################

//*****************************
//      L a n g u a g e
//*****************************
// Take current Language value from the Url or Google vars
LANG = doc.location.search.match(/hl=([^&]*)|$/);
LANG = (LANG && LANG[1]) ? LANG[1] : false;
if (!LANG){
	LANG = doc.location.search.match(/rls=org\.mozilla:(\w\w)|$/i);
	LANG = (LANG && LANG[1]) ? LANG[1] : false;
	if (!LANG){
		LANG = doc.location.search.match(/^\/intl\/([^\-\/]+)/);
		LANG = (LANG && LANG[1]) ? LANG[1] : false;
		if (!LANG){LANG = ((win.google && win.google.kHL) ? win.google.kHL : 'en');}
	}
}
if (LANG.length > 2) {
	LANG = LANG.toLowerCase().split('-');
	switch ( LANG[1] ){
		case 'cn':
		case 'tw':
		case 'br':
			LANG = LANG.join('-');
			break;
		default:
			LANG = LANG[0];
	}
}
isDefLng = (LANG == 'en');


// Set Default Lang Object
switch ( LANG ){
	case 'es':
	LNG = {
	about:'Acerca de',
	alerts:'Alertas',
	all:'Todo',
	avatars:'Avatares',
	blogs:'Blogs',
	books:'Libros',
	buttons:'Botones',
	by:'por',
	calc:'Calculadora',
	calendar:'Calendario',
	cliparts:'Prediseñadas',
	close:'Cerrar',
	cnt_box:'Recuadro',
	code:'Código',
	color:'Color',
	content:'Contenido',
	counter:'Numeración',
	day:'Día',
	defaults:'x Defecto',
	desc:'Descripción',
	dirhp:'Directorio',
	disabled:'Desactivado',
	docs:'Documentos',
	domain:'dominio',
	enabled:'Activado',
	extreme:'Máxima',
	faces:'Rostros',
	filetype:'Extensión',
	filters:'Filtros',
	finance:'Finanza',
	from:'desde',
	general:'General',
	grayscale:'Grises',
	groups:'Grupos',
	height:'Alto',
	here:'Aquí',
	high:'Alta',
	highest:'Muy Alta',
	highlights:'Resaltados',
	history:'Historial',
	hour:'Hora',
	huge:'Enormes',
	icon:'Iconos',
	ig:'iGoogle',
	images:'Imágenes',
	in_:'en',
	labs:'Labs',
	large:'Grandes',
	last:'último/a',
	linearts:'Delineadas',
	links:'Referencias',
	login:'Acceder',
	mail:'Correo',
	maps:'Mapas',
	mchrome:'Monocromo',
	medium:'Medianas',
	min:'Mínima',
	month:'Mes',
	more:'Más',
	new_tab:'Nueva pestaña',
	news:'Noticias',
	normal:'Normal',
	notebook:'Notas',
	one_level_up:'Un Nivel Arriba',
	only:'Solo',
	photos:'Fotos',
	picasaweb:'Picasa',
	prdhp:'Compras',
	preview:'Vista Previa',
	products:'Compras',
	reader:'Lector',
	ref_links:'Referencias',
	results:'Resultados',
	safemode:'Filtro',
	save:'Guardar',
	scholar:'Academico',
	search:'Buscar',
	search_in:'Buscar en',
	searchers:'Buscadores',
	self_tab:'Misma pestaña',
	sites:'Sitios',
	small:'Pequeñas',
	special:'Especiales',
	str_url:'Direcciones',
	styles:'Estilos',
	text:'Texto',
	this_:'este',
	title:'Título',
	toolbar:'Barras',
	translate:'Traducir',
	trycache:'intentar Cache',
	ui_bar_adv_search:'Barras de busqueda avanzada',
	ui_bar_menu_icons:'Mostrar iconos en menus',
	ui_bar_menu_static:'Hacer Menus Estáticos',
	ui_bar_right_panel:'Activar Panel de la Derecha',
	ui_bar_sch_icons:'mostrar iconos',
	ui_bar_searchers:'Barra de Otros Buscadores',
	ui_bar_suggest:'Sugerencias de Google',
	ui_bar_suggest_lngs:'multi-idioma',
	ui_bar_username:'Acortar el nombre de usuario',
	ui_chk_upd:'Buscar Actualizaciones',
	ui_css_defaults:'Ajustar los estilos predeterminados de',
	ui_css_mozBrd:'Bordes Redondedos (Firefox)',
	ui_grl_run_over:'Ejecutar este script en',
	ui_grl_show_logo:'Mostrar Logo FX',
	ui_img_cnt_type:'Mostrar opciones de Tipo de Contenido',
	ui_img_col_type:'Mostrar opciones de Estilos de Colores',
	ui_img_color:'Mostrar opciones de Color de Imagen',
	ui_img_ext_size:'Mostrar opciones de Tamaño Exacto',
	ui_img_filetype:'Mostrar opciones de Tipos de Imagenes',
	ui_img_prev_onThumb:'ejecutar sobre la imagen',
	ui_img_prev_rt:'tiempo de reacción',
	ui_img_preview:'Vista previa de imagen original',
	ui_img_safe:'Mostrar Filtro de Contenido',
	ui_img_sizebar:'Mostrar opciones de Tamaño de Imagen',
	ui_img_sz_icons:'solo iconos',
	ui_res_autopages:'Auto-Paginación',
	ui_res_autopages_nb:'mostrar barra de navegación',
	ui_res_autopages_rt:'sensibilidad de carga',
	ui_res_counter:'Enumerar Resultados',
	ui_res_ref_icon:'Cambiar Links de Ref. a Iconos',
	ui_res_ref_pos:'alinear a la derecha',
	ui_res_remove_ads:'Ocultar Patrocinadores (Publicidad)',
	ui_res_remove_tracks:'Eliminar Seguimientos de enlaces (Historial de Google)',
	ui_res_sitefilter:'Habilitar capacidad de filtrado por sitio',
	ui_res_targetLnks:'Abrir accesos en',
	ui_res_th_lft:'alinear a la Izquierda',
	ui_res_th_sz:'tamaño escala',
	ui_res_thumbs:'Vista Previa de Sitios',
	ui_title:'Configuración del Usuario',
	ui_txt_cache:'Ctrl+Click / Shift+Click para continuar en Google Cache',
	ui_txt_gs:'Click para más sugerencias / DobleClick para buscar.',
	ui_update:'Instalar GoogleFx',
	url:'Dirección',
	video:'Video',
	webhp:'Web',
	week:'Semana',
	width:'Ancho',
	xlarge:'Muy Grandes',
	year:'Año',
	youtube:'YouTube'
	};
	break;
	case 'fr':
	LNG = {
	about:'À propos de',
	alerts:'Alertes',
	all:'Tous',
	avatars:'Avatars',
	blogs:'Blogs',
	books:'Livres',
	buttons:'Buttons',
	by:'par',
	calc:'Calculatrice',
	calendar:'Agenda',
	cliparts:'Designs',
	close:'Fermer',
	cnt_box:'Content Box',
	code:'Code',
	color:'Couleur',
	content:'Contenu',
	counter:'Counter Number',
	day:'Jour',
	defaults:'Défauts',
	desc:'Description',
	dirhp:'Répertoire',
	disabled:'Désactiver',
	docs:'Documents',
	domain:'domaine',
	enabled:'Activer',
	extreme:'Extrême',
	faces:'Visage',
	filetype:'FichierType',
	filters:'Filtres',
	finance:'Finance',
	from:'dès',
	general:'Général',
	grayscale:'Grayscale',
	groups:'Groupes',
	height:'Taille',
	here:'ici',
	high:'Haut',
	highest:'Plus haut',
	highlights:'Highlights',
	history:'Histoire',
	hour:'Heure',
	huge:'Énorme',
	icon:'Icônes',
	ig:'iGoogle',
	images:'Images',
	in_:'dans',
	labs:'Labs',
	large:'Large',
	last:'dernier/e',
	linearts:'Délimitées',
	links:'Links',
	login:'Connexion',
	mail:'GMail',
	maps:'Maps',
	mchrome:'Mono',
	medium:'Medium',
	min:'Min',
	month:'Mois',
	more:'Plus',
	new_tab:'New Tab',
	news:'Actualités',
	normal:'Normal',
	notebook:'NoteBook',
	one_level_up:'Un niveau au-dessus',
	only:'Uniquement',
	photos:'Photos',
	picasaweb:'Photos',
	prdhp:'Shopping',
	preview:'Preview',
	products:'Shopping',
	reader:'Lecteur',
	ref_links:'Référence Liens',
	results:'Résultats',
	safemode:'SafeMode',
	save:'Sauver',
	scholar:'Scholar',
	search:'Recherche',
	search_in:'Recherche dans',
	searchers:'Searchers',
	self_tab:'Current Tab',
	sites:'Sites',
	small:'Petit',
	special:'Spécial',
	str_url:'String URL',
	styles:'Styles',
	text:'Texte',
	this_:'ce',
	title:'Titre',
	toolbar:'ToolBars',
	translate:'Traduire',
	trycache:'tenter Cache',
	ui_bar_adv_search:'Advanced Search Bar',
	ui_bar_menu_icons:'Voir les icônes dans le menu',
	ui_bar_menu_static:'Faites statique menus',
	ui_bar_right_panel:'Activer le panneau de droite',
	ui_bar_sch_icons:'Voir les icônes',
	ui_bar_searchers:'D\'autres chercheurs d\'outils',
	ui_bar_suggest:'Google Suggest',
	ui_bar_suggest_lngs:'multi-langue',
	ui_bar_username:'Convertir nom d\'utilisateur pour nom court',
	ui_chk_upd:'Vérification de la mise à jour',
	ui_css_defaults:'Définir la valeur par défaut pour les styles de',
	ui_css_mozBrd:'Arrondi bords (Firefox)',
	ui_grl_run_over:'Run GoogleFx sur',
	ui_grl_show_logo:'Voir GoogleFx Logo',
	ui_img_cnt_type:'Voir les options de type de contenu',
	ui_img_col_type:'Voir les options de type de couleur',
	ui_img_color:'Voir les options de couleurs de l\'image',
	ui_img_ext_size:'Voir la taille exacte options',
	ui_img_filetype:'Voir les options de type de fichier',
	ui_img_prev_onThumb:'déroulera sur images',
	ui_img_prev_rt:'temps de réaction',
	ui_img_preview:'Aperçu de l\'image',
	ui_img_safe:'Voir Filtre de contenu',
	ui_img_sizebar:'Voir les options Taille de l\'image',
	ui_img_sz_icons:'icônes mode',
	ui_res_autopages:'Streaming Auto-Page',
	ui_res_autopages_nb:'afficher la barre de navigation',
	ui_res_autopages_rt:'ratio de détection',
	ui_res_counter:'Voir Counter Numéros',
	ui_res_ref_icon:'Ref-Texte des liens vers des icônes',
	ui_res_ref_pos:'aligner à droite',
	ui_res_remove_ads:'Masquer Google Ads (Commanditaire)',
	ui_res_remove_tracks:'Suppression pistes à liens (Google Histoire)',
	ui_res_sitefilter:'Activer la fonction de filtre de site',
	ui_res_targetLnks:'Cible par défaut des liens',
	ui_res_th_lft:'aligner à gauche',
	ui_res_th_sz:'ratio taille',
	ui_res_thumbs:'Preview Sites',
	ui_title:'Paramètres utilisateur',
	ui_txt_cache:'Ctrl+Click / Shift+Click pour continuer dans Google Cache',
	ui_txt_gs:'Right key or Click to more suggest / DoubleClick to search.',
	ui_update:'Install GoogleFx',
	url:'Url',
	video:'Vidéo',
	webhp:'Web',
	week:'Semaine',
	width:'Largeur',
	xlarge:'X-Large',
	year:'Année',
	youtube:'YouTube',
	};
	break;
	case 'ja':
	LNG = {
	about:'	?????',
	alerts:'??',
	all:'????',
	avatars:'????',
	blogs:'???',
	books:'??',
	buttons:'???',
	by:'??',
	calc:'??',
	calendar:'?????',
	cliparts:'???????',
	close:'???',
	cnt_box:'?????????',
	code:'???',
	color:'???',
	content:'??',
	counter:'??????',
	day:'?',
	defaults:'?????',
	desc:'??',
	dirhp:'??????',
	disabled:'???',
	docs:'??????',
	domain:'??',
	enabled:'??',
	extreme:'???',
	faces:'?',
	filetype:'??????',
	filters:'????',
	finance:'??????',
	from:'???',
	general:'????',
	grayscale:'???????',
	groups:'????',
	height:'??',
	here:'???',
	high:'??',
	highest:'??',
	highlights:'?????',
	history:'??',
	hour:'??',
	huge:'???',
	icon:'????',
	ig:'iGoogle',
	images:'??',
	in_:'??',
	labs:'??',
	large:'?',
	last:'???',
	linearts:'??????',
	links:'???',
	login:'????',
	mail:'???',
	maps:'???',
	mchrome:'????',
	medium:'??',
	min:'?',
	month:'?',
	more:'???',
	new_tab:'?????',
	news:'????',
	normal:'??',
	notebook:'??????',
	one_level_up:'1??????',
	only:'???',
	photos:'?????',
	picasaweb:'Picasa',
	prdhp:'??????',
	preview:'?????',
	products:'??????',
	reader:'????',
	ref_links:'?????',
	results:'??',
	safemode:'??????',
	save:'????',
	scholar:'??',
	search:'??',
	searchers:'??',
	self_tab:'?????',
	sites:'???',
	small:'???',
	special:'?????',
	str_url:'????URL',
	styles:'????',
	text:'??',
	this_:'??',
	title:'????',
	toolbar:'?????',
	translate:'????',
	trycache:'???????????',
	ui_bar_adv_search:'????????????',
	ui_bar_menu_icons:'??????????????',
	ui_bar_menu_static:'??????????',
	ui_bar_right_panel:'????????????',
	ui_bar_sch_icons:'??????',
	ui_bar_searchers:'?????????',
	ui_bar_suggest:'Google?????',
	ui_bar_suggest_lngs:'?????',
	ui_bar_username:'???????????????',
	ui_chk_upd:'??????????',
	ui_css_defaults:'???????????????',
	ui_css_mozBrd:'?? (Firefox)',
	ui_grl_run_over:'??GoogleFx??',
	ui_grl_show_logo:'??GoogleFx??',
	ui_img_cnt_type:'???????????????????',
	ui_img_col_type:'?????????????????',
	ui_img_color:'??????????????????',
	ui_img_ext_size:'?????????????????',
	ui_img_filetype:'????????????????????',
	ui_img_prev_onThumb:'???????',
	ui_img_prev_rt:'????',
	ui_img_preview:'???????',
	ui_img_safe:'????????????????????????',
	ui_img_sizebar:'?????????????????',
	ui_img_sz_icons:'??????',
	ui_res_autopages:'???',
	ui_res_autopages_nb:'???????????',
	ui_res_autopages_rt:'?????????',
	ui_res_counter:'??????',
	ui_res_ref_icon:'??????????????',
	ui_res_ref_pos:'???',
	ui_res_remove_ads:'????Google???(?????)',
	ui_res_remove_tracks:'?????????Google?????? (??)',
	ui_res_sitefilter:'???????????????',
	ui_res_targetLnks:'???????????????',
	ui_res_th_lft:'???',
	ui_res_th_sz:'????',
	ui_res_thumbs:'????????',
	ui_title:'??????',
	ui_txt_cache:'Ctrl+???? / Shift+???? ???????????',
	ui_txt_gs:'?????? ???? ??????? / ??????????????',
	ui_update:'Install GoogleFx',
	url:'Url',
	video:'???',
	webhp:'???',
	week:'?',
	width:'?',
	xlarge:'X-?',
	year:'?',
	youtube:'YouTube',
	};
	break;
	default:
	LNG = {
	about:'About',
	alerts:'Alerts',
	all:'All',
	avatars:'Avatars',
	blogs:'Blogs',
	books:'Books',
	buttons:'Buttons',
	by:'by',
	calc:'Calculator',
	calendar:'Calendar',
	cliparts:'ClipArts',
	close:'Close',
	cnt_box:'Content Box',
	code:'Code',
	color:'Color',
	content:'Content',
	counter:'Counter Number',
	day:'Day',
	defaults:'Defaults',
	desc:'Description',
	dirhp:'Directory',
	disabled:'Disabled',
	docs:'Documents',
	domain:'domain',
	domain:'domain',
	enabled:'Enabled',
	extreme:'Extreme',
	faces:'Faces',
	filetype:'Filetype',
	filters:'Filters',
	finance:'Finance',
	from:'from',
	general:'General',
	grayscale:'Grayscale',
	groups:'Groups',
	height:'Height',
	here:'here',
	high:'High',
	highest:'Highest',
	highlights:'Highlights',
	history:'History',
	huge:'Huge',
	icon:'Icons',
	ig:'iGoogle',
	images:'Images',
	in_:'in',
	labs:'Labs',
	large:'Large',
	last:'last',
	linearts:'LineArts',
	links:'Links',
	login:'Login',
	mail:'Mail',
	maps:'Maps',
	mchrome:'Monochrome',
	medium:'Medium',
	min:'Min',
	month:'Month',
	more:'More',
	new_tab:'New Tab',
	news:'News',
	normal:'Normal',
	notebook:'NoteBook',
	one_level_up:'One Level Up',
	only:'Only',
	photos:'Photos',
	picasaweb:'Picasa',
	prdhp:'Shopping',
	preview:'Preview',
	products:'Shopping',
	reader:'Reader',
	ref_links:'Reference Links',
	results:'Results',
	safemode:'SafeMode',
	save:'Save',
	scholar:'Scholar',
	search:'Search',
	searchers:'Searchers',
	self_tab:'Current Tab',
	sites:'Sites',
	small:'Small',
	special:'Special',
	str_url:'String URL',
	styles:'Styles',
	text:'Text',
	this_:'this',
	title:'Title',
	toolbar:'ToolBars',
	translate:'Translate',
	trycache:'try Cache',
	ui_bar_adv_search:'Advance Search Toolbars',
	ui_bar_menu_icons:'Show icons in menu',
	ui_bar_menu_static:'Make Menu Static',
	ui_bar_right_panel:'Enable Right Panel',
	ui_bar_sch_icons:'show icons',
	ui_bar_searchers:'Other Searchers Toolbar',
	ui_bar_suggest:'Google Suggest',
	ui_bar_suggest_lngs:'multi-language feature',
	ui_bar_username:'Convert username to short name',
	ui_css_defaults:'Set to defaults the styles of',
	ui_css_mozBrd:'Round Borders (Firefox)',
	ui_grl_run_over:'Run this script over',
	ui_grl_show_logo:'Show GoogleFx Logo',
	ui_img_cnt_type:'Show Content Type options',
	ui_img_col_type:'Show Color Type options',
	ui_img_color:'Show Image Color options',
	ui_img_ext_size:'Show Exact Size options',
	ui_img_filetype:'Show Image File Type options',
	ui_img_prev_onThumb:'set over thumbs',
	ui_img_prev_rt:'reaction time',
	ui_img_preview:'Show original on MouseOver',
	ui_img_safe:'Show SafeMode Switcher',
	ui_img_sizebar:'Show Image Size options',
	ui_img_sz_icons:'icons mode',
	ui_res_autopages:'Streaming Auto-Page',
	ui_res_autopages_nb:'show navigation bar',
	ui_res_autopages_rt:'scroll detection ratio',
	ui_res_counter:'Show Counter Numbers',
	ui_res_ref_icon:'Text Ref-links to Icons',
	ui_res_ref_pos:'align icons to Right',
	ui_res_remove_ads:'Hide Google Ads (Sponsor)',
	ui_res_remove_tracks:'Remove Google Tracks from links (History)',
	ui_res_sitefilter:'Enable site filter feature',
	ui_res_targetLnks:'Default Target of Links',
	ui_res_th_lft:'align thumbs to Left',
	ui_res_th_sz:'ratio size',
	ui_res_thumbs:'Thumbshots of sites',
	ui_title:'User Settings',
	ui_txt_cache:'Ctrl+Click / Shift+Click to continue in Google Cache',
	ui_txt_gs:'Right key or Click to more suggest / DoubleClick to search.',
	ui_update:'Install GoogleFx',
	url:'Url',
	video:'Video',
	webhp:'Web',
	week:'Week',
	width:'Width',
	xlarge:'X-Large',
	year:'Year',
	youtube:'YouTube'
	};
}


/***********************************/
/**  Common Global Variables	   */
/***********************************/
STYLES = SCRIPTS = '';

// Resolve Current Google Service through the URL
var $lastHost = doc.location.hostname.split('.')[0];
var $firstPath = doc.location.pathname.substr(1).split('/')[0];

if (GFX.Srv[$lastHost] != null){
	SERVICE = $lastHost;
} else {
	SERVICE = (GFX.Srv[$firstPath] != null) ? $firstPath : false;
}

URI = {
		hash		: doc.location.hash,
		host		: doc.location.hostname.split('.'),
		hostname	: doc.location.hostname,
		location	: doc.location.toString(),
		path		: doc.location.pathname.substr(1).split('/'),
		pathname	: doc.location.pathname,
		port		: doc.location.port,
		protocol	: doc.location.protocol,
		query		: {},
		search		: doc.location.search.substring(1).replace(/[a-z0-9_]+=&/ig,'')
};

// Convert URI Query in an Object
var items = URI.search.split('&');
for(var i=0,len=items.length; i < len; i++){
	var val = items[i].split('=');
	URI.query[val[0]] = unescape(val[1]);
};

URI.query.start = isNaN(URI.query.start) ? 0 : Number(URI.query.start);
URI.query.num = isNaN(URI.query.num) ? 0 : Number(URI.query.num);

QUERYS = {};
if(URI.query.q && URI.query.q.indexOf(':') != -1 && !isGoogleCache)
{
	var q = URI.query.q.replace(/\+/g,' ');
	var q  = q.split(/([\-]?\s*[a-zA-Z-]+\s*\:\s*[^\s]*)/g);
	var clean_q = '';
	
	for(var i=0, len=q.length; i < len; i++){
		var name = q[i].split(':');
		if(!name[1] || name[0].length < 3){
			clean_q += q[i];
			continue;
		}
		var val = name[1].replace(/^\s+|\s+$/g,'');
		name = name[0].replace(/^\s+|\s+$/g,'').replace(/\-/,'_');
		QUERYS[name] = ( QUERYS[name] ? QUERYS[name].concat([val]) : [val] );
	}
	QUERYS.q = clean_q;
} else {
	QUERYS.q = URI.query.q;
}

if(URI.query.tbs){
	var items = URI.query.tbs.split(',');
	URI.query.tbs = {};
	for(var i=0,len=items.length; i < len; i++){
		var val = items[i].split(':');
		URI.query.tbs[val[0]] = unescape(val[1]);
	}
} else {
	URI.query.tbs = {};
}

hasSomeQuery = (URI.query.q && URI.query.q.length) || (URI.query.as_q && URI.query.as_q.length) || (URI.query.as_oq && URI.query.as_oq.length) || (URI.query.as_epq && URI.query.as_epq.length) || (URI.query.as_eq && URI.query.as_eq.length) || false;

//  Rules for run services
var srvRule = {
				blogsearch	: hasSomeQuery,
				books		: hasSomeQuery,
				code		: URI.path[2],
				images		: hasSomeQuery && URI.path[0] && URI.path[0] == 'images',
				intl		: URI.path[2] && URI.path[2] == 'options',
				products	: hasSomeQuery,
				scholar		: hasSomeQuery,
				search		: hasSomeQuery && !QUERYS.define
};
canRun = {
				googlefx	: ( SERVICE ? GFX.Srv[SERVICE] : true ),
				gSuggest	: false,
				blogsearch	: false,
				books		: false,
				calendar	: false,
				code		: false,
				chrome		: false,
				docs		: false,
				finance		: false,
				groups		: false,
				history		: false,
				ig			: false,
				images		: false,
				intl		: false,
				local		: false,
				mail		: false,
				maps		: false,
				news		: false,
				options		: false,
				picasaweb	: false,
				products	: false,
				reader		: false,
				scholar		: false,
				search		: false,
				spreadsheets: false,
				translate	: false,
				video		: false
};

if ($defined(canRun[SERVICE])) {
	canRun[SERVICE] = ($defined(srvRule[SERVICE]) ? srvRule[SERVICE] : true);
}
canRun.gSuggest = GFX.gSuggest && !canRun.finance && !canRun.mail && (SERVICE != 'code') && URI.pathname != '/dictionary';

GFX.rThSize = (GFX.rThSize > 2) ? 2: ((GFX.rThSize < 0) ? 0.2 : GFX.rThSize);
if(GFX.rAutoPagRT < 1){ GFX.rAutoPagRT = 1; }

if(!isByRequest){
	DB.titleSz = CSS.res.tit.match(/font-size\s?:([0-9]+)/i);
	DB.titleSz = ( DB.titleSz && DB.titleSz[1] ) ? Number(DB.titleSz[1]) : 12;
}

} //END GFX_Init



/***********************************/
/**  GoogleFx First Run			   */
/***********************************/
var GFX_FirstRun = function()
{
	var doc = document;
	if (doc.location == top.location && doc.isGFXLoaded){return;}
	
	var docSize = doc.getScrollSize();
	
	// Windows Scroller Object
	WinScroller = new Fx.Scroll(window, {
		'fps'			: 50,
		'duration'		: 1000,
		'transition'	: 'quad:in:out'
	});
	

	BgBox = new Element('div',{
		'id'	: 'gfx-BgBox',
		'styles': {
			'display'	: 'none',
			'position'	: 'absolute',
			'top'		: '0',
			'left'		: '0',
			'z-index'	: '5555',
			'height'	: + docSize.y +'px',
			'width'		: + docSize.x +'px'
		}
	});

	
	/**	Private Functions
	*************************/
	var getQueryFromUrl = function()
	{
		if(!hasSomeQuery){return '';}
		var q = '';

		if(URI.query.q){
			q += URI.query.q.replace(/\+/g, ' ');
		} else {
			// with all
			q += (URI.query.as_q) ? URI.query.as_q.replace(/\+/g, ' ') : '';
			// with some
			q += (URI.query.as_oq) ? ' ' + URI.query.as_oq.replace(/\+/g, ' OR ') : '';
			// with exact
			q += (URI.query.as_epq) ? ' "' + URI.query.as_epq.replace(/\+/g, ' ') + '"' : '';
			// with not
			q += (URI.query.as_eq) ? ' -' + URI.query.as_eq.replace(/\+/g, ' -') : '';
		}
		return q;
	};

	var queryForm = getQueryFromUrl();
	var queryFormEncode =  encodeURL(queryForm);
	
	// Search in all Searchers
	//**********************
	var inAllSearchers = function()
	{
		var q = encodeURL(($nd('//input[@name="q"]') || '').value);
		
		var type = $el('gfx-searchers').firstChild.value;
		var sites = SEARCHER[type];
		var local = sites[LANG] ? sites[LANG] : sites.en;

		sites = sites._commons;

		for (var n in local)
		{
			if (sites[n])
			{
				if (local[n][0]){ sites[n][0] = local[n][0]; }
				if (local[n][1]){ sites[n][1] = local[n][1]; }
			} else {
				sites[n] = local[n];
			}
		}
		
		for (var name in sites)
		{
			var item = sites[name];
			var href = 'http://';
			href += item[0];
			if(typeof item[1] == 'object')
			{
				href += (item[1])[0];
				href += q;
				href += (item[1])[1];
			} else {
				href += item[1];
				href += q;
			}
			window.open( href, '_blank');
		}
	};

	var getSearchers = function (type, query)
	{
		var q = query || queryFormEncode;
		if (!SEARCHER[type]){ type = 'search'; }
		var sites = SEARCHER[type];
		var local = sites[LANG] ? sites[LANG] : sites.en;

		// change & add items to common list
		sites = sites._commons;
		for (var n in local)
		{
			if(sites[n])
			{
				if (local[n][0]){ sites[n][0] = local[n][0]; }
				if (local[n][1]){ sites[n][1] = local[n][1]; }
			} else {
				sites[n] = local[n];
			}
		}

		var s = '';

		// Set the sites
		for (var name in sites)
		{
			var item = sites[name];
			s += '<a href="http://';
			s += item[0] || GM_log('Missing domain in: '+name);
			if(typeof item[1] == 'object')
			{
				s += (item[1])[0] || GM_log('Missing query in: '+name);
				s += q;
				s += (item[1])[1] || GM_log('Missing query in: '+name);
			} else {
				s += item[1] || GM_log('Missing query in: '+name);
				s += q;
			}
			s += '" title="' + name + '"';
			s += ' target="' + GFX.rTrgLnk +'">';
			if(GFX.bSrchsIcn)
			{
				s += '<img src="';
				if(item[2])
				{
					if( item[2].substr(0,1) == '/' )
					{
						s += 'http://';
						s += item[0];
						s += item[2] || GM_log('Wrong FavIcon set: '+name);
						if( item[2].substr(-3) != 'ico' ){ s += '/favicon.ico'; }
					}else{
						s += item[2] || GM_log('Wrong FavIcon set: '+name);
					}
				} else {
					s += 'http://';
					s += item[0];
					s += '/favicon.ico';
				}
				s += '" width="16px" height="16px" />';
			}
			s += name;
			s += '</a>';
		}
		return s;
	};

	// Add Searchers ToolBar & Menu
	//******************************
	var addSearchers = function (target, type)
	{
		if(!GFX.bSrchs) {return;}
		if(!target) {
			GM_log('Target object not found in: '+type);
			return;
		}
		var query = queryFormEncode;

		// Top Menu
		var itemBar = new Element('a', {
			'class'	: 'gb3 search',
			'href'	: '#',
			'html'	: LNG.searchers + ' &#9660;'
		});
		itemBar.set('onclick'	 , 'gmnu.open(event,"gfx-searchers-menu");return false;');
		itemBar.set('onmouseover', 'gmnu.open(event,"gfx-searchers-menu");return false;');
		
		var menuHolder = new Element('div', {
			'id'	: 'gfx-searchers-menu',
			'styles': {
				'display':'none'
			}
		});
		
		for(var name in SEARCHER){
			var a = new Element('a', {
				'class'	: name,
				'href'	: '#',
				'html'	: LNG[name],
				'events':{
					'click'	: function(ev){
						var self = ev.target;
						var sb = $el('gfx-searchers');
						sb.lastChild.innerHTML = getSearchers(self.className, query);
						sb.firstChild.nextSibling.innerHTML = self.title + ':';
						sb.firstChild.nextSibling.className = self.className;
						sb.firstChild.value = self.className;
					}
			}});
			menuHolder.appendChild(a);
		};

		//  ToolBar
		var toolBar = new Element('div', {'id' : 'gfx-searchers'});

		var comboBox = new Element('input', {
			'id'	: 'gfx-searchers-cbox',
			'title'	: LNG.search_in + ' ' + LNG.all,
			'type'	: 'checkbox',
			'value'	: type,
			'events': {
				'click' : function(ev){
					var frm =$nd('//FORM');
					var btn =$nd('//INPUT[@type="submit"]');
					if(ev.target.checked)
					{
						frm.addEvent('submit', inAllSearchers);
						btn.value += ' + ' + LNG.searchers;
					} else{
						frm.removeEvent('submit', inAllSearchers);
						btn.value = btn.value.replace(' + ' + LNG.searchers,'');
					}
				}
		}});
		
		var comboBoxLabel = new Element('label', {
			'class'	: type,
			'for'	: 'gfx-searchers-cbox',
			'title'	: LNG.search_in + ' ' + LNG.all,
			'html'	: LNG[type] + ':'
		});

		var sitesList = new Element('div', {
			'class' : 'siteList',
			'html'	: getSearchers(type, query)
		});

		GBAR.appendChild(itemBar);
		GBAR.appendChild(menuHolder);

		toolBar.appendChild(comboBox);
		toolBar.appendChild(comboBoxLabel);
		toolBar.appendChild(sitesList);
		
		toolBar.inject(target,(target.id=='header'?'bottom':'before'));

	};

	// Auto-Paging
	//*************
	var AutoPage = function(obj){
		var self = arguments.callee;
		var iframe, lastSrc, holder, xPathContent, xPathNext, nextUrl, prevUrl, prevStart, nextStart, isStreaming=false, retry = 0, iframeID, adjCnt = 0, isReverse = false, navbar, navContent;
		
		var createRequest = function()
		{
			retry = 1;
			iframe = new Element('iframe', {
				'id'		: 'gfx-stream',
				'styles'	: {
					'height'	: '0',
					'left'		: '0',
					'margin'	: '0 0 0 -10000px',
					'position'	: 'absolute',
					'top'		: '0',
					'width'		: '100%'
				}
			});
			iframe.inject(body);
			
			if(GFX.rAutoPagNav){
				navbar = new Element('table', {
					'id'	: 'gfx-nav',
					'styles': {
						'border-collapse'	: 'collapse',
						'bottom'			: '0',
						'direction'			: 'ltr',
						'margin-bottom'		: '5px',
						'margin-right'		: '10px',
						'position'			: 'fixed',
						'right'				: '0',
						'text-align'		: 'center',
						'z-index'			: '98',
						'max-width'			: '35%'
					},
					'html'	:
				'<tr valign=top>\
					<td class=b>\
						<a href="#pageTop">\
							<span class="csb ch" style="width: 44px; margin-left: auto;"></span>\
							<div class=prev style="margin-right:8px;font-weight:bold;">Previous</div>\
						</a>\
					</td>\
					<td>\
					<div  style="height:65px;width:150px;max-width:150px;overflow:hidden;overflow-y:hidden;overflow-x:scroll;">\
						<div id=nav-content></div>\
						</div>\
					</td>\
					<td class=b>\
						<a href="#pageBot">\
							<span class="csb ch" style="background-position:-76px 0; margin-right:34px; width:66px;"></span>\
							<div class=prev style="font-weight:bold;">Next</div>\
						</a>\
					</td>\
				</tr>'
				});
				navbar.inject(holder, 'before');
				navContent = $el('nav-content');
			}
		};

		var doRequest = function()
		{
			isStreaming = true;
			iframe.addEvent('load', onLoad);
			// Use "src" attibute instead of .contentWindow.location.replace because 
			// the latter makes the routine does not work under Opera
			if(isOpera){
				iframe.src = (isReverse ? prevUrl : nextUrl) + '#gfx-ap';
			} else {
				iframe.contentWindow.location.replace(nextUrl + '#gfx-ap');
			}
		};
		
		var onLoad = function()
		{
			// Prevent Double Ejecution
			if(isOpera){
				if(lastSrc == iframe.src){ return; }
				lastSrc = iframe.src;
			} else {
				if(lastSrc == iframe.contentWindow.location){ return; }
				lastSrc = iframe.contentWindow.location;
			}
	
			top.window.clearInterval(iframeID);
			iframeID = top.window.setInterval( onTimeOut, 100 );
		};
		
		var onTimeOut = function()
		{
			if (!isStreaming){ return; }
			
			var contentDocument = iframe.contentDocument;
			if (!contentDocument){ return; }
			
			var isLoaded = $nd('//var[@id="gfx-req-loaded"]', contentDocument.body, contentDocument);
			
			if (!isLoaded && retry < 20) {
				retry++;
				return;
			}

			retry = 0;
			top.window.clearInterval(iframeID);
			
			top.window.setTimeout(function(){
				var contentNodes = contentDocument.evaluate(xPathContent, contentDocument.body, null, 7, null);
				if(contentNodes)
				{
					var nodesLength = contentNodes.snapshotLength;
					var node, iNode;
					
					// Previous Page
					if(isReverse){
						var len = nodesLength-1;
						do {
							node = contentNodes.snapshotItem(len);
							iNode = contentDocument.importNode(node,true);
							$el(iNode).inject(holder, 'top');
						} while (len--);

						if(GFX.rAutoPagNav){
							holder.append(new Element('a',{'name':'pos_'+prevStart}),'top');
							navContent.append(new Element('a', {
								'href': '#pos_' + prevStart,
								'html':	'<span class="csb ch"></span>' + (Math.round(prevStart/10)+1)
							}), 'top');
							navContent.parentNode.scrollTop = 0;
						}
						
						prevStart -= 10;
												
						if (prevStart<0) {
							prevUrl	= (prevStart > -10)	? prevUrl.replace(/start=[0-9]*/,'start=0').replace(/num=[0-9]*/,'num='+(10+prevStart))	: false;
						} else {
							prevUrl	= prevUrl.replace(/start=[0-9]*/,'start='+prevStart);
						}

						WinScroller.start(0, (isOpera?win.innerHeight:doc.clientHeight));
						
					// Next Page
					} else {
						for (var i=0; i < nodesLength; i++) {
							node = contentNodes.snapshotItem(i);
							iNode = contentDocument.importNode(node,true);
							$el(iNode).inject(holder);
						}
				
						if(GFX.rAutoPagNav){
							holder.append(new Element('a',{'name':'pos_'+nextStart}));
							navContent.append(new Element('a',{
								'href'	: '#pos_'+nextStart,
								'html'	: '<span class="csb ch"></span>' + (Math.round(nextStart/10)+1)
							}));
							navContent.parentNode.scrollTop = 9999999;
						}
					
						var nextLink = $nd(xPathNext, contentDocument.body, contentDocument);
						
						if( !nextLink || nextStart > Number(nextLink.href.match(/start=([0-9]+)/)[1]) )
						{
							$el(top.window).removeEvent('scroll', onScroll);
							var nofilter = $nd('//a[contains(@href,"filter=0")]', contentDocument.body, contentDocument);
							if(nofilter){
								nofilter.inject(holder, 'bottom');
								nofilter.setAttribute('style', '\
									background-color	: #fdc;\
									display				: block;\
									text-align			: center;\
									width				: 100%'
								);
							}
						} else {
							nextUrl		= nextLink.href;
							nextStart	= nextUrl.match(/start=([0-9]+)/);
							nextStart	= (nextStart[1] ? Number(nextStart[1]) : 0);
							self.adjustCounter();
						}
					
					}
					
					isReverse = false;

				}
				iframe.removeEvent('load', onLoad);
				isStreaming = false;
			}, 250);
		};
		
		var onScroll = function(ev)
		{
			if( isStreaming ){ return; }
			var st = doc.body.scrollTop + doc.documentElement.scrollTop;
			if(st==0){
				if(prevUrl){
					isReverse = true;
					doRequest();
				}
				return;
			}
			// body.scrollTop returns 0 when body.clientHeight have the same value of body.scrollHeight and
			// documentElement.scrollTop returns 0 when documentElement.clientHeight have the same value of body.scrollHeight
			var ratePos = (doc.body.scrollHeight-st) / (doc.body.scrollTop?doc.body.clientHeight:doc.documentElement.clientHeight);
			// Ok.. now we have the right rate position
			if( ratePos < GFX.rAutoPagRT ){ doRequest(); }
		};

		self.adjustCounter = function(num){
			if(!nextUrl){ return; }
			adjCnt += num||0;
			if(/gfx_ac/.test(nextUrl)) {
				nextUrl = nextUrl.replace(/gfx_ac=[0-9\-]*/, 'gfx_ac='+adjCnt);
			} else {
				nextUrl += '&gfx_ac='+ adjCnt;
			}
		};
		
		// Syntax of Argument Object 
		//          { 
		//               holder    :    " string xPath of Holder "   or   Node element  , 
		//               content   :    " string xPath of Content to import  ( Single or Multi elements  )  "  ,
		//               nextLink  :    " string xPath of Next Page "  ,
		//          } 
		var init = function ( obj )
		{
			if (!GFX.rAutoPag){ return; }

			holder = (typeof obj.holder == 'string' ? $nd(obj.holder) : $el(obj.holder));
			if(!holder) { GM_log('holder not found with: ' + obj.holder); return; }

			xPathContent = obj.content;
			xPathNext = obj.nextLink || '//*[contains(@id,"nav")]/descendant::a[last()]';

			var nav = $nd(xPathNext+'/ancestor::*[starts-with(@id,"nav")] | '+xPathNext+'/ancestor::table');
			if(nav){ nav.hide(); }
			
			var next = $nd(xPathNext);
			if(!next){ GM_log('Next-Page not found with: '+xPathNext); return; }
			if(!next.href){ return; }

			nextUrl = next.href.replace(/num=[0-9]*/,'num=10');
			self.adjustCounter();

			nextStart	= Number(nextUrl.match(/start=([0-9]+)/)[1]);
			prevStart	= Number(URI.query.start||-1) -10;
			var currStart = (URI.query.start||1);

			if(nextStart <= currStart){ return; }
			
			var currentUrl = URI.location + (!URI.query.start ? '&start=0' : '');
			
			prevUrl	= prevStart > -10 ? currentUrl.replace(/num=[0-9]*/,'num='+(prevStart<0?prevStart*-1:10)).replace(/start=[0-9]*/,'start='+(prevStart>0?prevStart:0)) : false;
			

			createRequest();
			
			if(GFX.rAutoPagNav){
				body.append(new Element('a',{'name':'pageTop'}),'top');
				body.append(new Element('a',{'name':'pageBot'}));
				
				holder.append(new Element('a', {
					'name'	:	'pos_'+currStart,
					'styles': {
						'position'	: 'absolute',
						'top'		: '0',
						'margin-top': '-100px;'
					}
				}),'top');
				navContent.append(new Element('a', {
					'href'	: '#pos_' + currStart,
					'html'	: '<span class="csb ch"></span>' + (Math.round(currStart/10)+1)
				}));
				var len = (URI.query.num||10);
				var step = docSize.y / len;
				for (var i=10; i<len; i+=10){
					holder.append(new Element('a',{
						'name'	: 'pos_'+i,
						'styles': {
							'position'	: 'absolute',
							'top'		: '0',
							'margin-top': (i*step-50) +'px'
						}
					}),'top');
					navContent.append(new Element('a', {
						'href'	: '#pos_'+i,
						'html'	: '<span class="csb ch"></span>' + (Math.round((currStart+i)/10)+1)
					}));
				}
			}
			
			$el(top.window).addEvent('scroll', onScroll);
			
			var start = nextUrl.match(/start=([0-9]+)/);
			if(start[1] && Number(start[1]) < 50){
				doRequest();
			}
		};
		
		init(obj);
	};
	
	//  GUI Settings
	//**********************************
	var addGUI = function()
	{
		GFX_ui_Styles();

		var html = '\
		<div id=gfx-gui-dragger></div>\
		  <div class=header>\
			<span>\
			  <div>\
			  <img src="http://www.google.com/images/google_sm.gif" width=143 height=59 style="border:0;margin:0;" />\
			  <a id=gfx-logo href="'+ SCRIPT_URL +'" target="_blank">v'+ SCRIPT_VER +'</a>\
			 </div>\
			  <ul id=tabmenu>\
				<li><span href=# onclick=setTabMnu(0,event)>' + LNG.general + '</span></li>\
				<li><span href=# onclick=setTabMnu(1,event)>' + LNG.toolbar + '</span></li>\
				<li><span href=# onclick=setTabMnu(2,event)>' + LNG.results + '</span></li>\
				<li><span href=# onclick=setTabMnu(3,event)>' + LNG.images + '</span></li>\
				<li><span href=# onclick=setTabMnu(4,event)>' + LNG.styles + '</span></li>\
				<li><span href=# onclick=setTabMnu(5,event)>' + LNG.about + '</span></li>\
			  </ul>\
			</span>\
		    <span class=subtitle>\
			  <h1>' + LNG.ui_title + '</h1>\
			  '+ (INFO.newVer ? '<a href="http://userscripts.org/scripts/source/31950.user.js" target=_blank>' + (LNG.ui_update||'Install GoogleFx') + ' v' + INFO.lastVer + '</a>':'<br/><form action="https://www.paypal.com/cgi-bin/webscr" method="post" target=_blank><input type="hidden" name="cmd" value="_s-xclick"><input type="hidden" name="encrypted" value="-----BEGIN PKCS7-----MIIHLwYJKoZIhvcNAQcEoIIHIDCCBxwCAQExggEwMIIBLAIBADCBlDCBjjELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb20CAQAwDQYJKoZIhvcNAQEBBQAEgYCReS+v9uDqRGe+dEa4hu821rjpR2RXUwr/CB4yRHH1SW31QhXDjg6FeADQnFfG24tXPPo8paqFYi1gHUjZZllziGcZUGxguVjTtDhMi1V7J3LcZPLvNeToei9LbTd/Ve4sCRpVIZhsG3W3to8O9PwqFt1gXifRtKrY/Vd/IVKj+zELMAkGBSsOAwIaBQAwgawGCSqGSIb3DQEHATAUBggqhkiG9w0DBwQIi0ADjynCkXWAgYhj6amABcYKIbWTKenjwY3ye4QPRHM2NN2pBC3S+3F6U9+O0ld2od0ycZso9JvP4QcKTJrw8u9PiRC1Lb5H8y+brQqTVcyU6g7oqTwFsFGowQeNg4ACktq6JNv139OpV41Mh8h5Escr0I2/YGcnOApwm22nXoI/o7PSUs+WFNoggwJ5TY3vSxy7oIIDhzCCA4MwggLsoAMCAQICAQAwDQYJKoZIhvcNAQEFBQAwgY4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgNVBAsUCmxpdmVfY2VydHMxETAPBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tMB4XDTA0MDIxMzEwMTMxNVoXDTM1MDIxMzEwMTMxNVowgY4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgNVBAsUCmxpdmVfY2VydHMxETAPBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDBR07d/ETMS1ycjtkpkvjXZe9k+6CieLuLsPumsJ7QC1odNz3sJiCbs2wC0nLE0uLGaEtXynIgRqIddYCHx88pb5HTXv4SZeuv0Rqq4+axW9PLAAATU8w04qqjaSXgbGLP3NmohqM6bV9kZZwZLR/klDaQGo1u9uDb9lr4Yn+rBQIDAQABo4HuMIHrMB0GA1UdDgQWBBSWn3y7xm8XvVk/UtcKG+wQ1mSUazCBuwYDVR0jBIGzMIGwgBSWn3y7xm8XvVk/UtcKG+wQ1mSUa6GBlKSBkTCBjjELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb22CAQAwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQUFAAOBgQCBXzpWmoBa5e9fo6ujionW1hUhPkOBakTr3YCDjbYfvJEiv/2P+IobhOGJr85+XHhN0v4gUkEDI8r2/rNk1m0GA8HKddvTjyGw/XqXa+LSTlDYkqI8OwR8GEYj4efEtcRpRYBxV8KxAW93YDWzFGvruKnnLbDAF6VR5w/cCMn5hzGCAZowggGWAgEBMIGUMIGOMQswCQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxFDASBgNVBAoTC1BheVBhbCBJbmMuMRMwEQYDVQQLFApsaXZlX2NlcnRzMREwDwYDVQQDFAhsaXZlX2FwaTEcMBoGCSqGSIb3DQEJARYNcmVAcGF5cGFsLmNvbQIBADAJBgUrDgMCGgUAoF0wGAYJKoZIhvcNAQkDMQsGCSqGSIb3DQEHATAcBgkqhkiG9w0BCQUxDxcNMDkwNTExMDE1MTAzWjAjBgkqhkiG9w0BCQQxFgQU6vfADb9kgxpy3/NuWBnxop9zG+UwDQYJKoZIhvcNAQEBBQAEgYB3qTHregEWmfzogHULhUs3u61a0KGv+ylqbpZODh6eFpUawaC7tDIp3AOJ+HxMDugLgd9IuFu+uZkD2VUB7M5pXJ9sETFXpGoKK7CnRa4+eCaCGl8XQgCFMIyXEVHMLNZ4h75hl93iSLgfLIjw7M2PqR4/9Htqa/BuzBqgnC2sxA==-----END PKCS7-----"><input type="image" src="https://www.paypal.com/en_GB/i/btn/btn_donate_SM.gif" border="0" name="submit"><img alt="" border="0" src="https://www.paypal.com/es_XC/i/scr/pixel.gif" width="1" height="1"></form>') +'\
		     </span>\
		  </div>\
		  <div class=tabs>\
			<fieldset id=gfx-tab-0>\
				<legend>' + LNG.general + '</legend>\
				<div class=services>\
					<span class=subtitle>'+ LNG.ui_grl_run_over +'</span><br>';
					
					// Sort GFX.Srv object based on current Language
					var list = [];
					for (var o in GFX.Srv){
						if(LNG[o]){ list.push([LNG[o].toLowerCase(), o]); }
					}
					list.sort( function(a,b){return ( a[0] > b[0] ) - ( a[0] < b[0] );} );
					
					var len = list.length;
					var rows = Math.ceil(len/3);
					var dbrow = rows*2;
					len--;
					for(var i=0;i < rows;){
						html += '<label><input name=GFX.Srv.'+ list[i][1] +' type=checkbox ' +
						(GFX.Srv[list[i][1]] ? 'checked=checked' : '') +
						'>'+ LNG[list[i][1]] +'</label>';
						if( i+rows > dbrow ){ break; }
						html += '<label><input name=GFX.Srv.'+ list[i+rows][1] +' type=checkbox ' +
						(GFX.Srv[list[i+rows][1]] ? 'checked=checked' : '') +
						'>'+LNG[list[i+rows][1]]+'</label>';
						if( i+dbrow > len ){ break; }
						html += '<label><input name=GFX.Srv.'+ list[i+dbrow][1] +' type=checkbox ' +
						(GFX.Srv[list[i+dbrow][1]] ? 'checked=checked' : '') +
						'>' + LNG[list[i+dbrow][1]] + '</label>';
						i++;
					}

		html += '	</div>\
				<hr width=90% align=center>\
				<label><input name=GFX.gFxLogo type=checkbox \
				' + (GFX.gFxLogo ? 'checked=checked' : '')	+ '>'		+ LNG.ui_grl_show_logo		+'</label>\
			</fieldset>\
			<fieldset id=gfx-tab-1>\
				<legend>' + LNG.toolbar + '</legend>\
				<label><input name=GFX.gSuggest type=checkbox \
				' + (GFX.gSuggest ? 'checked=checked' : '')	+ ' \
					onclick="this.parentNode.nextSibling.className=(this.checked?\'sub\':\'sub disabled\');">\
				'														+ LNG.ui_bar_suggest		+'</label>\
				<label class=sub><input name=GFX.gSuggestLng type=checkbox \
				' + (GFX.gSuggestLng ? 'checked=checked' : '')+ '>'		+ LNG.ui_bar_suggest_lngs	+'</label>\
				<label><input name=GFX.bMnuFix type=checkbox '
				+ (GFX.bMnuFix ? 'checked=checked' : '')
				+ ' onclick="this.parentNode.nextSibling.className=(this.checked?\'sub\':\'sub disabled\');"'
				+ '>' + LNG.ui_bar_menu_static + '</label>\
				<label class=sub><input name=GFX.bMnuIcn type=checkbox '
				+ (GFX.bMnuIcn ? 'checked=checked' : '')
				+ '>' + LNG.ui_bar_menu_icons + '</label>\
				<label><input name=GFX.bShtName type=checkbox '
				+ (GFX.bShtName ? 'checked=checked' : '')
				+ '>' + LNG.ui_bar_username + '</label>\
				<label><input name=GFX.bAdvSrc type=checkbox '
				+ (GFX.bAdvSrc ? 'checked=checked' : '')
				+ '>' + LNG.ui_bar_adv_search + '</label>\
				<label><input name=GFX.bSrchs type=checkbox '
				+ (GFX.bSrchs ? 'checked=checked' : '')
				+ ' onclick="var o = this.parentNode.nextSibling; o.className = o.nextSibling.className = (this.checked ? \'sub\' : \'sub disabled\');"'
				+ '>' + LNG.ui_bar_searchers + '</label>\
				<label class=sub><input name=GFX.bSrchsIcn type=checkbox '
				+ (GFX.bSrchsIcn ? 'checked=checked' : '')
				+ '>' + LNG.ui_bar_sch_icons + '</label>\
				<label><input name=GFX.bRgtBox type=checkbox '
				+ (GFX.bRgtBox ? 'checked=checked' : '')
				+ '>' + LNG.ui_bar_right_panel + '</label>\
			</fieldset>\
			<fieldset id=gfx-tab-2>\
				<legend>' + LNG.results + '</legend>\
				<label><input name=GFX.rNoAds type=checkbox \
				' + (GFX.rNoAds ? 'checked=checked' : '')+ '>'		+ LNG.ui_res_remove_ads	+'</label>\
				<label><input name=GFX.rNoTrack type=checkbox \
				' + (GFX.rNoTrack ? 'checked=checked' : '')+ '>'	+ LNG.ui_res_remove_tracks	+'</label>\
				<label><input name=GFX.rAutoPag type=checkbox \
				' + (GFX.rAutoPag ? 'checked=checked' : '') + ' \
					onclick="this.parentNode.nextSibling.className=(this.checked?\'sub\':\'sub disabled\');">\
				'														+ LNG.ui_res_autopages		+'</label>\
				<label class=sub>' + LNG.ui_res_autopages_rt + ': \
					<select name=GFX.rAutoPagRT>\
					<option value=1' + (GFX.rAutoPagRT < 1.01 ? ' selected' : '') 
					+ '>' + LNG.min +'</option>\
					<option value=2' + ((GFX.rAutoPagRT > 1 && GFX.rAutoPagRT < 2.01) ? ' selected' : '')
					+ '>' + LNG.normal + '</option>\
					<option value=3' + ((GFX.rAutoPagRT > 2 && GFX.rAutoPagRT < 3.01) ? ' selected' : '')
					+ '>' + LNG.high + '</option>\
					<option value=4' + ((GFX.rAutoPagRT > 3 && GFX.rAutoPagRT < 4.01) ? ' selected' : '')
					+ '>' + LNG.highest + '</option>\
					<option value=5' + ((GFX.rAutoPagRT > 4) ? ' selected' : '')
					+ '>' + LNG.extreme + '</option>\
					</select>\
				</label>\
				<label class=sub><input name=GFX.rAutoPagNav type=checkbox \
				' + (GFX.rAutoPagNav ? 'checked=checked' : '')+ '>'		+ LNG.ui_res_autopages_nb	+'</label>\
				<label><input name=GFX.rThShots type=checkbox \
					onclick="$el(\'gfx-preview-tn\').style.display=(this.checked?\'inline-block\':\'none\'); var o=this.parentNode.nextSibling; o.className = o.nextSibling.className = (this.checked ? \'sub\' : \'sub disabled\');" '
				+ (GFX.rThShots ? 'checked=checked' : '')
				+ '>' + LNG.ui_res_thumbs + '</label>\
				<label class=sub><input name=GFX.rThLft type=checkbox onclick="var o = $el(\'gfx-preview-tn\'); o.style.float = (this.checked ? \'left\' : \'right\');" '
				+ (GFX.rThLft ? 'checked=checked' : '')
				+ '>' + LNG.ui_res_th_lft + '</label>\
				<label class=sub>' + LNG.ui_res_th_sz + ': '	+ '\
				<select name=GFX.rThSize onchange="var o=$el(\'gfx-preview-tn\');var v=this.options[this.selectedIndex].value;o.style.minWidth=(120*v)+\'px\';o.style.minHeight=(90*v)+\'px\';o=$el(\'PV_content_holder\');o.style.minHeight=((90*v)+10)+\'px\';">'	+ '\
				<option value=0.5'  + ((GFX.rThSize < 0.51) ? ' selected' : '')
				+ '>50%</option>\
				<option value=0.75' + ((GFX.rThSize > 0.5 && GFX.rThSize < 0.8) ? ' selected' : '')
				+ '>75%</option>\
				<option value=1'    + ((GFX.rThSize > 0.8 && GFX.rThSize < 1.1) ? ' selected' : '')
				+ '>100%</option>\
				<option value=1.25' + ((GFX.rThSize > 1 && GFX.rThSize < 1.3) ? ' selected' : '')
				+ '>125%</option>\
				<option value=1.5'  + ((GFX.rThSize > 1.3) ? ' selected' : '')
				+ '>150%</option>\
				</select>\
				</label>\
				<label><input name=GFX.rCounter type=checkbox \
					onclick="var o = $el(\'PV_res_cnt\'); o.style.display = (this.checked ? \'inline-block\' : \'none\');" ' 
				+ (GFX.rCounter ? 'checked=checked' : '')
				+ '>' + LNG.ui_res_counter+'</label>\
				<label><input name=GFX.rRefIcn type=checkbox '
				+ (GFX.rRefIcn ? 'checked=checked' : '')
				+ ' onclick="this.parentNode.nextSibling.className = (this.checked ? \'sub\' : \'sub disabled\'); var o = $el(\'PV_res_ref\'); o.innerHTML = (this.checked ? \'<a class=ch></a><a class=sm></a>\' : \'<a>in Cache</a> - <a>Similar pages</a>\'); $el(\'PV_res_ref\').className = (this.checked ? \'res_ref repos\' : \'res_ref\');"'
				+ '>' + LNG.ui_res_ref_icon + '</label>\
				<label class=sub><input name=GFX.rRefIcnRgt type=checkbox '
				+ ' onclick="var o = $el(\'PV_res_ref\'); o.className = (this.checked ? \'res_ref repos\' : \'res_ref\');"'
				+ (GFX.rRefIcnRgt ? 'checked=checked' : '')
				+ '>' + LNG.ui_res_ref_pos + '</label>\
				<label class=sub>' + LNG.ui_res_targetLnks + ': <select name=GFX.rTrgLnk>\
					<option value=_blank' + (GFX.rTrgLnk == '_blank' ? ' selected' : '')
					+ '>' + LNG.new_tab + '</option>\
					<option value=_self'  + (GFX.rTrgLnk == '_self' ? ' selected' : '')
					+ '>' + LNG.self_tab + '</option>\
					</select>\
				</label>\
				<label><input name=GFX.rSiteFlt type=checkbox '
				+ (GFX.rSiteFlt ? 'checked=checked' : '')
				+ ' onclick="var o=$el(\'PV_res_url\');o.innerHTML=(this.checked?\'<a class=qs></a>\':\'\')+\'www.domain.url - 99k\';"'
				+ '>' + LNG.ui_res_sitefilter + '</label>\
			</fieldset>\
			<fieldset id=gfx-tab-3>\
				<legend>' + LNG.images + '</legend>\
				<label><input name=GFX.iSafe type=checkbox '
				+ (GFX.iSafe ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_safe + '</label>\
				<label><input name=GFX.iSzBar type=checkbox '
				+ (GFX.iSzBar ? 'checked=checked' : '')
				+ ' onclick="this.parentNode.nextSibling.className = (this.checked?\'sub\':\'sub disabled\');"'
				+ '>' + LNG.ui_img_sizebar + '</label>\
				<label class=sub><input name=GFX.iSzBarIcn type=checkbox '
				+ (GFX.iSzBarIcn ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_sz_icons + '</label>\
				<label><input name=GFX.iColType type=checkbox '
				+ (GFX.iColType ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_col_type + '</label>\
				<label><input name=GFX.iColor type=checkbox '
				+ (GFX.iColor ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_color + '</label>\
				<label><input name=GFX.iCntType type=checkbox '
				+ (GFX.iCntType ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_cnt_type + '</label>\
				<label><input name=GFX.iExactSz type=checkbox '
				+ (GFX.iExactSz ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_ext_size + '</label>\
				<label><input name=GFX.iType type=checkbox '
				+ (GFX.iType ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_filetype + '</label>\
				<label><input name=GFX.iPrev type=checkbox '
				+ (GFX.iPrev ? 'checked=checked' : '')
				+ ' onclick="this.parentNode.nextSibling.className=(this.checked?\'sub\':\'sub disabled\');"'
				+ '>' + LNG.ui_img_preview + '</label>\
				<label class=sub><input name=GFX.iPrevNoIcn type=checkbox '
				+ (GFX.iPrevNoIcn ? 'checked=checked' : '')
				+ '>' + LNG.ui_img_prev_onThumb + '</label>\
				<label class=sub>' + LNG.ui_img_prev_rt + ': '
				+ '<input name=GFX.iPrevRT type=text size=8 onblur="closeList();" value='
				+ GFX.iPrevRT + '>\
				<span onclick="addList(this,\'0,100,200,300,400,500,600,700,800,900,1000\');" \
				class=virtual-list-icon>&#9660;</span></label>\
			</fieldset>\
			<fieldset id=gfx-tab-4>\
				<legend>' + LNG.styles + '</legend>\
				<input id=gfx-css-value type=hidden>\
				<select id=gfx-css-list>\
				  <option id=CSS.cmn.btns>'		+ LNG.buttons		+'</option>\
				  <option id=CSS.cmn.btnsOv>'	+ LNG.buttons		+' (Over)</option>\
				  <option id=CSS.cmn.body>Background '+ LNG.color	+'</option>\
				  <option id=CSS.res.box>::'	+ LNG.cnt_box		+'</option>\
				  <option id=CSS.res.cnt>::'	+ LNG.counter		+'</option>\
				  <option id=CSS.res.tit>::'	+ LNG.title			+'</option>\
				  <option id=CSS.res.dsc>::'	+ LNG.desc			+'</option>\
				  <option id=CSS.res.hl>::'		+ LNG.highlights	+'</option>\
				  <option id=CSS.res.url>::'	+ LNG.str_url		+'</option>\
				  <option id=CSS.res.ref>::'	+ LNG.ref_links		+'</option>\
				</select>\
				<button id=gfx-css-reset type=button>'	+ LNG.defaults	+'</button>\
				<table class=gfx-css-styles style="margin-top:10px;">\
				<tr>\
				<td>\
					Font-Family:\
				</td><td colspan=3>\
					<input id=CSS_font-family type=text size=44 onblur="closeList()"><span onclick="addList(this,\'Arial,Arial Black,Comic Sans MS,Courier New,Georgia,Impact,Times New Roman,Trebuchet MS,Verdana\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				<td>\
					Font-Size:\
				</td><td>\
					<input id=CSS_font-size type=text size=8 pattern="[0-9\.]{1,2}(px|pt)" onblur="closeList()"><span  onclick="addList(this,\'6pt,7pt,8pt,9pt,10pt,11pt,12pt,13pt,14pt,15pt,16pt,17pt,18pt,19pt,20pt,21pt,22pt,23pt,24pt,25pt,26pt,27pt,28pt\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Font-Weight:\
				</td><td>\
					<input id=CSS_font-weight type=text size=8 pattern="\s*(bold|normal|[0-9]00)\s*" onblur="closeList()"><span onclick="addList(this,\'bold,normal,100,200,300,400,500,600,700,800,900\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				<td>\
					Font-Color:\
				</td><td>\
					<input id=CSS_color type=text size=8 pattern="^#[0-9A-Fa-f]{3}$|^#[0-9A-Fa-f]{6}$" onblur="closeColorPicker();"><span onclick="addColorPicker(this);" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Background-Color:\
				</td><td>\
					<input id=CSS_background-color type=text size=8 pattern="^#[0-9A-Fa-f]{3}$|^#[0-9A-Fa-f]{6}$" onblur="closeColorPicker();"><span  onclick="addColorPicker(this);" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				<td>\
					Border-Width:\
				</td><td>\
					<input id=CSS_border-width type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span onclick="addList(this,\'thin,medium,thick,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Border-Color:\
				</td><td>\
					<input id=CSS_border-color type=text size=8 pattern="^#[0-9A-Fa-f]{3}$|^#[0-9A-Fa-f]{6}$" onblur="closeColorPicker();"><span onclick="addColorPicker(this);" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr>\
				</table>\
				<button type=button onclick="var o=$el(\'gfx-css-adv\').style; if(o.display==\'none\'){o.display=\'block\';this.innerHTML=\' &#9650;  \';}else{o.display=\'none\';this.innerHTML=\''+LNG.more+'&#9660;\';}">' + LNG.more +'&#9660;</button>\
				<table id=gfx-css-adv class=gfx-css-styles style="display:none;">\
				<tr>\
				<td>\
					Text-Decoration:\
				</td><td>\
					<input id=CSS_text-decoration type=text size=8 pattern="\s*(none|underline|overline|line-through|blink)\s*" onblur="closeList()"><span  onclick="addList(this,\'none,underline,overline,line-through,blink\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					White-Space:\
				</td><td>\
					<input id=CSS_white-space type=text size=8 pattern="\s*(normal|pre|nowrap)\s*" onblur="closeList()"><span  onclick="addList(this,\'normal,pre,nowrap\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				<td>\
					Border-Style:\
				</td><td>\
					<input id=CSS_border-style type=text size=8 pattern="\s*(none|hidden|dotted|dashed|solid|double|groove|ridge|inset|outset)\s*" onblur="closeList()"><span  onclick="addList(this,\'none,hidden,dotted,dashed,solid,double,groove,ridge,inset,outset\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
				</td>\
				</tr><tr>\
				</tr><tr>\
				<td>\
					Margin-Top:\
				</td><td>\
					<input id=CSS_margin-top type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span  onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Padding-Top:\
				</td><td>\
					<input id=CSS_padding-top type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span  onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				<td>\
					Margin-Left:\
				</td><td>\
					<input id=CSS_margin-left type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Margin-Right:\
				</td><td>\
					<input id=CSS_margin-right type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span  onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				<td>\
					Padding-Left:\
				</td><td>\
					<input id=CSS_padding-left type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span  onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Padding-Right:\
				</td><td>\
					<input id=CSS_padding-right type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span  onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				<td>\
					Margin-Bottom:\
				</td><td>\
					<input id=CSS_margin-bottom type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span  onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Padding-Bottom:\
				</td><td>\
					<input id=CSS_padding-bottom type=text size=8 pattern="[0-9\.]{1,2}p[t|x]" onblur="closeList()"><span  onclick="addList(this,\'0px,1px,2px,3px,4px,5px,6px,7px,8px,9px,10px,11px,12px,13px,14px,15px,16px,17px,18px,19px,20px,21px,22px,23px,24px,25px,26px,27px,28px,29px,30px,31px,32px,33px,34px,35px,36px,37px,38px,39px,40px,41px,42px,43px,44px,45px,46px,47px,48px,49px,50px\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr><tr>\
				</tr><tr>\
				<td>\
					Float:\
				</td><td>\
					<input id=CSS_float type=text size=8 pattern="\s*(none|left|right)\s*" onblur="closeList()"><span  onclick="addList(this,\'none,left,right\');" class=virtual-list-icon>&#9660;</span>\
				</td><td>\
					Clear:\
				</td><td>\
					<input id=CSS_clear type=text size=8 pattern="\s*(none|left|right|both)\s*" onblur="closeList()"><span  onclick="addList(this,\'none,left,right,both\');" class=virtual-list-icon>&#9660;</span>\
				</td>\
				</tr>\
				</table>\
				<hr width=90% align=center>\
				<label><input name=GFX.mzBrd type=checkbox '
				+ (GFX.mzBrd ? 'checked=checked' : '')
				+ '>' + LNG.ui_css_mozBrd + '</label>\
			</fieldset>\
			<fieldset id=gfx-tab-5 style="margin-left: 160px;width:450px;">\
				<legend>' + LNG.about + '</legend>\
				<div class=about>\
				<span>\
				<img src="http://www.google.com/intl/en/logos/Logo_40wht.gif" width=128 height=53 style="border:0;margin:0;" />\
				<a class=gfx href="'+ SCRIPT_URL +'" target=_blank><span>v'+ SCRIPT_VER +'</span></a>\
				<br><a href="'+ SCRIPT_URL +'" target=_blank>'+ SCRIPT_URL +'</a>\
				</span>\
				<br>\
				<br>\
				<h3>Script Author</h3>\
				<h1>Pablo Custo</h1>\
				<br>\
				<h3>Translations</h3>\
				<h2>Dutch by Jerone</h2>\
				<h2>German by Watermelonman</h2>\
				<h2>Polish by Mr.Cat<h2>\
				<br>\
				<h3>Thanks to</h3>\
				<h2>UserScripts.org<h2>\
				<h2>Script\'s Users<h2>\
				<br>\
				<br>\
				Google FX is not endorsed or affiliated with Google Inc.\
				<br>\
				GOOGLE TM is a trademark of Google Inc.\
				</div>\
			</fieldset>\
		</div>\
		<div id=gfx-preview>\
			<fieldset id=PV_cmn_body class=cmn_body onmousedown=cssOp(this)>\
			<legend>' + LNG.preview + '</legend>\
			<div id=PV_hd_box class=hd_box onmouseup=cssOp(this)>\
				<div id=PV_hd_mnu class=hd_mnu onclick=cssOp(this)>\
					<ul>\
					<li class=search>'+ LNG.search +'</li>\
					<li class=images>'+ LNG.images +'</li>\
					<li class=news>'+ LNG.news +'</li>\
					<li class=intl>'+ LNG.more +'</li>\
					</ul>\
				</div>\
				<div>\
				<div style="display:inline-block;">\
				<img src="http://sites.google.com/site/gfxscript/_/rsrc/1242344225206/Home/logom.png" width="100" height="40">\
				</div>\
				<input id=PV_cmn_intxt class=cmn_intxt onclick=cssOp(this) type=text size=12 value="Search...">\
				<input id=PV_cmn_btns class=cmn_btns onclick=cssOp(this) type=submit value=" Normal ">\
				<input id=PV_cmn_btnsOv class=cmn_btnsOv onclick=cssOp(this) type=submit value=" Over ">\
				</div>\
			</div>\
			<br>\
			<div id=PV_res_box class=res_box onmouseup=cssOp(this)>\
				<div id=gfx-preview-tn></div>\
				<div id=PV_res_hld>\
					<span id=PV_res_cnt class=res_cnt onclick=cssOp(this)>\
						01\
					</span>\
					<span class=res_tit>\
						<span id=PV_res_tit onclick=cssOp(this)>\
							Title of </span>\
						<span id=PV_res_hl onclick=cssOp(this)>\
							<em class=res_hl>\
								results\
							</em>\
						</span>\
					</span>\
					<br>\
					<span class=res_dsc>\
						<span id=PV_res_dsc onclick=cssOp(this)>Description of result with </span><br>\
						<span class=res_hl><em id=PV_res_hl onclick=cssOp(this)>highlighted</em></span>\
						<span id=PV_res_dsc onclick=cssOp(this)>word...</span>\
					</span>\
					<br>\
					<span id=PV_res_url class=res_url onclick=cssOp(this)>\
						'+ (GFX.rSiteFlt ? '<a class=qs></a>' : '') +'www.domain.url - 99k\
					</span><br>\
					<span id=PV_res_ref onclick=cssOp(this) class="res_ref'
					+ (GFX.rRefIcnRgt ? ' repos' : '') + '">'
					+ (GFX.rRefIcn 
						? '<a class=ch></a><a class=sm></a>' 
						: '<a>Cache</a><a>Similar Pages</a>'
					  )
					+'</span>\
				</div>\
			</div>\
		  </fieldset>\
		  </div>\
		  <div class=btns>\
			<button id=gfx-gui-save type=button>' + LNG.save + '</button>\
			<button id=gfx-gui-close type=button>' + LNG.close + '</button>\
		  </div>\
		  <span style="position:absolute;float:left;margin-top:-13px;margin-left:10px;color:#777;font-size:10pt;">\
			Google FX is not affiliated with Google Inc.\
		  </span>\
		  <script>setTabMnu(0);</script>\
		';
		
		var gui = new Element('div', {
			'id'	: 'gfx-gui',
			'html'	: html
		});
		gui.inject(body)

		$el('gfx-gui-close').addEvent('click', hideSettings);
		$el('gfx-gui-save').addEvent('click', saveSettings);
		
		return gui;
	};

	var makeSettings = function(event)
	{
		var gui = addGUI();
		
		if(win.setTabMnu){
			win.setTabMnu(0);
		}

		var dragGUI = gui.makeDraggable({
			handle: $el('gfx-gui-dragger')
		});
		
		var cssList = $el('gfx-css-list');
		var cssPreview = $el('gfx-preview');
		var cssValue = $el('gfx-css-value');
		var cssStyleList = doc.evaluate('//input[starts-with(@id,"CSS_")]', doc.body, null, 7, null);

		var lastPrev;
		
		var setStyleList = function(idx)
		{
			// Clean Values
			for(var i=0,len=cssStyleList.snapshotLength; i < len; i++){
				var el = cssStyleList.snapshotItem(i);
				el.value='';
			}
				
			// Define CSS variable name
			var name = cssList.options[idx].id.split('.');
			var styles = CSS[name[1]][name[2]];
			cssValue.value = styles;
			var curPrev = cssPreview.getElement('*[class='+name[1]+'_'+name[2]+']');
			curPrev.setAttribute('style', styles);
			if(lastPrev){ lastPrev.style.border=''; }
			lastPrev = curPrev;
			curPrev.style.border = '1px solid #c00';
			styles = styles.split(/\s*;\s*/);
			for(var i=0,len=styles.length; i < len; i++){
				var v = styles[i].split(/\s*:\s*/);
				if(!v[0]) continue;
				var el = $el( 'CSS_' + v[0] );
				if(el){ el.value = v[1]; }
			}
		};

		var cIdx = 0;
		setStyleList(cIdx);

		var saveLstChanges = function(event)
		{
			var name = cssList.options[cIdx].id;
			GM_setValue(name, cssValue.value);
			name = name.split('.');
			CSS[name[1]][name[2]] = cssValue.value;
			cIdx = cssList.selectedIndex;
			setStyleList(cIdx);
		};

		cssList.addEvent('change', saveLstChanges);
		cssPreview.addEvent('click', saveLstChanges);

		AlertBox = new SexyAlertBox({
				OverlayStyles   : {
					'background-color': '',
					'background-image': 'url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAYAAACNMs+9AAAABGdBTUEAANbY1E9YMgAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAA/SURBVHjaYmRgYEhjIAIABBATA5EAIICIVggQQEQrBAggohUCBBDRCgECiGiFAAFEtEKAACJaIUAAEa0QIMAAs2IAetPOw3cAAAAASUVORK5CYII=")',
					'opacity': '1'
				},
				showDuration    : 0,
				closeDuration   : 0
		});
		
		var resetValues = function(ev)
		{
			cIdx = cssList.selectedIndex;
			var name = cssList.options[cIdx].id;
			var label = cssList.options[cIdx].innerHTML;
			AlertBox.confirm(LNG.ui_css_defaults+': <b>' + label + '</b>?', {
				onComplete:
					function(isOk) {
						if (isOk) {
							GM_setValue(name, '');
							name = name.split('.');
							CSS[name[1]][name[2]] = '';
							cssValue.value = '';
							setStyleList(cIdx);
						}
					}
			});
		};

		$el('gfx-css-reset').addEvent('click', resetValues);

		var setPreview = function(ev)
		{
			ev.stopPropagation();
			var target = $el(ev.target);
			var pattern = target.get('pattern');
			if(target.value != '' && pattern){
				if(!new RegExp(pattern.replace(/\\/g,'\\')).test(target.value)){
					return false;
				}
			}
			var styles = '';
			
			for(var i=0,len=cssStyleList.snapshotLength; i < len; i++){
				var el = cssStyleList.snapshotItem(i);
				if(!/[a-z0-9\.\-]/.test(el.value)){ continue; }
				styles += el.id.replace(/^CSS_/,'') + ':' + el.value + ';';
			}
			cssValue.value = styles;
			var name = cssList.options[cIdx].id.replace(/CSS/, 'PV').replace(/\./g, '_');
			$el(name).set('style', styles.replace(/[a-z0-9\.\-]+:null;/g, ''));
		};
		
		for(var i=0,len=cssStyleList.snapshotLength; i < len; i++){
			var el = $el(cssStyleList.snapshotItem(i));
			el.addEvent('blur',setPreview);
			el.addEvent('keyup',setPreview);
		}

		BgBox.show();
		gui.show();
		gui.style.left = Math.round((body.clientWidth-780)/2) + 'px';
		gui.style.top = '20px';
	};
	
	var saveSettings = function(event)
	{
		var o, x;
		var inputs = doc.evaluate('//div[@id="gfx-gui"]//INPUT[starts-with(@name,"GFX.")]', doc.body, null, 7, null);
		for(var i=0,len=inputs.snapshotLength; i < len; i++){
			var el = inputs.snapshotItem(i);
			var value = (el.type=='checkbox' ? el.checked : el.value);
			GM_setValue(el.name, value);
		}

		var selects = doc.evaluate('//div[@id="gfx-gui"]//SELECT[starts-with(@name,"GFX.")]', doc.body, null, 7, null);
		for(var i=0,len=selects.snapshotLength; i < len; i++){
			var el = selects.snapshotItem(i);
			var idx = el.selectedIndex;
			GM_setValue(el.name, el.options[idx].value);
		}

		var cssList = $el('gfx-css-list');
		var cssValue = $el('gfx-css-value');
		var name = cssList.options[cssList.selectedIndex].id;
		GM_setValue(name, cssValue.value);
		name = name.split('.');
		CSS[name[1]][name[2]] = cssValue.value;

		if(isOpera){
			OP_setValues('GFX');
			OP_setValues('CSS');
		}

		window.setTimeout(function(){doc.location.reload();}, 50);

		return false;
	};

	var hideSettings = function(event)
	{
		BgBox.hide();
		var gui = $el('gfx-gui');
		gui.hide();
		var vlist = $el('virtual-option-list');
		if(vlist){ vlist.parentNode.removeChild(vlist); }
		return false;
	};

	var showSettings = function(ev)
	{
		var gui = $el('gfx-gui');
		if (!gui){
			makeSettings();
		} else {
			BgBox.show();
			gui.show();
		}
		if(ev) {
			ev.preventDefault();
			ev.stopPropagation();
		}
		return false;
	};

	if(!isOpera) GM_registerMenuCommand('Google Fx '+LNG.ui_title, showSettings, null, null, "");

	/*-----------------------------
	  : : END Private Functions   */

	// ====================
	//  GoogleFx Logo
	// ====================
	if(GFX.gFxLogo)
	{
		var target = $nd('//img[contains(@src,"/images/hp2.gif")] | //img[contains(@src,"/nav_logo")] | //a[@id="logo"] | //img[@width="150" or contains(@alt,"Google") or contains(@src,"/intl/")][contains(@src,"/images/") or contains(@src,"logo")] | //div[contains(@style,"/intl/") or contains(@style,"/images/")][contains(@title,"Google")] | //img[contains(@src,"/logos/")] | //div[@id=":rc"] | //table[@id="frame"]//div[@title="Google"]');
		if(target) {
			var logo = new Element('a', {
				'id'	: 'gfx-logo',
				'href'	: '#',
				'title'	: 'Google FX v' + SCRIPT_VER + ' - '+LNG.ui_title,
			});
			logo.addEvent('click', showSettings);
			logo.store('tip:title', 'Google FX v' + SCRIPT_VER);
			logo.store('tip:text', LNG.ui_title);
			
			if(target.offsetWidth < 170){
				logo.setStyles({
					'position'			: 'absolute',
					'margin-left'		: '150px',
					'min-height'		: '54px',
				});

				if(isOpera) {
					logo.setStyle('margin-top','15px');
				}

				var st = target.get('style');
				if(st && /images\/firefox/.test(st)){
					logo.setStyles({
						'margin-left'		: '75px',
						'margin-top'		: '-4px',
						'min-height'		: '25px',
					});
				}

				if(target.nodeName == 'IMG') {
					target.getParent().getParent().setStyles({
						'width'		: '188px',
						'min-width'	: '188px'
					});
					logo.inject(target.parentNode,'before');
					
				} else {
					target.getParent().setStyles({
						'width'		: '185px',
						'min-width'	: '185px'
					});
					logo.inject(target,'before');
				}
				
				
			} else {
				target.style.display = 'inline-block';
				logo.setStyle('margin-bottom',(isOpera ? '10px' : '20px'));
				logo.inject(target,'after');
			}
		}
	} // END Show Logo

	// ====================
	// Top Bar Checks & Fills
	// ====================
	
	var HEADER = $el('header') || $nd('//*[@id="header"]');
	
	var GBAR_BG = new Element('div',{id:'bgBar'});
	GBAR_BG.inject(HEADER || body);
	
	var GBAR = $el('gbar') || $nd('//*[@id="gbar"]');
	
	var gbimenu='\
	<a href="http://blogsearch.google.com/?" class="gb2 blogsearch">'	+LNG.blogs+	'</a>\
	<a href="http://groups.google.com/grphp?" class="gb2 groups">'		+LNG.groups+		'</a>\
	<a href="http://books.google.com/bkshp?" class="gb2 books">'		+LNG.books+			'</a>\
	<a href="http://scholar.google.com/scholar?" class="gb2 scholar">'	+LNG.scholar+		'</a>\
	<a href="http://code.google.com/intl/'+LANG+'/'
	+(hasSomeQuery ? '#q='+queryFormEncode : '')+'" class="gb2 code">'	+LNG.code+			'</a>\
	<div class=gb2><div class=gbd></div></div><a></a>\
	<a href="https://docs.google.com/?" class="gb2 docs">'				+LNG.docs+			'</a>\
	<a href="https://www.google.com/calendar/render?" class="gb2 calendar">'+LNG.calendar+	'</a>\
	<a href="https://www.google.com/reader/view/?" class="gb2 reader">'	+LNG.reader+		'</a>\
	<a href="https://sites.google.com/?" class="gb2 sites">'			+LNG.sites+			'</a>\
	<a href="https://www.google.com/alerts?" class="gb2 alerts">'		+LNG.alerts+		'</a>\
	<a href="https://www.google.com/notebook?" class="gb2 notebook">'	+LNG.notebook+		'</a>\
	<a href="https://www.google.com/history?" class="gb2 history">'		+LNG.history+		'</a>\
	<div class=gb2><div class=gbd></div></div><a></a>\
	<a href="http://maps.google.com/maps?" class="gb2 maps">'			+LNG.maps+			'</a>\
	<a href="http://picasaweb.google.com/lh/view?q='
	+queryFormEncode+'&psc=G&filter=0" class="gb2 picasaweb">'			+LNG.picasaweb+		'</a>\
	<a href="http://www.youtube.com/results?search_query='
	+queryFormEncode+'&aq=f" class="gb2 youtube">'						+LNG.youtube+		'</a>\
	<div class=gb2><div class=gbd></div></div><a></a>\
	<a href="http://www.google.com/prdhp?" class="gb2 products">'		+LNG.products+		'</a>\
	<a href="http://finance.google.com/finance?" class="gb2 finance">'	+LNG.finance+		'</a>\
	<a href="http://www.google.com/intl/'+LANG+'/options/" class="gb2 intl">'+LNG.more+	' &raquo;</a>';
	
	if (!GBAR && GFX.bMnuChk)
	{
		GBAR = new Element('div', {
			'id'	: 'gbar',
			'html'	: '\
			<nobr>\
			<a href="http://www.google.com/webhp?" class="gb1 search">'			+LNG.webhp+		'</a>\
			<a href="http://images.google.com/images?" class="gb1 images">'		+LNG.images+	'</a>\
			<a href="http://video.google.com/videosearch?" class="gb1 video">'	+LNG.video+		'</a>\
			<a href="http://news.google.com/nwshp?" class="gb1 news">'			+LNG.news+		'</a>\
			<a href="https://mail.google.com/mail/?" class="gb1 mail">'			+LNG.mail+		'</a>\
			<a href="http://translate.google.com/translate_t?hl='+LANG+'" class="gb1 translate">'+LNG.translate+'</a>\
			<a href="#' 
				+' onclick="gmnu.open(event,\'gbi\');return false;" onmouseover="gmnu.open(event,\'gbi\');return false;" class="gb3 intl">'
				+ LNG.more +' &#9660;</a>\
			<div id=gbi>' + gbimenu + '</div></nobr><div class=gbh style=left:0></div><div class=gbh style=right:0></div>'
		});
		GBAR.inject(body, 'top');
		
		STYLES += 'body { margin-top : 24px; }';
	}

	//Because some regions don't have all menu's links
	else if(GFX.bMnuChk)
	{

		var addMenuItem = function (obj, href, label, class)
		{
			var item = new Element('a',{
				'class'		: class,
				'href'		: href,
				'html'		: label
			});
			item.inject(obj,'before');
		};
		
		// Icons Feature
		var addIcons = GFX.bMnuIcn ?
			function(el){
				var service = el.href.match(/^https?:\/\/(\w+)\.(\w+)\.[^\/]+\/\??(\w+)?.?/);
				if (service){
					var newclass = (service[2]=='google' ? service[(service[1]=='www' ? 3 : 1)] : service[2]);
					$el(el).addClass(newclass);
					if(!isDefLng && LNG[title]) el.innerHTML = LNG[title];
				}
			}
			: function(){};

		var gbi = $el('gbi') || $nd('//*[@id="gbi"]');
		if(!gbi){
			var a = new Element('a', {
				'class'	: 'gb3',
				'href'	: 'http://www.google.com/intl/'+ LANG +'/options/',
				'html'	: LNG.more + ' &#9660'
			});
			GBAR.appendChild(a);
			gbi = new Element('div', {
				'id'	: 'gbi',
				'html'	: gbimenu
			});
			GBAR.appendChild(gbi);
		}

		var items = '';
		var nodes = doc.evaluate('//*[@id="gbar"]//a', doc.body, null, 7, null);
		var lastGBar = $nd('//*[@id="gbar"]//a[contains(concat(" ",@class," ")," gb3 ")][contains(@href,"/options/")]') || $nd('//*[@id="gbar"]//a//small/ancestor::a');
		
		for(var i=0,len=nodes.snapshotLength; i<len; i++) {
			var el = nodes.snapshotItem(i);

			el.removeAttribute('onclick');
			items += el.href.split('?')[0];
			
			addIcons(el);
			
			if(el.href.contains('://video.') && el.href.contains('/?'))
			{
				el.href = el.href.replace(/\/\?/, '/videosearch?');
			}
			else if(el.href.contains('://www.youtube'))
			{
				var lnk = el.href.split('?')[0];
				el.href = lnk + '?search_query=' + queryFormEncode + '&aq=f';
				el.setAttribute('target', GFX.rTrgLnk);
			}
			else if(el.href.contains('://picasa'))
			{
				var lnk = el.href.split('home?')[0];
				el.href = lnk.replace(/picasa\./,'picasaweb.') + 'lh/view?q=' + queryFormEncode + '&psc=G&filter=0';
				el.setAttribute('target', GFX.rTrgLnk);
			}
		};
		items = items.replace(/https?\/\//ig, ';');

		var lastGbi = nodes.snapshotItem(nodes.snapshotLength-1);

		// Add Links to NavBar
		if(SERVICE != 'video'		&& items.indexOf('video')		== -1)
			addMenuItem(lastGBar, 'http://video.google.com/videosearch?',	LNG.video,		'gb1 video');
		if(SERVICE != 'translate'	&& items.indexOf('translate')	== -1)
			addMenuItem(lastGBar, 'http://translate.google.com/translate_t?',	LNG.translate,	'gb1 translate');


		//Add Links to NavBar -> More..
		if(SERVICE != 'alerts'		&& items.indexOf('alerts')		== -1)
			addMenuItem(lastGbi, 'https://www.google.com/alerts?',			LNG.alerts,		'gb2 alerts');
		if(SERVICE != 'blogsearch'	&& items.indexOf('blogs')		== -1)
			addMenuItem(lastGbi, 'http://blogsearch.google.com/?',			LNG.blogs,		'gb2 blogsearch');
		if(SERVICE != 'books'		&& items.indexOf('books')		== -1)
			addMenuItem(lastGbi, 'http://books.google.com/bkshp?',			LNG.books,		'gb2 books');
		if(SERVICE != 'calendar'	&& items.indexOf('calendar')	== -1)
			addMenuItem(lastGbi, 'https://www.google.com/calendar/render?', LNG.calendar,	'gb2 calendar');
		if(SERVICE != 'code'		&& items.indexOf('code')		== -1)
			addMenuItem(lastGbi, 'http://code.google.com/intl/'+LANG+'/'
			+ (hasSomeQuery ? '#q='+queryFormEncode : ''),					LNG.code,		'gb2 code');
		if(SERVICE != 'docs'		&& items.indexOf('docs')		== -1)
			addMenuItem(lastGbi, 'https://docs.google.com/?',				LNG.docs,		'gb2 docs');
		if(SERVICE != 'finance'		&& items.indexOf('finance')		== -1)
			addMenuItem(lastGbi, 'http://finance.google.com/finance?',		LNG.finance,	'gb2 finance');
		if(SERVICE != 'groups'		&& items.indexOf('groups')		== -1)
			addMenuItem(lastGbi, 'http://groups.google.com/grphp?',			LNG.groups,		'gb2 groups');
		if(SERVICE != 'history'		&& items.indexOf('history')		== -1)
			addMenuItem(lastGbi, 'https://www.google.com/history/lookup?hl='+LANG, LNG.history, 'gb2 history');
		if(SERVICE != 'maps'		&& items.indexOf('maps')		== -1)
			addMenuItem(lastGbi, 'http://maps.google.com/maps?',			LNG.maps,		'gb2 maps');
		if(SERVICE != 'picasaweb'	&& items.indexOf('picasaweb')	== -1)
			addMenuItem(lastGbi, 'http://picasaweb.google.com/lh/view?q='
			+ queryFormEncode + '&psc=G&filter=0',							LNG.picasaweb,	'gb2 picasaweb');
		if(SERVICE != 'products'	&& items.indexOf('/pr')			== -1)
			addMenuItem(lastGbi, 'http://www.google.com/products?',			LNG.products,	'gb2 products');
		if(SERVICE != 'reader'		&& items.indexOf('reader')		== -1)
			addMenuItem(lastGbi, 'https://www.google.com/reader/view/?',	LNG.reader,		'gb2 reader');
		if(SERVICE != 'sites'		&& items.indexOf('sites')		== -1)
			addMenuItem(lastGbi, 'https://sites.google.com/?',				LNG.sites,		'gb2 sites');
		if(SERVICE != 'scholar'		&& items.indexOf('scholar')		== -1)
			addMenuItem(lastGbi, 'http://scholar.google.com/scholar?',		LNG.scholar,	'gb2 scholar');
		if(URI.host[1] != 'youtube' && items.indexOf('youtube')		== -1)
			addMenuItem(lastGbi, 'http://www.youtube.com/results?search_query='
			+ queryFormEncode + '&search_type=&aq=f',						LNG.youtube,	'gb2 youtube');

		lastGBar.setAttribute('onclick','gmnu.open(event,"gbi");return 0;');
		lastGBar.setAttribute('onmouseover','gmnu.open(event,"gbi");return 0;');
		lastGBar.setAttribute('href','#');

	}

	if(GBAR)
	{
		// Fix Url Query on Menu Links
		var query = URI.search;
		if(!$defined(URI.query.hl)) query += 'hl=' + LANG;

		if(URI.search.split('&')[1])
		{
			// Clean query bad arguments
			query = URI.search.replace(/&?tab=[^&]*|$/,'');
			switch(SERVICE){
				case 'images':
					query = query.replace(/&?img[a-z_]*=[^&]*|$/g,'');
					query = query.replace(/&?as_filetype=[^&]*|$/,'');
					query = query.replace(/&?safe=[^&]*|$/,'');
					query = query.replace(/&?sa=[^&]*|$/,'');
				break;
			};
		}
		var nodes = doc.evaluate("//div[@id='gbar']//a[@href][not(contains(@href,'picasaweb.') or contains(@href,'youtube.') or contains(@href,'mail.') or contains(@href,'/options') or contains(@href,'code.') or contains(@href,'/history'))][not(contains(concat(' ',@class,' '),' gb3 '))]", doc.body, null, 7, null);

		for (var i=0,len=nodes.snapshotLength; i < len; i++) {
			var el = nodes.snapshotItem(i);
			el.href = el.href.split('?')[0] + '?' + query;
		};

			
		// Remove @gmail.com from User name TopBar
		if(GFX.bShtName)
		{
			var guser = $el('guser') || $nd('//*[@id="guser"]');
			if(guser)
			{
				var uname = guser.getElement('B');
				if(uname && uname.innerHTML.indexOf('@') != -1)
					uname.innerHTML = uname.innerHTML.replace(/@[a-z]+\.com/,'');
			}
			else
			{
				var guser = $nd("//div[@id='gbar']/following::*//a[contains(@href,'accounts')]");
				if(guser){
					guser.setAttribute('class', (guser.getAttribute('class') || '') + ' login');
					var uname = guser.getParent().getElement('B');
					if(uname && uname.innerHTML.indexOf('@') != -1)
						uname.innerHTML = uname.innerHTML.replace(/@[a-z]+\.com/,'');
				}
			}
		}

		// Add GUI Settings access item to TopBar Menu
		var gbi = $el('gbi') || $nd('//*[@id="gbi"]');
		var a = new Element('a', {
			'class'	: 'gb2 gfx',
			'href'	: '#',
			'html'	: 'Google Fx'
		});
		a.addEvent('click', showSettings);
		var div = new Element('div', {
			'class'	: 'gb2',
			'html'	: '<div class=gbd></div>'
		});
		gbi.appendChild(div);
		gbi.appendChild(a);
	}
	
	
	// Fixes for Search Request Form
	//***********************************
	var FORM = $el(doc.forms[0]) || $nd('//FORM');
	
	if(FORM) {
		// We need to increase the number of search results for the first request
		// if the Right Panel is enabled
		if (GFX.bRgtBox && !URI.query.num) {
			var el = new Element('input', {
				'name'	: 'num',
				'type'	: 'hidden',
				'value'	: '50',
			});
			FORM.appendChild(el);
		}
		
	}




	// =======================
	// First Run over WebSearch
	// ========================
	if(canRun.search)
	{
		// Static Header
		if(GFX.bMnuFix)
		{
			STYLES += '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight+5) +'px !important;\
			}\
			';
			if(HEADER) HEADER.addClass('fixed');
		}

		// Auto Streaming Pages
		// -----------------------------
		AutoPage( {
			holder	: '//*[@id="res"]',
			content	: '//*[@id="res"]/div'
		} );

		// Add Other Web Searches
		//-----------------------------------
		addSearchers((HEADER || $nd('//*[@id="res"]')), SERVICE);

		//  Advance Search ToolBars
		//----------------------------------
		if(GFX.bAdvSrc)
		{

			// by Date ToolBar
			STYLES += '\
			select[name=as_qdr] {\
				display			: none;\
			}\
			';

			var value = (URI.query.as_qdr ? URI.query.as_qdr : 'a');
			var target = $el('sft') || $nd('//*[@id="sft"]');

			if(target)
			{
				var url = doc.location.href;
				url = url.replace(/[?&]as_qdr=[^&]*|$/g, '');
				
				var list = {h:[LNG.hour,false], d:[LNG.day,false], w:[LNG.week,false], m:[LNG.month,false], y:[LNG.year,false]};
				if(list[value]) list[value][1]=true;
				var html = '';
				for(var name in list){
					var item = list[name];
					html += '<a class="'+ name + (item[1] ? ' on' : '') + '" '
						+	'href="'+ url +'&as_qdr=' + (item[1] ? 'a' : name) + '" '
						+	'title="'+ LNG.last + ' ' + item[0] + (item[1] ? ' '+LNG.enabled : '')
						+	'"></a>';
				};
				var bar = new Element('div', {
					'id'	: 'bydate',
					'html'	: html
				});
				bar.inject(target)
			} else
				GM_log('"sft" element not found');

			//"Search in:"  ToolBar
			var value = (URI.query.as_occt ? URI.query.as_occt : 'any');
			if(target)
			{
				var url = doc.location.href;
				url = url.replace(/[?&]as_occt=[^&]*|$/g, '');
				
				var list = {title:[LNG.title,false], body:[LNG.text,false], url:[LNG.url,false], links:[LNG.links,false]};
				if(list[value]) list[value][1]=true;
				var html = '<small>'+ LNG.search_in +':</small>';
				for(var name in list){
					var item = list[name];
					html += '<a class="' + name + (item[1] ? ' on' : '') + '" '
						+	'href="' + url
						+	'&as_occt=' + (item[1] ? 'any' : name) + '" '
						+	'title="' + LNG.in_ + ' ' + item[0] + (item[1] ? ' '+LNG.enabled : '')
						+	'">'
						+	item[0];
						+	'</a>';
				};
				var bar = new Element('div', {
					'id' 	: 'searchIn',
					'html'	: html
				});

				bar.inject(target);
			} else
				GM_log('"sft" element not found');
		}


		//  Domain Filter
		//--------------------------
		if(QUERYS.site){
			var o = $el('prs') || $nd('//*[@id="prs"]');
			if(o){
				var html = '<span style="font-size:12px;"><b>' + LNG.only + ': ';
				for(var i=0,len=QUERYS.site.length; i<len; i++){
					var oneLess	= QUERYS.site[i].replace(/\/$/,'').split('/');
					var title	= oneLess.slice(0,oneLess.length-1).join('/');
					oneLess		= oneLess.slice(0,oneLess.length).join('/');
					html += QUERYS.site[i] + '<a class="qs_n" style="float:none;vertical-align:middle;" onmouseup="onlyFrom(\''+ oneLess +'\',event);return !1;" title="'+ LNG.one_level_up +': '+ title +'"></a>';
				}
				o.innerHTML += html + '</b></span>';
			}
		}

		//  Right Panel Feature
		//---------------------------------
		if(GFX.bRgtBox) {
			
			STYLES += '\
			#gfx-rBox {\
				position			: absolute;\
				display				: inline-block;\
				width				: 36%;\
				min-width			: 36%;\
				max-width			: 36%;\
				top					: 0;\
				margin-top			: 158px;\
				margin-left			: 62%;\
				padding-left		: 10px;\
				font-size-adjust	: 0.48 !important;\
			}\
			';
			
			var bRgtBox = new Element('div', {
				'id'	: 'gfx-rBox'
			});
			bRgtBox.inject(body);
			
			var rBoxCnt = 0;
			
			//  Related Searches to Right Panel
			//-----------------------------------------------------
			
			// Calc Box
			var calcBox = function()
			{
				var node = $nd('//*[@id="res"]//table//img[@src="/images/calc_img.gif"]/ancestor::table');
				if (!node) return;
				var box;
				if( !(box=$el('gfx-rBox-calc')) )
				{
					box = new Element('div', {
						'id'	: 'gfx-rBox-calc',
						'class' : 'calc-rBox',
						'html'	: '<h3 class=r>'+LNG.calc+'</h3>'
						}).inject(bRgtBox);
				}
				box.appendChild(node);
				rBoxCnt++;
			};
			calcBox();
			
			// Top Search Revision
			var relTopBox = function()
			{
				var node = $el('trev') || $nd('//*[@id="trev"]');
				if (!node) return;
				STYLES += '\
				#trev {\
					'+ CSS.res.box + ';\
					display				: table;\
					padding				: 0;\
					margin				: 0 !important;\
					margin-top			: 3px !important;\
					margin-bottom		: 3px !important;\
					min-width			: 99%;\
					background			: #ffc;\
					border-color		: #dda;\
				}\
				#trev TABLE TD {\
					display				: block;\
					padding-left		: 10px !important;\
				}\
				';
				STYLES += (isOpera ? '\
				' : '\
					#trev {\
						'+ (GFX.mzBrd ? '\
							' + CSS.cmn.mzBrd + '\
						' : '') +'\
					}\
				');
				bRgtBox.appendChild(node);
			};
			relTopBox();
			
			// Broad Search Revision
			var relBotBox = function()
			{
				var node = $el('brs') || $nd('//*[@id="brs"]');
				if (!node) return;
				STYLES += '\
				.e {\
					'+ CSS.res.box + ';\
					display				: table;\
					padding				: 0;\
					margin				: 0 !important;\
					margin-top			: 3px !important;\
					margin-bottom		: 3px !important;\
					min-width			: 99%;\
				}\
				#brs CAPTION {\
					margin				: -1px;\
					margin-bottom		: 1px;\
					padding-left		: 30px;\
					font-weight			: bold;\
					background-color	: #eef;\
					border				: 1px solid #bbe;\
					background-image	: url("http://www.google.com/intl/en/options/icons/search.gif");\
					background-repeat	: no-repeat !important;\
					background-position	: left center !important;\
				}\
				#brs TD {\
					display				: inline-block;\
					padding-left		: 10px !important;\
					width				: 130px;\
				}\
				#brs CAPTION B {\
					display				: none;\
				}\
				';
				STYLES += (isOpera ? '\
				' : '\
					'+ (GFX.mzBrd ? '\
						.e {\
							' + CSS.cmn.mzBrd + '\
						}\
						#brs CAPTION {\
							-moz-border-radius-topleft		:'+ (GFX.mzBrdTL - 3) +'px;\
							-moz-border-radius-topright		:'+ (GFX.mzBrdTR - 3) +'px;\
							-moz-border-radius-bottomleft	: 0;\
							-moz-border-radius-bottomleft	: 0;\
						}\
					' : '') +'\
				');
				bRgtBox.appendChild(node.parentNode);
			};
			relBotBox();
					
			// Movies Board
			var movieBox = function()
			{
				var node = $nd('//*[@id="res"]//form[@action="/movies"]/ancestor::li');
				if (!node) return;
				node.addClass('movieBrd');
				STYLES += '\
				.movieBrd {\
					'+ CSS.res.box +';\
					display				: table;\
					padding				: 0;\
					min-width			: 99%;\
				}\
				.movieBrd TABLE {\
					margin-left			: 10px;\
					margin-right		: 10px;\
				}\
				' + (isOpera ? '\
				' : '\
					'+ (GFX.mzBrd ? '\
						.movieBrd {\
							' + CSS.cmn.mzBrd + '\
						}\
					' : '') +'\
				');
				bRgtBox.appendChild(node);
			};
			movieBox();
			
			// CSS Style for Images, Movies, Blogs, Books and News Section
			STYLES += '\
			.blogs-rBox, .calc-rBox, .images-rBox, .maps-rBox, .news-rBox,\
			.trans-rBox, .video-rBox, .wiki-rBox, .youtube-rBox {\
				'+ CSS.res.box + ';\
				display				: table;\
				min-width			: 99%;\
				padding				: 0;\
				margin				: 0 !important;\
				margin-top			: 3px !important;\
				margin-bottom		: 3px !important;\
			}\
			.trans-rBox {\
				padding-left		: 10px;\
			}\
			.blogs-rBox .r, .calc-rBox > .r, .images-rBox .r, .maps-rBox .r,\
			.news-rBox > .r, .video-rBox > .r, .youtube-rBox > .r, .wiki-rBox > .r {\
				display				: block;\
				min-height			: 18px;\
				margin				: -1px;\
				padding-left		: 45px;\
				font-weight			: bold;\
				border				: 1px solid;\
				background-repeat	: no-repeat !important;\
				background-position	: left center !important;\
			}\
			.blogs-rBox .r EM, .images-rBox .r EM, .maps-rBox .r EM, .news-rBox .r EM,\
			.video-rBox .r EM, .wiki-rBox .r EM, .youtube-rBox .r EM {\
				border				: 0;\
			}\
			.blogs-rBox .ts, .images-rBox .ts, .maps-rBox .ts, .news-rBox .ts,\
			.video-rBox .ts, .wiki-rBox .ts, .youtube-rBox .ts {\
				margin				: 10px;\
			}\
			.blogs-rBox .r {\
				background-color	: #eee;\
				border-color		: #ccc;\
				background-image	: url("http://www.google.com/intl/en/options/icons/blogsearch.gif");\
			}\
			.calc-rBox > .r {\
				background-color	: #bde;\
				border-color		: #ccc;\
				background-image	: url("http://www.google.com/images/calc_img.gif");\
			}\
			.images-rBox .r {\
				background-color	: #eea;\
				border-color		: #cc9;\
				background-image	: url("http://www.google.com/intl/en/options/icons/images.gif");\
			}\
			.maps-rBox .r {\
				background-color	: #bbd;\
				border-color		: #99c;\
				background-image	: url("http://www.google.com/intl/en/options/icons/maps.gif");\
			}\
			.news-rBox > .r {\
				background-color	: #bbd;\
				border-color		: #89d;\
				background-image	: url("http://www.google.com/intl/en/options/icons/news.gif");\
			}\
			.video-rBox > .r {\
				background-color	: #bdb;\
				border-color		: #9c9;\
				background-image	: url("http://www.google.com/intl/en/options/icons/video.gif");\
				margin-bottom		: 10px;\
			}\
			.wiki-rBox > .r {\
				background-color	: #555;\
				border-color		: #333;\
				color				: #ddd;\
				font-weight			: bold;\
				background-image	: url("http://www.wikipedia.org/favicon.ico");\
				margin-bottom		: 10px;\
			}\
			.youtube-rBox > .r {\
				background-color	: #dbb;\
				border-color		: #c99;\
				background-image	: url("http://www.google.com/intl/en/options/icons/youtube.gif");\
				font-weight			: bold;\
			}\
			.calc-rBox TD > IMG {\
				display				: none;\
			}\
			.images-rBox > DIV {\
				margin-left			: 10px;\
			}\
			.images-rBox > DIV IMG{\
				display				: inline-block;\
				float				: left;\
				margin				: 3px;\
				margin-right		: 8px;\
			}\
			.images-rBox .gl {\
				display				: block;\
				float				: left;\
				padding-top			: 20px;\
				min-width			: 99%;\
			}\
			.images-rBox #irl_c A {\
				display				: block;\
				float				: left;\
				margin-left			: 10px;\
			}\
			.maps-rBox {\
				text-align			: left;\
			}\
			.maps-rBox TABLE TD {\
				display				: block;\
			}\
			.maps-rBox .ts {\
				margin-left			: 25px !important;\
			}\
			.news-rBox TD[style*="padding-right: 10px"],\
			.news-rBox TD[style*="padding-top:5px;padding-right:10px;font-size:78%;"],\
			.news-rBox .ts > TD IMG {\
				display				: none;\
			}\
			.news-rBox .gl, .news-rBox .l {\
				display				: block;\
				float				: left;\
				margin				: 0;\
			}\
			.news-rBox .gl A {\
				display				: inline-block;\
			}\
			.video-rBox .ts {\
				margin-top			: 0;\
				margin-bottom		: 3px;\
			}\
			.video-rBox .ts TD {\
				width				: auto !important;\
			}\
			.video-rBox .ts .ts TD {\
				display				: block;\
			}\
			.video-rBox .ts .ts TD A {\
				margin-top			: 10px;\
			}\
			.wiki-rBox .gl {\
				display				: none;\
			}\
			.wiki-rBox .s {\
				margin-right		: 10px;\
			}\
			.video-rBox LI, .wiki-rBox LI, .youtube-rBox LI {\
				list-style			: none;\
				margin-left			: 8px !important;\
			}\
			.youtube-rBox LI .r {\
				font-size-adjust	: 0.42 !important;\
			}\
			.video-rBox CITE,\
			.youtube-rBox CITE {\
				display				: none;\
				font-size-adjust	: 0.42 !important;\
			}\
			' + (isOpera ? '\
			' : '\
				'+ (GFX.mzBrd ? '\
					.blogs-rBox, .images-rBox, .maps-rBox, .news-rBox, .trans-rBox,\
					.video-rBox, .wiki-rBox, .youtube-rBox {\
						' + CSS.cmn.mzBrd + '\
					}\
					.blogs-rBox .r, .images-rBox .r, .maps-rBox .r,\
					.news-rBox > .r, .video-rBox > .r, .youtube-rBox > .r, .wiki-rBox > .r {\
						-moz-border-radius-topleft		:'+ (GFX.mzBrdTL - 3) +'px;\
						-moz-border-radius-topright		:'+ (GFX.mzBrdTR - 3) +'px;\
						-moz-border-radius-bottomleft	: 0;\
						-moz-border-radius-bottomleft	: 0;\
					}\
				' : '') +'\
			');
				
			// Images Box
			var imagesBox = function()
			{
				var node = $nd('//*[@id="res"]//a[contains(@href,"images.google.")]/ancestor::div[contains(concat(" ",@class," ")," g ")] | //*[@id="res"]//a[contains(@href,"images.google.")]/ancestor::li');
				if (!node) return;
				node.addClass('images-rBox');
				bRgtBox.appendChild(node);
				rBoxCnt++;
			};
			imagesBox();

			// Videos Box
			var videoBox = function()
			{
				var nodes = doc.evaluate('//*[@id="res"]//a[contains(@href,"video.google.")]/ancestor::li | //*[@id="res"]//a/img[contains(@src,"video.google.")]/ancestor::li', doc.body, null, 7, null);
				if (!nodes || !nodes.snapshotLength) return;
				var box;
				if( !(box=$el('gfx-rBox-video')) )
				{
					box = new Element('div', {
						'id'	: 'gfx-rBox-video',
						'class'	: 'video-rBox',
						'html'	: '<h3 class=r>'+LNG.video+'</h3>'
					}).inject(bRgtBox);
				}
				
				for(var i=0,len=nodes.snapshotLength; i<len; i++)
				{
					var el = nodes.snapshotItem(i);
					el.className = '';
					box.appendChild(el);
					box.appendChild(doc.createElement('br'));
				}
				rBoxCnt += nodes.snapshotLength;
			};
			if(!URI.query.tbs.vid){
				videoBox();
			}

			// YouTube Revision
			var youtubeBox = function()
			{
				if( QUERYS.site && /youtube/i.test(QUERYS.site) ) return;
				var nodes = doc.evaluate('//*[@id="res"]//a/img[contains(@src,"img.youtube.")]/ancestor::li', doc.body, null, 7, null);
				if (!nodes || !nodes.snapshotLength) return;
				var box;
				if( !(box=$el('gfx-rBox-youtube')) )
				{
					box = new Element('div', {
						'id'	: 'gfx-rBox-youtube',
						'class'	: 'youtube-rBox',
						'html'	: '<h3 class=r>YouTube</h3>'
					}).inject(bRgtBox);
				}
				
				for(var i=0,len=nodes.snapshotLength; i < len; i++)
				{
					var el = nodes.snapshotItem(i);
					el.className = '';
					box.appendChild(el);
					box.appendChild(doc.createElement('br'));
				}
				rBoxCnt += nodes.snapshotLength;
			};
			youtubeBox();
			
			// Maps Revision
			var mapsBox = function(){
				var node = $nd('//*[@id="res"]//a[contains(@href,"maps.google.")]/ancestor::li');
				if (!node) return;
				node.addClass('maps-rBox');
				bRgtBox.appendChild(node);
				var el = node.getElement('.r');
				node.insertBefore(el, node.firstChild);
			};
			mapsBox();
			
			// Blogs Revision
			var blogsBox = function()
			{
				var node = $nd('//*[@id="res"]//a[contains(@href,"blogsearch.google.")]/ancestor::*[contains(concat(" ",@class," ")," g ")]');
				if (!node) return;
				node.addClass('blogs-rBox');
				bRgtBox.appendChild(node);
			};
			blogsBox();
			
			// News Revision
			var newsBox = function()
			{
				var node = $nd('//*[@id="res"]//a[contains(@href,"news.google.")]/ancestor::li');
				if (!node) return;
				node.addClass('news-rBox');
				bRgtBox.appendChild(node);
			};
			newsBox();
			
			// Wikipedia  Revision
			var wikiBox = function()
			{
				if( QUERYS.site && /wikipedia/i.test(QUERYS.site) ) return;
				var nodes = doc.evaluate('//*[@id="res"]//a[contains(@href,".wikipedia.org/wiki")]/ancestor::li', doc.body, null, 7, null);
				if (!nodes || !nodes.snapshotLength) return;
				
				var box;
				if( !(box=$el('gfx-rBox-wiki')) )
				{
					box = new Element('div', {
						'id'	: 'gfx-rBox-wiki',
						'class'	: 'wiki-rBox',
						'html'	: '<h3 class=r>Wikipedia</h3>'
					}).inject(bRgtBox);
				}

				for(var i=0,len=nodes.snapshotLength; i < len; i++)
				{
					var el = nodes.snapshotItem(i);
					el.className = '';
					box.appendChild(el);
					box.appendChild(doc.createElement('br'));
				}
				rBoxCnt += nodes.snapshotLength;
				
			};
			wikiBox();
			
			// Translate Revision
			var transBox = function()
			{
				var node = $nd('//*[@id="res"]//a[contains(@href,".google.") and contains(@href,"/translate_s")]/ancestor::li');
				if (!node) return;
				node.addClass('trans-rBox');
				bRgtBox.appendChild(node);
			};
			transBox();
			
			// Google Ads
			if(!GFX.rNoAds){
				var node = $el('mbEnd') || $nd('//*[@id="mbEnd"]');
				if(node) bRgtBox.appendChild(node);
			}
			
		} // end RightBox
		
		AutoPage.adjustCounter(-rBoxCnt);
	}


	// =====================
	// FirstRun over Images
	// ======================
	else if (canRun.images) {

		// Header Feature
		//--------------------
		if( !HEADER )
		{
			HEADER = new Element('div', {'id' : 'header'});
			if(GFX.bMnuFix) HEADER.addClass('fixed');
			HEADER.appendChild(GBAR_BG);
			HEADER.appendChild(GBAR);
			var o = $el('guser') || $nd('//*[@id="guser"]');
			if(o) HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.left='0';
			HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.right='0';
			HEADER.appendChild(o);
			var o = $nd('//form[@name="gs"]');
			if(o) HEADER.appendChild(o);
			body.insertBefore(HEADER, body.firstChild);
		} else {
			if(GFX.bMnuFix) HEADER.addClass('fixed');
		}

		// Fix Header
		//-------------
		if(GFX.bMnuFix){
			STYLES += '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight) +'px !important;\
			}\
			';
		}
		
		//--------------------------------------
		//   I M A G E    T O O L B A R
		//----------------------------------------
		if(GFX.iColor || GFX.iColType || GFX.iCntType || GFX.iExactSz || GFX.iSafe || GFX.iSzBar || GFX.iType) {
		
		var imgBar = new Element('div', {
			'id'	: 'gfx-tbar',
			'html'	: '<b class="title">' +LNG.filters+ ': </b>'
		});
		
		
		if(GFX.iCntType || GFX.iSzBar)
		{
			var sd = $el('sd') || $nd('//*[@id="sd"]');
			if(sd){
				sd = sd.getParent();
				sd.innerHTML = '';
				sd.id = 'sd';
			}
		}
		
			
		//  SafeMode
		//-----------------
		if(GFX.iSafe)
		{
			var o, nd, f, url;

			var isSafeOff = (URI.query.safe && URI.query.safe == 'off');
			var value = (isSafeOff ? 'off' : 'images');

			if(!$defined(URI.query.safe)){
				var links = doc.evaluate('//a[contains(@href,"/images?")]', doc.body, null, 7, null);
				for(var i=0,len=links.snapshotLength; i<len; i++){
					var el = links.snapshotItem(i);
					el.href = el.href.contains('safe=') ? el.href.replace(/safe=[^&]*|$/g, 'safe='+value) : el.href + '&safe=' + value;
				};
			}

			var url = doc.location.href;
			url = url.replace(/[?&]safe=[^&]*|$/, '');

			imgBar.append(new Element('div',{
				'class'	: 'safe',
				'html'	: '<a class=' + (isSafeOff ? 'off' : 'on') + ' href="' + url + '&safe=' + (isSafeOff ? 'images' : 'off')+'" title="'+LNG.safemode+' '+(isSafeOff ? LNG.disabled : LNG.enabled)+'"></a>'
			}));

		} // END Image SafeMode


		// Size Bar
		//----------------
		if(GFX.iSzBar) {
			var imgsizes = {icon:false,small:false,medium:false,large:false,xlarge:false,xxlarge:false,huge:false};

			var sizeBar = new Element('div', {
				'class'	: 'sizes'
			});
			
			// Image Size
			//--------------------
			function addSizeObj(id, value, label){
				var chkOff = (isOpera?'opacity:0.3;':'-moz-opacity:.30;');
				sizeBar.append(new Element('input', {
					'id'	 : id,
					'value'	 : value,
					'type'	 : 'checkbox',
					'checked': imgsizes[value],
					'events' : {
						'click'	: function(ev){
									imgsizes[value] = this.checked;
									this.nextSibling.setAttribute('style', (this.checked ? '' : chkOff) );
						}
					}
				}));
				var lbl = new Element('label', {
					'for'	: id,
					'title'	: label,
					'style'	: (imgsizes[value] ? '' : chkOff),
				});
				if(GFX.iSzBarIcn) {
					lbl.className = value;
				} else {
					lbl.innerHTML = label;
				}
				sizeBar.append(lbl);
			}


			var values = URI.query.imgsz;
			values = (values ? values : 'icon|small|medium|large|xlarge|xxlarge|huge');
			values = values.split('|');
			var n, i=0;
			while ( (n=values[i++]) ) { imgsizes[n] = true; }

			var iSizes = $nd('//input[@name="imgsz"]');
			if (!iSizes)
			{
				iSizes = new Element('input', {
					'name'	: 'imgsz',
					'type'	: 'hidden'
				});
				(FORM||body).append(iSizes);
			}

			addSizeObj('szi'	,'icon'		,LNG.icon);
			addSizeObj('szs'	,'small'	,LNG.avatars);
			addSizeObj('szm'	,'medium'	,LNG.small);
			addSizeObj('szl'	,'large'	,LNG.medium);
			addSizeObj('szx'	,'xlarge'	,LNG.large);
			addSizeObj('szxx'	,'xxlarge'	,LNG.xlarge);
			addSizeObj('szh'	,'huge'		,LNG.huge);

			if (FORM) {
				FORM.addEvent('submit', function(ev){
					var sz = [];
					for (n in imgsizes) { if(imgsizes[n]) sz.push(n); };
					iSizes.value = sz.join('|');
					return 1;
				});
			} else {
				GM_log('Image Service : Form not found!');
			}

			imgBar.append(sizeBar);

		} // END Image Size Bar

		// Content Type
		//--------------------
		if(GFX.iCntType) {
			var iCntType = $nd('//select[@name="imagetype"]');
			if(iCntType){
				var x = iCntType.options.length;
				while(x--){
					iCntType.options[x].value=iCntType.options[x].value.replace(/.*imgtype=([a-z]*).*/i,'$1');
				}
				iCntType.set('name','imgtype');
				iCntType.set('onchange','doc.forms[0].submit();return 0;');
			} else {
				iCntType = new Element('select', {
					'name'	: 'imgtype',
					'html'	: '\
						<option value="" selected=selected>'+ LNG.content +'</option>\
						<option value="">'		+ LNG.all		+ '</option>\
						<option value=news>'	+ LNG.news		+ '</option>\
						<option value=face>'	+ LNG.faces		+ '</option>\
						<option value=clipart>'	+ LNG.cliparts	+ '</option>\
						<option value=lineart>'	+ LNG.linearts	+ '</option>\
						<option value=photo">'	+ LNG.photos	+ '</option>\
					',
					'onchange'	: 'doc.forms[0].submit();return 0;'
				});
			}
			iCntType.set('class','imgtype');
			imgBar.append(iCntType);
			
			if($defined(URI.query.imgtype)){
				var x = iCntType.options.length;
				while(x--){
					if(iCntType.options[x].value==URI.query.imgtype){
						iCntType.options[x].setAttribute('selected','selected');
						break;
					}
				}
			}
		} // END Image Content Type
		
		
		// Color Type
		//-------------
		if(GFX.iColType)
		{
			var value, url;
			value = URI.query.imgc ? URI.query.imgc : 'color';
			url = doc.location.href;
			url = url.replace(/[?&]imgc=[^&]*|$/, '');
			var url2 = url;
			
			var color = URI.query.imgcolor ? URI.query.imgcolor : false;
			if(color){
				color = color.split(',');
				switch(color[0]){
					case 'black': case 'gray': case 'white': break;
					default: color[0]='';
				}
				if(color[1]){
					switch(color[1]){
						case 'black': case 'gray': case 'white': break;
						default: color[1]='';
					}
				}
				url2 = url2.replace(/([?&])imgcolor=[^&]*|$/, '$1imgcolor='+color.join(','));
			}
					
			imgBar.append(new Element('div' ,{
				'class'	: 'imgc',
				'html'	: '\
					<a class="color'+ (value=='color' ? ' on' : '') +'" '
						+'href="'	+ url + '&imgc=color" '
						+'title="'	+ LNG.color	+ '" rel=" "></a>\
					<a class="gray' + (value=='gray'  ? ' on' : '') +'" '
						+'href="'	+ url2 +'&imgc=gray" '
						+'title="'	+ LNG.grayscale	+'" rel=" "></a>\
					<a class="mono' + (value=='mono'  ? ' on' : '') +'" '
						+'href="'	+ url2 +'&imgc=mono" '
						+'title="'	+ LNG.mchrome+'" rel=" "></a>'
			}));
				
		} // END Image Color Type
		
		// Image Color
		//---------------------
		if(GFX.iColor)
		{
			var value, url;
			value = URI.query.imgcolor ? URI.query.imgcolor : '';
			url = doc.location.href;
			url = url.replace(/[?&]imgcolor=[^&]*|$/, '');
			
			var ctype = URI.query.imgc ? URI.query.imgc : 'color';

			imgBar.append(new Element('div', {
				'title'		: LNG.color,
				'onclick'	: 'gooColorPicker(event)',
				'style'		: 'display:inline-block;',
				'html'		: '\
					<div class="sc-icon '+(URI.query.imgcolor?'sc-hide':'sc-show')+'" style="background:'+value+';">\
						<div style=background:#c33></div>\
						<div style=background:#e6ba40></div>\
						<div style=background:#155eb1></div>\
						<div style=background:#109618></div>\
					</div>\
					<span style="float:left;"><small>&#9660;</small></span>'
				}).append(new Element('div', {
					'id'	: 'sc-dropdown',
					'html'	: '\
					<div class=sc-block style="width:96px">\
						' + (ctype == 'color' ? '\
						<div><a href="'+url+'&amp;imgcolor=pink" style="background:#ff98bf" title="Pink" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=red" style="background:#c00" title="Red" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=brown" style="background:#885418" title="Brown" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=orange" style="background:#fb940b" title="Orange" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=yellow" style="background:#ff0" title="Yellow" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=green" style="background:#0c0" title="Green" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=teal" style="background:#03c0c6" title="Teal" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=blue" style="background:#00f" title="Blue" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=purple" style="background:#762ca7" title="Purple" rel=" "></a></div>\
						' : '') + '\
						<div><a href="'+url+'&amp;imgcolor=black" style="background:#000" title="Black" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=gray" style="background:#999" title="Gray" rel=" "></a></div>\
						<div><a href="'+url+'&amp;imgcolor=white" style="background:#fff" title="White" rel=" "></a></div>\
					</div>\
					<a href="'+url+'" style="white-space:nowrap;clear:both;padding-left:2px;color:#00c;display:block" title="All colors" rel=" ">'+LNG.all.toUpperCase()+' '+LNG.color.toUpperCase()+'</a>'
			})));
				
		} // END Image Color Type
		
		
		// Exact Size
		//-------------
		if(GFX.iExactSz)
		{
			var width,height,vlist;
			
			if(URI.query.imgw && URI.query.imgw != ''){
				var query = $nd('//input[@name,"q"]');
				query.value = query.value.replace(/ ?imagesize:[x0-9]*/ig,"");
			}
			
			width = URI.query.imgw ? URI.query.imgw : '';
			height = URI.query.imgh ? URI.query.imgh : '';
			
			var iWidth  = doc.createElement('input');
			var iHeight = doc.createElement('input');
			iWidth.setAttribute('name','imgw');
			iHeight.setAttribute('name','imgh');
			iWidth.setAttribute('size','3');
			iHeight.setAttribute('size','3');
			iWidth.setAttribute('title',LNG.width);
			iHeight.setAttribute('title',LNG.height);
			iWidth.className = 'exactsize';
			iHeight.className = 'exactsize';
			iWidth.value = width||'';
			iHeight.value = height||''; 
			var vlist = doc.createElement('span');
			vlist.setAttribute("onclick", "addList(this,\',,16,32,48,64,128,256,320,480,640,800,900,1024,1200,1248,1600,1856,2574,2720,2784,2832,3744,3861,4080,4256,5616\',\'max-height:65px;\');");
			vlist.className='virtual-list-icon';
			vlist.innerHTML='&#9660;';
			imgBar.appendChild(iWidth);
			imgBar.appendChild(vlist);
			imgBar.appendChild(doc.createTextNode('x'));
			imgBar.appendChild(iHeight);
			imgBar.appendChild(vlist.cloneNode(true));
			
			
		} //END Image Exact Size
		
		
		// File Type
		//-------------
		if(GFX.iType)
		{
			var iFileType = $nd('//input[@name="as_filetype"]');

			if (!iFileType)
			{
				iFileType = new Element('input', {
					'name'	: 'as_filetype',
					'value'	: URI.query.as_filetype || '',
					'type'	: 'hidden'
				});
				iFileType.inject(FORM||body);
			}

			if(URI.query.as_filetype && URI.query.as_filetype != ''){
				var query = $nd('//input[@name="q"]');
				query.value = query.value.replace(/ ?filetype:[a-z0-9\|]*/ig,"");
			}
			
			imgBar.append(new Element('select', {
				'class'	: 'filetype',
				'html'	: '\
					<option value="">'		+ LNG.filetype +'</option>\
					<option value="">_'		+ LNG.all.toUpperCase() +'_</option>\
					<option value="jpg"'	+ (/jpg/i.test(iFileType.value)  ?' selected':'') +'>.JPG</option>\
					<option value="jpeg"'	+ (/jpeg/i.test(iFileType.value) ?' selected':'') +'>.JPEG</option>\
					<option value="gif"'	+ (/gif/i.test(iFileType.value)  ?' selected':'') +'>.GIF</option>\
					<option value="png"'	+ (/png/i.test(iFileType.value)  ?' selected':'') +'>.PNG</option>\
					<option value="bmp"'	+ (/bmp/i.test(iFileType.value)  ?' selected':'') +'>.BMP</option>\
					<option value="tif"'	+ (/tif/i.test(iFileType.value)  ?' selected':'') +'>.TIF</option>\
					<option value="tiff"'	+ (/tiff/i.test(iFileType.value) ?' selected':'') +'>.TIFF</option>\
					<option value="psd"'	+ (/psd/i.test(iFileType.value)  ?' selected':'') +'>.PSD</option>\
					<option value="ico"'	+ (/ico/i.test(iFileType.value)  ?' selected':'') +'>.ICO</option>',
				'onchange'	: 'frm=doc.forms[0];frm.as_filetype.value = this.options[this.selectedIndex].value;frm.q.value=frm.q.value.replace(/ ?filetype:[a-z0-9]*/ig,"");frm.submit();return false;'
			}));
			
		} // END Image FileType
		
		
		var sft = $el('sft') || $nd('//*[@id="sft"]') || FORM || HEADER;
		sft.parentNode.insertBefore(imgBar, sft);
		
		} // END Image Bar Section


		//  Site Filter
		//--------------------------
		if(QUERYS.site){
			var sd = $nd('//td[@id="sd"] | //span[@id="sd"]/ancestor::td');
			if(sd){
				var html = '<span style="font-size:14px;"><b>' + LNG.only + ': ';
				var len = QUERYS.site.length;
				for(var i=0; i<len; i++){
					var oneLess = QUERYS.site[i].replace(/\/$/,'').split('/');
					var title = oneLess.slice(0,oneLess.length-1).join('/');
					oneLess = oneLess.slice(0,oneLess.length).join('/');
					html += QUERYS.site[i] + '<a class="qs_n" style="float:none;vertical-align:middle;" onmouseup="onlyFrom(\''+oneLess+'\',event);return !1;" title="'+LNG.one_level_up+': '+title+'"></a>';
				}
				sd.innerHTML = html + '</b></span>';
			}
		}
		
		// Add Other Images Searches
		//-------------------------------------
		addSearchers( ($el('ssb') || HEADER || $el('ImgCont').previousSibling), SERVICE);

		// Auto Streaming Pages
		// -----------------------------
		AutoPage( {
			holder	: "//*[@id='ImgContent']",
			content	: "//*[@id='ImgContent']/table"
		} );
		
		if(URI.host[1] == 'googlelabs'){
			FORM.style.marginTop = '30px';
			HEADER.style.height = '120px';
		}
	}


	// ===============================================================================
	// Main News
	// ===============================================================================
	else if(canRun.news)
	{
		// Header Feature
		//--------------------
		if( !HEADER ){
			HEADER = new Element('div', {'id' : 'header'});
			if(GFX.bMnuFix) HEADER.addClass('fixed');
			HEADER.appendChild(GBAR_BG);
			HEADER.appendChild(GBAR);
			var o = $nd('//*[contains(concat(" ",@class," ")," gaiaNav ")]');
			if(o) HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.left='0';
			HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.right='0';
			HEADER.appendChild(o);
			var o = $el('search-header') || $el('browse-header');
			if(o) HEADER.appendChild(o);
			body.insertBefore(HEADER, body.firstChild);
		}
	
		STYLES += '\
		#gbar, #guser {\
			margin-top				: 0px;\
		}' + (GFX.bMnuFix ? '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight+17) +'px !important;\
			}\
		' : '');

		if(hasSomeQuery) {
			var o = $nd('//*[contains(concat(" ",@class," ")," leftnav ")]/following-sibling::*');
			if(o) o.className = 'rightnav';

			// Add Other News Searches
			//-------------------------------------
			addSearchers((HEADER || $nd('//*[contains(concat(" ",@class," ")," search-sub-header ")]')), SERVICE);

			// Auto Streaming Pages
			// -----------------------------
			AutoPage( {
				holder	: '//*[@id="main-table"]//table//td[contains(concat(" ",@class," ")," search-middle ")]',
				content	: '//*[@id="search-stories"]',
				nextLink: '//div[@id="pagination"]/descendant::a[last()]'
			} );
		}
		
	}

	// ==============
	// Google Books
	// ==============
	else if(canRun.books)
	{
		// Header Feature
		if( !HEADER ){
			HEADER = new Element('div', {'id' : 'header'});
			if(GFX.bMnuFix) HEADER.addClass('fixed');
			HEADER.appendChild(GBAR_BG);
			HEADER.appendChild(GBAR);
			var o = $nd('//*[@id="guser"]');
			if(o) HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.left='0';
			HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.right='0';
			HEADER.appendChild(o);
			var o = body.getElementsByTagName('table')[0];
			if(o) {
				o.id = 'sft';
				HEADER.appendChild(o);
			}
			body.insertBefore(HEADER, body.firstChild);
		}
		
		STYLES += (GFX.bMnuFix ? '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight+17) +'px !important;\
			}\
		' : '');
		
		// Add Other Books Reviews
		//----------------------------------
		addSearchers( (HEADER || $el('results_bar')), SERVICE);

		// Auto Streaming Pages
		// -----------------------------
		AutoPage( {
			holder	: '//*[contains(concat(" ",@class," ")," scontentarea ")]',
			content	: '//*[contains(concat(" ",@class," ")," scontentarea ")]'
		} );
		
	}

	// ==============
	// Google Blogs
	// ==============
	else if(canRun.blogsearch)
	{

		// Header Feature
		if( !HEADER ){
			HEADER = new Element('div', {'id' : 'header'});
			if(GFX.bMnuFix) HEADER.addClass('fixed');
			HEADER.appendChild(GBAR_BG);
			HEADER.appendChild(GBAR);
			var o = $nd('//*[@id="guser"]');
			if(o) HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.left='0';
			HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.right='0';
			HEADER.appendChild(o);
			var o = body.getElementsByTagName('table')[0];
			if(o) {
				o.id = 'sft';
				HEADER.appendChild(o);
			}
			body.insertBefore(HEADER, body.firstChild);
		}

		STYLES += (GFX.bMnuFix ? '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight+2) +'px !important;\
			}\
		' : '');
		
		// Add Other Blogs Searches
		//----------------------------------
		addSearchers( (HEADER || $nd('//TABLE[contains(concat(" ",@class," ")," ttt ")]')), SERVICE);

		// Auto Streaming Pages
		// ----------------------------
		AutoPage( {
			holder	: '//*[@id="m"]',
			content	: '//div[contains(concat(" ",@class," ")," ln ")]/following-sibling::*'
		} );

	}

	// ==============
	// Google Groups
	// ==============
	else if(canRun.groups)
	{

		if(hasSomeQuery){
			// Header Feature
			//---------------------
			if( !HEADER ){
				HEADER = new Element('div', {'id' : 'header'});
				if(GFX.bMnuFix) HEADER.addClass('fixed');
				HEADER.appendChild(GBAR_BG);
				HEADER.appendChild(GBAR);
				var o = $nd('//*[@id="guser"]');
				if(o) HEADER.appendChild(o);
				var o = doc.createElement('div');
				o.className='gbh';o.style.left='0';
				HEADER.appendChild(o);
				var o = doc.createElement('div');
				o.className='gbh';o.style.right='0';
				HEADER.appendChild(o);
				var o = $nd('//*[@id="sft"]');
				if(o) HEADER.appendChild(o);
				body.insertBefore(HEADER, body.firstChild);
			}
		}

		STYLES += (GFX.bMnuFix ? '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight) +'px !important;\
			}\
		' : '');
		
		// Auto Streaming Pages
		// ----------------------------
		var res = $el('res') || $nd('//*[@id="res"]');
		if(res) {
			var holder = doc.createElement('div');
			holder.id = 'gfx-holder';
			res.parentNode.insertBefore(holder, res);
			holder.appendChild(res);
			AutoPage( {
				holder	: '//div[@id="gfx-holder"]',
				content	: '//*[@id="res"]'
			} );
			
		}
	}
	
	// ==============
	// Google Scholar
	// ==============
	else if(canRun.scholar)
	{
		// Header Feature
		//---------------------
		if( !HEADER ){
			HEADER = new Element('div', {'id' : 'header'});
			if(GFX.bMnuFix) HEADER.addClass('fixed');
			HEADER.appendChild(GBAR_BG);
			HEADER.appendChild(GBAR);
			var o = $nd('//*[@id="guser"]');
			if(o) HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.left='0';
			HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.right='0';
			HEADER.appendChild(o);
			var o = body.getElementsByTagName('table')[0];
			if(o) {
				o.id = 'sft';
				HEADER.appendChild(o);
			}
			body.insertBefore(HEADER, body.firstChild);
		}
		
		STYLES += (GFX.bMnuFix ? '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight) +'px !important;\
			}\
		' : '');
		
		// Auto Streaming Pages
		// -----------------------------
		/*
		if(GFX.rAutoPag){
			var holder = new Element('div',{'id' : 'gfx-holder'});
			var nav =$nd('//DIV[@class="n"]');
			if (nav){
				body.insertBefore(holder, nav);
				AutoPage( {
					holder	: "//DIV[@id='gfx-holder']",
					content	: "//P[@class='g']",
					nextLink: "//DIV[@class='n']/descendant::a[last()]"
				} );
				nav.style.display = 'none';
			}
		}
		*/

	}

	// ==============
	// Google Products
	// ==============
	else if(canRun.products)
	{
		// Header Feature
		//---------------------
		if( !HEADER ){
			HEADER = new Element('div', {'id' : 'header'});
			if(GFX.bMnuFix) HEADER.addClass('fixed');
			HEADER.appendChild(GBAR_BG);
			HEADER.appendChild(GBAR);
			var o = $nd('//*[@id="guser"]');
			if(o) HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.left='0';
			HEADER.appendChild(o);
			var o = doc.createElement('div');
			o.className='gbh';o.style.right='0';
			HEADER.appendChild(o);
			var o = body.getElementsByTagName('table')[0];
			if(o) {
				o.id = 'sft';
				HEADER.appendChild(o);
			}
			body.insertBefore(HEADER, body.firstChild);
		}

		STYLES += (GFX.bMnuFix ? '\
			BODY {\
				margin-top			: '+ (HEADER.offsetHeight) +'px !important;\
			}\
		' : '');

		// Remove Top Sponsors
		//----------------------------
		var ads = $nd('//*[@id="ps-top-ads-sponsored"]');
		if(ads) ads.parentNode.parentNode.removeChild(ads.parentNode);
		
		// Add Other Products Searches
		//----------------------------------
		addSearchers( (HEADER || $el('ps-titlebar')), SERVICE);

		// Auto Streaming Pages
		// -----------------------------
		var list = $nd('//table[contains(concat(" ",@class," ")," list ")]');
		if(list) {
			var holder = doc.createElement('div');
			holder.id = 'gfx-holder';
			list.parentNode.insertBefore(holder, list);
			var refine = $nd('//*[@id="ps-restrict-query"]');
			if(refine) {
				var refItems = refine.nextSibling.nextSibling;
				holder.appendChild(refine);
				holder.appendChild(refItems);
			}
			holder.appendChild(list);
			AutoPage( {
				holder	: '//div[@id="gfx-holder"]',
				content	: '//table[contains(concat(" ",@class," ")," list ")]'
			} );
		}
		
	}

	// ==============
	// Google Videos
	// ==============
	else if(canRun.video)
	{
		
		// Add Other Video Searches
		//----------------------------------
		if(hasSomeQuery) {
			addSearchers( ( $el('videoheader').lastChild || $el('resultsheadertable') ), SERVICE);
		}
		
		// Auto Streaming Pages   DON'T WORK... :(
		// -----------------------------
		/*
		AutoPage( {
			holder	: "//td[@id='search-results-td']",
			content	: "//div[@id='search-results']",
			nextLink: "//*[@class='gooooogle']/descendant::a[last()]"
		} );
		*/
	}

	// ==============
	// Google Translate
	// ==============
	else if(canRun.translate)
	{
		
	}

	// ==============
	// Google Maps
	// ==============
	else if(canRun.maps || canRun.local)
	{
		// Fix Header
		//---------------
		if(!HEADER) HEADER = $el('header') || $nd('//*[@id="header"]');
		var tb = $el('search') || $nd('//*[@id="search"]');
		var o = $el('guser') || $nd('//*[@id="guser"]');
		if(o && tb) {
			HEADER.insertBefore(o, tb);
			HEADER.insertBefore(GBAR, tb);
		}
		var o = doc.createElement('div');
		o.className='gbh';o.style.left='0';
		HEADER.appendChild(o);
		var o = doc.createElement('div');
		o.className='gbh';o.style.right='0';
		HEADER.appendChild(o);

	}

	// ==============
	// Main Options
	// ==============
	else if (canRun.intl || canRun.options)
	{
		GFX.bMnuFix = false;

		var o = $nd('//td[@id="hc-links"]/font');
		if(!o){
			o = $el('0a') || $nd('//*[@id="0a"]');
			if(o) o = o.parentNode;
		}
		if(o) o.innerHTML = '';
	}

	// ==============
	// Picasa Web
	// ==============
	else if(canRun.picasaweb)
	{

		// Relink Images
		//--------------------
		var isLH = URI.path[0]=='lh';
		var isAlbum = !isLH && URI.path[1] != undefined;

		if(isAlbum)
		{
			var doRelink = function(){
				top.window.setTimeout(function(){
					var nodes = doc.evaluate('//a[@class="goog-icon-list-icon-link"][@href]', doc.body, null, 7, null);
					for(var i=0,len=nodes.snapshotLength; i<len; i++) {
						var a = nodes.snapshotItem(i);
						var path = a.firstChild.src.split(/\/s[0-9]{2,3}\//);
						if(!path[1]) continue;

						a.href = path[0] + '/' + path[1];

						if(GFX.iPrev) {
							a.setAttribute('onmouseover', 'showImg(event,"'+path[0]+'","'+path[1]+'");');
							a.setAttribute('onmouseout', 'hideImg();');
						}
						
					};
				}, (isOpera ? 1000 : 150));
			}
			doRelink();

			// Image Preview
			//--------------------
			if(GFX.iPrev) 
			{
			SCRIPTS += 'var iPrevRT=' + GFX.iPrevRT + ';';
				SCRIPTS += '\
				var iPrevID;\
				var isPreview;\
				var iPrev = doc.createElement("div");\
				iPrev.id="gfx-iprev";\
				doc.body.appendChild(iPrev);\
				function showImg(ev,path,file){\
					if(isPreview) return false;\
					isPreview = true;\
					hideImg();\
					var tn = ev.target;\
					var w,h;\
						if(tn.clientWidth > tn.clientHeight){\
						w = 576;\
							h = parseInt(tn.clientHeight*576/tn.clientWidth);\
					}else{\
							w = parseInt(tn.clientWidth*576/tn.clientHeight);\
						h = 576;\
					}\
						var pos = {x:ev.pageX,y:ev.pageY};\
						var width = Math.max(pos.x, doc.body.clientWidth - pos.x) - 10;\
					var height = doc.documentElement.clientHeight-10;\
					if(w > width){var r=width/w; w=parseInt(w*r); h=parseInt(h*r);}\
					if(h > height){var r=height/h; w=parseInt(w*r); h=parseInt(h*r);}\
					iPrev.style.left = (pos.x > w ? pos.x - w - 30 : pos.x + 30) + "px";\
					iPrev.style.bottom = parseInt((height - h)/2) + "px";\
					var img = doc.createElement("IMG");\
					img.onerror = function(){\
						iPrev.removeChild(img);\
						var sp = doc.createElement("span");\
						sp.className = "error";\
						sp.style.width = w + "px";\
						sp.style.height = parseInt((h+30)/2) + "px";\
						sp.style.paddingTop = parseInt((h-30)/2) + "px";\
						sp.innerHTML = "ERROR 404";\
						iPrev.appendChild(sp);\
					};\
					img.setAttribute("width", w);\
					img.setAttribute("height", h);\
					iPrev.appendChild(img);\
					iPrevID = window.setTimeout(function(){\
						img.src = path + "/s576/" + file;\
						iPrev.style.display="block";\
					},iPrevRT);\
						return false;\
				};\
				function hideImg(){\
					isPreview = false;\
					window.clearTimeout(iPrevID);\
					if(!iPrev.firstChild) return;\
					iPrev.removeChild(iPrev.firstChild);\
					iPrev.style.display="none";\
					return false;\
				};\
				';
			}
		}

	}


	// ====================
	// Google Suggest Script
	// =====================
	if(canRun.gSuggest){

		var qInput = $nd('//input[@name="q"]');
		
		if(qInput){
		qInput.setAttribute("size","60");
		SCRIPTS += '\
		function doSecond() {\
			var iquery=doc.evaluate(\'//input[@name="q"]\',doc.body,null,9,null).singleNodeValue;\
			if(!window.google) window.google={};\
			else if( window.google.ac ){\
				window.google.ac = function(){};\
				var target = iquery.parentNode;\
				var qClone = iquery.cloneNode(true);\
				target.replaceChild(qClone, iquery);\
				iquery = qClone;\
				iquery.setAttribute("size","60");\
				iquery.setAttribute("autocomplete","off");\
				iquery.autocomplete = "off";\
				iquery.focus();\
				iquery.className = iquery.className+" selected";\
			};\
			(function(){\
			var form, input_q, table, q_val, curVal, lastVal, reqVal="";\
			var args = "";\
			var data=null, curRow=null;\
			var row=-1, isVisible=0, isOver=0;\
			var head=doc.getElementsByTagName("head")[0];\
			var domainURL="http://clients1.google.com";\
			var pathURL="/complete/search?hl=";\
			var fixLng = {af:"", az:"", be:"", bh:"", br:"", bs:"", co:"", cy:"", eo:"", eu:"", fa:"", fo:"", fy:"", ga:"", gd:"", gl:"", gn:"", hi:"", hy:"", ia:"", is:"", jw:"", ka:"", kk:"", km:"", kn:"", ku:"", ky:"", la:"", ln:"", lo:"", mi:"", mk:"", mn:"", mo:"", mr:"", mt:"", ne:"", nl:"de", nn:"no", oc:"", or:"", ps:"", qu:"", rm:"", sd:"", sh:"", si:"", sn:"", so:"", sq:"", sr_ME:"", st:"", su:"", sw:"", ta:"", tg:"", tk:"", to:"", tt:"", tw:"", ug:"", ur:"", uz:"", xh:"", yi:"", yo:"", zu:""};\
			var kHL = doc.location.search.match(/hl=([^&]*)|$/i)[1] || (window.google.kHL ? window.google.kHL : "en");\
			if(fixLng[kHL]!=null) kHL = fixLng[kHL].length ? fixLng[kHL] : "en";\
			function installAC(input,d,e){\
				form = input.form || doc.forms[0];\
				input_q = input;\
				input_q.setAttribute("autocomplete", "off");\
				input_q.addEventListener("blur", hideAC, false);\
				input_q.addEventListener("focus", onFocus, false);\
				input_q.addEventListener("keydown", onKeyDown, false);\
				input_q.addEventListener("keyup", onKeyUp, false);\
				if(window.opera) \
					window.addEventListener("mousewheel", function(){if(isVisible)hideAC();}, false);\
				else \
					window.addEventListener("DOMMouseScroll", function(){if(isVisible)hideAC();}, false);\
				curVal = lastVal = q_val = getValue();\
				table = doc.createElement("table");\
				table.cellSpacing = table.cellPadding = "0";\
				table.className = "gac_m";\
				table.onmouseover=(function(){isOver=1;});\
				table.onmouseout=(function(){isOver=0;});\
				doc.body.appendChild(table);\
				hideAC();\
				setPos();\
				window.addEventListener("resize", setPos, false);\
				var style = doc.createElement("style");\
				doc.getElementsByTagName("head")[0].appendChild(style);\
				var addRule = function(obj, att){\
					var rule = obj + " { " + att + " }";\
					style.sheet.insertRule(rule, style.sheet.cssRules.length);\
				};\
				addRule(".gac_m","display:none; cursor:default; line-height:15pt; border:1px solid gray; margin:0; position:fixed; z-index:99999; background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAABdJREFUGFdj/P///zMGdAASRMcMlAkCAFPzUaF4TK5sAAAAAElFTkSuQmCC) !important;");\
				addRule("tr.gac_a","font-family:arial,hevetica,sans-serif !important; color:black;");\
				addRule("tr.gac_a:hover, tr.gac_a.hover","background-color:#47b; color:white; text-shadow:0 0 2px #bbb;");\
				addRule("tr.gac_a.hover","background-color:#58c;");\
				addRule("tr.gac_a:hover td, tr.gac_a:hover td b, tr.gac_a.hover td","color:white;");\
				addRule(".gac_c","white-space:nowrap; overflow:hidden; font-size:10pt; text-align:left; padding-left:2pt; padding-bottom:1px; font-family:arial,hevetica,sans-serif !important;");\
				addRule(".gac_c b","font-weight:bold;color:#00b;");\
				addRule(".gac_d", "white-space:nowrap; overflow:hidden; font-size:8pt; text-align:right; color:#494; padding:0 3px; font-family:arial,hevetica,sans-serif !important;");\
				addRule(".gac_e td","padding:0 2pt 1pt; text-align:right; font-size:9pt; line-height:14pt; opacity:0.8; font-family:arial,hevetica,sans-serif !important;");\
				addRule(".gac_e span","color:black; text-decoration:none; cursor:pointer; font-weight:bold; padding:0 3px;");\
				addRule(".gac_e select","display:inline-block; border:1px solid transparent; padding:0 1pt; text-align:right; font-size:8pt !important; font-weight:bold; line-height:11pt; color:#368; background-color:#eee; font-family:arial,hevetica,sans-serif !important;");\
			};\
			function hideAC(ev){\
				if(isOver) return;\
				table.style.display = "none";\
				isVisible = 0;\
				reqVal = lastVal = "";\
				if(ev){\
					ev.target.className = ev.target.className.replace(/\s*selected/,"");\
				}\
			};\
			function showAC(){\
				table.style.display = "table";\
				setPos();\
				isVisible = 1;\
			};\
			function nodeText(nd,str){\
				nd.appendChild(doc.createTextNode(str));\
			};\
			function getValue(){\
				if(!/:/.test(input_q.value)) return (curVal=input_q.value);\
				args = input_q.value.split(/([\-]?\s*[a-z]+\s*:\s*[^ ]*)/);\
				var len = args.length;\
				for(var x=0; x < len; x++){\
					if(args[x].lastIndexOf(":") == -1 && args[x].length){\
						curVal = args[x];\
						args.splice(x,1);\
						break;\
					}\
				}\
				args = args.join("");\
				return curVal;\
			};\
			function setValue(value){\
				input_q.value = value + (args ? " " + args: "");\
				q_val=value;\
			};\
			function getGlobalPos(obj,offset){\
				var pos=0;\
				while(obj){\
					pos += obj[offset];\
					obj = obj.offsetParent;\
				}\
				return pos;\
			};\
			function setPos(){\
				if(table){\
					table.style.left = getGlobalPos(input_q,"offsetLeft") + "px";\
					table.style.top = getGlobalPos(input_q,"offsetTop") + input_q.offsetHeight-1 + "px";\
					table.style.width = input_q.offsetWidth + "px";\
				}\
			};\
			function onFocus(ev){\
				lastVal = "";\
				getValue();\
				send();\
				if(ev){\
					ev.target.className = ev.target.className+" selected";\
				}\
				return 0;\
			};\
			function onKeyDown(event){\
				if (!isVisible) return;\
				var key = event.keyCode;\
				(key==40 && select(row+1));\
				(key==38 && select(row-1));\
				(key==39 && send());\
				if(key==13){\
					if(event.ctrlKey){\
						send();\
						event.preventDefault();\
						event.stopPropagation();\
						return false;\
					} else {\
						hideAC();\
						send();\
						event.cancelBubble=true;\
						form.submit();\
					}\
				}\
				else if(key==27){\
					setValue(curVal.replace(/ +/g," "));\
					isOver=0;\
					input_q.focus();\
					hideAC();\
					event.cancelBubble=true;\
					return event.returnValue=false;\
				}\
			};\
			function onKeyUp(event){\
				var key=event.keyCode;\
				if(key!=40 && key!=38){\
					getValue();\
					send();\
				}\
			};\
			function onMouseOver(){\
				if(curRow && curRow.className=="gac_a hover") curRow.className="gac_a";\
				curRow=this;\
				row = parseInt(curRow.alt);\
			};\
			function onMouseDown(event){\
				setValue(this.completeString);\
				onFocus();\
				(event.ctrlKey && form.submit());\
				return true;\
			};\
			function select(idx){\
				if (!reqVal && curVal){\
					lastVal="";\
					scriptReq();\
					return;\
				}\
				if (!data || data.length<1) return;\
				if (!isVisible){\
					showAC();\
					return;\
				}\
				var len=data.length-2;\
				if(curRow) curRow.className="gac_a";\
				if(idx > len) idx=0;\
				else if(idx < 0) idx=len;\
				row=idx;\
				curRow=data.item(idx);\
				curRow.className="gac_a hover";\
				setValue(curRow.completeString);\
				return;\
			};\
			function send(){\
				setPos();\
				q_val = curVal;\
				if(q_val == "") hideAC();\
				else scriptReq();\
			};\
			function scriptReq(){\
				if(lastVal!=curVal && curVal){\
					var req=$el("gs_Req");\
					if(req) head.removeChild(req);\
					req=doc.createElement("script");\
					req.setAttribute("id","gsReq");\
					req.src = domainURL + pathURL + kHL + "&q=" + encodeURIComponent(curVal);\
					head.appendChild(req);\
				}\
				lastVal=curVal;\
			};\
			function Suggest_apply(a){\
				if(a[0] != curVal) return;\
				reqVal=a[0];\
				table.innerHTML="";\
				q_val = q_val.replace(/ +/g," ").replace(/(^ | $)/g,"");\
				var ln = q_val.replace(/[:\"\/\$]/g,"").length;\
				var cnt=0;\
				var len=a[1].length;\
				for(var d=0; d < len; d++){\
					var e=a[1][d];\
					if(e){\
						var el=table.insertRow(-1);\
						el.ondblclick=function(){\
							form.submit();\
						};\
						el.onmousedown = onMouseDown;\
						el.onmouseover = onMouseOver;\
						el.completeString = e[0];\
						el.completeId = e[2];\
						el.alt = cnt;\
						el.className = "gac_a";\
						el.title = "' + LNG.ui_txt_gs + '";\
						var g=doc.createElement("td");\
						var gb=doc.createElement("b");\
						nodeText(gb,e[0].substr(0,ln));\
						g.appendChild(gb);\
						nodeText(g,e[0].substr(ln));\
						g.className="gac_c";\
						el.appendChild(g);\
						var k=doc.createElement("td");\
						nodeText(k,e[1].replace(/([0-9])[^0-9\.\,]([0-9])/g,"$1\.$2").replace(/[^0-9\.\,]+.*$/,""));\
						k.className="gac_d";\
						el.appendChild(k);\
						cnt++;\
					}\
				};\
				if(cnt > 0){\
					var n=table.insertRow(-1);\
					var ft=doc.createElement("td");\
					ft.colSpan=2;\
					n.className="gac_e";' + (GFX.gSuggestLng ? 'n.style.backgroundColor="#d0d0d7";':'') +
					(GFX.gSuggestLng ? 'addLng(ft);' : '') + '\
					var m=doc.createElement("span");\
					n.appendChild(ft);\
					ft.appendChild(m);\
					nodeText(m,"x");\
					m.onclick=function(){\
						isOver=0;\
						hideAC();\
						lstVal=reqVal="";\
					};\
				}\
				row=-1;\
				data=table.rows;\
				(data.length > 0 ? showAC : hideAC)();\
			};' +
			(GFX.gSuggestLng ? '\
			var lng = {ar:"Arabic",am:"Amharic",bn:"Bengali",bg:"Bulgarian",ca:"Catalan",zh_CN:"Chinese",zh_TW:"Chinese (Tr)",hr:"Croatian",cs:"Czech",da:"Danish",en:"English",et:"Estonian",tl:"Filipino",fi:"Finnish",fr:"French",de:"German",el:"Greek",gu:"Gujarati",iw:"Hebrew",hu:"Hungarian",id:"Indonesian",it:"Italian",ja:"Japanese",ko:"Korean",lv:"Latvian",lt:"Lithuanian",ms:"Malay",ml:"Malayalam",no:"Norwegian",pl:"Polish",pt_BR:"Portuguese (Br)",pt_PT:"Portuguese (Pt)",pa:"Punjabi",ro:"Romanian",ru:"Russian",sr:"Serbian",sk:"Slovak",sl:"Slovenian",es:"Spanish",sv:"Swedish",ta:"Tamil",te:"Telugu",th:"Thai",ti:"Tigrinya",tr:"Turkish",uk:"Ukrainian",vi:"Vietnamese"};\
			function setLng(){\
				kHL=this.options[this.selectedIndex].value;\
				onFocus();\
				return false;\
			};\
			function addLng(target){\
				var sel = doc.createElement("select");\
				sel.onchange=setLng;\
				for(var n in lng){\
					var o=doc.createElement("option");\
					var label = lng[n];\
					n = n.replace("_", "-");\
					o.innerHTML = label;\
					o.value = n;\
					if(kHL==n) o.selected = "true";\
					sel.appendChild(o);\
				}\
				target.appendChild(sel);\
			};' : '') +
			'window.google.ac={i:installAC,h:Suggest_apply};\
			})();\
			window.google.ac.i(iquery);\
		};\
		window.setTimeout(doSecond,400);';

		}
	}

	addStyle( STYLES );

	// Add JavaScript Object to the Body 
	var sc = doc.createElement("script");
	sc.setAttribute("type","text/javascript");
	sc.innerHTML = SCRIPTS;
	body.appendChild(sc);

	
	BgBox.inject(body, 'top');
	$el(top.window).addEvent('resize',function(){
		var docSize = doc.getSize();
		BgBox.setStyles({
			'height': docSize.y +'px',
			'width': docSize.x +'px'
		});
	});
	
	GFX_FirstRun = function(){};
};
// END GoogleFx FirstRun










//########################################################################
//  G O O G L E   F X    B Y     R E Q U E S T
//########################################################################
var GFX_byRequest = function()
{
	var doc = document;
	if (doc.location == top.location && doc.isGFXLoaded) return;

	if (!isGoogleCache) {
	
	// Remove Url Click Tracks
	// -------------------------------
	if(GFX.rNoTrack)
	{
		var o = doc.evaluate('//a[contains(@href,"/pagead?") or contains(@href,"/url?")]', doc.body, null, 7, null);
		var i = o.snapshotLength;
		while(i--) {
			var el = o.snapshotItem(i);
			var url = el.href.match(/[\?&](url|q|adurl)=(https?:[^&]+)/i);
			if(url)	el.href = unescape(url[2]);
		}

		var o = doc.evaluate('//div[@id="res"]//a[@onmousedown]', doc.body, null, 7, null);
		var i = o.snapshotLength;
		while(i--) {
			var el = o.snapshotItem(i);
			el.removeAttribute('onmousedown');
		}
		
		if(win.rwt) win.rwt = function(){};
		if(win.clk) win.clk = function(){};
	}


	// ============================
	// Main & by Request Web Search
	// ============================
	if(canRun.search)
	{
		var results = doc.evaluate('//*[@id="res"]//li[contains(concat(" ",@class," ")," g ")]//h3//a', doc.body, null, 7, null);
		
		if (!results) {GM_log('Error! Results elements not found!');return;}

		if(isByRequest) {
			var botRev = $el('brs') || $nd('//*[@id="brs"]', doc.body);
			if(botRev) botRev.parentNode.removeChild(botRev);
		}
		
		
		//  setCounter  function
		//----------------------------
		var cntStart = URI.query.start;
		cntStart = (cntStart) ? parseInt(cntStart) : 0;
		cntStart += (URI.query.gfx_ac) ? parseInt(URI.query.gfx_ac) : 0;

		var setCounter = GFX.rCounter ?
			function(el)
			{
				new Element('span', {
					'class'	: 'cnt',
					'html'	: ++cntStart
				}).inject(el,'before');
			}
			: function(){return;};
   
		//  setThumbshot   function
		//---------------------------------
		var setThumbshot = (GFX.rThShots) ?
			function(el)
			{
				el.setAttribute('target', GFX.rTrgLnk);
			   
				var a = new Element('a', {
					'href'	: el.href,
					'target': GFX.rTrgLnk,
					'title'	: el.href,
					'class'	: 'tn',
				}).append(new Element('img',{
						'src'	: 'http://open.thumbshots.org/image.pxf?url=' + el.href
					}));

				el.parentNode.parentNode.insertBefore(a, el.parentNode.parentNode.firstChild);
			}
			: function(){return;};

			
		//  Runtime
		//---------------------------------
		for(var i=0,len=results.snapshotLength; i<len; i++)
		{
			var el = results.snapshotItem(i);
			setCounter(el);
			setThumbshot(el);
			el.setAttribute('title', el.href);
		};

		
		// Site Filter
		//--------------------------------
		if(GFX.rSiteFlt)
		{
			var classType, textMode, injectMode;
			if(GFX.rRefIcn){
				classType	= 'qs';
				textMode	= '';
				injectMode	= 'before';
			} else {
				classType	= 'onlyFrom';
				textMode	= '['+ LNG.from + LNG.this_ + LNG.domain +']';
				injectMode	= 'after';
			}
		
			var cites = doc.evaluate('//div[contains(concat(" ",@class," ")," s ")]//cite', doc.body, null, 7, null);
			for(var i=0,len=cites.snapshotLength; i<len; i++){
				var el = cites.snapshotItem(i);
				var url = el.innerHTML.replace(/<[^>]+>/g,'').replace(/ \- .*$/,'').replace(/\s/g,'').replace(/^https?:\/\//,'');
				url = url.split('/');
				if( /[\.\?]/.test(url[url.length-1]) ) url = url.slice(0,-1);
				url = url.join('/').replace(/\/$/,'');

				var qSite = QUERYS.site ?  RegExp(url).test(QUERYS.site.join(',')) : false;

				new Element('a', {
					'class'		: classType + (qSite ? '_n' : ''),
					'html'		: textMode,
					'title'		: (qSite ? '- '+url : LNG.only +' '+ LNG.from +': '+ url),
					'onmouseup'	: 'onlyFrom("' + url + '",event);return !1;'
				}).inject(el, injectMode);

			};
		}

		// Reference Icons
		if(GFX.rRefIcn) {
			var links = doc.evaluate('//li[contains(concat(" ",@class," ")," g ")]//a[not(contains(concat(" ",@class," ")," l "))]', doc.body, null, 7, null);

			for(var i=0,len=links.snapshotLength; i<len; i++){
				var el = links.snapshotItem(i);
				var href = el.href;
				
				el.target = GFX.rTrgLnk;
			   
				if( href.indexOf('://translate.') != -1 )
				{
					el.title = el.innerHTML.replace(/nbsp\;/g,' ').replace(/\&/g,'');
					el.innerHTML = '';
					el.parentNode.removeChild(el.parentNode.firstChild);
					el.parentNode.removeChild(el.parentNode.lastChild);
				}
				else if( href.indexOf('q=cache:') != -1 )
				{
					el.title = el.innerHTML.replace(/nbsp\;/g,' ').replace(/\&/g,'');
					el.innerHTML = '';
					el.className = 'ch';
				}
				else if( href.indexOf('q=related:') != -1 )
				{
					el.title = el.innerHTML.replace(/nbsp\;/g,' ').replace(/\&/g,'');
					el.innerHTML = '';
					el.className = 'sm';
				}
				else if( el.id && el.id.substring(0,3)=='gnl' )
				{
					el.title = el.innerHTML.replace(/nbsp\;/g,' ').replace(/\&/g,'');
					el.innerHTML = '';
					el.className = 'nt';
				}

				if(el.previousSibling && el.previousSibling.nodeType == 3)
					el.parentNode.removeChild(el.previousSibling);
				if(el.nextSibling && el.nextSibling.nodeType == 3)
					el.parentNode.removeChild(el.nextSibling);
			};
		}
		
	}

	// ========================
	// Main & by Request Images
	// ========================
	else if(canRun.images && URI.host[1] == 'google')
	{
		var thumbs = doc.evaluate('//*[contains(@id,"tDataImage")]/a/img', doc.body, null, 7, null);
		var ref = doc.evaluate('//*[contains(@id,"tDataText")]/font//span[contains(concat(" ",@class," ")," a ")] | //*[contains(@id,"tDataText")]/font//font', doc.body, null, 7, null);
		var x = thumbs.snapshotLength;

		// Image Relink
		//-----------------------
		while(x--)
		{
			var img = thumbs.snapshotItem(x);
			var a = img.parentNode;
			var rf = ref.snapshotItem(x);
		   
			var data = {};
			var href = a.href.split('imgres?');

			if(href[1]){
				href = href[1].split('&');
				var i = href.length-1;
				while(i--){
					var db = href[i].split('=');
					data[db[0]] = decodeURL(db[1]);
				}
			} else {
				continue;
			}
			
			/*
			if(!data.imgurl){
				data.imgurl = 'http' + img.src.split(':http')[1];
				data.imgrefurl = data.imgurl;
				data.w='false';
				data.h='';
				data.sz='';
			}
			*/

			a.target = GFX.rTrgLnk;
			a.setAttribute('class', 'gfx_tn');
			a.setAttribute('rel', data.imgurl);
		
			if(isOpera) a.href = data.imgurl;
			else a.setAttribute('onmouseup', "this.href='" + data.imgurl + "';return 1;");
		
			a.removeAttribute('onclick');
		
			// Thumbs References holder
			a.parentNode.setAttribute('class', 'gfx_img');
			var iRefs = new Element('span');

			// Image Preview
			if(GFX.iPrev) {
				if(GFX.iPrevNoIcn) {
					a.setAttribute('onmouseover', 'showImg(this,'+data.w+','+data.h+','+data.sz+');');
					a.setAttribute('onmouseout', 'hideImg(200);');
					//a.setAttribute('onmousedown', 'hideImg(200);');
				} else{
					var iPrev = doc.createElement('a');
					iPrev.className = 'preview';
					iPrev.setAttribute('onmouseover', 'showImg(this,'+data.w+','+data.h+','+data.sz+');');
					iPrev.setAttribute('onmouseout', 'hideImg(200);');
					//iPrev.setAttribute('onmousedown', 'hideImg(200);');
					iRefs.appendChild(iPrev);
				}
			} else {
				a.title = data.imgurl + ' - ' + data.w + 'x' + data.h;
			}

			// Site Filter from Image Source
			var sfUrl = parseUrl(data.imgurl);
			sfUrl = sfUrl.host + sfUrl.path
			sfUrl = sfUrl.split('/');
			sfUrl = sfUrl.slice(0,sfUrl.length-1).join('/');

			var isSF=false;
			if( QUERYS.site && (isSF = RegExp(sfUrl).test(QUERYS.site.join(','))) ){
				sfUrl = sfUrl.split('/');
				sfUrl = sfUrl.slice(0,sfUrl.length-1).join('/');
			}
			var iSF = doc.createElement('a');
			iSF.className = 'qs' + (isSF ? '_n' : '');
			iSF.title = LNG.only + ' ' + LNG.from + ': ' + sfUrl;
			iSF.setAttribute("onmouseup","onlyFrom('" + sfUrl+"',event);return !1;");
			iRefs.appendChild(iSF);
			
			// Search by Similar Images
			var tbnid = data.tbnid.replace(/\:$/,'');
			var byImg = doc.createElement('a');
			byImg.className = 'sm'
			byImg.title = LNG.search + ' ' + LNG.by + ' ' + 'Similar';
			byImg.target = GFX.rTrgLnk;
			byImg.href = 'http://similar-images.googlelabs.com/images?qtype=similar&tbnid=' + tbnid + '&q=' + URI.query.q;
			iRefs.appendChild(byImg);
			
			//add Thumbs Refs
			iRefs.inject(a, 'after');
			
			// Info area
			//---------------------------
			rf.parentNode.parentNode.className='gfx_info';
			rf.className='gfx_lnk';
			
			// RefLink
			var refUrl = data.imgrefurl.replace(/^https?:\/\//,'').split('/')[0];
			if(refUrl.length > 30) refUrl = refUrl.substr(0,30) + '...';

			// Site Filter from RefLink
			var sfUrl = parseUrl(data.imgurl);
			sfUrl = sfUrl.host + sfUrl.path
			sfUrl = sfUrl.split('/');
			sfUrl = sfUrl.slice(0,sfUrl.length-1).join('/');

			var isSF=false;
			if( QUERYS.site && (isSF = RegExp(sfUrl).test(QUERYS.site.join(','))) ){
				sfUrl = sfUrl.split('/');
				sfUrl = sfUrl.slice(0,sfUrl.length-1).join('/');
			}
			
			// Add RefLink, Site Filter & Try Cache to Info area
			rf.innerHTML =  '<div class="icn"><a class="qs'+(isSF?'_n':'')+'" title="' + LNG.only + ' ' + LNG.from + ': ' + sfUrl + '" onmouseup="onlyFrom(\''+sfUrl+'\',event);return !1;"></a><a class="ch" href="/search?q=cache:' + encodeURL(data.imgrefurl) + '" target="_blank" title="' + LNG.trycache + '"></a></div>' + '<a href="'+ data.imgrefurl +'" target="'+GFX.rTrgLnk+'" title="'+ data.imgrefurl+'">' + refUrl +'</a>';

			while(rf.nextSibling){
				rf.parentNode.removeChild(rf.nextSibling);
			}

		}

	}
	// ==============
	// Main News
	// ==============
	else if(canRun.news && hasSomeQuery)
	{
		var o = doc.evaluate('//*[contains(@class,"story")]', doc.body, null, 7, null);
		var len = o.snapshotLength;
		if(!len) return;

		// Counter Results
		//----------------------
		if(GFX.rCounter)
		{
			var start = URI.query.start;
			start = (start ? parseInt(start) : 0);
			for(var x=0; x<len; x++)
			{
				var el = o.snapshotItem(x).getElementsByTagName('a');
				if(!el[0]) continue;

				var sp = doc.createElement('span');
				sp.setAttribute('class','cnt');
				sp.innerHTML = (start + x + 1);
				el[0].parentNode.insertBefore(sp, el[0]);
			}
		}

		// ThumbShots
		//------------------
		if(GFX.rThShots)
		{
			var x = len;
			while (x--)	{

				var el = o.snapshotItem(x).getElementsByTagName('a');
				if(!el[0]) continue;

				el[0].target = GFX.rTrgLnk;
				el[0].title = el[0].href;

				var a = doc.createElement('a');
				a.href = el[0].href;
				a.target = GFX.rTrgLnk;
				a.title = el[0].href;
				var img = doc.createElement('img');
				a.className = 'tn';
				img.style.display="inline-block";
				img.src = 'http://open.thumbshots.org/image.pxf?url=' + el[0].href;
				a.appendChild(img);
				el[0].parentNode.insertBefore(a, el[0].parentNode.firstChild);

			};
		}
	}

	// ==============
	// Google Blogs
	// ==============
	else if(canRun.blogsearch)
	{
		var o = $nd('//*[@id="m"]/div//a[contains(@href,"scoring=")]/ancestor::table', doc.body);
		if(o) o.parentNode.removeChild(o);

		// Runtime
		//---------
		if( $el('m') || $nd('//*[@id="m"]', doc.body) )
		{
			//  setThumbshot
			var setThumbshot = GFX.rThShots ?
			function(el,href)
			{
				el.append(new Element('a', {
						'href'		: href,
						'target'	: GFX.rTrgLnk,
						'title' 	: href,
						'class'		: 'tn',
					}).append(new Element('img', {
						'src' : 'http://open.thumbshots.org/image.pxf?url=' + href
					}))
				);
			}
			: function(){return;};
		
			//  setCounter
			var cntStart = URI.query.start;
			cntStart = (cntStart) ? parseInt(cntStart) : 0;
			cntStart += (URI.query.gfx_ac) ? parseInt(URI.query.gfx_ac) : 0;

			var setCounter = GFX.rCounter ?
				function(el)
				{
					new Element('span', {
						'class'	: 'cnt',
						'html'	: ++cntStart
					}).inject(el,'before');
				}
				: function(){return;};
		
			// Fix Results
			var items = doc.evaluate('//a[starts-with(@id,"p-")]', doc.body, null, 7, null);

			var holder = doc.evaluate('//p[contains(concat(" ",@class," ")," g ")]', doc.body, null, 7, null);

			for(var i=0,len=items.snapshotLength; i<len; i++){
				var el = items.snapshotItem(i);

				el.target = GFX.rTrgLnk;
				el.title = el.href;

				var cnt = new Element('div', {'class' : 'cont'});
				
				var p = $el(holder.snapshotItem(i));
				var tb = el.getNext();
				cnt.append(el);
				cnt.append(tb);
				
				setThumbshot(p, el.href);
				setCounter(el);
				p.append(cnt);
				
			};
			
			// Clean last orfan holder
			var o = holder[holder.snapshotLength-1];
			if(o && !o.hasChildrens) o.parentNode.removeChild(o);
			
		}

		
		if(GFX.rAutoPag)
		{
			var nav= $el('navbar') || $nd('//*[@id="navbar"]', doc.body);
			if(nav) nav.hide();
		}
	}

	// ==============
	// Google Cache
	// ==============
	} else {
		var o = doc.evaluate('//a[@href][not(contains(@href,"search?q=cache:"))]', doc.body, null, 7, null);
		var x = o.snapshotLength;
		while ( x-- )
		{
			var el = o.snapshotItem(x);
			el.setAttribute('onmousedown', 'var url=decodeURIComponent(this.href.replace(/^.+q=cache:/,"").replace(/^http:../,""));if(event.ctrlKey || event.shiftKey || event.button == 1){ url = "/search?q=cache:" + encodeURIComponent(url); this.href=url; } else this.href = "http://"+url; return 1;');
			el.setAttribute('title', LNG.ui_txt_cache);
		}
		
	} // end isNotCache

	
	
	
	// Complete Process Element for byRequest
	var el = doc.createElement('var');
	el.id = 'gfx-req-loaded';
	doc.body.appendChild(el);
	
}; // END GoogleFx byRequest







// =====================
// Global GFX Styles
// ====================
var GFX_Styles = function(){
	if(isByRequest) return;

	var docSize = doc.getScrollSize();
	
	STR = '\
	BODY {\
		'+ CSS.cmn.body +'\
		padding				: 1px;\
		margin				: 0;\
	}\
	A {\
		color				:'+ DB.Color.link +' !important;\
		text-decoration		: none;\
	}\
	A:hover {\
		text-decoration		: underline;\
		text-shadow			: 0 0 5px #888;\
	}\
	A:visited {\
		color				:'+ DB.Color.visited +' !important;\
		text-decoration		: underline;\
	}\
	IMG {\
		border				: 0;\
	}\
	INPUT[type="checkbox"] {\
		display				: inline-block;\
		vertical-align		: middle;\
	}\
	INPUT[type="text"], INPUT[name="q"] {\
		padding				: 2px;\
		border				: solid 1px #ccc;\
		font-weight			: bold;\
	}\
	INPUT, BUTTON {\
		font-family			: Arial, Hevetica, sans-serif;\
		border				: solid 1px #ccc;\
	}\
	#gfx-nav, #navbar {\
		background-color	: white;\
		opacity				: .45;\
	}\
	#gfx-nav .csb{\
		background-image	: url(/images/nav_logo3.png);\
		background-repeat	: no-repeat;\
		height				: 26px;\
		display				: block;\
	}\
	#gfx-nav .ch{\
		cursor				: pointer;\
	}\
	#gfx-nav:hover, #navbar:hover {\
		opacity				: .90;\
	}\
	#gfx-nav #nav-content {\
		width				: 100%;\
		height				: 60px;\
		max-height			: 60px;\
		white-space			: nowrap;\
	}\
	#gfx-nav #nav-content .csb {\
		background-position	: -60px 0;\
		width				: 16px;\
	}\
	#gfx-nav td{\
		padding				: 0;\
		text-align			: center;\
	}\
	#gfx-nav A, #gfx-nav .cur, #navbar A {\
		display				: inline-block !important;\
		color				: #000;\
		clear				: both;\
		font-weight			: bold;\
		font-size			: 10pt;\
		border				: 1px solid transparent;\
	}\
	#gfx-nav A:hover, #navbar A:hover {\
		border				: 1px solid #c9d7f1;\
	}\
	#sd {\
		font-size			: 12pt;\
	}\
	#sft LABEL {\
		font-size			: 9pt;\
	}\
	#sft INPUT[type="radio"] {\
		margin				: 0;\
		vertical-align		: bottom;\
	}\
	#sft INPUT[name="q"]\
	{\
		border				: 1px solid #ccc;\
		margin				: 1px;\
	}\
	INPUT.selected\
	{\
		border				: 2px solid #9bf !important;\
		margin				: 0px;\
	}\
	#gfx-logo {\
		position			: relative;\
		display				: inline-block;\
		min-width			: 33px;\
		min-height			: 25px;\
		z-index				: 123;\
		background-repeat	: no-repeat;\
		background-position	: left top;\
		background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACEAAAAZCAYAAAC/zUevAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAABNxJREFUSEuFlc1rXFUYxk+mJtF2pEgWtflOZvIxmUwy+f7OxpUWBF2I4KZ/gBvFtSs3tnQhFDdSrCW6EF0URVy3Mgh25cqtoLUWFIzOYJrkHn/PyXmvp9eAgYf33HPPnN/zvuc9N127u7u+VCo5730uF/80pz/Frq6uEPWXZZk7ODi4y3A3nbf36W+0Nt1H7/Qbzee/3dnZ8dL29nYeNZa2trZybW5u5uO1tTW/uLh4BznTwsKCk5rNZojz8/Nubm4uxEajEcaSxrOzs7nq9bpzBixCzYDgpo2NDS+trKx4QMGEwc2ATJgBgwp8GlwGggnBUgMCGszi+vq6V/arq6shLi8ve2B30uwFT7O17ItZC6o5xVqt5mZmZk5MpFkLQIZttG8Ctl8U0JuWfZq5GTF4WgHLXGBJJiRnmStbaWlp6W82v4SBKjEX0DBWjOorwq3s/1d+q0JeCZU3FfAOkEZ6xiq1nbXABfhTQMtkXmbzIMY9VnZlnFYAcO/09HSZWFacmJgou/S8debAZaJu4GKjWbkVVXLiW6xpYSyIuRbgdwB3G9zKTjzD3Pu815rW1NRUa3R0tOUEltTxseE6bFYrXjFrNDvj5KznMfEL5tWsnt95YIdALqXNx1yJ59dRhjzv/fj4uJ+cnLznDE4vBBNk1kaV066ZMi+eNxtq8zdZf8TvPO+9IGT5A+BhqwLQVeYfCC5Vq1XpT47jRSe4iX7QJm1glfRqpWOrgJU6ZvsM8QtMZKwNRoAfYeQKehJ1A/4AZWaAKhxj4D2M9Lp4JYMRmaCkx/oGoK/J7j8C9gkbXbRrlly1dQw8kAlVQpFStwE9j6Em6/6iETXn6QNV4Ut0jvdOXzw1Y5BMmNJ5O2udN5vdZ9NK0mzhrgM4g94Frn4IRgQFcpf331ON8Kw+AP6wUqmsIMf4xITAKdSaTHAzoBJLZiL92ACQCUeWZxl/JhOAA5Tn49TA2NjYPsZew8ATMoGpfythMDWXSmllVUap2Pg+qggquAQ8F4BNnn9S2U0yoUbUMQC+jXox43g+MaESW5aKbK6Guk3cI+6lkU330HXmL5wCdzpfYD3Eq6zLzATPgnvAOo5RGTATivrg5JlrDLRNOSsqt7JNMy5mLqiqoBgNKOpIbghsJmRAlZAJsn9FFTCNjIycmFAFzAwbdNBsPOO0zI+V/RS40xkz/wIGfrfszYBMaDw8PHwPcJ/gprwSdu5s0kGN9JxtrE5Os9ezJHjs9D7efxs/RH5oaEjZH5oBmQB8yPx1KnEOQ07SP5fQeNbRbNJBdQNbqQslfwweTXQD+1BAVUFNCOCY+DZG/ohHEeYxcci7V1WJYELXKLlO2qDDRjMp1DJOs7brpe5Wc/H8HO9/kwF9CwYHBzMgH6PzgK4BP2KsSgQNDAx8x5pniU5XTHc5b6JoomZAazgreREer9l51n3Fu0yV0DGgh8CaMdunMfKN4MRgAgO+v7//c+LZYKJwlTpsNlXM2q6VZZ5eMda/gR6p5IKQnUA3GHfbuQN7mfG+GdE65h6x9rK6OZyhCUgbTShjAysavHi9eB5n4x+TMsvIr2hQBqiIAyb1oE9VgVipEKnGz+r2UAnraIBtVLWvmeApWGO7WnF8GdhH6JbEu5voJWClCA9GZIjni4xvSYxD1G/+AaP8HCC1gNEnAAAAAElFTkSuQmCC") !important;\
	}\
	A.ch, A.sm, SPAN A.fl, A.nt, A.qs, A.qs_n {\
		display				: none;\
		margin				: 0;\
		padding				: 1px;\
		width				: 16px;\
		height				: 16px;\
		min-width			: 16px !important;\
		min-height			: 16px !important;\
		opacity				: 0.65;\
		background-repeat	: no-repeat;\
		background-position	: center center;\
		border				: 1px solid transparent;\
	}\
	SPAN A.fl {\
		background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA0pJREFUOE9dUn1MzHEYf/gjWtMifyDNmZeMtfIy/NfUphdpRpmxsam8ZCjdauQqMjJFnWQ2Lt1JonfRqWSuu1Py1paX7FRoFedyl6Ou38vH9/dLM57t2ef7/ey7z/N5nu8zCQCdLyihj9195OY+hcjNjejnDxIFkWiqO5HA0fHUveTlNY2kt5MnT6K/wc45+cWk1tSgodEkp7HlCdqMZugbjXJWVD9EzHYleJ4nl8tFEk6kIBVJTM0hfYMZ+D6EIc1V9ERvxmibGT174+E4uA+jnAshobGsOGQH/yelnCikqrpmDCQl4enCBWhfsgSdm6JgSU+HqfgWyqr0skCL+VXQo5bnfqbWDh9zawc9Nj73tzucRMpjF2QBKSzpKrzbGgNbRhoM64Ix0VrjQ/M/rdU3tKBOb5Rbo+SUc6RvMqI/+zQ65vni7cZIdC5QoDpgJeTW/sSrr0DZO+HPjZdRckZphbdlgb4tm1ChOgWzrwL9++LQ/vkLJGdsYHC5RhCS2wva9RrPeu3gftllPixyDyhVWy8L1Aetx4rLodgY4Al0fYbhVjkqKxogskrtH+wIyOrFqtM9SC4fZIwToigwgf2gLPUNWeBo5g4kW7dDcdcHadHhuDvFE3cqHkEQRaSU9iFKMwD9awfmHOmCzTHMBESsj2AOcnLHBeZFeiP8RSBW1/rK54tn1JB4MA9L0y3IabJi8JsTM5PeQ2voZ6yI4DA2g/gEFd2rb0axugiLbnjCX6lAW00rCi5rYGgywWIHZhzqxKxj3ZiVasHUw+8RfeUT0+WxLjQe9OLlG8q/UjqSm10kLktUYM3L2VgbFAKPYHfEKqOQqbVi0Xkb6rrtePDpB7KarZiR2AXrKIewiLjxzeL5MeIFgQryLlbOzCD4lXrBX+ODpUUeoFB/7MkzsYpOcA4rhh3f4HGwCyfu27Bz2wGQIPDEJsqQIwiYm6nKHJ6f5I3l5XPhp/XC4oTpqK0uZ9/GwTZkw+iv79AZBnHzqRMbwtkQ2TSZC1FGjmciwOzHlcYy7xg3KCJ8odx/FhpdFfIKrkNdqEX+JR0013TQFZdAdfLSeAuSAwklN2PcGFntg5Sw+wBcw1wJ4wMZHyjhRLLdCGQ7Kd9/AxdCsy6Zkjf5AAAAAElFTkSuQmCC") !important;\
	}\
	A.ch {\
		background-image	: url("data:image/gif;base64,R0lGODlhEAAQAPcAAP//////+v3//P///v7+/v7//P/+/f//7P39//79/v3+/fz+///9+f3+/Pz+9/79/Pr+/vv9+v799fv/7/v+9fv9/vr9/vv9+fv9/Pz9/Pn8+fn+8fr+5/T/9f/7/f379v77/vf9+fb8/vr79/f7+ff7/v76+//5//j6/P/4+/j5//b6/fr68/35+vX5+vv49/X5/Pv3/vz3+fr45vf3+Pj49/T4+v744f334vX1+vT29Pn09/713/X19ev46ur46u319+f35/D22PD21v3yrOnw+e/v7/3xsuXy5OTw+P/q6P7q5+Hv+Prtvv7o5f3xkuLu99bx1+bq7/rpuPXrqtjp8tPo9+7kx/bjverntvXni9fk8fPkovHf2+7lovfjcfjicMLqw8/g8MDpwMDowfPbrP7Uzbvovc3e7sXf8bjnuNnZ4/LYmczb5fPVkMDgwbPltPTUivbXYbHhs/3EvfHMirzT6fzBusHP6LLS66rZqbnP5rfP563P9Zzdna7P6fu8tfu6tLPK2JnZk+W+uf24rJHak/23rJDYkKTJ56TI5sHCzbzA5LfC1rbB0YjTiIHUgpbE5p/B46DB4q25z5DA45i623TQd3LPc3bOc4y92qO02oS23uSdkYWy0uWajX+sznGt0/uLfX2kzIGh32qn2GOn2fiEdvt9anmX0vN3Ze54anuR0V+Xz1Ob0WeR21yUzYSKtXSJ2vtuW1eQyHyEwHGD33CE0nODyvFlUGeA0/ZhTEiGxDCLyyyHyfZXQvpVPzx/xRt7xRt7xi5ytTlqzCBwvkRhixlotxBlujBaww9ktzVZqAxhtQtgtABKtQBCkiArnxAsuv///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAANMALAAAAAAQABAAAAj4AKcJFHjnlKo6bJoMXDgt0K9dnQhd8aJFzhSGjXJ96vJChIgRM6h84TJQka9FOmBsuqUrVR8gQgaFESgMlIs0ymjtsdMKWqwYZzBN++OqTZFkozhsuAACliMJPhCp4eTJBilnRiaE6CCjgoMBA+AYCiUIQzBiLDyoXas2CiRIbxYUa9bjg4W7dwEAqKLJjx4Nr55ZUkFADK9lyGhYqFSKzCMkVqQtQ/NgR45JzGowMZVn2qU5FBhFW8ZniyRjx0hEGiZwTKYhAvDUwiXLFispa3olGpgFzJEDAYIHWFJoFSWGWOQ8IcLDzCFUwAAxHFjGTZxZougwDAgAOw==") !important;\
	}\
	A.sm {\
		background-image	: url("data:image/gif;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAALCAIAAAD5gJpuAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAMhJREFUKFNjbFsf+u/fv79///758+fvn99AEox///7zd3reCQZM0LI26D82kNprjFWcoWGlP1aJxE4DuLheSPvVu88hXIbaZT5YNcS06iBrgOthqFzkgawhZ9IFLGj6mfN3PkD0MJTOc0bWYJ64YM25r5ho40OQKo/0uQyFs+yRNQSXb8FEOdPPAI2A2pA7zQqrH7yKFbH7IWOiOVYNbvny2EMJV3g758hij4fETn1gCIY3aAbXqPmVK3sVKboVyANVO2bJYNUAAGtdm70c5CcaAAAAAElFTkSuQmCC") !important;\
	}\
	A.nt {\
		background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAIAAACQkWg2AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAj9JREFUOE+V0N9rUnEUAPD6G4JiL0bQ6qEgKBhBb+spt4KKgohF2RI3jLWI5rUl82XUYjmCJH9FXremtTRz3c05pqHTNdtmutm05N7p0mtX3JzXXVO8J2USNSTowPfhyzmfczhnJwDs+K8oA4qiZLJ+FEWtVqvX6w2HwyRJMgxTTpUDf/zAuK9urLmZoahK9/JD0XGVytLdLeXzBQJBu0gkFom6xGLxewzLfPHHzx6mhSfH63djXG4VKBSfrBMuqUQqvNF2jXezp2cAQfrEiOzKrUszc8HsE2SDd9zTsHeIw6kCpXIWj6zGVqNBi94yatGh2kH05RvjyEVHvcLe51/EP3S1YwfryElbFaiU03gKogVI297ieSA2AF+Dzrmm3uyFAbalTXtZ0NHhcE1vrVTZQaVyJlMlGiA38SqZhzQNvQutSIL7tHhdGDh1YGDPI9fDreoq0Gic+PfiWglyY/pcEQZDCuG3E3K29U749BE5B7He/V1dBVqt83OYITbhh3GQoOG27YwcePdWzh1Tc64aWliW3Q6Ghz3+xUyyANSILkKD/jm/0bD/kHLX0fsNvvn4n9XVCUaj1z1D0iyUzC+iifXzTY3tnaL+d9hsiHXYv9YAGLYwOklENoEc0vhieffHYJaBCAVzBJjNvhpgaipgsoQIBkidYjkD8ys/IynI5CARL6CouwbweMKG1/5KwvBscXndYg6oVXa12mUyzRNEugZYWopJJEaF0qHVTTtdoVgsVSr9dZntV9rW49/fXyLJQRtab5rFAAAAAElFTkSuQmCC") !important;\
	}\
	A.qs, A.qs_n {\
		display				: inline-block;\
		float				: left;\
		cursor				: pointer;\
	}\
	A.qs {\
			background-image	: url("data:image/gif;base64,R0lGODdhDAAMALMAAGdufoqSoI6VopOap52kr6etuLK4wLW7w8XK0dbZ3ODi5eHk5uXn6uzv8Pj4+v///ywAAAAADAAMAAAERBDISee7Lrv75HPNgiBLs0kNI14k00jKeFzHqEiJwXFGIhWD3WVQ+A0EgguSaAwELk4mwEAYWK8EgwRhKHi/BkRlLIkAADs=") !important;\
	}\
	A.qs_n {\
		background-image	: url("data:image/gif;base64,R0lGODdhDAAMALMAAIBMS7hwb7lzcbt3dsB/fsWKicuVk86Yl9erq+G/v+jRz+jR0e3Z1/Pl5fz49////ywAAAAADAAMAAAEPRDISee7OMvnuu+P1DBLaTKNpCBH6yKKlBjZZSRSMdTPUOQDgXDoAwaOyCLAQBg4nwSDBGEoWK8GRGUriQAAOw==") !important;\
	}\
	SPAN A.fl:hover, A.ch:hover, A.sm:hover, A.nt:hover, A.qs:hover, A.qs_n:hover {\
		opacity				: 1;\
		border				: 1px solid #abf;\
	}\
	#bgBar {\
		position			: absolute;\
		top					: 0;\
		left				: 0;\
		height				: 22px;\
		width				: 100%;\
		z-index				: -1;\
		background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAAWCAYAAAABxvaqAAAABGdBTUEAAK/INwWK6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAABwSURBVHjaYmBgYNAFCCAmIPEDIIBAxE+AAAIRfwACCEwABBCI+AsQQCDiP0AAgQgGgAACEwABBCYAAghMAAQQmAAIIDABEEBgAiCAQAQjQACBWQABBGYBBBCYAAggEMEEEEBgFkAAgQmAAAITAAEGACXuBkV5zlS1AAAAAElFTkSuQmCC");\
	}\
	#gbi, #gbar, #guser, #header #gb {\
		background-color	: transparent !important;\
	}\
	#gbi {\
		display				: none;\
		visibility			: visible;\
		min-width			: 115px;\
		border				: 1px solid #abd;\
		position			: absolute;\
		top					: 0;\
		margin-top			: 20px;\
		padding				: 1px;\
		z-index				: 1001;\
	}\
	#gbar {\
		position			: absolute;\
		float				: left;\
		left				: 0;\
		top					: 0;\
		z-index				: 1000;\
	}\
	#gbar, #guser {\
		top					: 1px;\
		min-height			: 18px;\
		margin				: 0;\
	}\
	#guser {\
		float				: right;\
		padding				: 0 !important;\
		margin-right		: 6pt !important;\
		margin-top			: 0px !important;\
	}\
	#gbar nobr, #guser nobr {\
		cursor				: default;\
	}\
	#gbar A, #gbi A, #guser A, #header #gb A {\
		text-decoration		: none !important;\
		white-space			: nowrap;\
		margin				: 0 !important;\
		height				: 0;\
		font-size			: 0;\
	}\
	#gbi A {\
		display				: none;\
	}\
	#gbar A[href], #gbi A[href], #guser A[href], .login,\
	#header #gb A, B.gb1\
	{\
		display				: inline-block;\
		height				: 19px;\
		font-family			: Trebuchet MS, Arial, Hevetica, sans-serif;\
		font-size			: 10pt !important;\
		font-weight			: bold;\
		border				: 1px solid transparent;\
	}\
	#guser A[href] {\
		padding-top			: 0;\
	}\
	#gbar A:hover, #gbi A:hover, #guser A:hover, .login:hover,\
	#header #gb A:hover\
	{\
		border				: 1px solid #c9d7f1;\
		background-color	: #ebeff9 !important;\
		text-decoration		: none;\
		color				: '+DB.Color.link+' !important;\
	}\
	#gbar A:visited, #gbi A:visited, #guser A:visited, .login:visited,\
	#header #gb A:visited\
	{\
		color				: '+DB.Color.link+' !important;\
	}\
	#gbar A.gb1, #gbi A.gb1, #guser A.gb1 {\
		padding-right		: 3px;\
	}\
	A.gb2[href] {\
		display				: block !important;\
		min-width			: 100px;\
		padding				: 1px 10px !important;\
	}\
	#gbar B.gb1, #gbi B.gb1, #guser B.gb1 {\
		margin				: 0px;\
		padding				: 0 4px;\
		border				: 1px solid #999;\
		border-top			: 1px solid #777;\
		border-bottom		: 1px solid #fff;\
		background-color	: #fff !important;\
		color				: #777;\
	}\
	#gbar B.gb2, #gbi B.gb2, #guser B.gb2 {\
		font-size			: 10pt;\
		color				: gray;\
	}\
	.gbh, .gbd {\
		top					: 22px;\
		height				: 0;\
		width				: 100%;\
		border-top			: 1px solid #c9d7f1;\
		font-size			: 0;\
	}\
	.gbh {\
		position			: absolute;\
	}\
	#header.fixed {\
		position			: fixed;\
		top					: 0;\
		left				: 0;\
		vertical-align		: top;\
		padding				: 0 3px;\
		width				: 100%;\
		margin				: 0;\
		min-width			: 780px;\
		min-height			: 70px;\
		background-color	: '+DB.Color.header+';\
		border-bottom		: 1px solid gray;\
		z-index				: 99;\
	}\
	#header.fixed #sft {\
		margin-top			: -10px;\
		vertical-align		: middle;\
		font-size			: 10pt;\
		width				: 100%;\
	}\
	#gfx-searchers {\
		position			: relative;\
		display				: block;\
		width				: 99%;\
		min-height			: 19px;\
		white-space			: nowrap !important;\
	}\
	#gfx-searchers * {\
		display				: inline-block;\
		clear				: left;\
		font-family			: Arial, Hevetica, sans-serif;\
		font-size			: 10pt;\
		font-weight			: bold;\
		vertical-align		: middle;\
		margin				: 0;\
		padding				: 0;\
	}\
	#gfx-searchers LABEL {\
		color				: #777 !important;\
		margin-left			: 2pt;\
		padding-left		: 18px !important;\
		min-width			: 16px;\
		min-height			: 16px;\
		background-repeat	: no-repeat;\
		background-position	: left center;\
		vertical-align		: middle !important;\
		cursor				: pointer;\
	}\
	#gfx-searchers LABEL:first-letter {\
		text-transform		: uppercase;\
	}\
	#gfx-searchers .siteList {\
		display				: inline-block;\
	}\
	#gfx-searchers A {\
		font-size			: 8pt;\
		margin-left			: 3px;\
		min-height			: 19px;\
		background-color	: transparent !important;\
		color				: #444 !important;\
	}\
	#gfx-searchers A IMG {\
		margin-left			: 1px;\
		margin-right		: 2px;\
		max-width			: 16px;\
		max-height			: 16px;\
	}\
	#gfx-searchers A:hover {\
		color				: #000 !important;\
		padding-top			: 1px;\
		margin-left			: 5px;\
		min-height			: 18px;\
		text-shadow			: 0 2px 7px #555;\
	}\
	#gfx-searchers A:hover IMG {\
		margin-left			: 0;\
		margin-right		: 1px;\
		width				: 18px;\
		height				: 18px;\
	}\
	#gfx-searchers-menu {\
		position			: relative;\
		display				: none;\
		border				: 1px solid #abd;\
		margin-top			: 0;\
		width				: 100px;\
		padding				: 1px;\
		z-index				: 1001;\
	}\
	#gfx-searchers-menu A {\
		display				: block;\
		font-size			: 10pt;\
		font-weight			: bold;\
		color				: #555 !important;\
		text-decoration		: none !important;\
		min-height			: 16px;\
		padding				: 2px;\
		padding-left		: 23px;\
		width				: 79px;\
		border				: 1px solid transparent;\
		background-repeat	: no-repeat;\
		background-position	: 2px center;\
		cursor				: pointer;\
	}\
	#gfx-searchers-menu A:first-letter {\
		text-transform		: uppercase;\
	}\
	#gfx-searchers-menu A:hover {\
		text-decoration		: none !important;\
		color				: #000 !important;\
		border				: 1px solid #c9d7f1;\
		background-color	: #ebeff9;\
	}\
	.virtual-list-icon {\
		display				: inline-block;\
		font-size			: 11px !important;\
		border				: 1px solid #999;\
		color				: #000;\
		background-color	: #fff;\
		padding				: 3px 2px 2px 2px;\
		margin-top			: 0;\
		margin-left			: -6px !important;\
		cursor				: default;\
	}\
	SPAN.virtual-list-icon:hover {\
		color				: #fff !important;\
		background-color	: #000 !important;\
	}\
	#virtual-list {\
		position			: absolute;\
		display				: block !important;\
		overflow-y			: auto;\
		overflow-x			: hidden;\
		margin				: 0;\
		margin-top			: 2px;\
		padding				: 0;\
		max-height			: 130px;\
		border				: 1px solid #333;\
		background-color	: white;\
		z-index				: 9010;\
	}\
	#virtual-list SPAN {\
		display				: block !important;\
		margin				: 0;\
		padding				: 2px;\
		padding-left		: 3px;\
		padding-right		: 5px;\
		font-family			: Arial,Hevetica,sans-serif;\
		font-size			: 9pt;\
		color				: #000;\
		text-align			: right;\
	}\
	#virtual-list SPAN:hover {\
		background-color	: #33f !important;\
		color				: white;\
	}\
	#virtual-list SPAN.selected {\
		background-color	: #777;\
		color				: white;\
	}\
	#colorpicker {\
		position			: absolute;\
		display				: block;\
		margin-top			: 2px;\
		margin-left			: -30px;\
		z-index				: 9010;\
	}\
	#colorpicker SPAN {\
		display				: inline-block;\
		min-width			: 24px;\
		min-height			: 24px;\
		border				: 2px solid #000;\
		margin-left			: 2px;\
	}\
	#colorpicker IMG {\
		width				: 100px;\
		height				: 192px;\
		padding				: 7px;\
		background-color	: #888;\
		border				: 3px solid #333;\
		cursor				: crosshair;\
	}\
	.tip {\
		padding				: 1px 5px;\
		background-color	: #fdfdfd;\
		border				: 1px solid #99a;\
		opacity				: .9;\
	}\
	.tip-title {\
		font-family			: "Trebuchet MS", Georgia, serif;\
		color				: #10c;\
		font-size			: 10pt;\
		font-weight			: bold;\
	}\
	.tip-text {\
		font-family			: Arial, Hevetica, sans-serif;\
		color				: #003;\
		font-size			: 8pt;\
		padding				: 5px;\
	}\
	#SABox-Box {\
		padding-bottom: 24px;\
		background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAe4AAAAYCAYAAAA8qx7mAAAABmJLR0QA/wD/AP+gvaeTAAAABGdBTUEAAK/INwWK6QAAA+xJREFUeNrsnb9u01AUh2/SAKUpolJVKhaeoFvFwAQVE0jwBkh0oQMvgFgoD8GKgA1GMqKqMAFDpSKYWJDYqrYKEpAmjf/gS+8tJ6f3OmnJ5u+Tjuy4jh05w5ffOW5Sy/PcWGoFxWLC1WlXZ4o6W9RUUdM/2u03jUajaQAAAGBspGm6d35m5nqx+quoTlF7RfWK2neV2iqcndcjx8hD23q9XpvLCwAAMF663e5uzL16Q71kh1ws/67v7O5+5fICAACMF+HXXPn3iJ/rQ4418KRPm5sfuLwAAADjxfk1L0vanpqacVuR2xn3KTM447bVnJubm/ny+fPzZrN5kcsMAADw//zudLYWFhbubG9v/7APzcF8W864++Zgxp0Nm3H7ynwVB91vtVovuMwAAADjofX69TPrV+lb5eGREndDpO5Jn7h9vXv79v7i4uItLjcAAMDJ2djYaF29du2JS9q/ReLuirSdlCXuPFDe/omvGzdvPl1fX3+ZZVnCZQcAADge1p/Wo9an0q+RxD2QvEOt8iMtcmf61B+40+n0bt2+/eru8vKDra2tb7wFAAAAo2G9eW9l5aH1qPWpkLZ3bahlfohslXuRh9rl8ia1KVd2fbLg7OPV1StLS0uXZ2dnL8zPz1/ibQEAABiQ9fd2u72ztrb28dHq6vtut+vb4XbZMf++dEXflHbYJv/182fWnJ4+Im455/Z3l59y4pby9jXptp8W+064Y9REAQAAVAk9ck6diPtOyj0h7j0l7Z7YNxXizq24G0NOljoJJ07IfSVmo15UIqSvxY3AAQCgCsIOudQ7Uotbp2vZMg/eWd4oOXHmZOtPVldJWu6XBBJ3HXEDAEDFxZ0FEndfJOueibTGzb8Ztxkm7jxywiQi7VRIuyEStxY38gYAgCpIW4tbJ26fuvfN4A+JhKQ9NHHnTq7+ZMYdJJbIU5G2G0LcOnEjbAAAqJLAYwFYy1suZeLOZeK28+2yxK2jvmyZx8TdF0lbi5ukDQAAVUzeWtw6eevl0H8Fi4k7N0fb4frF6E8QMWkz4wYAgKpKO/S9KIkSeCLWMyHvQ2HLtF2WuI2yfFYibi1sL+26EjbiBgCAKonbCAlnkfSdKmGXfk95mbjlrNuok+dOylLcWtih2TbiBgCAqog7lLp16zwz8R8WyUNpe5TEXVOfHnQL3Qs8JGySNgAAkLzLBa63mzJpDxO3icT0TAg5M/F5NmkbAABI3fG5d+hvpkzao4g7lL7lukzktcgSeQMAQBWlrZO3iYh6JGEfV9xGCTrUPo/JGWEDAEDVBR5K4uY4wj6JuE3gk8Hfx9PnztVKXigAAACcQNIh/ggwAHcLSjUeqy03AAAAAElFTkSuQmCC") !important;\
		background-repeat	: no-repeat;\
		background-position	: bottom left;\
	}\
	#SABox-InBox {\
		padding-top: 15px;\
		background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAe4AAAAPCAYAAAAieCzFAAAABmJLR0QA/wD/AP+gvaeTAAAABGdBTUEAAK/INwWK6QAAAj9JREFUeNrs27GOEkEcgHFmZzYRTIAQghaE2BgrbY6EEHobC7TwFez0BdTqfALtLHgFQ+EzEBKsrIwJT0AIUF0iy7oDu5e5yQwsuo3e90s2O7fccslu8d3/bhFxHJcAAMC/QZlfCCFuvJg36sI+EQAAOMU54urKanaaOvcHOiItPOtjxwAA+K/77DuWdjQ+N+bOifuMYAtjLxzHXMEm4ACA2xbs+Mj+ep01Nk/Ac4XbiLZwRDsw1q7vI9oAgNsa79gRa73tjHU2ge8Dfire6oxglxyhDnq9Xjgej5/V6/UXUsqHSqnHyXHJfQMAYC/abrffoyj6uVqtvgyHw6/T6fSXFe+d0V7f9H543Qy79e9re3o2J+z9NplMHnW73c9JrC+4LwAAnJZE/NtsNnvV7/d/pMHeWRG3p/TDIu21L9yuaGfB1tN0sFgs3jQajXfJOXe4DQAA5Je092q5XH5oNpsf9UR+JODX8c4TbjvaMt2r9Xp9Wa1WX3PpAQD4c5vN5lOtVnuvB/E02pFv+s56HTjex36oLIu23tR8Pn9JtAEA+Hu6p7qrpcMzZ9IYkr0PeAee93L+iXwwGNztdDqXXGoAAIrRbrff6r4a0XZ9Wssbbvvp8Wza1r8JhKPR6LmU8j6XGQCAYoRh+ED3VS+NyTuwJm9xbOL2PZCmWq3WUy4xAADFSvuq8kzdvs9xOyfucrn8hMsLAECxKpXKRTpxu54uv+HUxC2NcKtknG9yeQEAKFbS13tZa432Oifu3wIMAMytqJfzQp0bAAAAD3RFWHRDb21tZW50AFByZXZlZCF226z+AAAAAElFTkSuQmCC") !important;\
		background-repeat	: no-repeat;\
		background-position	: top left;\
	}\
	#SABox-BoxContent {\
		padding: 0px 20px;\
		background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAe4AAABSCAYAAACSaDfsAAAABmJLR0QA/wD/AP+gvaeTAAAABGdBTUEAAK/INwWK6QAAAdxJREFUeNrs2t1Kw0AQgNHOpio+ie//OD6J+Bsn0EJcd2MuugHhHBgSStKL3HxM2pjn+bSIiOVQLjPlnHPucu5zHnIe89rnEwBwc9nhpzy85LzmvOW853zkfOZ8LbM0u6zvqc6jivjZYwWAYa6tnS79jUabf4S7Fe31Bj55pgAwzLSKdunE+1e4ozHXm23cADB2466DHXW8y8YX1Ft38UwBYJhSbdzRu6gX7dYrcwBgXLijsXH/Ge6t1+UAwLhwb74mP+2IcXTOAYDb2mpu9zfu1k0h2gBwWLxja2EuO6pv4waAYzfu7mdlx6pu4waAYzfu7mtzfzgDgH9EuAFAuAEA4QYA4QYAhBsAEG4AEG4AQLgBAOEGAOEGAIQbABBuABBuAEC4AQDhBgDhBgCEGwAQbgAQbgBAuAEA4QYA4QYAhBsAEG4AEG4AQLgBAOEGAOEGAIQbABBuABBuAEC4AQDhBgDhBgCEGwAQbgAQbgBAuAEA4QYA4QYAhBsAEG4AEG4AQLgBAOEGAOEGAIQbABBuABBuAEC4AQDhBgDhBgCEGwAQbgAQbgBAuAEA4QYA4QYAhBsAEG4AEG4AQLgBAOEGAOEGAIQbABBuABBuAEC4AQDhBgDhBgCO8S3AAAIeE0W7GnIWAAAAAElFTkSuQmCC") !important;\
		background-repeat	: repeat-y;\
		background-position	: top left;\
	}\
	#SABox-BoxContenedor {\
		padding: 0px 0px 5px 55px;\
		text-align: left;\
		font-family:Verdana;\
		font-size:12px;\
		color:#000;\
	}\
	#SABox-BoxContenedor h1,#SABox-BoxContenedor h2,#SABox-BoxContenedor h3,#SABox-BoxContenedor b,#SABox-BoxContenedor strong{\
		text-align: left;\
		font-family:Verdana;\
		font-size:12px;\
		color:#000;\
		font-weight:bold;\
		margin:0px;\
		padding:0px;\
	}\
	#SABox-Buttons{\
		text-align:right;\
	}\
	#SABox-Buttons input {\
		margin: 10px 10px;\
	}\
	#SABox-BoxContenedor #BoxPromptInput{\
		margin-top:5px;\
	}\
	#SABox-Box .BoxAlert {\
		background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAMAAABg3Am1AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAOpQTFRF+qIvEhMS9oAI+I8Z94cQ5q1/+IoT/PXv+ZEb+evf9nwE2oVA/Kw694MM9XwE1GgQ89a/9YgT3WkC7cKf9XsC4Jlg13Ig8oMQ+6g24m4E+ZQfTTod1moQ7XwM53UIh2Eo1HAg02AA53IEW0Qgwogz7HkI35w5+Zsn2mUA3Y9QIR0V1WEA9YUQ0JI2+IwV4mwC+JEbs34w6beP8oAM9n4G7aU78Myv32oC+Zcj+p4r9uDPLycY+6Uy5HAE3WcA6ncIPjAa53MG94UO94oT7XoKlmsr9oEKak4j5KRw2GMA+I0X/K8+9XkA////SUCmvwAAAkJJREFUeNqUlulW4jAYQLPQFZAutuwuICqg6Dg6OquzzzTpvP/rTJo0IUUo8fvTlJPL7Wm45wD+vXLA1k8/NJfLuWcOOJ2fhMxSxxhIvmHfx58tU8A7Jfd5/tseOYZA8mznbMjkrxngZa5fAIicekbAfMwFTHGUmABBhn0BIJJ5BkBz3MjLsWfz/UBwiQ8l4OMs2As0u1yADrhi3NwHBJHLBQBwhXsZ7AFOhIBQyq+Nt816II4wf/oepVxxiKN3tUDabZcCqeie1AFxXwko7XGF+xjXAGnIBQPKhxTr9lW6G7D6brHnoCUArsjxzd0uwOmESBNQessVYcfZAVh9UgrOh2wobfHTc/vWdsAZrbjgiX03hPAjuwz4oVdK0gDriAtQ8TALCJmhVJCJtQ1gYXLBRQEMIZwW16cXJa2B5GilBHQK4TFfoM2SFOAtia8E9FgCF8Vn91pJCkhmPMyGeKG/IDyja4U9SzYBWT4Q284U8H6jJKDC1AX0GsLzctmolgSq5QN5yBDKlSgJy5JApfwfchf9ulDLakkCCL6I8ntq18MftfxUKQno5RO6dYheEhBhuhsCdXBaSVGsgLL8W7XlDfvxqddULakA4kesdyMOWh2E+g3im7gE0qt2pRt+DHDxsL4frEsCqnxNwJ5pOL3WbrWSQBHmpuDliJJCVhKQ5aO6/bSFZElAlo9A7Yg9ZOQBa0Jy42ElgY6NzAG0GoHsFQKmyJihbb7/e5gCK7KJ8YTRHXutncx4UmvHv5ma+S/AAMZeScTXQU/VAAAAAElFTkSuQmCC") top left no-repeat;\
	}\
	#SABox-Box .BoxConfirm,\
	#SABox-Box .BoxPrompt {\
		background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAMAAABg3Am1AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAArVQTFRF+/v7/f39+vr57+/v+fn4iIqF+fn5/v7+9fX1+vr6///+xMXC9vb19/f2wcK/2trZzc7M8fHv8/Px9fXz9PTz7+/t6urp/Pz7wMK/cp/PnbzdxNfru7y61tfU+Pj48PDwsLGv4uLi5eXk8PDv09PS7u7utbazx9ns4eLh7Ozs8fHxj5GN29vby8zKxcXEpqekl5mU6+zs0tPRu726ssri+/v63t7e09TS1NTTv8C/1tfVwcLAuMvftri0jo+Ltre0s7Sx3d3c3ujxxsfErK2q7O3svdHl0uDv29vanqCcz8/P9fX0t7i18fP0oaOg8/Pzl5iUlZaS5ufn5+fnrcfinJ2a097qwNHi4ODg6enpw8TC6+zrra+rsMfe5Orv4ODf6urqr7Gty9vt8PDu6erqssrj6erp2OPv5Ovz6O3z8vLyvL26i6/W1NXTtMvktMzlia7V6uvqlbbalLba2OPu6u/11tfW1dbU4uny7/DvuM7kz9DOz9DNtc3m3eXt4uvyzM3L5Oz19/f36/D19ff5w8XCw8PB19/n5+fmvb685+joyMnHkrPXz9DPsbKv2eTwnZ+b1tbV3+To5u312tvatLay8vPyz8/Ntba00NHPuLm26/H3ztvpg6rUwNDh2+Prrsfi8vT29fb2l5mVuMzgyNjos8vkzd3trcbhv9Ln39/dzs7N3d7d7vL1vdLmssng2uXx2OTw0tPQ3eXu6eno9vf3q62pxsfF6e/13N3csbKu3d3b6Ojn6vD22OPwlpiU7/L2gqnTuru59PTy7/P4s7Wy7/P37vDw29zax9nr8vX54+Pik7XZpqik7/L1ra2qvr+81uHt8fT23ufu4OHg4Ojxuc3hia7U8vLx5+rreqTRtbWz5ufl8vT1o7/c2tra/Pz85eXl7u7s8/Py2dnZ+Pj3////vrxIrgAAA4FJREFUeNq0lvd/00YYhy0bL+zEQGpcSbGNt2PjQeyQQXYCCXuEWVbponSXLuhmdLD3pmVvKLMtm7ZAF2UV6B6+WHd/Bxp3cuVBP+2nfX6wv6/0PpZ1ku6kQv8Q1b8RPKe6ZVHT/17CVMva7K3mJkuqoDChCVXALIxo6hVjTyWDiVCpy+mHUDd7TV2xkkSDWxJqUG4/dDc8j1JK9IbiiChYKNy0fUZzc/MhMaZPeyouprOAKNFTIQTYwB6WnS7EryOpdC4696cK4fw2CK8F2Hl8PGbX5RFK0P2S0CXDxwH2M/6rnAhfjR2QTA54ExtY0KtlvnyIPSp8e4mwi9mbTDLMWKVgJLzEBlaJodKuKxG5NYX/2Mw8KlVEUBHa2HNSqLSrDRk+Yl6VAhHkoW5jcaiyVxQRhu5mRg+VIha6ysK3l3B42m4jl/D30cw+krGAKAHzuxe0WXxyhh+wWcxWeQyJoOc5O8IEs29MS/x1tfot5hl5ECWhm7hzYLntp04lVgjj+23zHz9pI0hC02PCj8Uoa2cO0HI9NYd5Vj5HUdBNfI1/vN4xwdz+zofRCP2mN77RE0QhpJm4oua3IaYi3PRe7eTJtStxgbSKk3L3o5BKwzPh9mUi1NL0j5/T9Id5hcTLj0iCRlNqKrKKjBv3s9W6k35OqhSCIdEPgJDKiQXYPcMP9GIpIG2Lh/S3N3oBAKNUc7HQpYfMYfqXLVJCWm/jBrUkeL4AAir0hCSo7yP8QT81BUekBcFFdW7JWBqUBP1wUTCaMcfpD0g08wIAq4e0iAeJLOGLhcLM53DygorcYTfp7+TJQxQAePvXdnFQ+WgQ51a9Y4EppcN8/+IdEnVYAAdvFHuQehoAPjIZP2CiyFO0jH5BfqKIAIC30Y5avE6UEfQU5gg9k0QqI4BgtK54XWa6L38Sdc0FnVguC6N875c2ZIT0SJhvqp60/s8+PL6QwSiUrwyWBVfpQGdOO/SP4Thf3hXIwXHV9ZNMIvKhLOFWjuP65hXK+D1ctJdA68gI3hUcJmzljAUFzKBhMfzvwlVC7cgnDOf+SnW9Sdzqr+aL3vlX0b4Ko6o1HhTG+iqfy/ILjt4Kg4vu8EME6zluPFVgnc42uF5hC4pFXVTBhZ3qyDIGjYn5N97zTcD4oPIo4w+E43/36uDwdbhc/MVyucpCRuFSO/+bd43/V7grwACdKHnFGscLRwAAAABJRU5ErkJggg==") top left no-repeat;\
	}\
	#SABox-Box .BoxInfo {\
		background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAMAAABg3Am1AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAwBQTFRFcZvReZ3D8vX6RXKsrsXcpr/YrMTiscbc3MZP0rY9mrXSuZoNvtDjdprC3ejyiqzW2uXv1LpJXY3K2OTw5uz0/e50oLrV89054+ry7PL5zNrp6O710t3r/epQhabI5OzzqMDZMTY0s7W2tcndk7HP6vD20N7sdZ3PnrjU//OJyagSZoy7bHB24Or01uLy3ObvwNLk/fz1/OlPzdzqvM/ilrLQw6MLlbTdrMLaiajKcWk8uMvf/v35yNjopL3W49GI/uxSxdbn/+1Wxtbmzt3swdHiqMDbgaLHurxraZbN/etcs8je+eRGYI7KZZPMkK7NpsHihqra/v7/xNTkfZ2jsbt11eLv28Vn//ziwtTlo7/jb5nQ//J5fqTWnrra7tUywtLk/+1M9/Ped5/Sj63N+fv9xNXmbJjOxNbmmbfd4uz15OvzVH6z7fL3bpK+orzW5Mkl2OPx/va51N/sydfm9uBDfqDFhafVrcTb4M5907MR1+Lt5+304cUi5Oz0uc3htsvg/OlJu83gkKeQ5Ov39vn81N7s+PTjdJfA1uPvr8bhytrpPGunWoO1Z5TNjq/X9N5BjavOza4h/OhM/v79/P3+5+730d/tytrqytnpcJa/vdHq+uZLu87jus3kus3jk7LdlrPXiKvY6e/12bsW4Ojy4OjwfaPWfaLRxtfnw9XsTXmv/+tM5e726u/25u332uXzwdPmzq0L29VgXInHoLfK4Ofxen5/pKSd+fn59+dY7e7u+/z+0LMzt8zo274al7ff/OpN5O33z97w5O713ef06t6n/OpV3MAc48MXx9Xl8+a27eOz89cq7dQtXGJmYV1E38x9VITAxNLjV4jJi6rL1eHu1uHs175U58so6Mwl6c8r/fCC/vKTyqcGzasH8/P1ytnr/OhH386D/vjM4ctmk7LXm7ngwNLol7PQh6fLfaTYdJaidpmnfZuukbHYlLPceqLTeaLUuc7p/v38/f7/9ONRfYGDf56cYI/LYpHNyNjuZ5TK5daVtsvn////3jeFKQAABDRJREFUeNpi+I8FtK7X1W2R3INN6j8DhgjTbPPYPl9fP4ZFAXpEaGhli/6pvtCNhaUijv+PX+xhQhr0kn/2N1lbaJiacrF6J7CvEDrAhFfDqp8rmpRVGDs6eBkZeRkt84NZ+pZL4tGw6uezBC7GWe5c7MHBrhYqZYyWygnqnyRxatD7udDVsoMxv7hGPjs7Wz7b+WWZO2uJemwqDg2tDOWX5XjL2OXlJSQkgDqypfXF5NwVgv2icGhg83RV4S3TqMkGq5YH2iIt7G8pZ832awNWDUzRcRWMZe5SIkD1IjUiQFAjX+N91dK/JPlAEDYNsz1NrvK6ywmIyIvU1NRIA0FNzeLgFP9X7F3pklg0TF3Qr8xrWqFyrUYEpHoxEADJBFMVf6609N9YNDAxuOVflclualIFqlunCgE1FVxcpv6n+J9j0XAo3TvFeymfs1h1bq7qOqD5qqpMTFJ2+RpcKk2ZgVg0bPCc6++8dOlKLpZikW3FMjIybfoy3hUsrPkaL73Zlqdi0ZBu4i+1TYJHg3d1Hvu1OpHsQpMO5pyJFaxiXOxdsVg0HE2f+7JEWsBiVk+ehrdB2sqVjjwNFsy9DQp2Gk0BgVMxNUgydLHaSXmLla224DExiHRcudJ5Jc9q9wYL1nzjRQFY/BB0gD845SUXqwazv4FBhu0MY+MZamLMLA3srE1p0brYIu5wX6SFihgri3bexkhbY7Xp06d3luUY1LOzXOs3x5o0pgYukhHTUGAXE9u40nh6mLCwsFTOq5VzvWe+iG7BnvgKfecFiymw1BtEznQJE+Dj053fY2FrwqP/MQBXfjigXszOytLAk9atVsiXdShrYq/tShOBruhWXBqm+XVJKYA1TBfWXTstq9e9M81Wn+Ef7iwaFX29oQKsAWjD2pnMrNM3Fm/sm4Zbw3rPuWkVDSYQPxwxzXHp3igQtwhfqRHbX13hPd8AHEq62nnXHNOy1QPwafgcXc3DUs+TNtNleqfBag2pjdUPfY/i0yDpF7fNxLveIM12RhgXs4Fj8VL19/hLvn1XupYVpxlsdLQ15s3rlNjwOpmJQNm69VLgRgFggSGialziEuX7jolQYXza4+2V6PIF5uYfFiVveePBYUNIg/3JXZtDm0NCQn40h27+dvbpFAIavnCec9KMSVwDBImJiT6b7AkV94ac8Qfv3DGCAofbew0JOany/gQjr0aYhlpBQk76H17gVBQhCtEhO+HeX0JOstkr7vDEUPQRRMOcEzcI27B/znFRuJNuEuGHKp+7RV6ToRqWCBJ0Eqe4V8R/qJOMHMQnJRHUcMshRvOgLBA4ODjJFkxqJ+SkM5OWFIj7+JSWlorH11ZpHSPcEmi/yMnNbQYE3NyCj5OIaTr8/79D0crK6oLi96/EtTX+/3+gowgE53f/J1bD/y9K27cr7fxPvAZ8ACDAAGs0fKq7KL/qAAAAAElFTkSuQmCC") top left no-repeat;\
	}\
	#SABox-Box .BoxError {\
		background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAMAAABg3Am1AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAblQTFRFvgAAvQAA0S0tvAAAxAAA6ZaWzi0t0i0tuwAA+e/vuAAAtQAAxQAArRAQuQAAugAAhQAAsQAAu0BA14+P3Z+frQAACgAAewAAtTAwyyIi7s/P0X9/rhAQtwAAyR8fsiAgrgAArwAAf39/swAAyRcXzS0tySIiuw4O9N/fzHBwn5+fZwAAtgAAKQAAzCoqQEBAEBAQqwAAMDAwzCIisgAAICAgxwsLcHBwz8/PyAsLsSAgtAAAv7+/qgAAvAsLr6+vzCcnxmBgrAAAzCUlxQYGsAYG7+/vuAMDuwsLyCIisgMDvRERyyUlMwAAxBwcrwMDzScn4q+v39/fwVBQkAAAzSIiUFBQwxQUXAAAwxcX3mJiSAAAxxQUxxcXxw4OxRcXyxcXxxwcPgAArBAQj4+PqAAAsAAAwhcXvQ4O3WJiuQYGp0BAyBERvwsL2UdHyxwcnQAAyx8fxhcXwAYGhwAAyhwcyhkZyxkZyh8fxgYGnAAAxx8fyhcXYGBg6paWGhAQxBcXyBcXtlBQzioqxA4OvgsLyAgI0C0tvwAA0y0tzy0twAAAwgAAwwAAwQAA1C0tAAAA1S0t////pzS0agAAAtNJREFUeNqM1vV/onAYwHGdHgoeKOI8z5w1u9Z567h1x617t+vuLsFtgH/xISEyEH3/hs/zmS8c+05N8aaoLWaFmxiwNWaLysYa6SWIOeYjZ60jYQAIj7SeReYdGKgSgC53xBsmDCTNIg1E2Btxu8BqAQafDxI4LYETg+cwphiAy2PNAEnLkEDz2DIoD5y+6TROK8LT0z7nzcAJbyr9eOFNNmGnNHDCcwStgpgTCi4Afeuq+0yx7gMrAuskQNcATFrFAJtKk7UCMj2FCQEIH+B0TfgBDPKB6x5A1wG45+IC0O0l6wlIrxtkA+yCoOtCXGBs4NjG6wvwbUcpiKIzdJ1m0CgT2CaIegNiwsYEQ08MSneopOHPEBNYQzipAq9g+GFlAvcON3n69VZ1fLHjZoImL3fxoKiCDz41MkFjcx2BgXO/HDAXD2sEDQwu2ONq1aCB870U+F42VPPvpr2e0sf6m1AYSRC8969KH2sMufzb3d29v/9udVWcyf169uEaipUejWD+tqij4/Pb1gVmDkgsvNnV6/WattKjAfbr9DKjHY9DoRd3WaHQ7ugVy9gPso+3+bq8eKWmgLCPd9HWppMPLxUYURv3J3pkLlxWdV2Wh45A/hBAjdKRooL51CUcMz2QVjJSpIV6hGOmuJhD8oUq8jwN1L8oHpWPTpkir0YD+X+KR2Wn3dIGacShAgi1fIsLQSdF2V8nIdmSVuRJWuwUFeeCLopht6AenbYKowct7VNZLjimuMLvQbRanQIk6Wf3KaqPDfoozsZJb9JcuWhkIabgyQa/ssTdQwt/mbI09pogYyUzZAoOW1L8Qid/0+NCQR2uDAdNJg+EmBkI5DH1BnMrh8L0i/h7CGSFF2cTz3N+1MRC/bmPiVlh0j5Q+V90PNAuDKjUVuIOK7GVKr/YEpd9E+gKZChl2bXjpSrfZvrigUwmK662Z9YCA12Slf8CDABsANFro6KJwgAAAABJRU5ErkJggg==") top left no-repeat;\
	}\
	BUTTON,\
	BUTTON[type="submit"],\
	INPUT#stxemailsend,\
	INPUT[type="submit"],\
	INPUT[value="Cancel"],\
	INPUT[value="Discard"],\
	INPUT[value="Download"],\
	INPUT[value="Save"]\
	{'
		+ CSS.cmn.btns + ';\
		cursor				: pointer;\
	}\
	BUTTON:hover,\
	BUTTON[type="submit"]:hover,\
	INPUT[type="submit"]:hover\
	{'
		+ CSS.cmn.btnsOv + '\
	}\
	' +
	(GFX.bMnuIcn ? '\
	#gbar A, #gbi A {\
		font-family			: Trebuchet MS, Arial, Hevetica, sans-serif;\
		padding-left		: 18px !important;\
		min-width			: 16px;\
		min-height			: 16px;\
		background-repeat	: no-repeat !important;\
		background-position	: left center !important;\
	}\
	#gbi A {\
		padding-left		: 20px !important;\
	}\
	#header .alerts, #gbar .alerts { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAwRJREFUOE9dU21Ik1EUPnun9En03Z++oPoVSERElH8qosKigkIqoqAM6QtDqCipyNAyQ40i3YZIGgiWi2aS6PyatbapKypwpeKkdK6W4te29+vp3rsW1guHc+59z/Pc59x7jgEAcTMYDKTrIEnSiQikahIZJUnE/B//FEWhhIQEsY5jRMBN13XhwzJIYUSATrqmkKap5PP5Zvj9/jnxvHgu9wKkaSxZ12goFCHTvcNZJTnHXJ+6IxRVsYb/r6l5tbS4uDjTZrMVapomDouTiIBvgvnB4ASV52/sbnxAKMk9gNIHmd7O9g+7AoHBhVZrdZ/FYplmNpsreb6qqgL7hyAmNxwFmfLT3Wg3AN6ZmPy8C7aiDfA425MnJkbJWm1VKyoq5nZ1dZm4Mk4iSlAUVfgOb3dSq0WC3kmQm9iWYxbw7SDKs9eh0zuwvN/vW1FXV6cGAoGDTMUSrkSw8CAYDMxtc37a7366GGgnqA4j9Fbmncsg96Yg/+pmeLvk5eHwGFOrctwiUUI0GhWnNzc3JDk9XzLsVWk/tTaCxgi0NgO0lhjJpIdQeHs7XB39ySx/ffxFhAJZlsnlerfK7enIePempbmnmt2pi5XCiPCW3cd7wmibEV9rCNX35+PureMOn697iyiBMzmdznlms6Wgvv7Vx+9DKtU+XhYD2Y0Itkp4fNmAszsI6ckSclMID9MIL2ues4djJXCWUCg0y2QyldleWku9H/s2NVRdGel5QXiSl4jzuw3IYuBrOwn3T67E6W3TUXY9Cf6BEWhKJNaeXIXb7S5stDdkBH+ExpzNz2Y7KggXUiTc2UPIOyQhe68RN/YRbp5ZgFp7E8KTkSOsS3P+aePx8bGtnKznayPlX5RwdWsCClMJRcwenSJkZ25EQ6MDkYhyTlXlWCNNnQXe93zzjaNqdUkmAx4lFKQlIu/SWpRaCuDrHY6ykv+CdX6JUwcj3ue/hkdXvG2tRH2tCa/tHvi/T0IHTkCXxZ3FZ+EfBf9PJVuzNhSWyoBJqsLBsSGaeuhvzR+nban4jnYAAAAASUVORK5CYII=") !important;}\
	#header .blogsearch, #gbar .blogsearch { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAvNJREFUOE9tU2tIk2EU/mbaP0EW0Y8QQX/M/klieFlaKaQwE1NKzX6I5QhN6AIrtqCGJaTOKV3UUWmgP1ravJTGNjdbuGlzpjjnBZ02NVEo1jaduO3pez/6hlkHDi+cc97nPOfGCQQCFBHyEvX5fFRY2EEqNPQAYyfi9/upkJAQisPhUACY9y/n7u4u49irbrcnpO+95pDyjSqc2AkwSbA/LmjY2nJTw0Zj1D2JNLOw8MrL43GnVtIzcnD7lgT9H/pL6Y9h/wOh1te/U2KxtDI+/vRkSrIAl4qu4XGNDHq9HoZPg3C5nDCbzVAqleMkO2HLMmHKb2hopC5eKIHJNIalpUXYbFbYFxdgm1/CmNWOmdk5rK06YDAYaJC30/vLofLz8iihsAyra6uwjJnxddKK9hY5Nj6KMPfuLvQGIyzTdvj8Puh1g+hWqYr2MqGuV1SAx4uFvF4Ou92OL6OjmJq2ofPpTfSUUpirOoLZLgnNbo3+B6i1OgzpdQUsE6YegUCwweenYGGRlGDDyMgIRidm8aqjC7U3sqGuDMdCbTR+fa6B3+PEgFqHlW/2ggA9GWYKGo02m8vlolEux+bmJrRaLd2TYXi3t+DcAbTDE2h9WAqNiIufL3hYNnWi7nk7IcRlukovSkR6egZSU9MwNTWFuto69Pb1YXvby9AmMu/4gfqmdjwR5aDgcgnuiKrh6uxQU16vl2ExOKg7FxkZiQdSKRQKBZpbFKiqqoJELEZb22tIpfchLLuKpLSzOBrDg0HZAc+QCnQP/MHZns/NneHz+VB1d6OnpxfV1dUoLCxCYmIS4uLiUF5egeamZ+AnJ+OjRkuIRTHZ2eUwmkz5sbHH0EJnLy4uRkLCCWRmZkEmk3kcDsckHXuSxDudzsNul+tMwE83kd0qsqbESWdcjo6OQZlQiIGBgXna9ojWCHZsbBx7E8FbIBdHwKxWa5bFMt5KB/BJELH/aTTtJyUHGBubmAHYu9ss8s7OTrA09tSBf6/xN8MKuN/q3mGhAAAAAElFTkSuQmCC") !important;}\
	#header .books, #gbar .books { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAxhJREFUOE9NU1tIVFEU3ffOaGoPMILQMtCUnIokJIsCo48kCMqfHhJBkKSGqKQSg85Hmlj9GANlmlo6o+k0WGlS+EpnHF8TgppYampaqKUhFjl6H6tz7zjihcPenLP3WuucuxcHgJTPE5VcECXiCcRxHHEaDZEkEafVsiCShuNJZj08z6t9aqMsy2qUJJnlkpqvL9as5uKqu3btzNOz3uzZUIoa2pzR/T3Ogp7Cx/G1VkuWpdhorKoxW15azc/qml7da3e0XlfqBEFwK5AYiySDpqemd+fm5NWWlVXCcCoSr81FqLuVjBcFBgwNDuBK4gn0vO9CfO41DH8bjlX6VAXKnYEVynnWWxSVYEGL8yMsgYEY8fOCiXHY3+3CeOVWpBgIvlYfaIoInX2dGeq7CYLIQECzszN0MKln6nzpH1CsHW99twP7wmEJ2I0v/Qx/kZB2ZzPoE0FbwsPpdOYq5LS8IrJTmYrr+s7qrlYjIqkRB242wRYZjr/ZBtSHheBRWhBsqUeRcIkB3CBoYpiqbptRVSApV1ie157L6R6NM//D1jPVMJlMGDgUBvHCZbzcuQ2vH+zF0F0d9JlaUBYDuEjo6O4oVwEU+X39A3uOGb4ipggIjmvA4NwErOGhTBfB5KVBr80L02+8kZahAXUT+BqCw2G3qgCAQIanrfdOG5cRqv+JlPx6LMHlU6oLAzgOJi0DaN6CeYc/UvU+oFamoIwpcNjdCia/j9DZ/NGVSxVg0hzosrVXLTBV5QfCIPE8KjZ5w17vhzGLP9LSfcC38vB6zsPR2WFUHzHHWIOo7HGE5wMhcZVYcS3sH2cD8jRohyqwnq2+FoI0pEGins13L1sVhLb2D7mqgqWFubaCJ7WIiSuG2do0qGz+ZTP/8LYeJTodqk+eQnJ6BO5kBuNmRiwO39fhuP4IRj+PZaiDtDbzAVidjWb5Hs9Is5mPnnC58haAxplfQun45GKnLKJC/OEqxG+5mdXq1CvIDEVkJpIYsxLXzSUKbnCPmVQjufdk9n8U9rW/4Haj4sKNhlJypcgTPblCKG8g+g+/1oaoQ0UYdQAAAABJRU5ErkJggg==") !important;}\
	#header .calendar, #gbar .calendar { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA4JJREFUOE+Vk11M21UYxg9kmpgZr5xGkxmTmWmWTI3xwhsvduemJm5z6GTGLJsM0U3kazIY0g9IYYwhFIiIDqJCKWyjK9C1dO0Y3y0FCoU1Y0ChtNCWrTDY0K305783xsQr3+TJOe857/OeJyfvEweI/8TjVRHZ2BBEo0LESbdSyZYntoi4rdukJHbwr4g1eLC+Ft+h1z5fWV39VpZM8f6RU3lfHPj2fEZCZoUqIaO8Qlp/+Cy9KCUtX5lQWaneY+3U7dh4uP7M5uamEFHplXJ1xd6XE1S8nWtjd94Eu5VedimX2CFb5KU8Py/mLPBcpodtqWO8kGzhlcRqvk5LlwRGnxYxBSczs06Jw3aeSp5GfGpDfHSD+EM3ePLwTeL2mxAfGhD7zYj3Yvs+4o96OPJVjkRlu4hEIiIp9Tv1JzILJQ2jNHdMUq8ZoKHNzu+6XpqNNtrsUzT2uLloGKeqwU5SyRDJGQqim5F3xframsjNyzaGw0FWVu8xOTNDIHyP5XCY62NuTJomZn6uwlKgpF9dwcQtJ9Nzc8gVckKhpWTh9/mEQlHg6R52M37rDvWXjRit/djdU7TXa3Dt28voHw0Y5AUMv7aLa109dDncFJacw+l0/CgmJ1yiUFWEucfGiMOO5ko7eoOJ/lEnupqLjB44iMs2iFOq6f/yBF0uF5beYeTllXS0t3aKq/rLIk8pQ2/ppW9ggDqtHr3RwsjQEDU6E60//YpXloM5LYPusdvMB5dp6xuj+BctLVqNV5SVFwvVhVJ05l5JgYO6Zj2tBjM2xyh6UxdXe+z03fGg7Rvmun2S+dAKBscsFzRdlJ1XIVJOJommS1rp09aY9i4SDN9n8e4q/uX7BFceSvk63sAKvuU1vHcfMOAOom6f4mythY8PfYBITEwULW1WBt0h2vtnuTbkQz/glTBPpzPEb1YPdeYZak3T1Bqnqbd4qDHOoB308urrOxFKeZ44XViOM7SJ1RXg5u1VbD7onoO2yQg6V4RLY49oGvkTrfORhL+weiEpK5+W5kakSYyKMznZc0eTTpCvyCe3SIW8rJKzpVWkZBdxPF3OsXQFx9IUpCrVZBVX8Y2slNNnvo9N4nHJC7EmvOkYsp0bd403znlmVT7v/OeLC96DgcWFfcEl/57gku+d0JLvjYDfuzPgX9gu4VmJs/UfM8XGOeaJ/4PHEidmxL8BsVS++gB270YAAAAASUVORK5CYII=") !important;}\
	#header .code, #gbar .code { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAztJREFUOE9VU3tIk1EcvWJBEAQFRZRmRSSRSSo9LEozI+eIUsFHQVpKkkY6yU2nMXNMh9PldPlI2WfqoqUNcxi9Nlz4R5HhIDR1qUhhzmaaLbe5x+n7RrN14T64l3N+v3M418/pdBJ/f3/iHSsrK8Q0byZjn4zEYv2NNQSEzWIlEOLXQ/zo1Y9efAcAYrFY/N++fReofdOf/Vj9pLf9wUOXpK4G6ekZqK+/B5vdnup2u4l3MhjvJI9Vj1Atq0ddYxNuF5bhekYO2EksxB48g/DQMOTzOJgzfX/BAFwul4eEOXt3UlUlyW9uU0BUIsKBy/sRLN2D4zePIrL8MII7diPh+gW8G3gPl9O53rfyagdms5lUy+5CrezBqZwohGiCcao4CkcaIpDCSkEiOxGtCgWsVuvW5eVlQu//d8AwUQ/aWnu6enGVcwVH5GGIL2UhnstGeGsYYtKjUVzAh3Hic43BYFi3tLS01tcLjxatVre3ma4iuSNBdFkU4vksxHCjkcXLQpW4GiWlt/H0aQ/Gx8bJ9PT0OcYLrx+edr7PzW0US6VQNFGI48SCJ+SBqm2HuKIGUlkd+vv1mDWZTn/5Nhs+/+PH9v9MdDgcxE0zNjTdH2htV0JFdaGxoQU5ubnQ6rTUb4tly/TXryQrLdWWFXsGnQpqdHBwkMtgmAwRu93u6UKn06XwBeUoEQiQei4JVSIJFhYXPW/DIyNE91prHnj5CgVFRejs7qKvsctGG+rxgNFDJ3BtX1/fUFpyMgp4paiouYsKkUhJtbXFzs2aSK9GQz15/gwKgRBvahsBh5PNkBOPBJpkZmaGiEQitV6vh7hSjJHhYUxOTKKzoxNqjUZgW/xJqDwu8vaEIGVLIGTHzuLjS23+ageMFJVKVX/rViEyMzNRWVkJuVwOZXc3nnUop+RxCcXZO/aCczIGhTv3QXjoJEYHP/zLtDdZNNE2o9EYOTU5lWYYGipS3+BaJCHHcD4gCNmhEeBvCkJL0kXM/1yYoDEbVj8II4Nx1TeuSwsLpDD1krY09ARqI+MgDAjBoxwOftmsE3C7Nzlp+asA30/iDcpfss36oQ/KaxGR6KuWwQmo3A7HBm+xPw8Wkm4Og28rAAAAAElFTkSuQmCC") !important;}\
	#header .dirhp, #gbar .dirhp { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAnpJREFUOE+FU0tPU1EQvrLgL7hw48a4VOMvICZuWBhQWZiICkaLD0wLYgTUJoiNIZJQTbAQLVpbSwJqISQqIYbYFou1NpAqj2JbtZS2QKmo0Ps4n+ec24sgCZ5kMt/Mmflm5t4z2wAI6w8hRCgoKBAI0fzI26p/02EEmiiKwrEsy1QkFRNZUBTCfaIsUqzGaHqDoTnXSFf/kq8vxMg0kjUm0Ja1oMhEXDB23NCZfc2DjtEnPdS/fTGVEYY8Q/u0GNYZH19rmelYIrrD0FZ988p9fTK87IGCFGYRQM09Q6rVaRq3edtx0ngiHvCNnWbxkiQJgiKrMzExua6/kBCncBlZRKieRzA2jCpzFcXsZKn8RHl7GUY9fifvYEVc4cnd/b27BmI2ChcwlfNhRvbzlFLjYaTFGBYxhTD1pzHB/WfajiMZS9p5cm5JLjxlLPexymlM0sYnIWEOw9/6UGGq5AmMcJaE6N1nZBDF7Y8NiAa/pjiBobGm+NO8m0IJod9uTOa8apW7lVjKfqcogxkxgIgcoBFJ9P+yo9naxEKKhZd9Q4Ll7Z0Usx6+64SlxwLnoBOPRtpQ9+wiviAIf+Y1J1mgFmiPja8uIRP9MU2NQuHarQY0Weuhq9XhfLUOR88egMs+AJujC8/fdKO1ywyXtxedbjNEJDCD92ixt/DqEqF/gYLQYjzbmZnNVlC8m400HhrTP7B2rNbqL2PPwb04VlKG+rp6OPyPYRq5ikR4jn1J9TcysIUU5e9KHE/tHw4VleKC4RyrfkQWZbovRCVgT5gL8rsgqZfsTlbUF5eXnVTv13KY3rIDjZiRiWKOxxKQDQv1vxE2Ldu/2/gHUzTqYGQWFFkAAAAASUVORK5CYII=") !important;}\
	#header .docs, #gbar .docs { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAzZJREFUOE9dU0lMU1EUfXHhxkSJEl24MVFjXDjFIS5YmBgxmIBxAcG0qCEhLiRoYiwosQwKElqg2loqGsUw92PjQGUQjSMULEMQjC0tlCIUGipj2z+0Pf73kS78yc179/57zj3v5l4iCAIJhUJRW/PpuXaPRCIkJPoAiNH844QyTQbzi+eii4MkHA5LYEG0YDBIWJYlPMdLyRQoEfEUHCasGHtYerzMaroLhfwCJfgdrUwB/1tYCG2iMUFYJXxaZ47xNhP0ZBHcLymmBHujILvDscHIGM+UV1Roc5XK9tu3lb+u38wJ6HRai5i4zTUDoldt/Q6GoCgxFp4QPgFcDHE6nOS+VqspLVUvtbxtg8/nA0Jh2F1j6LdakXw2ATNTS96mxhqH7yVBxwUCpraVVk8XeJaQlPOpaGtrw4J7hgaxvBzAqH0UDvsIPO5JtPTYEJ+sRnfLRixWEygykiAAlYgI6wQhRMi3r10ulUoN55gbQ71DmLCNYoXl4QkCw24WHXUVuJi2C7AQvL68HvdOxqBTk0VrbQ7zHJF6UFJS7O3qtoDjBIx7WGirTSi4dABfUgjKjxB4O/aANxBkxB+CqSQdWUn70WMdqaZYiWB83Hn6bkGB9ITHVbVoL0vAdOM5dN3YiQfqcuiePMcd2V7YXF5025fwsXcYmRnXaPo+wnKsRKIpV9W3fLCguVYDW0Es5ucWwA6p0PNKjRzlI1QYatDU3IzlxRX4Zseg1z/E8MgwpOGhBIODfbsbGAbOqRUc3rEePPyYfnUVDmsVuAUfuMAfPKs0wGwywvTajPbOThQVFa0OD502ehYWFnZRXVeuKjArNpGfeIfRGjmaHt/D1OwSJlxuTE17xHNCem5cXBykSfT7V1UwDKMZGBhAU0M9ntbUgfVNIvg5D94fbyQA/dggC2u/FSnJyTAYnoBwHBdVEAj4j+Xn53GMkcH5VPk/yBw8kzZYLH3IvZULmUyGK5mZ0On09H+ntDCRyOpCURXZ2dk/8/LyIZOnQWeoguJGNlLlF5FzS4HWVjPm5+ffi3mnRNtOMdGto0SUIBj0J87N/ZGqV+r16OvvX1N/VLxsWesZVU43+S/ixfW37Wtb+wAAAABJRU5ErkJggg==") !important;}\
	#header .finance, #gbar .finance { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA1xJREFUOE+lkf1Pk1cUx6/OqNM5gyPExLj5k/qTBjLjyF7coi7i3HQoP0BHGWOujXQSDdJSYGS8jALilLZQXwqKFhnSOqGCJG4Zw0hbCNtisaVQ7INVC760fd6KVPrd8/Rf8CTnnuTc8/2cc89dFAPIIvIaBgFAUdRic3fP0pv9t9609vW+1WO9udr8+413rnWZkzp+61xrajclmq5cTmi71Pb2+fOGlefqzi4bHrAtEbUkGuGJpvXSZ7en/Ljr8cLufYBh77TgPjgmfbAJOet/TlyxjaDdPoS+oWfI/escEpITBT2WkAjHkU5r74FQdAH3Zl/AF6TxkOYxy8/hMcMhwEUQngNmmBj8bBTU0wgSHeuxuTBVBCwnLMsSYeScKaHQ5nsE3/MQnoQZQTSHIBtBiOcwGBjB1YleGEZakdL2MSThIuxVZYqAlXGAxWI+Gpx7CefjGfhDYYQjEdARFngF5N6WY9P195Ey8DV238nCV8OH8QvXiZT8D0XAasIwDLGYu8qY+SjGZ5+B5ufFi7gF2BkkGTegIXQNhdQpqCk95O4qnGSv4qMTaWLJGsIKgLaOjgb+VQw0t4CnXBD/PB+Da3oU+7sl2Dn6HeTeKkjdJfj+/k/IdBbBwHUjTZ0hApIIQ78gLaZufYBnsNWUiu3Wz7GuJxkbb32APbYc/DhVh28nSpE3UQ75/QpkjhWhle9HhjpbBKwldDhIBrsc1V8OSLDDLkVX8C5UlBbFfgNk45VC13LkucuR6y6DwlWNb5xK1NHtSFdliYBEgpcxkqZOV75n346mgAVlHj0Kx3/F8cmTODxWjqOeGhRM1kLuqUSx9wzyhR1c5PqQoYpPkEDAg3xSsle2P1gIyZgSR8arIHGpkO1WQ+pUI9dVFn9/tksNmftnHLp3HM3cDXyhiu9gFQENImsqzvyBr0cBVQvl9GkoHmpQ4K+HwlcLBaVBvr8GMqoCpU+EX6AqYZzvR6pqjwhYQRbCUbIle1vaOucuJP9xEJ8O5CD574PYZz8CqUOFPEcJ5P9WQDqsxLERDZSj9TjkOYHN6VtFwFKCWIwM/Tn4rlZz+ljr2ZZSY/OFmpZmY6NerzPqdNqOJl2TpVF75rJWpzXotdoGIVY0Vp9S2u8MKQTAG0Q4Xsv/B7U6qnSgwbYPAAAAAElFTkSuQmCC") !important;}\
	#header .groups, #gbar .groups { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAmpJREFUOE99Uz1ommEQfqU6WCnNWHBuZ9duGYod3DJYcGqcTEQaJBHFLtJBCYhVqRoXhzqI4KKI4GAQQgIJIropEUFES7DB+BM1/lzvrv0+zNKDl/vu3nvuu3vuXgUACDrr9VoolUohyf39vXh8fBQKhUJotVrZ//T0JFQqFfsJJzabDR8yBoPBy0Kh8LFYLEbK5XLn+voa8BvOz89vr66urKvVaofiUMs4sVwuGZzNZj/FYrFfKGj+FQxkPZvNgJIFg8HV5eXlN3S9k3AMTqfTn/FSBno8HnA4HBCNRiEcDsN8Pudk2BacnJxAJpOh2EOqRHQ6HeF0OmVwvV6HQCAg2xqNBrAdtrvdLoxGIzAYDIDtjtClFC6XS1xcXDwrmYx+vw9qtRqwLTnZdDqFh4cHCIVCEIlEyP9aWCwWUavVOAj7Yt1oNJBeATc3N2wjyawXiwXc3d1BMpkEr9dLrjcCWRd+v58DcESscXyApMp/lsgcDocwmUzA7XZDqVSi+1dM4tHREeTzeRlAH6lUCnQ6nVwVETkej6FSqcDx8TGFDLCyF5wAS7s1mUxwenrKJZLkcjmw2WxyUhpltVoFn88HvV6P/D9o+Xgp0LBSfzQ2s9kMer1eBjabTeYikUhAPB6HdrvNd8jXW1pA3ihKgucL+tetVgsODg44aG9vD3Z3d5k03ESeAP5ogeAP0gZzAiqFDmJ+Go1GsFqtsL+/D3a7HSghAVF+Y8xXipPAXIHkIC6wz/e0gWdnZ0wWMU6zR6FNUm8DKZ7Os2z/nN8JQbPHaouoddtA6eHJLRBo20nPFauiV7fzP6BUwR+8xRMFMpWXLQAAAABJRU5ErkJggg==") !important;}\
	#header .history, #gbar .history { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA1gDZAOY0x3h0AAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH1QMSEioFMzSNQQAAAyVJREFUOMttk11oW3UYxn/n5CRrsiRN99Va1xJYP9iF4jp0G0Oj7kKZIIexC8cYMyrODYR545x4KX5cWB1oQXCrDKooc8YiiM51i0XbzRarzHZdmYklMrd1MW1Nm3P+5/9/vdguZvWFh5cHnofn6mex5Nr2nkpJYFyjVMYonTa+wqigqD2VN0GQu/Z9tnJ73rrdtGdPu6J1r0lFU/duaKS9KUaghfHpOUaGp/EnShXjB9nyz/tzS4fpeHqgt2N/Xp79oiRTc4EsaJE3+ibklSPDMr0o8lPFyPMnCrJq6zFJtnf3Li27nQe+k/cu/C1XPZEbvkjZFxkrzMq5ybIUFkQuzouMzYq8OTonDV09Emt+zQUItT91OoWRMw8/3lm3t2sFAhhAC0Tjy4gloygDh57L8sPZAR5zXfw1DVz46pdH7fD979uijRtrTqZ2bW7EN+Dpm6ppqBnwDLy4L8up/pNs3LadGc/QcncLyfamlAm0a0ugM+m2FdQ5NjVzs7R469c0HNqX5Zv+kxzs+Ri9/gHOlKoUZj1aH1qPUUHGMUqn16yM0vf5JJn77mDtnUlCFnQffoHIYoVc34c8cuAwf3oWawUifsBkfoqVTfUYFaRtowIsgZnr88xUNTMe3PCgUCySyWQA+LrndY4+s506v0rCFsLVKlHHAhEco4JitVxly44unJBFxRPqQsLBt48yPnSWY8OXSKxuoqaFmjgsT4R4cM9mhj4bBSg6Rqn8pXPFJ7dsbWVZ2Cbu2MTDFnWrG0i7OxABZYSFQJhXhpBvCMRi/MsxgLxtlM5Nj1+pXJ26RsyxSIQtkhGb+ohFKgypMCQjFsmIRTxsE3NsSqNFiiOFCpCzrwzsrhg/yH7y7iCmpog5FstDkAjBwpzHfLlKPAQxxyLqWOhqjeMvnQDIQnclBLDwR/9FE92WHvp28p7OjlWsa6kn7sCnuQl+HfmdTZtaCQTOD17m5V0fcL301zvQfeQ/MCXWveUaX/Vu3NCcemLnXbS11qO1YfK3Mh8d/5Hzg5crt5Zz/0sjQLTx1ZQJtGt8lTG+ShsVIEaKQB7IQfe/cP4H9CafRq2cwK8AAAAASUVORK5CYII=") !important;}\
	#header .igoogle, #gbar .igoogle { background-image:url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA1JJREFUOE9Fk39M1HUYxz8ozM21CTRJquE/Nl2uX4xsZNmalr+WVLq12cSkdKtNW5F1/HLaKWiYDOeE3SYYB1MoQrhxmXA7yRAZow7jJAwOECI65ToM7uL74159vt84+myfzz7P53me9/M8n+d5C0BEIpH5rSmKUEIhYbxHd6/nF1FuyRX63JumqqbO8DMPU9B1oc8pDHnq/t+i/dtL8bUH9r/23bYN/u7nV+L87JPZ+4qaaOg1TTP9zEOfE4z7nYHBxEardaM9Y0tz2/YN+Pe9Ae9thR1rGUuJoy4j497d6ZlUE0QGFJG5qMFgUDjz8goLF4iQZ1M6HNsP+fvggx2ob76A8tIqeHopwThBXdoaxicDFjODWXlcr6lOLE1+aLBAJlSQsgLbkgeYeWcLWuarqBnp6OtXo6cuQ390IcrDCxiWdlXPpDLq81lESVpa0CqEal35OC57HWNTMxx65S3OyEjhzalEXlyB9mQSLI8jvFQwkSDwLYmlVYJ8Gp+AaD1ZPFz8/m6mVfnH5tK5N62Rv3EnNgmirk6AR2IIJwn+ejCGocWyBOn8UVIyN6+6wuYnVp+z3pwK+P9z1yWQrkoQlbzNb3NeGoeSFxGMF3QuFFhjF7P35XcpyMk2zHeZHbCftRaNDNzCPzHGZGASVTNAFIJhnSPb9nAxVmCPEWStWseuA3aKSxtx1BQhfZeZc9B0sfw5r+cGXS1fY8vfyU/trbR7R+gfG8frmyB3614yX7ew/dAVjp5yY/uyFLezRqYckdCyhGst9cvbWurxdnzPwbUC1zc2jjff5queSTr6fDiaeiksaeaLk/X83PknJdYcfh/pq45EZBsjRMTIgDexquwYd/o9ZKcLfnRUUn6pm9yGHhpvePF09OFsuE535x+cPn4Ce/lhNE1dpxqDpCizsgw9zlF7drDydD4Vh3fjqCgipPzDr7+NEgioXHP34mod4ESehdpzRxWZepUxyvNciI6y23mhuUKClJUUcMb9LLar6+kavUKba5Aj2Vm4m89rsuQ1Bm+iHJpnlWyfeb87Pvxxw2X7rc+bHiOzbBFZlU9RlPMh/T0/GG17QlWV/wkYZWOUzoYySuE+/9Cmy7dbDnqGuk6h6Xvke0pUH7U3bP8FTYG/6vilZVEAAAAASUVORK5CYII=") !important;}\
	#header .images, #gbar .images, #gfx-preview .images { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAu5JREFUOE9lk2tIk1EYx48aRWGi+KEPEdGnCoIgAoMyKJsxioqCsCAoSZRWGIhals5p4RBSM3Vlad4WZfNaWo6m00i8d/GSt9VybqFOSfO2vXvff+ccL6g98Oc9z/P+nv95z3l53AAQFpIkEXd3dyKyp5sbcaNaHSJNPZZLrI0joigSF5PLRQRBIMyQyW4f9a2qqorsHzQF0nwLlTurT0xLm+ec8GZrtimFpeWmpeZvg7a0htZ+WGx/0NbRjdynGiTHKVDw4AYe35FBm59JUWx1OueZAcj4DIjFZt9VVv4mNPnera8V+SroMq4jR3kSeQlnUJERineFd2F4X4GW2iIUpd5kBjJILkKyU5TG4tQwZEQehSZGjuqcKNRok1BXrYPZbIbNNoF5hi/GQH83UiLYqaDm91eZGwdjYTSGJwRqCIgr4JVLaTH5+cuC9PDDsI7N6blBaXERyY47t6ZtCaflR+mYtgHClBazEyOgeyAr8gha25rt3MDQ1EMKYk7AMmIHCfeArqqEm3WNziOodhIl9Jo6VZ604oe+UgKTdRKFyQoYqosZ5k3oJ5P0CDl6Oz7C2NSCccsYNziuNoKc1sJLrgLsO+jZEjFm3IRZaxaqdTo8SVIwbDv/C2m3L0L/SrPqGD/6TDiwNwB1jSY4LLEY1RO8jCd4VqBDb1s97occZHwgN3idl9qsib3ADVyCk2tt/OotR2vnb37J1kkX1GH+DMngBg2GSnXi5X3/NbECa5iadcA0NIlGQwXKMsORnxCECLkvvnwfMnODngGzLO78TkzTZNg2ivZPH1DwUAn1tWOIOrsbigAfhMu8obzij6z4EOhe5Ahtn7uGKd7ODRzAtmC/DYgI9MWl/esQfMgLyqsyPE9XoeZt2bx1bKp7VkIlZaOpAqi8WB8THwi68CzW5sGgr/k7A9TTPJXqFNUeqo1LMHu6BAeVQJxOgQ/iykHyocD6BVgiostBnI45CjopKPBpZZuJ0uJ7cWEI/wEHce3pHc5EJwAAAABJRU5ErkJggg==") !important;}\
	#header .intl, #gbar .intl, #gfx-preview .intl { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAklJREFUOE+lU89rE1EQHinUgjkIStu/QPCSQy8q9CBCCcUKihcrFO1BEI9KFYtIg0UCUYtowMb8qIdaQaIQ48GAFUykUCWpaCFtIqvJqts0aEObJk1293PeJpukQry4MLw333zzvdn3ZnYAoP/6hEAr03Wd6qZWeF8mXROrTqjFGg4LaZpGWrlsrAaJMXP9+xATb5BUdXsl7AshkVhUQTlF2p39+vKgknnfky9gl8BFvE7SGVAkiX6GQt1ZSWo3TyyUQMn5kSNLAVKlgAXpF4TF1wc25ZWtW8b9gU8q8mbOZpsJEpW9DD0lUsNH++UC47+T7v0fPKTFoiHk14Hv31JYetaJ5fAhbGo4S2UmxQcGph2c+MrvxxrLBqf8mGZ/9tQFJNLHkAjbGW18+ZUklDft+CJlP1FOlukJkyfs20mp62OYYjyjHEckeB+9h0+guLFaV/kVtWAhHgOlXC4KMPFjPF4NVioYvjqKxUceOBmPzg1Cnu+HpasX6yUz34/Z24TljP6DPvMvPGZiqa+vrm4dH8fz7k6MUQfepiUkZsSlX2MLAWuXEXMT7jmuCP6I8UwTTmfkLosUrVbA5wOGhnCD/TtOh8Lxjsi7hfP24T1wX2rDzXM78cDlEclhfsY2UisVIdLle+gtXuSkUbZBNu/kJN8vTgLVhhKW24BtCzjD+73CF820rdPywL6ELJ/m1+oxkmodqXH7AtWmEqZp3NK1Tq12ogCrlTRI7AvcxJrnwjy9XoERrJUkhLSmOWg1C6Zwy0lsruZf+z/rSrlywgYKyAAAAABJRU5ErkJggg==") !important;}\
	#header .mail, #gbar .mail { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAp9JREFUOE+VU89LG1EQfhHEkygqiFTEoz/QevDqQfRo2uBB9LYevLQ3D95EiVFzUYMk+QeEgtYeipS2mJx6UAm0qHGTNCbamGTRaDQ/NJvN7n6dfTFFsFD64OO9fW/mm29mdky6ojCoKmMAM9XUMFNVFR3pbDKxysKfAxiKRW7LqquZicCNn0LXdf6tP94/3StvT+1Z1uNh0sKCINlsQkmSBMiyoKdSAtJpQSfw/fpaQCYjkFIh5XYLibk5Ie3zCUTewCSrlfnb23HU3Izw8DAKokgBAO3hASiVoBUKgK5Dvb1FbGoKhy0tCDY2wjs7CwXoY1dOJwv29+PUbIbY1cWR9/nKJAqZ0CrGYggPDcHf2oroxAQiHR3YXVxEsUIg0sX55CTy+/sQe3rgb2tDemuLO+f39hDo7cUxRc56vbh0OiGSAs/ycllBihQEursRHR3lDnIkgtDAAI4pWnx6GmJnJyfNHxzw9yRFPmlqwm6FwEghQLLPxsagU87GUhIJnI2M4EdtLU4HByEHg6CCQVdVJMjxe0MDviwtgaz72KXLxU6IIGoQkJEaj0M7OoR6dYXL9XWUkknogQBKoRB/v7Db8a2+Hp9IicoJSIFBEBkfhxYOQ/P7ockyNK6FCmmQUjFVUoFoFGdWKzx1ddix2R4J3C7mf9mL09cWqJmMURgoJFUhpyIRGbtCqVHFOfHPmRl8phR2SAGR97Fzu535qMKhV2bI5HhPJAYecrkyslnc035/cwOZkhCp/1/pB96Zny8rON/cZB8tFryjFD5Qi96vrmJ7be05jHu3C5tv3mKDurS9slJWQMrYNeD4lck4YslkGZL0dyQSjvjdneOiUHDc5HIO8n3BqDUMmvZsqAzif8EYNG6k0QRqRPJfeJza3/nB49pB/3HdAAAAAElFTkSuQmCC") !important;}\
	#header .maps, #gbar .maps { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAvFJREFUOE9dU21Ik1EUfgf9CYIkCqGIfvUnyB/WryDqRxZJRRDMLCrTzIiM1MQUIUERxU/KaVrTUtP5tZxN3XBvc8s53ap9Wbk5ZWamhs1y6HTbu6d7NzdWF879Oud5zrnnnsMDwPwz6JnHC19xfj/D/GfDI3oqgUEJ/FSIId1TWV3zMZ6NNbL3hu9CutDq4zjG5/MFCUDAHLmge7lKH1NfW1k0qP7axJpdbYoRQ5nFqM+wTpgumj5+OL44P3dodfV3NLHdFoieeg5EwPkCBHqloA/283iWnUSO5ZjQtoM1f4dt/jtGPlkglStgMhjQ090+PWm1VhMQ8U7D9wcJKnLOzpnYPLQWpmBEGAt2+C26R6fQKtNAaVuExbmOny4PTRyKS0rp26n3YPgikfyUuP4MNK3p8G4KoBLwMb0wC6lKi2ZhPSSsCiLZEJxLPxC1fQeBII8kyx0AqwekxzR1R2DU5UL66hHeiFtw42QsnspcaGqWoN80g+lf61jxbODq5UQMKRTrBLeTAbfB+AhBR21G4/ve+5AoZJBIxETXCGMbHwpZFpRaFtqpJUjZUbCD/ZiwWHqIQTRNfPibhF2NkzplDTh0Iiv7CfobrmNAcAkV+clQV8cj+XQMxvRmLCzML9OIvd7gFwcmsdy6T1R0FKK6TAyUXIOpJQ3NTQKIa7MgLj5HTKjDchi/OGCz2UwUQ2uA/mCAoKWG3zcmjINVeRtOaw6M3Xx8szViXF2F2pwE2BVpsL8rQ2nlC8zO2HMpJlQ3TGFxTZRZ0wwvNwm4eomOJQXYhvHPRmClEsu6PPzRJMLhsCPtTibN/EEKDlUuY9BrhhuEnXPllS+RdPMh4k5cQO7jahQUVqEgvxgdr58DHicSrqRAp9M6It8fzsFWfe/3c5vxc46ZHJPR0NUvk+vSU1MX7957gN17DuBWEq1MxNC3U/twBDQcmtHIZgo1zNa6i6yHiewNASNtw8BAT2w1FfVCSTfdbsZNhJZ6pJNIgr97/c1FF4Q9mgAAAABJRU5ErkJggg==") !important;}\
	#header .movies, #gbar .movies { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAsxJREFUOE9tU2tIk1EY/tpI6Ec/rD/iJfoRIoRR/XAS3exCFwosKwrSNiVhMzCiHxWSRV425yrK6RQvqZTz2pyKil0su9CWuY20ZW4hIxAj1Kw5v2/fns45a2NBB57vvOc57/u8L+d73xUAOLoCgQAnkUg4URQZoqKiGB+5wIETeIGTSqXMl8XSDw2I3KkNIrjwc4HxFF6vN2zTM00YFogkqP3OYsm1WEcLWozGO+U6XXOZVtfW2NhYNdg/cGto6MlFErw1lJCp+P1+ti8uLsaXatT2Mq0WnZ0dEHge23ekITFxA766puEX/bCOWlFdXYXZ7z+usApoKYIgMAGjsUP/sKUV0dFrkJS0kVBAfX0D7A4blnkffSzExSWgt6cHDU3N9DqeBQpCsIJ7d4ufUpYnmQcGBmGz21Ci1mD4xQgc4+Mwm7tRW1vHhA01NXTThx+GF4SVj7omq02Dbng8HqyPiWUZnROfoFDIIZGuQsbJMyy4r68fj00maq5jAs6Pk5vNXa+/9DQcRcYuDufzK9D+wQEnv8wC6Jr+PIS3z1tRWHgTqakpcLunKB38jUfkp1ylZlLSLDl+46DKS4GckyAvIRaZ25KRdiwHCZvqiLcMl/IzmWClwUC31UzgalEBnvW9wbWc27ivb8JvQj7YuRc3iH7vlhhkqV+R4HKk59pZsHV0DFqNhpprmcC+7j3gKjmYWkdwLjMfFYY6OGjZBJ5AgHx9BGNYmrdAoy6DTCYjfRagasF2rLisb+eucyi3lgBL+KXVlfLp+w9AqVBApbyA7OwcnM1S4eCh4ygqJj5BxWTawUxgeU443FbcBuVpJex2+27KEWhmZmfeT0w4nS6Xe2p+Yc5GuJcEJ+h9qPnCPU1Ild02Nuz1+eShPv8r9M8M8GSYRDE4B+EKRH9wmCJBL2mWMER/eFKB4MRS/yAZEMMtHcoecvhfFZHcH/MdxJHAmpCYAAAAAElFTkSuQmCC") !important;}\
	#header .news, #gbar .news, #gfx-preview .news { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAohJREFUOE99U0toU0EUfWmNWrVFFEGk0C7cutSNNeAioeKmxIWtXQl2EbAluKlZFPwgkQRBmrQh1khCS15QTBprYhVeq4/kWWgSbElTGiMYRaIg+GnENJ8e5w5GXj84cJk7c8+9nDtzrgaAsHmp72q1mkBnrVYrkK/RaISGhgZhfX2d7zxYB9FerVaFSqXCd4ptZ2r8huTtwIVCYd9LWT7t83odoy6Xk2GOEo4YcKbqA/nJZFJQFKU1FAr1iqIYEUX/z6eRCKampuBwOuF0DufVrDlduigWi4LVavXE4/GPfr8f4XAYsZgM99gY80OoVcpIJeYxMuLEXFwOUE65XBaEtbU1XiCTybTr9Xrkcjl2BDye+0im3uND/jtiyjJE3yjGn6Vxcfwr3Hdu4NuPVR1voVQq8QLpdLrNYDDAaDTCPGDmRaToEN7MdWJeuYfJyRTe5pcxnUoik1mEEn9NkL2bX1lvsVjy3T3n0aHTYzL4ECi7GM6E6ZgEORgDZqJwPQ7i5vVrVKD932tKknQyGAw+mZ2dLcmvZNhst7Fz90E4HFGsfrGhCjMioefAwCCOHzmMW8PDVCDBGSQSiStMTOjv7wcrBKYB3kJAnMD+A63sMRdQ/NyHCqIIDLlxaM8uHmfLUG/hLPvvd3a7HS0tLWhubobJZOIIn8+DU7pu5mWZWSDsaMJijn4SL9gPNm6ntGMrKyth9qAVJlUsLS2gp7cPv399woWuRlwdvEvJVabGNhITV6JawtTSX2vKZrMPdLoOdHWdQ+cZAy71XabkErMTG5RICXV9k09zoDp3zsxIAa93QmGxR5RcnxPCbGmhzogKkdJUjLYMHsX+O3FEk4wKkeDqtNVs/wCmg8x6BIOC/QAAAABJRU5ErkJggg==") !important;}\
	#header .notebook, #gbar .notebook { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA0lJREFUOE9tk39M1GUcxx+g+kNr/tEKStraIRllqyWGi2sRKaGuRQw2U0KXOAYRPxx40NixpjuFraFwZFqceNfVyQ85FRM6MYNpbXIlAlPIDkPPElFshPcL7tXz/R7oP32393fPPs/n/f48n19hgJiZnRUPRUSI//+mpHmhRLh6rfiHhYU9cPX7fKqxzd4VWVZalZxf9fmGdwp3lsR9pK+OSS+tfSpDp1ubU755bVFlqvSLVAMGg2JWBlXO6u9YV/fSFZlFrtUnpwk3DLJ4/ygx5r9ZdniSmO9hdec03U9q0B/q9KgCgRmVp4iIBrNdbKzYTcNeIyV5BWjKm1h1dJI3rS4SD43wesOvPNIF1fHJrI+K5h/IUUVmQiIieWMhh0928eyrqThueNF0wMojkzy/f5gljWMk2P5C2zbFM/bbjJwTFBscboUYCPhFUKYijF/Z2CojO93jtB9vZ2vlDiKLzHw2EMRx4QrfnXFy9ZqLWAuYzr1PbeVjDLn5MCQSCNXAYDzYb9hjJD4lizKbA6E/z7sOH78MDtPU7aTZOUbBTz40X96iuV6wpcI8KnkP+/1zAjvrG7evy/mEjgu/c2/6DhkWJ7F7nCQdmSDOfJ1oWdQXD/Sz/Ft4fG8xmsxHmXSz5X4XZiEqJbsQW/txYl95m8SsfBbpjpHQOkG86TLab1ysbBrmDaubBMs0i3WCtE3bJZ8o4fV61TR21Zm61+WVcbSnj7vSkNTxLytMIyRaRtDKjiS1jPOW/SbPtUrWvl3UGLdxpveSVXi8flWg97dLa7LydfT2D/Ja4hq0G3KJM15E2wPRjTdYVO1kga6dp3Nr+fnqPXy371K+ex+SHJwDonxHzVh28ae0tZ6g4tQVRHoNSz4oIVevp+6LOq79MYDr5jjW82OIF9ZDwNOi9tLnC6iv+LrZXr88LZuWgT+V/PjxdCejd6awXrzFewfOsnBTHQtSt/FyZqm/p+esS7qkqMRgMDTXCgb7h2xiWTppB/sIz7MgVulYmlFGiaF+4vrlIYf0Uar/xLz/fWJotkPjaWo+YdtcUMUPp0/1gedjaXtJImKe5JUvVoboQRvlUsxvV3Buy+SlunkKvD6/8EgEZAAl5XkonP8AuFeXE70HNqIAAAAASUVORK5CYII=") !important;}\
	#header .orkut, #gbar .orkut { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAIAAACQkWg2AAAABGdBTUEAAK/INwWK6QAAAAlwSFlzAAAASAAAAEgARslrPgAAAnFJREFUeNpNkktIVHEUxr//fcx9zduRO5XaWGIjWUgiBAOaULZI3USbFrmxVbRs5SZo0y5oXVRaEAQtKiITrIVtcgyZwkWNaT6G0RmbmTuP+5p7/y1GJw9ncTh8H4fv8COUUhxUQauKHpgZzc7XAIhRxaP6qjUaDnmbGq456VqpPJtOPUoVv+3YJRMAHxBahztik2driZis7HtI44KulVK3P/yeXgYIAAYsABf1hqZ3arB7KiFIvn2DqZeXJt6uv0qx4JV2f+fNvvD5o7Tu7n7a+PPku5GvunB67iR67l0QBIVo5Uru9Y/FiXcAoiMnzj2+wrT69JoDQPax5s/84o03haUsy3OJ2Wt8XwdDakb6YdKFE4hH+mfGTUna3tazBStbsLY2dLSFB6bHxYhi28bqg6TsAaOvF7VUHsCxq6dIQN4t2hZAAAKYQC5vCidb1JEYgOLyjp2rMPWS4dh1BqzSHbJMF43UjbCADdh1+OItAOyy5dZsBhwB4MKhtsuwTfGBh6IhbW4YUfV5giJA/37NiDLLEbgAPWiZZ0SOlpayAKSol4/IjHA8pF7qBMjGzMre3K+240qQZz2ABwjy7JF2aet5Kju/DiA61uUoEsnlNbq6u3D5pVU0xIjSe39IHetm/SIARzM2n6ZW7i5Yuu7vjAx+vl7mJEIp1Y1q5kUqOfnehcOADcQj/jOtAArJbGWt4MIRg8rA9GhguMvnVQ7QMKp7c+nlWx/LmwUK939oMKG42v9sVDmtNnAiTVo1rUK0Wm5+be/LVi1dBMfIMb96MRYeilm80ASWHMY7kylJXk4QCKGUEkJBLNMN+ZXDj/4HpLIczArza/IAAAAielRYdFNvZnR3YXJlAAB42nNMyU9KVfDMTUxPDUpNTKkEAC+cBdSuDKlNAAAAAElFTkSuQmCC") !important;}\
	#header .picasaweb, #gbar .picasaweb { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAtNJREFUOE9VU21IU1EYPlPrt2DQz0JLk4SigiydoIYfRAWDpCySEAR1+xFhUYQZWKCtiAhNpElqWqkZZmlo+MNEV2YW4ue0aer6WNucsrl777lP59y7O+vCyzn3Oc/znvfr6AAQbsqn0xFZpES3KZyEqUjog0yVc50uTOHr2F75ZFlWAG5UosG9TAIu55aAbTYmYJ+NloGt/JytjK9yNF3ohwYPbGNLB+zWyd61lN1YiI7GcmoyHIn74R//2s8cHFHFG04IZHYzVYHh3ilTqaFuvaLkHRZGbFg9mQlHdgYcaZlY7WyAy5JOhYBU9G8khAGK2DHvjC/Lqcfj8h7UXu7ApdMv4XS54C7IxWJMHHyjVrjvb4OnzUiZA71MJSUNIgXzrit7a6262IFH17tx81wTxt7PrDDHmxk5wpF5dNrbYoG31QCXOQH++dEZdhZGJZEot/vW1qPuFbeh5moXzHnNmPmyyMWEiqJSOJFzxgfMbksaPA/18PbdZpljhywGVAe/lzw7r+U0oqnkFSamHAxCJI9MVuqjpiis/SSuqsNYqT8G9wsTKJAiS4Lq4Pucc3vbhXa0Di0g5e5HBomRPmGdBBjBL7GVOfH+mCDLd2Lxq2ofPO1Gfkk2lVgEAu+AIEZ1MjHJe41DFYNKBNpshFaRzcrzQnALjDRzzi4lAp/ahfBT1cMTxNCKsw1zsNjrFvL7rtiLrBX2wsFbihV/MNuNIw9wpr8UfmCeaSIo74IYmj66l+S2IbVyGsc/nwepiQV5qgd5chCkKVm1SoKC/jJ+e5ZAWYG5Ax6iRNVxHrL/MZmqZ4Wk/lyQ2gSQliyQZ+kgjYkglnjcGK1lxUcJq+DGKIfewsabONFge/PJ0FOIpO586LvyYRwox6T72xgT5/LJ1TShCDRA5JEwQrBwcWzls5/BbA/HxOD08f1/j0mrNAe58fwCrEUCqzI33kpNrHE0zV/FL8AdX3cilgAAAABJRU5ErkJggg==") !important;}\
	#header .reader, #gbar .reader { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA4VJREFUOE9Fk21Mk1cUxw+ikG748mFbNmr2wS57+WCsC26JgQWTZSZzsmz75kSEuUSj28JC9kKFbG6mMEqt4kRYlqBDmBjFUnyrILLy0tqWp6WVtk+LFKh9SkvpC7AVtPx3KUZPcnLvPff+f+fck3vTuLCHHiWTJMrMpFezXqJ1q0Q0HZ2itWuy6B7bS1+XRZINEhLoMYGI1lA6JYI8pa9enVqTPugkz6xA5oBb+tPAOelh7UnTzt5qZGuO4LlLRciofR/nW8/A+GeDVF9YCEP9KVgRFxuDLhqadhMFH0Wp0dIpzW/5Ct92KXBg6Cxyjxeg7F0xLkuyYcvIQIgIcebTzG8z1xt6YUGEzMIIUWgxRvvVcsj6GoD5JJYtkJMLXL3C1kFgYQYQvHisVGJuw3o4GODyy68wwH/7jYH7RO45gcq76vCprhTfjzagxdKO4TdFGD1RjTbHQ9R2jKDdGU2Bly1S/iMuMkifohp24DUKI0aNhmt5rx/9AcWnu1BUaYJkbzsyd2sg2t2J5z/RgHLPoe1gBeDiUhC+shznGcSG+CwFMEGlKh1++8OOu/0j0Ood6DQKkLeNYnvZIEQFt5D2sRbiQzeRrK8DfF4sMcidjdnol8lABt5BRXIOmj43C7uwOBdh4/KRFePcEeSVDoDyb0Cmuo5IwXtokW5B8yYJnB1qUK99mI4obdhV4YWMVVF3VcDJK37UdwiwuOeegooVVtDOazjKSq9k3irNATc7AdYDH/3S9A/eKHJh0Bp4KgCSUPeHobz08EksjvzjNmzcLoe77Sws85OnuAX/euKTXuq+b/lwc6EOJTWT6ObiaO6OwR9eTAl1wxH8dTvEZgncYD3a+s0grIlY3LY4QcaAi8gQcpE/Obn5y1+7Idlnh2c8jn8XErhwdx6R+ZXkJTUuFFf14a29F/CdQot+r01ljXjINM2zpyzw9IBV0aQddIo/68EZ9VSqiQ+EBEqqLNhxuB3ij+qwatsxZOTIUdU0AH5hQmUKucm8/JSNQTdxUQ85Yq53tn3RgT2KcXxdewcvfnCCiX5G2tvHsGnX79hXqUFrzzDs8TEfF10RD7HPlvoQenaXAGtmRaMOW4tvYW1eNV7YocTnMjW7vxWOGR+mECjzLo3v4WaeiVMVmMMrAXOYJz98qprmXpz+24ixhIBZhHrGlsalphmeJXHSvSn+SeZnkP8BHMu0boJ/Hl0AAAAASUVORK5CYII=") !important;}\
	#header .scholar, #gbar .scholar { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAmJJREFUOE99U29IE2EYf4lAKPzYpwi/FxJoHxSySKwPabT6WAzKieuPGi2wMilpFZkGVkw0M4qg2cht3lqM2IdWlMpkhJAYMosp7sy2Zs7V3e3u1/ve7fbHyIO79+55nt/z/H7P8xwBQNa7FUXJ+hM/k9l33f5fMAuQZVn1/1pOkmF7pzvgs2GwrycuKjjI7Cxm3eqynFb9Xu5h1e+lT4AQRDhggNvxjKf2IkkS/k2gUctVn+dXCWe/6kEqAXzrAqYb0G2ppnhsl6U/hQl0MKuq0++zddVK399R0ygF16P1WBm+zEsswRYWs0aCQnUxsKTaXdx7shS2TgJBcA4TOhorMRVhWBj0AgVdVShIFEXVNsL5izeQWhw2eJH80QJC6hBLLCxTX2U6rRVQm6iPg526YyI0TZ70t3qACG5frqFgC0Zc9BNoFEUGZky18aoPRkfJdNzDfSADD0w+II7o7BDMxqMoKTmBu90+5BfTd4eCc4vyyv28LOA6OwvMUL8fF80HsLn4Js41v4D3dRA7Sy/NrawIpVrRPAZRPkbOG+vMoXGnAHyF/PkpjenF9dO7MLe4iDu3/Bh2TmFs/OOOR4+9QZZAk5KRcLJiE9qqCXpunEFolGl9iTe9NegfcNC+YndLs/2UqWGQ9WAj1V+l0c9jcM/aFHdcq8DQ/XrY6JLMTFjQfqWdAZwseCHKb02lYlRajjobd5bBqowjnRcO4a1tL8zlBE3H9zPwmLaRuR7pjStgkE6r+14U5gVHh2kf2ozliNCtpde2jE+dkp4o/+8sGGMm+x56WhlY37S1gEIWIH8Bzs3pk+rKRfgAAAAASUVORK5CYII=") !important;}\
	#header .sites, #gbar .sites { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA2VJREFUOE9NlH9M1HUYx7+btZrWZqwS15ayZVtrq/6JVaZEFi6XSVITRoglAtMOCag7uoGzE0PIeRBFHCAQ0CaECHPO9Ph1pkyOu+MAL04OkFsoLUvGwR33i1ef+/Jj/fH+Pt/v83nez/N8nz3vj7S4uCiFAEiBQFDyzs/L7yH8Ou2RIj+Iw8li7IrP6wvIZ36/X7YSghwU8Pt8q8RBWJ+qLjyWHBaGRoTk7s6hqObyNr3elLmSKGRDhaVgMLhKdApnZkn9a/seCfOXC2KNgPrpTbyxZxtPxUu8vGsNR7/UVb30TqIO94MPlzoQj2lfUDr1c9uOIxue7WsWLoPAySc2E6eKJeZiLO9qNrIr7XWilDreVOkJT6xF8dUJQWW9lH+8VMpTn7S3P76OfkEsezScFPVBEoc/I6Z2C9GJEezN17G/cYHUhrvsLr5O+Md1bIkMjYXnpJ0vRjGrSKNx7cOoVUoK7teTMLCdqENPknGiiC96IFsgoW6IF3I7eSu1kv4bv/ON5jQez0KSNC5+Ie+Vrb5qUb3cdZ3j5niiP3mPkbtQIg6PdrvYXjnK+99baLhwDbxOiq1BMo5psQ3ZtPIMVEWltVNXezj7Szt7WiOIUx+gTUz0085ZdlSN8oPezrRjmNofz+Act/PQYSOH8iowGm8a5ATlNeeiB51T1FWco/pSC3O2QV5N/4li4yyTY7eZGbdy/vwFVPkarGYTlc29tDS3cvlKx5ycoLfP/IxOV4PFaqa5pYcF9wP2J6eFhsT47RF+6zJwVd9Ft+Eak04nA33dGLo6UJ+qXNo4l8u9NktVwNzsfTKUBcLl5e2YWNzeRRxjdxga/oNbt2z03riJwzGOecCKIutrtGVVSMHA0moeUX7bOTE2Snxyulw56aACo9XO33/dw2S2YOw3MXFngi7RjSIzj8ami34RtkFyuz1yAk2hNru9rZXIrdHMuOZQKnOpqmnC457HYhlg5t9/KP6uRK7859S9S4KzMRDwS5LXuySKIZvj+Z17k9CWN3C6VEd2Vg4JBzLkbvpNJlIOZ1Fb3xQQnx+F4gPLna/qYFkkESG7LynFo+8Qy1J4hvKKatI/z2HE7rgizjaBEN+yfmQxrUg55PQvZxWBjwmss9knN5dV1IeaiBFY4/P55fj/XwH/ASG1x60y5yvBAAAAAElFTkSuQmCC") !important;}\
	#header .products, #gbar .products, #header .prdhp, #gbar .prdhp { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAulJREFUOE9Vk9lPE1EUxq9Go4kY9ckHn1wS9Q8gwQQTY0KMwQcMiYqSqJFEpSItu1RpJcSKKFuVTaBoNBFBXGLDogEiS9FQVFYpSxcsAQwkrG1n5s7nnZm2ysOde+feOb/znTnfJQCIKIpEmqlICRUEeR0YFouF8Dwvvwe++/88uCkfUgVkt9lCvMvL21wu11ajsbjXZDLB6+MPCYJIKKVKjD/pOjLHgifnZsPMfT/4r3YnKiuqGQ9obWtDv9UiLYkg8OuSykRAyfzm3AVzryoWr7VJKI2LRVXdC3w2N8ExbEW8vgj94/b2oPyAApHVLW1mFiUbsnYS/Mo04EtYOKoIQYmpAI1Ggv2nLqNjGYgrtCImKsw97Rw6ragRCHv4ZEDZI03Z4poKYwu1aOqrQftAIl7VvoW58hoOpjQhNKYQPaYNMJcT1L/7/l6K4TieEJ/HKwOe1rREWM0R8PzWYdCiZVtXMfitDO6BPGAuGTkagvluFlxE0D8iSj+EJaeEtUhp24TTuytbe9wDZAEuDehEGlbGE+CbSsD0eC5Sb2zGdAeB8RbBwMhqg9x2ykqQWkKp0mf1zSstmFEBjnTAmcpAbMymwj16D9rkjZjpJCi+vR02OwqDCqQFz3MywPik5lJ3YxSweBfCZAqoIwWYYQDbfWRoNsLVSmDICMX8KtQAMx3roKxA8JfR1ePYazIeBTw6cBNJoM40BkjB1KgBOvUmDH8kePwgWvLDGVHk5FjFwn4HrnhBnlWcd0O4w0pIAz/GFLjUWHDmoiRnByabCZprZUA4pT4GgAKQfc5+iDS/fK5vHe46As6uYa8M5NMxQD4+VIfgZx3Bw+xICbCPl9ofUCBJCVyUpSVE5uWXuxLjr0OvPYZP9Sew9keP0iyC9MRcDA0JSwywRVItsoTrbp4kKWhV4LDDyV8sKGjoVMXFIPrkWXAUPDs/EChZ+vZfCbK3FSUcxzGTKJ3xjz1sLmbnuyXzyCX778Jf9nbm/s74jXwAAAAASUVORK5CYII=") !important;}\
	#header .specialsearches, #gbar .specialsearches { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAj1JREFUOE+FkktoU0EUhicabyo0vsBERSGCYsGFigqiCzULaRaCIjYLF2KLVKrForRQEcGKC4sLaUWKoo2vhQsXEtF2oRTbhYKBLFxELe6kYkBt1Jvcx9zfc+Zm0hsNOHBg5pz5//nOzIQACD30PBQKCdu2RTgcViXXdYVhGCJYrxNx4X/heV7DPXVJ3sSnsVk+nzcSiTWX4rHY/enpj62c49rfRv8YOI6jcul0WythUn8GVq5aQSkslVIKjiBt3UJtqBKc7bt84fTJfbg+MoSwsZgN2lnIBEGKBgQVlRvo635azAoUnjCFgAdkG7VRM2BXjWeWy+L4iTNfn2WGgIqFlo1bkEqlmELoFjVFHYEuJpPJrtFMhgVqjI+PKYpcLjcaNFHPqnvSp1uWKbZv3Wyy0LUrgEfwNBY2RRCPLZc0bQ6+Ro3Atv3bP9p5fm1H5zmlmi2V4LqsAQ4cOqIozN/WXk3Bh8/dqud/pqmb2wbfv+pXop+l73CU/gvWxSKIRnkLesldeNL/L2QgVfBiJrvzYvm5wCxFcBTePMLrAQFrTEB+6nlMnvPZQBHIqhN12/xu2CjO3CPxhED+7npcG76N7p5+rG6i3AeBzw8EipNp9t7tH6wM/IkL3Mi9LeDhyBXc6W1Bx55F2LBpF/YfbMOp9sO4dSyCiReTsEhNLSQ0tUKvvukymg9STH2z4Zn+5evx4xfwkhZXAWeH/3K+rvYtOcHteK5Nps4CesQlRBeniErXmUc5qjvV/XPf+Q+LqsCuAgxPwwAAAABJRU5ErkJggg==") !important;}\
	#header .translate, #gbar .translate { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA0pJREFUOE9dUn1MzHEYf/gjWtMifyDNmZeMtfIy/NfUphdpRpmxsam8ZCjdauQqMjJFnWQ2Lt1JonfRqWSuu1Py1paX7FRoFedyl6Ou38vH9/dLM57t2ef7/ey7z/N5nu8zCQCdLyihj9195OY+hcjNjejnDxIFkWiqO5HA0fHUveTlNY2kt5MnT6K/wc45+cWk1tSgodEkp7HlCdqMZugbjXJWVD9EzHYleJ4nl8tFEk6kIBVJTM0hfYMZ+D6EIc1V9ERvxmibGT174+E4uA+jnAshobGsOGQH/yelnCikqrpmDCQl4enCBWhfsgSdm6JgSU+HqfgWyqr0skCL+VXQo5bnfqbWDh9zawc9Nj73tzucRMpjF2QBKSzpKrzbGgNbRhoM64Ix0VrjQ/M/rdU3tKBOb5Rbo+SUc6RvMqI/+zQ65vni7cZIdC5QoDpgJeTW/sSrr0DZO+HPjZdRckZphbdlgb4tm1ChOgWzrwL9++LQ/vkLJGdsYHC5RhCS2wva9RrPeu3gftllPixyDyhVWy8L1Aetx4rLodgY4Al0fYbhVjkqKxogskrtH+wIyOrFqtM9SC4fZIwToigwgf2gLPUNWeBo5g4kW7dDcdcHadHhuDvFE3cqHkEQRaSU9iFKMwD9awfmHOmCzTHMBESsj2AOcnLHBeZFeiP8RSBW1/rK54tn1JB4MA9L0y3IabJi8JsTM5PeQ2voZ6yI4DA2g/gEFd2rb0axugiLbnjCX6lAW00rCi5rYGgywWIHZhzqxKxj3ZiVasHUw+8RfeUT0+WxLjQe9OLlG8q/UjqSm10kLktUYM3L2VgbFAKPYHfEKqOQqbVi0Xkb6rrtePDpB7KarZiR2AXrKIewiLjxzeL5MeIFgQryLlbOzCD4lXrBX+ODpUUeoFB/7MkzsYpOcA4rhh3f4HGwCyfu27Bz2wGQIPDEJsqQIwiYm6nKHJ6f5I3l5XPhp/XC4oTpqK0uZ9/GwTZkw+iv79AZBnHzqRMbwtkQ2TSZC1FGjmciwOzHlcYy7xg3KCJ8odx/FhpdFfIKrkNdqEX+JR0013TQFZdAdfLSeAuSAwklN2PcGFntg5Sw+wBcw1wJ4wMZHyjhRLLdCGQ7Kd9/AxdCsy6Zkjf5AAAAAElFTkSuQmCC") !important;}\
	#header .video, #gbar .video { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAA1tJREFUOE81U21IU1EYPuqfCoug/lSUf5IogoIICoIoovoRBEVBRVCURGYff8JMi4jKytbKzBAztbL8qll+XN1cajbd3F2zzWWaa8vphtvadNN93N3t6Zy7uvBy3nPu+7yfz5uSSCRICiEEVJielpZGNULmQiEyG5wlKamEpKcvIgvmz5Pe4/E4SU1lNgzBDoAkmFAw08cnJhfW1DeeeNuoMKs+9UDV/Rk19Q1obPpY6Z/2ZUj2/2yBRDJq/N9Dc7squ7SiKuDx/kEsFkMkGsVMcA4/fzlRq2jH5WuF0GjMaupkhRgXKZZmEYuJUuQWpSr7dV0jVQEhKiAYDCIUCiESieCPx4sPLVrcLn6LvJLTMOhHXNRsiSiKyRLcXu9CecnTQJQau90ehMNh0Mzg9/vh8/kRmJnGp14lanuL0DNzCjl3szA7E70hlcAc0BpPDOj1+DFqxfgvK/w+H01VA71eBwPPg+vg0D52G3VjxyEzbMSu2sXgFK0ixaYTQRDIs4Ymc5/NCY63YMzjQbtaDcuQGUbzMHiTCQ6rE4/rCrC9fgMy5cew4MJ9HMgrZtWuJS5BJKoj+9F75jxMu3YgsD4TWhr124gVd2RPUCgrhXXMBn7AhH1l33Gz2YfnA3OQVSuQiIs5ZDIM0pO7E4prp8AVHEH8ZDr69V9h/GHDXVkx7shLYRoexc/hQbR9d6KMtyNf40JBRQNtgVhEAgLIxzeX8PrWVmTlX4Vgz4CuIw/m4QkMDpnAD1rgGLejSF6CPYcu4mz+QzzUOZErL2clHJBI1Kn8UNlTvgqb92ajrHQ3wu4XaOW+gKdN1Gq10On6YXe4sGrNbnR1dsE+MoT6903MwXJC50Uc7lAG13wOLx6vxtJl65CV+0riQSwagRgTJG74pwNYkbkVfbyZgt/BNeXuZxOkRIpJo9ToLequ5oOoesNh5frD2LTtKK4/qEKNQoXKujZcLSzHlfsvca+4HPpBI/O5RSKSRGVKS0bP7v5RV0V1NabcThrBC61xBH0GCyZcXtjGJ6Hu7oSOToh+j0S6VAybXCbKabZljJ6TU+4bitY2kVN1wmA0UvmKNqUKLR1K2H47WNpbGJBhpBKSl+QmMq/sZAxjJKG7kBMKhYtYt1nD2D8W6P/2Mtxf2P7wzixyZEUAAAAASUVORK5CYII=") !important;}\
	#header .webhp, #gbar .webhp, #header .search, #gbar .search, #gfx-preview .search { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAkRJREFUOE+FU01oE1EQfgiCSKH4U/Dk2bMgehehghcL9pBWEw+K0ZrEgx5CKax6KNSCP42hagRBCqYVJMW2/qGlrD0Ym1hLGzAJIdAGRdOy2TRlf/I5s8mmW9vig9l9+2bmm+99Myuq1arQdV0AEHOzs6LT5fpEhlQikeMzNo5xvu1zfgvTNC3nTDze+np8HMlUCh8mJxHs7cV139Vf5Du+XbIFwI9vyeQRWZZpu77WaHsl2IMb1wKLtG02DKPBZAMD/giHw2OapoGoQjcMqJqOdElHnpznXR3IZtN+jmMQZ7LFoFwuC0mSrNIMYJLxyikVvDeAgaERDD9/9oiDba02MfB4PFAUxUqkKhYQYCJOAH0joxgbjvZvy4AdkUjkvtfrbQhAwjIdcBvuvoyhkMue4zhb8A0M6vfa43a7C/5AAKqqNoDeTk3jzcQEf+8zdK12f2qp3VZLAwfqgVAoNNV/pw9dPh/C4Yd4R8ltbWeQSf+4RMHHDJqXf2diqwHZqyrKIa7KFUajQwNP2gV+Ly0wiIC53k4GsxBtY5XrFXZVKpXCyvIyy7Ay2H0WsYsCma+xe6QOFaWc+gA2+uqkxtciay4WixrRxxI1KORvxWepBRn5aX4V2GmLumkw2GEzon1LqVR6lZj5ggePX6D71G7kI4fxPdYzr65WDqJq1kbZaU6FHQKfnlvIzrd3XIB0UkAONmH6Y5S70/Q/BnU2tSJrVQzevnkLnSeOYvHnHwbYvyWAzcgpsGHU/lq+FlkX2WWyHX8BdRwKwvFIaz8AAAAASUVORK5CYII=") !important;}\
	#header .youtube, #gbar .youtube { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAAd1JREFUOE+Nkr1LA0EQxT3BRgtNBCGFH6TQgCEi8avQQrASLCxiULEQY5EmiEWCYKUo+StEBQVFVLQQSWchSNIIFiGQKBaWCVjYWPzc2eHkTKEODDN7N/Pmvdl1gAZjOI7TILkbzTdHfvxp0lQulwWFbDZro5zl+3/cLSKTydjmWCxGtVolGAzas0SLZvJCoYDP5/sB/j3FNFkpUiT10ihA0pDL5f4GcHfxG4BI9DBQYl6dLgNhUy8hGo1aFuI/ev6zqN9qlMHFBfT2gr8derrATGNkBMbGoNucIxGIxyG7C3d38PgIp6ceCYEAFIt229b6+qCtTRW+v8PAgOYypMsAnpxAOu0BkIlNTdo8Pa0+MwPDw1CpwNUVjI4ahn44Pob+fpiY8AAMDUFjI3x86PS3N9jbg9tbODiAWk3z/X14fYXnZ63/vgVh0NICn5+q//4ednbg8BDOzmBxUePNDZRKmKcKra11AM3NKkHY5POwvg5HR3B5CSsrcH0NDw+6zI0NzBP1AIyPgwsgDJ6eFEwAhLrY+Tlsbmr+8gIdHR6ApSUIh2FyEkIhvb6pKb2+wUGYndVcaK+uQmcnLCx4AOQtbG9DMgmpFCwvw/y8FonPzUEiAWtrKmdryzaLfwFUgKPXIE+2ZAAAAABJRU5ErkJggg==") !important;}\
	#header .special, #gbar .special { background-image :url("data:image/gif;base64,R0lGODlhEAAQAPYAAE9PTwAAACgoKB0dHTc3N1dXV15eXqGhoU1NTdnZ2QICAtra2kpKSt7e3iopKSspKi0tLS8vLz09PUBAQDMzMzU1NRISEhQTFBgYGA8PDwEBAQUFBQcHByYmJicmJicnJyIiIhsbGxwbGyAgIEFBQaurq7Szs7q6uoODg6GZnaOjo7u7u8/OztrZ2uzs7L+/v8HBwcrKyoCAgFJSUlVVVWFhYUdHR0hISE5OTmJiYnR0dHh4eHl5eWdnZ2xpam5ubgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAEAALAAAAAAQABAAAAd7gECCg4SFhg4BiRs+hoQBCgEaCiIsjYIgPx4MNRYqlkAGDT0lCToLnyMEOygEEi6fQAMdAxgzsEAyOAw3B7cFBgU5DYYfFwCDEQgwgwACgyYAx0AJhDQAK4bN0SHNsBDRFTaGDxwDCACngjwU0oMTMZYvABm9KS23JCeBADs=") !important;}\
	#gbar .gfx, #gfx-gui .gfx { background-image :url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAMCAYAAABr5z2BAAAACXBIWXMAAAsSAAALEgHS3X78AAABhUlEQVR42m2SMUsDQRCFby+KiBYpxSqSS5GEwBFC4AjX29j5D1KkEm0V0yjYCqksxMI/IJja0jZICkFEUQS1ldiYy+X8Ztk9zpiF4c29mXkzO7cqDMOE48gRFFNKOZPJxBmPx7vQPctbkzOdTjWqVquVZIutH8exE0VRKiAFszm6WRAEmiE5oujeBg12KezPa5BO0Gw2rcAzZDGbbK7jgyvERoSGVoQTcEVXNRoNvQMEnkAvk6C75HK5PUROiUueLyKu63bZ0RETx6perydmpA+wk9nBG3iHLSJyAx0iMqB4i9wHBFbx28r3/X9LlAViF7htc9cKybf4eXI+KV5jqiv8bVWr1fQEkD/gMLPEPnhsr4LAPu6J+NKAWJ7vL1WtVtMlQqZLlLeQnYrvMwo7COk3Ar8B/aLK5XJiVJ8hijO/URs72KTompwF/Fe4Av47KQVVKpUSM+afCazRcQkcIFBhigP8S/ARXEbkXHmel5jF6d8459EcArKLb5qsUzyC64E7Ev8FLyiJFbdE0HsAAAAASUVORK5CYII=") !important;}\
	.login {\
		display				: inline-block;\
		font-weight			: bold;\
		vertical-align		: bottom;\
		margin-left			: 2pt;\
		margin-right		: 6pt;\
		padding-left		: 18px !important;\
		min-width			: 16px;\
		min-height			: 16px;\
		background-repeat	: no-repeat;\
		background-position	: left center;\
		background-image	: url("data:image/gif;base64,R0lGODlhEAAQAPYAAGQwFHcjCOF8G3UlCriJJv/TAHx2dpddOv/RAPKvGMZmCP7+Hv//Y4lta/LOArSMKvTLAvXQAK2HRP/oAP/qAP/uDf/aANSaC1c9Hv/6RP/gav/uCP3rAP/kZZpQCfPHA//1AJ5vNlcwL8aKIv3lFvHBEbBMCP/mHvnKAsKaHf//m///T9eCF9qoB2dYWfrjAfTdAlomGOu4Dp5bRe3BB+G1Bf/qTb2IB///hrhxJf/gAPmjMuKuJrWDDPTaA2EwFP7UCfTbA5tePHZGEvnhAb2II+mxBf/eAMNyLbKKL/vkIfvRAfbgIP/PAPTkAP/8Ar54B/OzC8CAJfHYHU8yK9GPEc+bJv//Q2FOQffZAIyKiv//kreHGvniAbGLiNWwq///HfDHAYR/f/PTA1kwPfbaAv//WF5MQ6JjO//2j6Kiov/oBM+KddqrDNixrMCHV9uNA75bBHRvcvrMBP/vAH50dIp/duiCEueKHn10c///jv///wAAAAAAAAAAAAAAACH5BAEAAHsALAAAAAAQABAAAAedgHuCg4NsLGiEiYJuSEYUEAeKg19VS0c0N3BCkntSBU1rVmRQUTOKbx8IFiBTOTwMdw2JISg6dE9gGTgaJnmJXi0cTgsrWyoCIoojMGFMZnppOwOKRS8OSlc2HXgBYokEXWMkJwkKcTFqiQ9EEUAlHgAudYpJQWUVFz9ynBI+WXMyhnASlGLChh5UDAzcw6VGGwwLBdnBckZLxD2BAAA7") !important;\
	}\
	' : '') +
	(isOpera ? '\
	' :	( GFX.mzBrd ? '\
			#colorpicker,\
			#ssb,\
			#virtual-option-list,\
			.gac_m,\
			BUTTON,\
			INPUT,\
			TABLE[class="t bt"]\
			{\
			' + CSS.cmn.mzBrd + '\
			}\
			#gbar A[href], #gbi A[href], #guser A[href], .login, #header #gb A,\
			#gfx-searchers-menu A,\
			B.gb1\
			{\
				-moz-border-radius-topleft:4px;\
				-moz-border-radius-topright:4px;\
				-moz-border-radius-bottomleft:0px;\
				-moz-border-radius-bottomright:0px;\
			}\
			#gbi,\
			#gfx-searchers-menu\
			{\
				-moz-border-radius-topleft:0px;\
				-moz-border-radius-topright:0px;\
				-moz-border-radius-bottomleft:5px;\
				-moz-border-radius-bottomright:5px;\
			}\
			.tip {\
				-moz-border-radius-topleft:3px;\
				-moz-border-radius-topright:3px;\
				-moz-border-radius-bottomleft:3px;\
				-moz-border-radius-bottomright:3px;\
			}\
			'
		: '') + '\
		#sft #gfx-logo {\
			margin-top			: 10px !important;\
		}\
		A.ch, A.sm, SPAN A.fl, A.nt, A.qs, A.qs_n {\
			-moz-opacity		: .65;\
		}\
		SPAN A.fl:hover, A.ch:hover, A.sm:hover, A.nt:hover, A.qs:hover, A.qs_n:hover {\
			-moz-opacity		: 1;\
		}\
		#gfx-logo,\
		#nav:hover, #navbar:hover\
		{\
			-moz-opacity		: .90 !important;\
		}\
		.gac_e TD\
		{\
			-moz-opacity		: .80 !important;\
		}\
		#nav, #navbar {\
			-moz-opacity		: .45;\
		}\
	') +
	(GFX.rNoAds ? '\
	#adt,\
	#ad-label,\
	#mbEnd,\
	#rhssection,\
	#rhsa,\
	#tads,\
	.ra [bgcolor],\
	.ra,\
	[id^="tba"],\
	DIV[ads],\
	DIV[id=":rh"],DIV[id=":ri"],\
	DIV[id^=tpa],\
	DIV[spl],\
	IFRAME[name="google_ads_frame"],\
	TABLE[class=lads],\
	TABLE[style=""][align="right"],\
	TABLE[width="25%"][align="right"]\
	{\
		display				: none !important;\
	}\
	' : '');

	//****************************************************
	// SERVICES STYLES
	//****************************************************
	// WebSearch Service
	if (canRun.search) {
		STR += '\
		#header TD[style^="padding: 5px; background: rgb(255, 255, 153)"],\
		#header TD[style^="background:#ff9;padding:5px;"]{\
			display				: none;\
		}\
		#res > DIV {\
			margin-left			: 0 !important;\
		}\
		#res li.g[style]{\
			margin-left			: 8px !important;\
		}\
		#res .g, #mfr {\
			margin-top			: 0;\
			margin-bottom		: 3px;\
			margin-right		: 0;\
			margin-left			: 8px;\
			'+ CSS.res.box +';\
			display				: block;\
			min-width			: 60%;\
			max-width			: 60%;\
			min-height			: '+ (parseInt(90*GFX.rThSize) + 10) +'px;\
			padding-right		: 20px;\
		}\
		#res .g .g {\
			min-width			: 95%;\
			max-width			: 95%;\
		}\
		#res .g > HR {\
			display				: none;\
		}\
		EM {\
			border				: 0;\
			'+ CSS.res.hl + ';\
			border-top			: 0;\
			border-left			: 0;\
			border-right		: 0;\
		}\
		#res .g A.l {\
			min-width			: 100%;\
			display				: block;\
			margin-right		: 20px;\
		}\
		#res .g A.l {\
			'+ CSS.res.tit + ';\
		}\
		#mfr {\
			font-size-adjust	: 0.46 !important;\
			color				: gray;\
		}\
		#res .g SPAN.cnt {\
			'+ CSS.res.cnt +';\
			display				:inline-block;\
		}\
		.g CITE, .a {\
			'+ CSS.res.url +';\
		}\
		#res .g .gl A {\
			'+ CSS.res.ref +';\
		}\
		.g .s, .std {\
			'+ CSS.res.dsc +';\
			min-width			:100%;\
		}\
		.g .f {\
			font-size-adjust	: 0.46 !important;\
		}\
		.g .qSite {\
			display				: inline-block;\
			font-size			: 9pt;\
			margin-left			: 10px;\
			color				: #55f;\
			cursor				: pointer;\
		}\
		.g .w.xsm {\
			position			: relative;\
			display				: inline-block;\
			float				: right;\
			margin-top			: ' + DB.titleSz + 'pt;\
			margin-right		: ' + (isOpera ? 20 : 0 ) +'px;\
			color				: #d00;\
		}\
		.h {\
			font-size			: 20px;\
			color				: #3366cc;\
		}\
		.p {\
			font-weight			: bold;\
		}\
		.q {\
			color				: #00c;\
		}\
		.ts {\
			border-collapse		: collapse;\
		}\
		.ts td {\
			padding				: 0;\
		}\
		.w0 A.l {\
			min-width			:90% !important;\
		}\
		.w0 H3 + SPAN {\
			float				: right;\
			margin-top			: -25px;\
			display				: none !important;\
		}\
		.w0:hover > H3 + SPAN {\
			display				: inline-block !important;\
		}\
		A.tn {\
			position			: relative;\
			display				: inline-block;\
			margin				: 4px 0;\
			border				: 1px solid gray;\
		}\
		A.tn IMG {\
			display				: block;\
		}\
		A.tn, A.tn IMG {\
			width			: '+ parseInt(120*GFX.rThSize) +'px;\
			height			: '+ parseInt(90*GFX.rThSize)  +'px;\
		}\
		' + (GFX.rThLft ?
			'A.tn { float : left; margin-right : 7px;}'
			:
			'A.tn { float : right; margin-left : 20px;}'
			) +
		'\
		A.tn:hover {\
			border-color		: blue !important;\
		}\
		A.tn:visited {\
			border-color		: #707;\
		}\
		A.tn:visited > IMG {\
			background-color	: #faf;\
			opacity				: .9;\
			-moz-opacity		: .90;\
		}\
		#gfx-searchers {\
			margin-top			: -5px;\
		}\
		#bydate, #searchIn {\
			display				: table;\
			position			: absolute;\
			right				: 5px;\
			min-width			: 50%;\
			vertical-align		: bottom;\
			text-align			: right;\
			z-index				: 123;\
			cursor				: default;\
		}\
		#bydate {\
			margin-top			: -66px;\
		}\
		#searchIn {\
			margin-top			: -23px;\
			text-shadow			: 0 0 1px #999;\
		}\
		#bydate A, #searchIn A {\
			display				: inline-block;\
			font-family			: Arial, Hevetica, sans-serif;\
			font-size			: 7pt;\
			color				: #777;\
			margin				: 0;\
			padding				: 0 2px;\
			padding-top			: 3px;\
			min-width			: 18px;\
			min-height			: 14px;\
			border				: 1px solid transparent;\
			background-repeat	: no-repeat;\
			background-position	: center center;\
			opacity				: 0.8;\
		}\
		#bydate A:hover, #searchIn A:hover {\
			color				: #000;\
			text-decoration		: none;\
			border				: 1px solid #abf;\
			opacity				: 1;\
		}\
		#searchIn A:hover {\
			text-shadow			: 0 0 1px #999;\
		}\
		#bydate .on, #searchIn .on {\
			color				: #081 !important;\
			font-weight			: bold;\
			border				: 1px solid #7da !important;\
			background-color	: #dfe !important;\
			opacity				: 1;\
		}\
		#bydate .d {\
			background-image	: url("data:image/gif;base64,R0lGODlhEAAQAPYAAP///+Dv//z+/93u/+5EIv93Ven0//P5/+72/+Tx/7t3ZtTU1Jubm/j8/4aGhmaZzJnM/9BVQNlZQPJzWdWIgKqqqnd3d9JqWtN8cNYsCtguDM0uD9UrCcwiAM4kAtEnBc9AJvVaOPdePO9HJdsxD90zEeQ6GJXI+3ir3u7u7oe67YCy5XCj1szMzGqd0I/C9f3+//7+/v3+/vj4+Pn6+vT2+BoZGPv+//H5//7///z//+Lw/9/t/+z4/+fy/9Ph8m9wctXe43FwcW1wcpqcm7zByYSEhpSWlmtsbE1NSlVUU05KSE5MSVxbXGBhYNXj7+nt7gAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAFEALAAAAAAQABAAAAfNgFGCCxULgoOFh4cMKQyKjI6HEyMOLQ4hIiIhlQ4jE4ImBQoWCgWmoqQFJoIZBBAADwSys7IZgh0eHxwaJCUlJBocHx4dgq8AADAAMjM0yMgQgicC1DBQRE1HANQCJ4IvDQ0CN0FONkLb4S+CKgcHAjg1RUpGAO4HKoIrCAgCPgA9kiDJwQ/BCkEoDBgQsCPGkyVDACg0gEIQiwQJBPAA8IMJEB0YE7AQ5CKAyZMoTboQ9GCAy5cwXT4QJGEDiAgXMFCggOFCBBAbJEQJBAA7") !important;\
		}\
		#bydate .h {\
			background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAIGNIUk0AAHolAACAgwAA+f8AAIDpAAB1MAAA6mAAADqYAAAXb5JfxUYAAAJkSURBVHjahJNNSFRhFIafe8cZR3PoQpCKoUOKoWIyIOIqichFEFyiIBCEEEIKgqBNrWpZLWoRQYsYMWxVJrSQfhZNBGWQLcwRk7ExxXQyZ8Zx9P7MvaeFOTii+ezOx3k4B773KGxDbl/UsAwdO92BtRLEToH1J475M4LBkNJPamu/Uij36pP76sKOVqn5W9oo0fbjui5riQTuyGuOfLiWIssF5SVD2wcTffQgPB5+LLM/5mWT6WhMJj+O5uuZqVmRbkSOEy5c++5lfbwvLNmsIVux7ZwYhlXwlk5nRc4i0oy+Id+5pE08vJ+ci/+S//FmoF/evRgUEZHYeEyknqRUo6lITncP1WtVNRXsxtunT/j9apjqoyEcx+FgdSW0o7GKrmIZHf5g7Z5y641blFeUk06vYRg20XPDYNNRhJ0O+gIBpse+U1FbQ2lpMQCfnj9jXVxOdnXzdXAAx7YRERzHZWn0C42+VZQMQRVrBQAruYxp2vnJ87FRqppaAAid6aKhuYlczkVRFGRtlakDDQCoOJm4lclwuL2VQKAEx3EREU5duYkqCsnkCpZlYZoWfn8xZWV+ajo7yS0tAsRVjETE/BxBRADweFQURcHv91HXWI+mBfB6vfh8Xny+IjweFY9HpaHvBEAE6fVq0kNybmZBXNeVvcjlHJmOxgRIAtpGFs6jL11tE9O0dxS2sr5uioQQ+BekfBpPE5ZeZHFhOd8cG5uSifcj+Xou/mtTvrfjn8sxdAmRnLuuS+xbTBKJlCwuLMvUaFSkh8219V2vEUDq0cihk6IDmyCAkiEORIAhKDznvwMAD1jQ8deyYngAAAAASUVORK5CYII=") !important;\
		}\
		#bydate .m {\
			background-image	: url("data:image/gif;base64,R0lGODlhEAAQAPYAAPz+/3p6eu72//P5//j8/319fWZmmXJycjNm/4/C9Ye67YCy5d0zEW1tbWtra1RUWV1dXXl5eYCAgHZ2dnBwcHR0dP9nNtEnBf9pOeQwDNguDOczDNUrCc4kAswiANsxD/9sPf9xP+43DvQ4DfE4Dfc6DOo2DsobAvszAr8VAMMYAtIfA9spCeAsCtUkBtglB/g9Dv9tPP9xQv9pOv9mNv9nN/92SP94Sf92Rv90RP90Rf9jMuQ6GP9LGP9KF/9DD/9FD/9cK/9fL/9aKf9RIf9VIgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACwAAAAAEAAQAAAHrIAoJSMkIiYbGYgtLC8uKykwNTE5NzghGJc7QUU9QCc8NDMyNjogFqZCQ0Q+PyoeHRccGh8MtQwfGhwXHR4JBAQAAQDDwsS/CQoDA8HDAwHLwcoKCwICzAACAdbB1QsBEgXhARETFQcHFA0OEA++wMXXwcfJy8XO0M8D09XX2dvaAryBE0fOHDp17NzFg4fAgAEE9K7dG9bwITVrxfxRdIjgW7gC48qdS7fuQSAAOw==") !important;\
		}\
		#bydate .w {\
			background-image	: url("data:image/gif;base64,R0lGODlhEAAQAPYAAHp6evz+/+72//P5/90zEfj8/319fdUrCdsxD9EnBf9sPeQwDOczDNguDP9kNv9xP/9rPJXI+8wiAICy5Y/C9Ye67c4kAnJycv9pOf9nNuo2Du43Dv9KF/9IFv9LGOQ6GP9BDvg9Dv9FD/9DD9IfA9UkBtglB/szAr8VAMMYAsobAvc6DPE4DfQ4DfI6DNspCdYsCuAsCv92SP92RlRUWf94Sf9xQv9xQf90Rf90RHZ2dnR0dICAgHl5eWtra11dXXBwcG1tbf9wQf9cK/9aKf9fL/9eLv9RIf9QIP9ZKP9VIv9pOv9nOf9uP/9tPP9kNP9jMv9nN/9mNgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAFMALAAAAAAQABAAAAeWgFOCg4SFhoeEJystLBsaDAuQMS8mJSQoIVFOOTUzDxifUENKHiIqH1JLNjI4ChmuRURHHCMpLk9MTTdCEA69RklIHSAwEhYJBw0IBMwECA0HCRYSEQDW19jYERQFBQEAAeHg4t0UFQMD3+EDAOnf6BUTAgLqAQIA9N/zEwA8Bv8AeujYceECkCA+ftBAxLChw4cQpwQCADs=") !important;\
		}\
		#bydate .y {\
			background-image	: url("data:image/gif;base64,R0lGODlhEAAQAPUAAGdpej9h30pasImMkJCRkomQmIyMjPtjNVNbd4SEhZWWmMqvrKGqraatrqKoq5CTn6GjpKWmpvFAE/9LG+9HHOY8E88dAM0eA+08DfNMIvdbLfBZLv9fL/dXKu1PJPJTJvFUKL7IzKO7zejQzsnIxbrDxsbGx+7W1OPl6L7V7evv8trb2+7y9uDh47i6vMTg+7W8vtK5ts61s7K1uKOzw7/Bw7y/wr6+vuHw/8nJxHSKr6fL7O3W1YGgvbbC4cfH2ywAAAAAEAAQAAAGlsAJ56DpZCQXIdGIrHg2oA8FY3FCpdQfb3GSjWIAQYDrjflEoUaJAXMA0Gq2o5dC3Vq1FQlQv+dJOzQ2ES4QMw8AgoSGDzovLCYqOThij5GTYgUKBgQJAwAImpyeoEpFR0lDp01PUVNVrVgWYmRfAFq1ZnBrbW9pvHN9eHp8dsOAioWHiYPKjZaSlAHQYpmbnZ+h16QIQQA7") !important;\
		}\
		' + (URI.query.as_occt && URI.query.as_occt!='any' ? 'em {background-color:#dde !important;}' : '') +
		(isOpera ? '\
		' : '\
			#bydate A, #searchIn A {\
				-moz-opacity		: .80;\
			}\
			#bydate A:hover, #searchIn A:hover,\
			#bydate .on, #searchIn .on {\
				-moz-opacity		: 1;\
			}\
		') +
		(GFX.rRefIcnRgt ? '\
		.g .gl {\
			position			: relative;\
			display				: none;\
			float				: right;\
			min-height			: 24px;\
			min-width			: 16px;\
			margin-top			: -14px;\
			margin-right		: -20px;\
			text-align			: right;\
		}\
		.g:hover .gl\
		{\
			display				: inline-block;\
			clear				: none;\
		}\
		.g:hover SPAN A.fl\
		{\
			display				: block;\
		}\
		.g:hover .s > A.sm {\
			float				: right;\
			margin-right		: 0;\
			margin-top			: -31px;\
		}\
		A.ch, A.sm, SPAN A.fl, A.nt {\
			margin-right		: -20px;\
		}\
		.g A.ch {\
			float				: right;\
			margin-top			: -20px;\
		}\
		BUTTON.wci {\
			display				: none !important;\
		}\
		.gl A.ch, .gl A.sm, .gl A.nt, .gl .wci\
		{\
			display				: block !important;\
			clear				: none;\
		}\
		.gl A.ch, .gl A.nt\
		{\
			margin-right		: 0;\
		}\
		SPAN A.fl {\
			display				: none;\
			float				: right;\
			margin-top			: -25px;\
		}\
		' 
		+ (isOpera ? '\
			.f + A.ch {\
				margin-right		: -50px;\
			}\
		' : '\
			#sft #gfx-logo {\
				margin-top			: 0 !important;\
			}\
			#res .g, #mfr {\
				'+ (GFX.mzBrd ? '\
					' + CSS.cmn.mzBrd + ';\
				' : '') +'\
			}\
		') : '\
			.g:hover .gl,\
			.g:hover SPAN A.fl {\
				display				: inline-block;\
			}\
		') + '\
		#res {\
			margin-left			: 0 !important;\
		}\
		#tbd {\
			background-color	: transparent !important;\
			background-position	: top left;\
			background-repeat	: repeat;\
			border				: 1px solid #68d;\
			height				: 100%;\
			margin-left			: -10px;\
			position			: fixed !important;\
			z-index				: 9090;\
		}\
		#tbd:hover {\
			background-color	: #fff !important;\
		}\
		';
	}
	// Images Service
	//-----------------
	else if (canRun.images) {
		STR += '\
		#header.fixed {\
			min-width			: 100px;\
			height				: 105px;\
		}\
		#sft .tc .f,\
		#rstc,\
		INPUT[name="btnmeta=search=search"]\
		{\
			display				: none;\
		}\
		.fl {\
			font-size			: 7pt !important;\
		}\
		#ImgContent TABLE {\
			max-width			: 100%;\
		}\
		#ImgContent B {\
			border				: 0;\
			' + CSS.res.hl + ';\
			border-top			: 0;\
			border-left			: 0;\
			border-right		: 0;\
		}\
		#ImgContent TABLE TD FONT[size] {\
			font-family			: Arial, Hevetica, sans-serif;\
			font-size			: 8pt;\
			color				: #556;\
		}\
		#ImgContent TABLE TD FONT[color],\
		#ImgContent TABLE TD FONT[color] A {\
			'+ CSS.res.url + ';\
		}\
		.gfx_info .icn {\
			position			: absolute;\
			display				: inline-block;\
			width				: 42px;\
			min-width			: 42px;\
			height				: 18px;\
			min-height			: 18px;\
			margin-top			: -13px;\
			margin-left			: 165px;\
			text-align			: right;\
		}\
		.gfx_info .icn A {\
			display				: none;\
		}\
		.gfx_info:hover .icn A,\
		TD.gfx_info:hover DIV.icn A {\
			display				: inline-block;\
		}\
		.gfx_tn {\
			display				: inline-block;\
			border				: 1px solid #99a;\
			min-width			: 16px;\
			min-height			: 16px;\
		}\
		.gfx_tn:hover {\
			background-color	: #99f;\
			border-color		: #22b;\
		}\
		.gfx_tn:visited {\
			border-color		: #909;\
		}\
		.gfx_tn:visited:hover {\
			background-color	: #fce;\
		}\
		.gfx_img SPAN {\
			position			: relative;\
			display				: inline-block;\
			visibility			: hidden;\
			text-align			: right;\
			margin-right		: -16px;\
			margin-top			: -50px;\
			padding-left		: 3px;\
		}\
		.gfx_img SPAN A {\
			display				: block;\
			width				: 16px;\
			height				: 16px;\
			background-repeat	: no-repeat;\
			background-position	: center center;\
		}\
		.gfx_img SPAN .preview {\
			background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAnJJREFUOE+Vk21IU1EYx+/UqxvrbWG1tJBaCGIzxMxEQT+kK0GnifahUDOWShK07UMvaqkUgWUGviBiRFALY6yVkqB+0PqwxjDdaI4ahg4qTU0XrI1z7/13Fn68gj7w54Fznv/veQ6HR8LQkEgkEWr18eIMzXmDXHUihWFZRhZaCnhtw4PjwwNti4s/POE60aDeyOycwsbL3ZPclXd+3HSE0DwdQsMUQYt9DcbHZp9KlZS/IeCwKrnoYoddMNp49HwneLZC0EvzfXcA7bMcunwEV1v7vHL5jjhRiKbs2mjdkB+dPg5jPI9pgcfzBQLTGof+XwQDRECvaw7JRzNqRQEl+qe/rzt4PKHFU9TsCXIYWKEwwmOIAkfo2TDhkF9UaREFFN+yBlqcBO3eIPp9IZiWCcap2UHNNqpJKqcgQFNYNSIKOF3TudI6I6DNHUQffbs1wOEj7Ro2fqWaC2c6QWZ20StRQFKa9u2ND6u45ybonCOwBHmMU4CbGn9SCbS7ZcyO2NhDFaIAmUyRpdX3BboWBPQvCTD/5fF+HeCnZt/yKnS1rVDsjsvd8CsVioRLpcYef/esH1ZqmqCaoRpxeVFTfxd19Q9QVX5utFzNNm2PluwRBUmlO9MTU069LLhg/FZa0zifp9U5DxxU31HuVxUUaM6aXxiz4GxQwpDJvpFGMbs2nEbsIlm5rdrelQNhxgBPsxK6VHZQGimJ3TSEjWDkeUdkHS5zBQSPAa6mfShLjLJGR2wBEt65k/ExjxymSghf9Ph8WwlNAvt601P831qGYY/tjXn4yVyNeUsZ6nITsSXAOiQ6K15q1p1J/5OWmj/xD7/hcC/BgyHjAAAAAElFTkSuQmCC") !important;\
		}\
		.gfx_img:hover SPAN,\
		TD.gfx_img:hover SPAN {\
			visibility			: visible;\
		}\
		.gfx_img SPAN A {\
			position			: relative;\
			display				: block;\
		}\
		.gfx_tn IMG {\
			border				: 0 !important;\
			margin				: 1px;\
		}\
		.gfx_tn:visited > IMG {\
			background-color	: #faf;\
			opacity				: 0.85;\
		}\
		.gfx_tn:visited:hover > IMG {\
			opacity				: 0.8;\
		}\
		#gfx-tbar {\
			position			: absolute;\
			display				: block;\
			height				: 18px;\
			top					: 26px;\
			margin-left			: 195px;\
			white-space			: nowrap !important;\
			font-size			: 10px;\
		}\
		#gfx-tbar .title{\
			font-size			: 13px;\
		}\
		#gfx-tbar LABEL {\
			display				: inline-block;\
			min-width			: 16px;\
			min-height			: 16px;\
			margin-left			: 2px;\
			margin-right		: 5px;\
			vertical-align		: bottom;\
		}\
		#gfx-tbar .safe {\
			display				: inline-block;\
			vertical-align		: middle;\
			margin-right		: 5px;\
			font-size			: 10pt;\
		}\
		#gfx-tbar .safe A {\
			display				: inline-block;\
			width				: 20px;\
			height				: 20px;\
			margin				: 0 2px;\
			vertical-align		: middle;\
			opacity				: 0.80 !important;\
			background-repeat	: no-repeat;\
			background-position	: center center;\
		}\
		#gfx-tbar .safe A:hover {\
			opacity				: 1 !important;\
		}\
		#gfx-tbar .safe A.on {\
			background-image	: url("data:image/gif;base64,R0lGODlhFAAUAPYAAP///wAAAADIAADJAADKAP/+/9v32wnNCQDLAArNChXQFRbQFmPfYx7RHjrWOgjNCDDVMK3uraftp6HroSfTJw3ODQDHAEnaSQTMBP/7//7+/vv++zvXOwXMBcz0zDrXOh/SHyHRIR3RHTnWOU/ITyHSITTYNEW+RTPVM0PgQ07bTiTSJCXSJSTTJBzRHAbMBgbLBgjMCAvMCwrOCgXLBQDMAADGAADOAAXKBQTLBBLXEhLPEhTPFBnQGRXPFRHPEQzMDAvOCwzNDA/ODwzODLzxvLj4uLjuuL/zv8H0wcHxwcHwwa/ur6/sr6zurLDtsLbwtrbvtrPus8XzxfH68e/57+X55ff89/3//fr9+vj9+NH10c70zsf1x9L10uP449f219b21mDeYF/eX1/cX2HeYWThZGTfZGTeZFXcVVLbUk/bT1fdV1zeXFrdWlndWWXfZYXmhYTmhH7kfqDroKntqabrpqLrom7hbm7gbmzgbHDicH3lfXXqdXHicQAAACH5BAEAAH8ALAAAAAAUABQAAAf/gH+Cg3QXKzEHFGsSg42DHmk8CAQDAwQICm9bjoITCwICCRx+exwPoD1OjhMYA0R6HloABVdTcDMCBxGDXD4DLEcAwsMFAFIgAyJggmoCQEgAGwHT0wBZAE8JAmN/dzs2fcLV4gEawmUCPwYXAzrD4wDwSzQEfC0WKe8Bw/zCJgIOHgzIAwALuX39AJCoIeSAADwJqfU7cUMGhQEjEpLjBwGgCgQwumwcCSBJDgJx6iiwYIbkNHMA3AhQ8OUPGwFBmhystgGAnRcCGAjysmBACSMJCxSQ4GJAAwODIjjEQUYJlQwZqhRpgyEUFEdMGoDqgAINAwgdQIWIwulPGDFDFShVsrTgDNS2ggzM+VChwgc5VjgFAgA7") !important;\
		}\
		#gfx-tbar .safe A.off {\
			background-image	: url("data:image/gif;base64,R0lGODlhFAAUAPcAAP///8gAAMkAAAAAAMoAAPfb280JCf7//8sAAM0KCtAWFtAVFaqqqu2np84NDQoKCg4ODu6trc0ICNc6OtY6OtUwMNpJSd9jY9c7OyYmJtEeHuuhoSsrK9MnJycnJ/77+//9/f7+/swFBQEBAcwEBMcAAPv///TMzMwAACoqKiwpKTIxMVNSUi0sLCUpKdMkJNIkJCUjI9IlJc4AANtSUgICAjMzM0NCQkA/P0tLS+BDQ0lISL5FRURERNtOTjU1Ndg0NNUzM9tPT8YAAMhPT9Y5OcsGBgYGBs4MDA0NDQ8ODs8REc8SEs4PDw8QEAgICMwGBswICAkJCc4KCswMDM0MDMwLC84LC9EcHB4dHdAZGRsaGtEdHdEhIdIhIQIDA9IfHxMTE88UFNcSEssFBcoFBQMEBBsZGc8VFcsEBPG8vPO/v7u7u+64uPi4uPDBwcjGxvXHx/PFxfHBwfTBwfC2tu6srLKuru2pqeumpqmpqbKvr+6zs++2tu2wsOyvr+6vr/X09PH29vrx8eTn5/nv7/z39/39/fz+/vv7+/34+P36+vnl5c/Pz/XR0fTOzszLy8PNzfXS0tjc3Pjj4/bX1/bW1tnX1+uiouFkZGVlZd9kZN5hYd5kZN9lZW1tbeBubuBsbGdnZ2hoaN5gYFlYWN1ZWd1XV1NTU9xVVd1aWtxfX95fX19fX95cXFNeXpWUlJmZmZSTk4mHh5GRkZOamuugoKKioqCgoJuamqCfn+aFhXJycnNycuJxceFubuJwcHJ0dOR+fuaEhOV9fZ10dOp1dQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAMcALAAAAAAUABQAAAj/AI8JHGjLAowoBjoIaTCw4cATqcQgICBAAAEEC0w5cihwg4IAARJg8AUMgwSQWuw43EBCAJJQJxQBOGBIjqcpAQxEGPgIjQAZbQAAQCR0JgA+YARwqSSQRgAqawB8AFFU6CIAfhIEYHUME5MhxgDMwgFAloctPQCEEMopwJICFgSMKeZiwABcEJL8GPAp0SVCb8gQIPaihI49pSA8uBXGA4AavQDsCAYASAAKEgSAAgCixYBGouw+0AUry4paRFBUMRDgl1AOIxhwUJJhBAtUTs684jHDSgcBRYSm+BJLig0AA0YBuMELQIXLPhAYiQNAhZlcTzJoGtDqEJxJdNIQftiFZ0GJTIJiDGCTw+4RPWsBqAqwgNKxUwGu/Il0B0AgBrRAIhUAeUARwAUCSaKAAF64UZVQBxzQABYCaFDAQBGwVsYqcwxiggmFqOEKCSHV4RAgGoAkQhCdXFCBCCB10QdHx1hCShMUVWSRAptcSKNABQgzgQMOTDAMIxwFBAA7") !important;\
		}\
		#gfx-tbar .sizes {\
			display				: inline-block;\
			min-width			: 100px;\
			vertical-align		: bottom;\
			margin-right		: 5px;\
			font-size			: 10pt;\
			z-index				: 123;\
		}\
		#gfx-tbar .sizes LABEL {\
			background-position	: 1px 1px !important;\
			background-repeat	: no-repeat !important;\
			border				: 1px solid transparent;\
			display				: inline-block;\
			font-size			: 9px !important;\
			margin				: 0;\
			padding				: 0;\
			padding-top			: 3px;\
			padding-left		: 1px;\
			padding-right		: 1px;\
		}\
		#gfx-tbar .sizes LABEL:hover {\
			border				: 1px solid gray;\
		}\
		#gfx-tbar .imgc {\
			display				: inline-block;\
			vertical-align		: middle;\
			margin-right		: 5px;\
			font-size			: 9px;\
		}\
		#gfx-tbar .imgc A {\
			display				: inline-block;\
			width				: 16px;\
			height				: 16px;\
			vertical-align		: middle;\
			border				: 1px solid transparent;\
			background-repeat	: no-repeat;\
			background-position	: center center;\
		}\
		#gfx-tbar .imgc A.on {\
			border				: 1px solid #7da;\
			background-color	: #dfe;\
		}\
		#gfx-tbar .imgc A.color {\
			background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAACXBIWXMAAAsSAAALEgHS3X78AAAC90lEQVR42qWTXWyTZRSA3yxBILAl3HihJJKIoE5n4gWYGMOFl8ZAIgrZBOaM/Cxzo7Js/IS5366VOWqgwi5AMh1kEBVTwI2tsjGR4cIK2WRrNypFtrLhVlrGvu7r957HykWnxiu9eM45ORfPuTjnKED9Hx6FKWv6UXZPHlGqVy17wZfheKdvSe/egczooUBm9FTgmatdQ8oxEbE9az0cVvqfgj9pjnalK9/TuVm/vDJYfGcbx2KVtM/s45q5j+B0NSORjTJ266mB+20q1wqezBAzPitw37+cpvrXbXp1qCDsjB3hIIc5RBnNYuNn2c69ZE2iATP8CUbXytsz1SpXYF5KoPw7l2cFXX5HrJXdNFGgy/lMamnRbvxygIdSgdz7AD3wMVaPk6nyx33xr2uyUoL00HH71vEOXOIjX77DLWdoklP8pE8zzmnEaEQCTrQnD+k8gOFcq0deVLtTgifHunv3TAUp5lcq9CCNDNCm+5LT+4jrazDair54GN1QCAdLSNTv5OZz6W0pwdKJsWiNafC+NnBYBieS9Og4v0sciY0jVy8jx48hZaXI5jys8r34Fy24OSsYikWrJmHTCFQk+fIOdP4mhEJg+U2kPYB8fg4pqkM2FmOU2umfvziYEjzRYSR3rinsFgq9Glc7NHk03WeFySTSGEXX+tAbGsH2BRP5DnzzXzo/uwW7WbX2mwT7fxDe+lTYUSfUOQSPSzO8XzArLWRLCFndhmXvYPi10sQltbw0JSj+1ly66MMHN8q+ilPigjXrNdtzhKNbhB83a8JvC/rNGFIeIlzQwqV5eT0jNScyU4JITCu14m7OsjfCwV1V09Q5obYI3NnQkgMhG8xUW4zmD3JxccNwh9qQ7P7lkKbioppbjYVKDWUvXNB/Zc2qUMK+NcJR2zRnih7gfW8U74rrZsucc1e8qj47crIvQ8dn/v4LorUyTZlbU3k3U6nOHS8/dsHzepp38N208/6SOd976pXno0DVrefF1HNF/uWZ/it/ANCI42rmR10DAAAAAElFTkSuQmCC") !important;\
		}\
		#gfx-tbar .imgc A.gray {\
			background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAACXBIWXMAAAsSAAALEgHS3X78AAACp0lEQVR42o2TS0iiURiGZTbNYqZVGLUenGFCobUWhWTgsjZdCF21apMiktGiBJWS6AYGBVYk0f1iIGgFUYImahCoeVtIJUFFf7UaGd/5zoH+KZgZZvHxw/nP977Peb9zJOVyWQLgn/WnPa9r7xa9Xq/EaDTKTCaTY2ZmJraysiJsb28Lu7u7Ub/f70gkEt8eHh7EZvYVm4+Ojj4bDAZ9f39/anx8HCSGk5MTRKNRnJ+fIx6Pl4PBYHJ/f18fi8UqWbMocHh4+GFoaEg3PDxcJCeQE5LJJDKZDG5ubvD09ISXlxcUCgWsra0VZmdn9Xd3dx/FIzgcjq9Wq/WSOV5cXCAUCiESiSCVSuH6+hqPj4+4v7/nYkSBkZGR+PLyskIUmJiYsC0sLCCfz3Nc1swoWPPz8zN3Z2LhcJj/t1gsPzs7Oy2igMfjiTH0dDrNCXK5HG5vb3lzqVTiBGdnZ1hfX0cgEIDZbIZWqw2IGezs7Ainp6fi2YvFIj83beB1dXUFZjA5OQm3242+vj5oNJqcSEDKwsHBAU+cOb0NTxAEZLNZUPoYGxvD3Nwcent70dzcnOcCDGN0dDS2tLSE4+NjljK2tra446sYjZCvT01NYXV1FT09PVCpVH6RgJCsdHng8/kwPT0NukygqXA3ukTY2Njg64uLi9jc3IRarS6RgFkkILQvra2tCbvdDjYNJsAwBwYG4HQ64XK5QGPjAQ4ODkKpVEbIoE4koPAkCoWiu7q6Oq/T6XgTC4zV/Pw89vb2eDGqxsbGbFNTUzeN+PdFYhS0+RMJdEml0nBNTU2poaEBbW1t6OjoQHt7O1paWn4Qepi+XZRX5esTePfSaIQVer2+rqqqylhbW+uVyWQpuVx+WV9f7yVsg81m+07TqXj7AP/6XP+3fgGmcsmABbcZ4QAAAABJRU5ErkJggg==") !important;\
		}\
		#gfx-tbar .imgc A.mono {\
			background-image	: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAACXBIWXMAAArvAAAK7wF9dopIAAACWklEQVR42q1Tz0tiURg1F4IrF40zjI6YkAWWQiZahliIBUk4jOI0jpQYuHGttHJ+CLOYIV20bRe4EvQPKMilMGNOQWiCpWIpaVFBm4wz370wVszQqgcX3uPdc+455ztXIHjORygUCkKh0OuVlZVvCwsLv202283MzMyNw+EoOJ3Or7Ozsy+fBEciES+Br+x2O8xmM4xGI8bGxjAyMgKv14u1tbULIn73D7ivr08QDoc/0qY7Bp6amgKdiq2tLWxvb4OUQKvVIp1OI5lMdun7MUkwGHyzvLx8zcAWiwWTk5OIx+PodDo4PT1lJ2NgYACJRAL1eh2xWOxicHDw3s7S0tIP8sfBJpMJBoMBgUAAh4eHODg4gN/v5wQbGxtotVooFAoYHx//0iMguSXmmQH1ej2XOzo6iunpaVitVsjlcp4JI2u32zg6OsLi4mK+R0Abu8QInU7HgcPDw3wxIrbm5uZ4Hs1mkytgBGT7ukdA7N2/wKGhIa6EfHJQLpfDyckJzs7OOAF7L5fLzOI9wcTERFGtVnOfGo0G6+vrfCMLsFqt9k5uNBo8xFKphPn5+V8PLXxXqVSQyWR8Aiw8tml1dRVutxupVAq1Wo2Dj4+Pkc1m0d/f//khgYwKcyWVSnlolUoFmUyGd4EVicrFfTOSYrEIGvk5FU/6qAtk44NSqbxjVnZ2dpDP5+FyuTjB5uYm981URaPRW7FY/Pa/daYg31MOlx6Ph4e3v7+P3d1d7O3t8Ub6fL5zkUjkfPJCkYpXCoUiRuPL06yvSO4lleynRCL5RL9fPOvtZc8fQAt9dmww1e4AAAAASUVORK5CYII=") !important;\
		}\
		#gfx-tbar .imgtype {\
			display				: inline-block;\
			vertical-align		: bottom;\
			margin-right		: 5px !important;\
			font-size			: 10pt;\
		}\
		#gfx-tbar .exactsize {\
			margin				: 0 !important;\
			padding-right		: 4px;\
			text-align			: right;\
			font-size			: 10px;\
		}\
		#gfx-tbar .filetype {\
			margin-left			: 4px !important;\
			margin-top			: 5px !important;\
			margin-bottom		: -1px !important;\
			text-align			: center !important;\
		}\
		#gfx-tbar .sc-block div{\
			background			: #fff;\
			float				: left;\
			font-size			: 2px;\
			margin				: 0;\
		}\
		#gfx-tbar .sc-block div:hover{\
			background:#000;\
		}\
		#gfx-tbar .sc-block div a{\
			border				: 1px solid #00c;\
			cursor				: pointer;\
			display				: block;\
			margin				: 3px;\
			height				: 16px;\
			width				: 16px;\
		}\
		#gfx-tbar .sc-block div:hover a{\
			border-color		: #fff;\
			cursor				: default;\
		}\
		#gfx-tbar .sc-icon{\
			background			: #fff;\
			float				: left;\
			font-size			: 1px;\
			margin				: 0 1px 0 5px;\
		}\
		#gfx-tbar .sc-icon div{\
			background			: #c33;\
			float				: left;\
			height				: 6px;\
			margin				: 1px 0 0 1px;\
			width				: 6px;\
		}\
		#gfx-tbar #sc-dropdown{\
			background			: #fff;\
			border				: 1px solid;\
			border-color		: #c9d7f1 #36c #36c #a2bae7;\
			margin-top			: -8px;\
			margin-left			: 22px;\
			padding				: 3px;\
			position			: absolute;\
			visibility			: hidden;\
			width				: 96px;\
			z-index				: 1;\
		}\
		#gfx-tbar .sc-show{\
			border				: 0;\
			height				: 15px;\
			width				: 15px;\
		}\
		#gfx-tbar .sc-hide{\
			border				: 1px solid #00c;\
			height				: 13px;\
			width				: 13px;\
		}\
		#gfx-tbar .sc-show div{\
			display				: block;\
		}\
		#gfx-tbar .sc-hide div{\
			display				: none;\
		}\
		#gfx-tbar .virtual-list-icon {\
			font-size			: 10px !important;\
			padding				: 1px 1px 1px 1px;\
			margin-left			: -3px !important;\
		}\
		#gfx-iprev {\
			display				: none;\
			position			: fixed;\
			border				: 1px solid black;\
			background-color	: #3B5998;\
			z-index				: 9090;\
		}\
		#gfx-iprev .high, #gfx-iprev .low {\
			position			: absolute;\
			top					: 0;\
			left				: 0;\
			margin				: 3px;\
			border				: 2px solid white;\
		}\
		#gfx-iprev .high {\
			z-index				: 1;\
		}\
		#gfx-iprev .error {\
			display				: block;\
			border				: 2px solid white;\
			font-size			: 24pt;\
			color				: #3B3978;\
			text-align			: center;\
			z-index				: 2;\
		}\
		#gfx-iprev .ratio {\
			position			: absolute;\
			top					: 0;\
			left				: 0;\
			display				: block;\
			margin				: 5px;\
			padding				: 0 5px;\
			font-size			: 24pt;\
			color				: #FFF;\
			background-color	: #777;\
			opacity				: 0.8;\
			z-index				: 2;\
		}\
		.loading {background-repeat	: no-repeat; background-position : center center; background-image : url("data:image/gif;base64,R0lGODlhIAAgAKUAAP///wAAAPr6+sTExOjo6PDw8NDQ0H5+fpqamvb29ubm5vz8/JKSkoaGhuLi4ri4uKCgoOzs7K6urtzc3D4+PlZWVmBgYHx8fKioqO7u7kpKSmxsbAwMDAAAAM7OzsjIyNjY2CwsLF5eXh4eHkxMTLCwsP///////////////////////////////////////////////////////////////////////////////////////////////////////yH/C05FVFNDQVBFMi4wAwEAAAAh+QQBCgA/ACwAAAAAIAAgAAAG/sCfcEgsKhSLonJJNBSIgqOgOGUqGZ0QVEosKBLWIqTTgQyjiuoP/QwTRx1OG63OHN3FAXkjpPePYHhEFGQOa1w/RxFuDw9FDmQUh2k/CYhDCYE/ExQUFQZEB2SgAlMLR219EUdJlRadFBcEQgUkA0oLGa0LXqhFDxqwEJphvQoRakQJDLAXeMbESwQbFKBupwWteBOCP9pFBQ7i4w6peKXopT8NB+3uB9bnR/Pz7O/t8W5o9Kjk4+ZhFqRD9w1Kt4JWFjgYAHBJlGjSPgwY8AGPnUtFEhiYOMAARFzGMiTbxPFDhiEilQjQpWoeMY4OtFlCsiDTglMKzM1UQETBNoRkOJ/46TUyk5teSfwkytntDKA/lCphxLOKJ1Q1VZsqnaQGzUeHCk46jUqLbNOxI88yYdUtCAAh+QQBCgA/ACwAAAAAIAAgAAAG/sCfcEgkJjQaQnHJJBIERAOFYigOmszPARGdVoeizgZbNBwO35+UOhx0OiJykXFoQNXeYajDKciJDmcPQmtfEG8Hf0UYZ36FPwUcHSNyBEpEBWcQeGwHb1dGCUMJAwMffm1nBAkGBgkObxREAhEKCgtCCx6lAwZ3AhgOSwYhwj8LBbYKqEMEvAMOuH/JthF3RQsTvCDTyqJkAgYDzFgLy9J/33/o2ALu7wLscvDv5sr31/P3+/wK6vr9FtBzJ6/cwHhNCiZUJCSBgnxNBPj7Q6vanwzKIA5BpqyAQmzUFGSA6LDatZFLBGSQVtHWP29GbC1YkCDBTFvkSioIxc6eNx+JDyHZIvmPCTVcQO/YysBwCNBvSRsObfqj1k4hUYVYbZr1R9enDCUydTp1SDKNXMtSlSOTYRAAIfkEAQoAPwAsAAAAACAAIAAABv7An3BIJAoajUJxySQmFkTH4eAoGppMxcATnVaHDQoDWyQMBoShlDo0UCgNcvFzhv7W35+FoknIiQVnE0J4Qg9vEH9FIGcCd14/CRoUFXICjkZnV4UQb1dEDGNCCwoKEZiEZwkCDg4CBG8bRCUjHR2fPxmlCgV2CyBKRRMWaT9uth0URQK7Cn5/BSLII4lMCbvBZCS2HAfZTAsFCqhYBh0ieXJ2f7hFC5fwl+t/8fCkzbvkZMz4/c3P9PwpeFdPwDwyBOsdHLKwScMm1/QxYQYQi4AIpSL80VVK4ihxpXr9Cbcrg75rGVGZXCIgg52LuyrGdFJqwYIET+59Q6nAyTgTIjp/8HME8mRFJiChDBVSKoMiI6WeLY3U8akQjD2FTP2B1erWr1GfMnM6ZOsPcR7FVrWqqObTIAAh+QQBCgA/ACwAAAAAIAAgAAAG/sCfcEgkLgYDQXHJNBYLyELR0WQmFBkidCAdlg6PalGgUCiF2+7PcTiUxMVIeZgeIg6NM1xIViTQUUIGbQN7T2ULP3UCDQcMcAJ6QgtlUnUDbVREEhJDlAoRklcKCwsFBaZtEEQDFRQUE0MZZQqodJJCBAhdExuvFBtjtH6GCQ2/FWFWtGpVF68aEH9VpmZ7ExQNBIY/iYaxTAuR45Hee1tI6Q7Dw7hVAx3x8h0c7LTTe/Dz8Rzi5OXcHKQbqGmJOTgH4Vxxt4QMvioC5IDaM6sMw24FmCUMl7FMBlyjQOn52DCDt4j3iKQcMqpUggSlKhEJOROmEZl9lHQE+ZBJK8dEOYV45Dakz7SgP0ZdbCKxqMUhTbkh/THVqFQsRKYqskZUGNeucBBxCwIAIfkEAQoAPwAsAAAAACAAIAAABv7An3BILCoUi6JySUwSBUdBscBcJhSZZ5QIGkyqRahCKhSTf4XBAAQuRo5D89CjdrbLxwR+LCSoHXdTR0lyC2ofbQJnQgtHVHIOalREH4iMRxGLV0g/ip1qBlwMBwcEQxlHCgV2BYtCCR5kBBCkBxBhqQp6dwIltQyhVqmTYBikDQOuRQsFfG0EByXEbXbPTAuK2YrVYAkG3+AGBLm5ykwGFOnqFBrkqbt36OvpGtja24E/BOHgpkvcVQDlu2KuyIAOB6YtEfBGQYQ7ITp04HDrXzNHAJU4oCCxQ4gBRTY5PJNBmYAMTgaM6LiByDsimxYsSJBApiMiBzhIhFmzyTVNORcXFTjAoM1FQlt+HMmSb88uOT82FWTSME7SH1XzQe10VQy8RFi0OBPSbCqvq03bDMoXBAAh+QQBCgA/ACwAAAAAIAAgAAAG/sCfcEgsKhSLonJJTBIFR0FRylQmFJlnlFhQJKrTrRCqoP7IBXAxchySzZm2Wusdi8nfOfeYfAuPEWoCZkILR2l+V2VGCkOGChGEin1Sj2lDGR8DA3k/cYdOPwWEhRlOCQabAwZhR3VzCyCqHwRMigqXYLKbDqFLC12kSwkDIMJMvmCdRQuDzoPJVQIO1NUOXa7Zx0sOB97fBw3Z2ctg3eDeDc3P0Hqi1tW5zO617lfbRAYUEOVKAmyQ5ligQEHDA2TYcEVTQmADQQoWWBG5FQmTsAEhHAgxUOEhAyKulk1akCDBAgcdOoggAkEDwYkmmxw6s0VEygETIUhQg63PNpYCHDqEcOfmSB4/Pw6khED0B8Cii4SM6MBBHhikNKP+GJDyox4oWaASotChRNNW+M4O4eMuCAAh+QQBCgA/ACwAAAAAIAAgAAAG/sCfcEgsKhSLonJJTBIFR0FRylQmFJlnlFhQJKrTrRCqoP7IBXAxchySzZm2Wusdi8nfOfeYfAuPEWoCZkILR2l+V2VFCXk/hgoRhIp9UpBpbmxIQ3GHTj8FhIUZTgtdh2FHdXqnkaJDigqYYK2OTKaLaoYFn7p6j7eDwoO9YKYFyMlkqqqvTAUD0dLRzMzOS9DT0gvDwsVVAsni19/Pvz9X10QOBwPqY5qBaggHBw0Gt628cwUQ9QcIHDBSJYnTKwMWCAhxwODfAyKqbFFid8ABAQoUGhAZ0KAekUa9Lv0Y0KHDgB8NMOJzM+CDmlNJSJpEp4GChXN2Vsk8+QMCLMaH5zQN2TmkAgUNtsD4EUJUiAGMEn5ByTK0JE8hGyhcxcnUKteXHDgI1BMEACH5BAEKAD8ALAAAAAAgACAAAAb+wJ9wSCwqFIuicklMEgVHQVHKVCYUmWeUWFAkqtOtEKqg/sgFcDFyHJLNmbZa6x2Lyd8595h8C48RagJmQgtHaX5XZUUJeT+GChGEin1SkGlubEhDcYdOPwWEhRlOC12HYUd1eqeRokOKCphgrY5MpotqhgWfunqPt4PCg72Cw8SqyblzZMrOqq9MzcoLx8S/1dbFQ9vSvz9X0UUFAw7dT5qBah4D7QS3rbzMBu0DHrNCsZJCBSEDSg4QYCrwod4EIqpsbejQwQABCBAIFDhwoAQRB/WINOrlgCGFHwYoUDDwowRFB00cKFBDgSHKkCPPNDiA4JuQAQwPCIFJ8sczAIo9f43owAETzyEMDjQQpwQCQwhDjgpxQPHDrxIdQhCRKgTCARA2i3ANyySBBg3v9AQBACH5BAEKAD8ALAAAAAAgACAAAAb+wJ9wSCwqFIuicklMEgVHQVHKVCYUmWeUWFAkqtOtEKqg/sgFcDFyHJLNmbZa6x2Lyd8595h8C48RagJmQgtHaX5XZUUJeT+GChGEin1SkGlubEhDcYdOPwWEhRlOC12HYUd1eqeRokOKCphgrY5MpotqhgWfunqPt4PCg72Cw8SqyblzZMrOqq9MzcoLx8S/1dbFQw7YvwUHHAPMq2AQIx0dI3Ody0UGFOkdFAZzuEcZohvyIyVCCRbqFSnggYoATeV+pONwYBYDChQmFBgwIBRFEERiEWGwodsQAhA3/HBw4EA3EBRn/bO1ZANEAiNLdltA0cOvIfAoQBBC0iQyT4owb1agoCFPT48/PlDcxuQBxAfcZA6ZOEDBrwEULBA5SsTAgCw3i3ANK6hBA5VgggAAIfkEAQoAPwAsAAAAACAAIAAABv7An3BILCoUi6JySUwSBUdBUcpUJhSZZ5RYUCSq060QqqD+yAVwMXIcks2ZtlrrHYvJ3zn3mHwLjxFqAmZCC0dpfldlRQl5P4YKEYSKfVKQaW5sSENxh04/BYSFGU4LXYdhR3V6p5GiQ4oKmGCtjkymi2qGBZ+6eo9MDgPDxAMOv2eDylIcHc7PHRC/ZKqqzdDO0nrU1QrCxcPHegvLyr1EBL/nVQkQGgZzULZMDxUUFBVznblKExv3FDZMmIPrSAZRDABWGDAGgbgnpMZoWiXkngYIjh4cOEBAgAMHAi4RiUVEAoN0QwpslFZgWJpTohqpgbDxpcs/WJAJcbCRISyomz8UvQLD4EADKi0HzNKEzMBGeEKSzsLzC8QBBFyARuWnM6rWrreGDV0SBAAh+QQBCgA/ACwAAAAAIAAgAAAG/sCfcEgsKhSLonJJTBIFR0FRylQmFJlnlFhQJKrTrRCqoP7IBXAxchySzZm2Wusdi8nfOfeYfAuPEWoQEEULR2l+V2VFCXk/Bh0dIwNEin1ShgppbmxIQgUhkR0kDkMFZkMLGU4LXYdFEByiB5tqrgoRqKYboiRzt45MDhQdlGqGBU5zBno/ykUEBtLTBgTNZwLZ2j8aFN7fFA/NZEflCt3g3uJ65OYK0dTS1noL2vbPXM34VQIDDaVqoARjYoDBgQMM5sQRo4QAhIMHIMwD06pcBl0PIDIAIWSBh1puVo3pVGfIwQYDzEwYMCDBgkYLMoFUpIDIhwcg+w1g5seVM65GagywlOLnx6trPwqwBFhUkS41H1g6Kfqj0zUCLCdSxdMswwAPdFB1eXqNKlIwfJoFAQA7") !important;}\
		' + (isOpera ? '\
			#sft #gfx-logo {\
				margin-top			: 0px !important;\
			}\
			#gfx-tbar .safe {\
				margin-top			: 5px;\
			}\
		' :	'\
			#gfx-searchers {\
				margin-top			: -5px;\
			}\
			#sft #gfx-logo {\
				margin-top			: 0 !important;\
			}\
			.gfx_tn {\
				-moz-border-radius-topright    : 4px; -moz-border-radius-topleft    : 4px;\
				-moz-border-radius-bottomright : 4px; -moz-border-radius-bottomleft : 4px;\
			}\
			.gfx_tn:visited > IMG {\
				-moz-opacity		: .85;\
			}\
			.gfx_tn:visited:hover > IMG {\
				-moz-opacity		: .8;\
			}\
			#gfx-tbar .safe A {\
				-moz-opacity		: .80 !important;\
			}\
			#gfx-tbar .safe A:hover {\
				-moz-opacity		: 1 !important;\
			}\
			#gfx-iprev .ratio {\
				-moz-opacity		: .80;\
			}\
		') +
		(GFX.bMnuFix ? '\
		#sft {\
			margin-top			: 0px !important;\
		}\
		#sft .tc {\
			padding-top			: 5px !important;\
		}\
		#sft .tb {\
			margin-top			: 20px !important;\
		}\
		' : '') +
		( (GFX.iCntType || GFX.imgFyleType || GFX.iSafe || GFX.iSzBar) ? '\
		#sft .tc[colspan="2"] {\
			display				: none;\
		}\
		#sft TD[width="100%"] {\
			vertical-align		: bottom !important;\
		}\
		#logo {\
			margin-top			: 0 !important;\
		}\
		' : '')+
		(GFX.iSzBar ? '\
			SELECT[name="imagesize"] {\
				display				: none;\
			}\
			#gfx-tbar .sizes INPUT[type="checkbox"] {\
			' + (isOpera ? '\
				' : '\
				display				: none;\
			') + '\
			}\
			' + 
			(GFX.iSzBarIcn ? '\
				#gfx-tbar .sizes LABEL {\
					width				: 18px;\
					height				: 18px;\
					margin				: 0;\
					padding				: 0;\
					margin-right		: 3px;\
				}\
				#gfx-tbar .sizes .icon {\
					background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAINJREFUOE9jdPL0+c/KwckAAv/+/2dgYmQCsv4z/PvLwMDEDBbGCe4+fs7AYOni/t/cyY0s7ODl/x9sgLaB0X+wtUAMYoMMJEYMboB7YOh/GKivrwcbQIwY9QxwCwj+LykrB8YgNsgFxIiNhkHw/9EwQAoDijMTKNURg138g1DUgWIBAAUa/KkMnngJAAAAAElFTkSuQmCC") !important;\
				}\
				#gfx-tbar .sizes .small {\
					background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAALVJREFUOE9jdPL0+c/KwckAAv/+/2dgYmQCsv4z/PvLwMDEDBbGCe4+fs7AYOni/t/cyY0s7ODl/x9sgLaB0X+QtaxsbP/VtHTAhhEjBjfAPTD0Pwi8f//+f39/P9gAYsSoaUDI/6PHjv/ftXvP/5raOqgLCIvBXeAWEPxfUlbuv5yyyn8HLx+wAcSIwQ2gOBYoNoCYKMMWtdSMBYrTAeEogyQuVHWDKBpBiYYY7OIfhKIO5AUAG2X61SsSg08AAAAASUVORK5CYII=") !important;\
				}\
				#gfx-tbar .sizes .medium {\
					background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAORJREFUOE9jdPL0+c/KwckAAv/+/2dgYmQCsv4z/PvLwMDEDBbGCe4+fs7AYOni/l/bwOg/SBcLK+t/Ti7u//yCQv/VtHT+mzu5gTEueQcv//9gA9wDQ/+DwJcvX/4/ffr0/8WLF//39/fDDcAlT10DHj1+/P/mrVv/z54993/Pnr3/29raUVyATR7JBSH/t+/Y+X/jps3/V6xc9X/O3Hn/yyurkAzALg83wC0g+L+krNx/WSXl/0oamv819Q3/O3j5wA3AJQ83ABbapNJwA0ajEZQSh3w0ghIKMdjFPwhFHSgdAAC+1yBkKn/P4AAAAABJRU5ErkJggg==") !important;\
				}\
				#gfx-tbar .sizes .large {\
					background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAKpJREFUOE9jdPL0+c/KwckAAv/+/2dgYmQCsv4z/PvLwMDEDBbGCe4+fs7AYOni/l/bwOg/H7/Af7BOKAaJwTAuOQcv//9gA+zcvP539vT+X7t2LRynpKX9B2F8cnAD3AND/x85fuI/Oqivr/+PT24QGeAWEIzVCyBxfHJwL4AUqevq/5eUlYNjmGZ8cnADRqPRHW9iISohDXw0Isc5PraLfxA4YcEwKB0AAPcgayxOgbWnAAAAAElFTkSuQmCC") !important;\
				}\
				#gfx-tbar .sizes .xlarge {\
					background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAXRJREFUOE+Vk01LAlEUhkdLMsyidi2M6MNIiz420U5ElEIwy35KErWotrbwF0g2UdmiFM20WkhFaG2aMVtpbkpaKCS6bt5mLiKU5p0OvHC5nPNw7gNXYV6wQ6XuZKQSAEapUIonMMIXwyjbyPWf9fr2wTDzFhuM07PQdGkhTbarVOhQq6HRdqOntw96wwTJnNnaENOiAwRgErfwswdIJBKIxeMIRyIIBE7AsizW3G6SlgCbcxV85gVSVatVlEol5HI5pNNpeL1eEiqAe85AEARUKhUUi0Vks1nwPC8fcP/wiM9yGe+FAvL5PDiOQzKZhMezS0LZwIXTUBhP0lAqhZvbO1li6xJtThd8fhaXV9eInEdxFgzJElsHWJdWMGIwol83AN3QMAb1Y5Ajlgqgia0DmgmSNqCJpQAaxUYvYgiKfra2d0iogN9iD4+Oseffx/rGJklLQDOxo8ZJjE/NiIP2Wmp/oZmDfwGkZjmxOJZ/9ElP+AZhJyPM6awnNwAAAABJRU5ErkJggg==") !important;\
				}\
				#gfx-tbar .sizes .xxlarge {\
					background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAWZJREFUOE9jdPL0+c/KwckAAv/+/2dgYmQCsv4z/PvLwMDEDBbGCe4+fs7AYOni/t/cyY0s7ODl/x9sgLaB0X8pWbn/IKsZGRn/s7CygjEnFzcY8wsKgbGiqvp/NS0dMAZZCjfABmhIckbW/zlz5oDx9BkzwHjChIlg3NbeDsa5+fn/0zIywRjFAPfA0P8Tp8/6DwJ///79/+XLFzB++vQpGF+8eBGM586d+7+/vx+MB6EBbT19/x89fvz/wcOH/2/eugXGZ8+eA+M9e/aC8cSJk/63tQHDA4gxvEChASH/S6pr/2/fsfP/tu07/m/ctBmMV6xcBcZz5s4D4/rGpv/llVVgjOaCkP9ZhcX/581f8H8uEM+cPQeMJ0+ZCsZd3b1gXFJe8T+vqBiMUQxwCwj+b2Jt918SmJik5OT/yyopg7GShiYYa+obgrG1Cyjx+IAxdQ2gOC9QbAAoDIjBLv5BKOpAmQkAU84mL1E8ZCsAAAAASUVORK5CYII=") !important;\
				}\
				#gfx-tbar .sizes .huge {\
					background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAjpJREFUOE/d0W1LU2EYB/CzadCL/ABBCtnjjLnGKq1ABuEiCbWn90G9CCshadbM6Akm9rBBNetFbpptmmVrZzsWmW3LzJLNMfbQzGTLbRBjjDE2JsH4d86N3RT0CboOP87hcK7/dbgvkVSxEz+Xl5lM6gcjlLisjBGJRIxwCc9ClZfzd5GYv69i1lRUkHdCFcSrGaax+TDaOzVIpVJEPB5HLBbDt8VFBAIBYnr6I9xuN1iWhbanh1I2tYDZf+gY1Jev4ncVi0Xk83lks1kkk0kiHA7D7/fD5XJBr9dT/1PA+e4rKJVKRKFQQC6XQyaTIQcqCAaD8Pl8cDqd0Ol0FD2Dc5pupNNpIsEf2hLfFI1GEQqFiZmZT5ia+gCO49Dbe5OiAcIa44kEIazv68ICvkQimJvzEW73e0xOvoPVaoVWy69xxV8Bse9LEAjNkfl5hPjVeTxewul0YWLiLcbGXvw7oK1DjUAwRHj5iR6vF59nZ8lkAceNw2ZjYTZbcO36DWrlD47i5Jl2vOEnCBz8x3YHBxtrx+iz58Tg4yEYTQMw9D3Axa5LFA04cfosxl+9Jli7gzRbX9ow8nSUMA0M4lG/EffuG3BB00XRgOOn2ug0y/AIzJZhDD0xo99oIgx9D3GXb751+w461J0UCVC1HkGD6gA212wjJNJaSGplqNkuh1Sxg5DX7Yaifg/qGpRobG6laMDefSqsrawi1q2vRmX1BlRt3ITqrRJii1QGiUwO2a56KJsO/qEFvwAICq3Hhj9GrAAAAABJRU5ErkJggg==") !important;\
				}\
			' :	'\
				#gfx-tbar .sizes LABEL {\
					font-size		: 9px !important;\
				}\
				#gfx-tbar .sizes INPUT {\
					margin-right	: 0;\
					margin-left		: 2px;\
				}\
			') +
		'' : '') +
		(GFX.iCntType ? '\
			#gfx-tbar SELECT {\
				display				: inline-block;\
				font-size			: 8pt;\
				margin-left			: 10px !important;\
				margin-top			: 5px !important;\
				margin-bottom		: 0 !important;\
			}\
		' : '') +
		(GFX.iColor ? '\
			td #sc-icon, td #sc-title, td #sc-title + small {\
				display				: none;\
			}\
		' : '');
	}
	// News
	else if(canRun.news) {
		STR += (hasSomeQuery ? '\
		.rightnav TABLE { max-width : 80% }\
		.story {\
			'+ CSS.res.box + ';\
			max-width			: 90%;\
			width				: ' + parseInt(DB.titleSz*36) + 'pt;\
			padding-left		: 5pt;\
		}\
		.story SPAN.cnt {\
			'+ CSS.res.cnt + ';\
			display				: inline-block;\
		}\
		.story > A[id|="u"],\
		.story .title {\
			'+ CSS.res.tit +';\
		}\
		.story .body {\
			'+ CSS.res.dsc +';\
			width				:100%;\
		}\
		.story .sources {\
			'+ CSS.res.url +';\
		}\
		.story .moreLinks {\
			'+ CSS.res.ref +';\
		}\
		.story B {\
			border				: 0;\
			'+ CSS.res.hl + ';\
			border-top			: 0;\
			border-left			: 0;\
			border-right		: 0;\
		}\
		TD.j {\
			padding				: 2pt;\
		}\
		' + (isOpera ? '\
			' : '\
			' + (GFX.mzBrd ? '\
				.story {\
					' + CSS.cmn.mzBrd + ';\
				}\
				' : '') + '\
			') + '\
		' + (GFX.bMnuFix ? '\
		#sft * {\
			margin-bottom		: 0;\
		}\
		#sft FORM {\
			margin-top			: 20px;\
		}\
		INPUT[name="btnmeta=search=search"] {\
			display				: none;\
		}\
		#search-header {\
			margin-top			: 10px;\
			margin-bottom		: 5px;\
		}\
		#search-header .search-form {\
			position			: absolute;\
			display				: inline-block;\
			padding				: 0px;\
			margin				: 0px;\
			margin-left			: 200px;\
			clean				: left;\
			top					: 0;\
			margin-top			: 50px;\
		}\
		' + (isOpera ? '\
			#sft * {\
				margin-top			: 0 !important;\
			}\
			#sft TABLE {\
				margin-top			: 5px !important;\
			}' : '') +'\
		' : '') +'\
		' + (GFX.rThShots ? '\
		.story {\
			'+ CSS.res.box +';\
			width				: ' + (GFX.rThLft ? parseInt(DB.titleSz*36) : parseInt(DB.titleSz*44)) + 'pt;\
			min-height			: ' + (parseInt(90*GFX.rThSize)+10) + 'px;\
		}\
		.story .title {\
			min-width			: 100%;\
			display				: block;\
			margin-right		: 20px;\
		}\
		A.tn {\
			position			: relative;\
			display				: inline-block;\
			margin-top			: 4px;\
			border				: 1px solid gray;\
		}\
		A.tn IMG {\
			display				: none;\
		}\
		A.tn, A.tn IMG {\
			width				: ' + parseInt(120*GFX.rThSize) + 'px;\
			height				: ' + parseInt(90*GFX.rThSize) + 'px;\
		}'
		+ (GFX.rThLft ? '\
			A.tn {\
				float			: left;\
				margin-left		: -'+ parseInt(135*GFX.rThSize) + 'px;\
			}\
			.story {\
				padding-left	: '+ parseInt(140*GFX.rThSize) + 'px !important;\
			}\
		' : '\
			A.tn {\
				float			: right;\
				margin-left		: 5px;\
			}\
			.story {\
				padding-right	: 5px !important;\
			}\
		') + '\
		A.tn:hover {\
			border-color		: blue !important;\
		}\
		A.tn:visited {\
			border-color		: #707;\
		}\
		A.tn:visited > IMG {\
			background-color	: #faf;\
			opacity				: .9;\
			-moz-opacity		: .90;\
		}\
		' : '') + '\
		' : (GFX.bMnuFix ? '\
			INPUT[name="btnmeta=search=search"] {\
				display				: none;\
			}\
			#browse-header {\
				margin-top			: 20px;\
			}\
			#browse-header .search-form {\
				display				: inline-block;\
				padding				: 0;\
				margin				: 0;\
				margin-left			: 50px;\
				margin-bottom		: 5px;\
			}\
			' : '') + '\
		');
	}
	// Books
	else if(canRun.books) {
		STR += '\
		.rsi TR {\
			'+ CSS.res.box + ';\
			display				: block;\
			width				: ' + parseInt(DB.titleSz*44) + 'pt;\
		}\
		.rsi B {\
			border				: 0;\
			'+ CSS.res.hl +';\
			border-top			: 0;\
			border-left			: 0;\
			border-right		: 0;\
		}\
		TD.j {\
			padding				: 2pt;\
		}\
		#sft TD + TD + TD {\
			width				: 100%;\
		}\
		' + (isOpera ? '\
			#header #gfx-logo {\
				margin-top			: 5px !important;\
			}\
		' : '\
			'+ (GFX.mzBrd ? '\
				.rsi TR {\
					' + CSS.cmn.mzBrd + '\
				}\
			' : '') +'\
		') + '\
		' + (GFX.bMnuFix ? '\
			#header.fixed {\
				min-height			: 70px !important;\
			}\
		' : '');
	}
	//Blogs
	else if(canRun.blogsearch)
	{
		STR += '\
		.g {\
			'+ CSS.res.box +';\
			display				: block;\
			min-width			: ' + parseInt(DB.titleSz*50) + 'px;\
			white-space			: nowrap;\
		}\
		.g DIV.cont {\
			padding-left		: 5pt !important;\
			vertical-align		: top;\
			display				: inline-block;\
			clear				: left;\
		}\
		.g SPAN.cnt {\
			'+ CSS.res.cnt +';\
			display				: inline-block;\
		}\
		.g A[id|="p"] {\
			'+ CSS.res.tit +';\
		}\
		.g .j FONT + BR + FONT {\
			'+ CSS.res.dsc +';\
		}\
		.g B {\
			border				: 0;\
			'+ CSS.res.hl +';\
			border-top			: 0;\
			border-left			: 0;\
			border-right		: 0;\
		}\
		.g A.f1 {\
			'+ CSS.res.url +';\
			display				: block;\
		}\
		INPUT#btnW {\
			display				: none;\
		}\
		#sft TD + TD + TD {\
			width				: 100%;\
		}\
		' + (isOpera ? '\
			#header #gfx-logo {\
				margin-top			: 5px !important;\
			}\
		' : '\
			'+ (GFX.mzBrd ? '\
				.g {\
					' + CSS.cmn.mzBrd + ';\
				}\
			' : '') + '\
		') + '\
		' + (GFX.bMnuFix ? '\
			#header.fixed {\
				min-height			: 100px !important;\
			}\
			'
			+ (isOpera ? '\
			#sft {\
				margin-top			: 0 !important;\
			}' : '') + '\
		' : '') + '\
		' + (GFX.rThShots ? '\
			.g {\
				min-height			: ' + (parseInt(90*GFX.rThSize) + 8) + 'px;\
				max-width			: ' + (parseInt(DB.titleSz*50) + parseInt(120*GFX.rThSize)) + 'px;\
			}\
			A.tn {\
				display				: inline-block;\
				position			: relative;\
				margin-top			: 4px;\
				border				: 1px solid gray;\
			}\
			A.tn, A.tn IMG {\
				width				: '+parseInt(120*GFX.rThSize)+'px;\
				height				: '+parseInt(90*GFX.rThSize)+'px;\
			}\
			P.g .cnt {\
				display				: inline-block;\
				position			: relative;\
			}'
			+ (GFX.rThLft ? '\
				A.tn {\
					float			: left;\
					margin-right	: 5px;\
				}\
			' : '\
				A.tn {\
					float			: right;\
					margin-left		: 5px;\
				}\
			') + '\
			A.tn:hover {\
				border-color		: blue !important;\
			}\
			A.tn:visited {\
				border-color		: #707;\
			}\
			A.tn:visited > IMG {\
				background-color	: #faf;\
				opacity				: .9;\
				-moz-opacity		: .90;\
			}\
		' : '');
	}
	// Groups
	else if(canRun.groups)
	{
		STR += '\
		#logo {\
			float				: left;\
		}\
		#sft .tb {\
			margin-top			: 16px !important;\
		}\
		#res > BR {\
			line-height			: 3px;\
		}\
		.g {\
			'+ CSS.res.box + ';\
			display				: block;\
			max-width			: ' + parseInt(DB.titleSz*50) + 'px;\
		}\
		.g TABLE {\
			min-width			: 100%;\
		}\
		.g > A {\
			' + CSS.res.tit + ';\
		}\
		.g .j {\
			' + CSS.res.dsc + ';\
			font-size-adjust	: 0.58 !important;\
		}\
		.g SPAN.cnt {\
			' + CSS.res.cnt + ';\
			display				: inline-block;\
		}\
		.g B {\
			border				: 0;\
			' + CSS.res.hl + ';\
			border-top			: 0;\
			border-left			: 0;\
			border-right		: 0;\
		}\
		.g .a {\
			' + CSS.res.url + ';\
			display				: block;\
			font-size-adjust	: 0.50 !important;\
		}\
		.g .f {\
			font-size-adjust	: 0.56 !important;\
		}\
		.g .f A {\
			font-size-adjust	: 0.72 !important;\
		}\
		#guser B {\
			font-size			: 10pt;\
		}\
		' + (isOpera ? '\
			#header #gfx-logo {\
				margin-top			: 3px !important;\
			}\
		' : '\
			'+ (GFX.mzBrd ? '\
				.g {\
					' + CSS.cmn.mzBrd + ';\
				}\
			' : '') +'\
		') + '\
		' + ((GFX.bMnuFix && hasSomeQuery) ? '\
			#header.fixed {\
				min-height			: 100px !important;\
			}\
			#myg_popup {\
				z-index				: 999 !important;\
			}\
			' + (isOpera ? '\
				#sft {\
					margin-top			: 0 !important;\
				}\
			' : '') + '\
		' : '\
			BODY {\
				margin-top			: 20px !important;\
			}\
			#guser {\
				margin-top			: -24px !important;\
			}\
		');
	}
	// Scholar
	else if(canRun.scholar)
	{
		STR += '\
		.g {\
			'+ CSS.res.box +';\
			display				: block;\
			min-width			: 63%;\
			max-width			: 63%;\
			padding-right		: '+(isOpera?6:20)+'px;\
		}\
		.g A.w {\
			min-width			: 100%;\
			display				: block;\
			margin-right		: 20px;\
		}\
		.g A.w {\
			'+ CSS.res.tit + ';\
		}\
		.g > FONT {\
			'+ CSS.res.dsc +';\
			min-width			: 100%;\
		}\
		.g > FONT B {\
			border				: 0;\
			'+ CSS.res.hl +';\
			border-top			: 0;\
			border-left			: 0;\
			border-right		: 0;\
		}\
		' + (isOpera ? '\
		' : '\
			'+ (GFX.mzBrd ? '\
				.g {\
					' + CSS.cmn.mzBrd + ';\
				}\
			' : '') +'\
		') + '\
		' + (GFX.bMnuFix ? '\
			#header.fixed {\
				min-height			: 100px !important;\
			}\
			#header .gbar {\
				position			: relative;\
			}\
			#sft {\
				margin-top			: 22px !important;\
			}\
		' : '');
	}
	// Products
	else if(canRun.products)
	{
		STR += (GFX.bMnuFix ? '\
		#ps-vertical-ads {\
			display				: none !important;\
		}\
		#header.fixed {\
			min-height			: 110px !important;\
		}\
		' + (isOpera ? '\
			#sft {\
				margin-top			: 0 !important;\
			}' : '') + '\
		' : '');
	}
	// Finance
	else if(canRun.finance)
	{
		STR += '\
		BODY {\
			margin-top			: 20px !important;\
		}\
		#gbar {\
			margin-top			: -4px !important;\
		}\
		#guser {\
			margin-top			: -24px !important;\
		}\
		' + (isOpera ? '\
			#gfx-logo {\
				margin-top			: 6px !important;\
			}\
		 ' : '\
			#gfx-logo {\
				margin-top			: 8px !important;\
			}\
		 ');
	}
	// Videos
	else if(canRun.video)
	{
		STR += '\
		TABLE#resultsheadertable {'
			+ CSS.cmn.mzBrd + '\
		}\
		BODY {\
			margin-top			: 1px;\
		}\
		#gbar {\
			position			: absolute;\
		}\
		#gbi A.gb2, #gbi B.gb2 {\
			display				: inline-block !important;\
		}\
		#guser {\
			font-size			: 13px !important;\
			font-weight			: bold;\
		}\
		#header {\
			position			: relative;\
			min-height			: 75px !important;\
			margin-bottom		: -2px !important;\
		}\
		#upload-control {\
			margin-top			: 15px;\
		}\
		.table-header DIV A,\
		.site-filter {\
			font-size			: 10pt;\
		}\
		.search-options * {\
			font-size			: 8pt;\
		}\
		.header-search {\
			margin-top			: 25px;\
		}' + (isOpera ? '\
			#header #gfx-logo {\
				margin-top			: 3px !important;\
			}\
			' : '') + '\
		' + (GFX.bSrchs ? '\
			#videoheader {\
				margin-bottom		: 0 !important;\
			}\
		' : '');
	}
	// Translate
	else if(canRun.translate)
	{
		STR += '\
		#source {\
			height				: 210px;\
		}\
		BODY {\
			margin-top			: 24px;\
		}\
		TABLE + BR + H1,\
		#content\
		{\
			margin-top			: -15px;\
		}\
		';
	}
	// Maps
	else if(canRun.maps || canRun.local)
	{
		STR += (GFX.bMnuFix ? '\
			#loadmessage, #slowmessage {\
				position			: absolute;\
				margin-top			: 25px;\
			}\
			#gbi A {\
				display				: inline-block !important;\
			}\
			#search {\
				margin-top			: 20px !important;\
			}\
		' + (isOpera ? '\
			#header #gfx-logo {\
				margin-top			: 5px !important;\
			}\
			' : '') + '\
		' : '');
	}
	// iGoogle
	else if(canRun.ig)
	{
		STR += '\
		BODY {\
			width				: 100%;\
		}\
		#gbar, #guser {\
			min-height			: 18px;\
			margin-top			: -4px !important;\
		}\
		#gbar {\
			position			: relative;\
			z-index				: 1001;\
		}\
		';
	}
	// GMail
	else if(canRun.mail)
	{
		STR += '\
		#gbar .gb1, #guser A {\
			margin			: -2px !important;\
		}\
		#guser B {\
			font-size		: 14px !important;\
		}\
		';
	}
	//  Calendar
	else if(canRun.calendar)
	{
		STR += '\
		#gbar {\
			position		: relative;\
			margin-top		: -4px !important;\
		}\
		.topCtrls {\
			padding-left	: 20px;\
		}\
		' + (isOpera ? '\
			#gfx-logo {\
				margin-top			: 5px !important;\
			}\
		' : '');
	}
	//  Docs
	else if(canRun.docs || canRun.spreadsheets)
	{
		STR += '\
		#gbar {\
			position			: absolute;\
			margin-top			: -4px !important;\
		}\
		#guser, #id_userinfo {\
			position			: absolute;\
			top					: 0;\
			right				: 0;\
			font-size			: 13px !important;\
			font-weight			: bold;\
		}\
		#masthead {\
			margin-top			: 20px;\
		}\
		#logo-section TD {\
			width				: auto;\
		}\
		#search-section TD {\
			width				: 100%;\
		}\
		#gfx-logo {\
			margin-top			: 5px !important;\
		}\
		';
	}
	//  Reader
	else if(canRun.reader)
	{
		STR += '\
		#gbar {\
			position		: relative;\
			margin-top		: -4px !important;\
		}';
	}
	// Google Chrome
	else if(canRun.chrome)
	{
		STR += (GFX.bMnuFix ? '\
			BODY {\
				margin-top			: 0;\
			}\
			#header {\
				position			: relative;\
				display				: inline-block;\
				min-height			: 30px;\
				padding-top			: 24px;\
			}\
			#container {\
				width				: 70%;\
			}\
		' : '');
	}
	// Picasa Web
	else if(canRun.picasaweb)
	{
		var $path = doc.location.pathname.split('/');
		var isLH = ($path[0] == 'lh');
		var isAlbum = !isLH && $defined($path[1]);
		
		STR += (isAlbum ? '\
		.gphoto-topnav {\
			margin-top			: 24px;\
		}\
		#guser A.lhcl_newfeatures, .lhcl_browserwarning {\
			display				: none;\
		}\
		.goog-icon-list-tooltip {\
			border				: 1px solid #888;\
			overflow			: hidden;\
			margin-top			: -35px;\
			max-height			: 45px;\
		}\
		.goog-icon-list-icon-img-div A {\
			display				: inline-block;\
			border				: 1px solid gray;\
			-moz-border-radius-topright		: 4px; -moz-border-radius-topleft	: 4px;\
			-moz-border-radius-bottomright	: 4px; -moz-border-radius-bottomleft : 4px;\
		}\
		.goog-icon-list-icon-img-div A:visited {\
			border-color		: violet;\
		}\
		.goog-icon-list-icon-img-div A:hover {\
			border-color		: #33c !important;\
			background-color	: #aaf !important;\
		}\
		.goog-icon-list-icon-img-div A:visited:hover {\
			border-color		: #a58 !important;\
			background-color	: #f9b !important;\
		}\
		.goog-icon-list-icon-img-div A IMG {\
			border				: 0;\
		}\
		.goog-icon-list-icon-img-div A:hover IMG {\
		}\
		.goog-icon-list-icon-img-div A:visited IMG {\
			opacity				: 0.7;\
			-moz-opacity		: .70;\
		}\
		.goog-icon-list-icon-img-div A:visited:hover IMG {\
			opacity				: 0.8 !important;\
			-moz-opacity		: .80 !important;\
		}\
		.goog-icon-list-searchicon-meta-by A {\
			font-weight			: bold;\
		}\
		#gbar {\
			margin-top			: -3px;\
		}\
		#guser {\
			margin-top			: -23px !important;\
		}\
		' + (GFX.iPrev ? '\
			#gfx-iprev {\
				display				: none;\
				position			: fixed;\
				border				: 1px solid black;\
				padding				: 3px;\
				background-color	: #3B5998;\
				z-index				: 9090;\
			}\
			#gfx-iprev IMG {\
				border				: 2px solid white;\
				opacity				: 0.95;\
				-moz-opacity		: .95;\
			}\
			#gfx-iprev .error {\
				display				: block;\
				border				: 2px solid white;\
				font-size			: 24pt;\
				color				: #3B3978;\
				text-align			: center;\
			}\
			#gfx-iprev .ratio {\
				position			: absolute;\
				top					: 0;\
				display				: block;\
				margin				: 10px;\
				font-size			: 24pt;\
				color				: #FFF;\
				opacity				: 0.9;\
				-moz-opacity		: .90;\
			}\
			' : '') + '\
		' : '');
	}

	// Opacity Backgrounds
	STR += '\
		.op40_black,\
		#gfx-gui-dragger:hover,\
		#gfx-BgBox\
		{background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAYAAACNMs+9AAAABGdBTUEAANbY1E9YMgAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAA/SURBVHjaYmRgYEhjIAIABBATA5EAIICIVggQQEQrBAggohUCBBDRCgECiGiFAAFEtEKAACJaIUAAEa0QIMAAs2IAetPOw3cAAAAASUVORK5CYII=");}\
		.op75_black\
		{background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAYAAACNMs+9AAAABGdBTUEAANbY1E9YMgAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAABASURBVHjaYmRgYNjPQAQACCAmBiIBQAARrRAggIhWCBBARCsECCCiFQIEENEKAQKIaIUAAUS0QoAAIlohQIABAECXANObzc9jAAAAAElFTkSuQmCC") !important;}\
		.op_white,\
		#gbi,\
		#tbd,\
		#gfx-searchers-menu\
		{background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAAAlwSFlzAAALEQAACxEBf2RfkQAAABdJREFUGFdj/P///zMGdAASRMcMlAkCAFPzUaF4TK5sAAAAAElFTkSuQmCC") !important;}\
	';

	var el = doc.createElement('style');
	el.type = 'text/css';
	el.innerHTML = STR;
	head.appendChild(el);
	delete STR;

}; // END GFX_Styles function








//  GUI Settings Styles
//----------------------
var GFX_ui_Styles = function(){
	STR = '\
	#gfx-gui {\
		color				: black;\
		background-color	: white;\
		border				: 2px solid #000;\
		display				: none;\
		max-width			: 780px;\
		min-width			: 780px;\
		position			: fixed;\
		text-align			: left;\
		z-index				: 9000;\
	}\
	#gfx-gui A {\
		cursor				: pointer !important;\
		text-decoration		: none !important;\
	}\
	#gfx-gui-dragger {\
		cursor				: move;\
		display				: block;\
		margin				: 1px;\
		min-height			: 63px;\
		min-width			: 778px;\
		position			: absolute;\
		width				: 778px;\
		z-index				: 9001;\
	}\
	#gfx-gui .header {\
		display				: block;\
		border-bottom		: 1px solid #999;\
		margin				: 0 10px;\
		margin-top			: 10px;\
	}\
	#gfx-gui .header A {\
		color				: #000 !important;\
	}\
	#gfx-gui .header SPAN {\
		display				: inline-block;\
		height				: 83px;\
	}\
	#gfx-gui .header .subtitle {\
		color				: #555;\
		cursor				: default;\
		font-size			: 10pt;\
		font-weight			: bold;\
		height				: 77px;\
		position			: absolute;\
		text-align			: right;\
		top					: 0;\
		width				: 97%;\
		padding-top			: 15px;\
	}\
	#gfx-gui .header .subtitle H1 {\
		background-color	: transparent;\
		display				: block;\
		margin				: 0px;\
		padding				: 0px;\
	}\
	#gfx-gui .header .subtitle A {\
		background-color	: #dfe;\
		border				: 1px solid #0b0;\
		display				: block;\
		float				: right;\
		margin-top			: 20px;\
		padding				: 1px 5px;\
		text-align			: center;\
		width				: 200px;\
	}\
	#gfx-gui .header .subtitle A:hover {\
		background-color	: #bed;\
		border				: 1px solid #0b0;\
		color				: #009 !important;\
	}\
	#gfx-gui #gfx-logo {\
		color				: #333;\
		display				: inline-block;\
		font-size			: 12pt;\
		font-weight			: bold;\
		margin				: 0 !important;\
		min-height			: 22px;\
		min-width			: 50px;\
		padding-top			: 25px;\
		position			: relative;\
	}\
	#gfx-gui a.gfx {\
		background-position	: left top;\
		background-repeat	: no-repeat;\
		color				: #333 !important;\
		display				: inline-block;\
		font-weight			: bold;\
		margin-left			: 0px;\
		margin-top			: 10px;\
		min-height			: 20px;\
		min-width			: 20px;\
		padding-top			: 13px;\
		position			: relative;\
	}\
	#gfx-gui a.gfx span {\
		margin-left			: 20px;\
	}\
	#tabmenu {\
		font				: bold 12px Verdana;\
		list-style-type		: none;\
		margin-left			: 10px;\
		margin-top			: 72px !important;\
		padding				: 1px 0;\
		position			: absolute;\
		top					: 0;\
		vertical-align		: bottom;\
		z-index				: 100;\
	}\
	#tabmenu LI {\
		display				: inline;\
	}\
	#tabmenu LI span {\
		vertical-align		: top;\
		display				: inline-block;\
		max-height			: 13px;\
		background-color	: #f6ffd5;\
		border				: 1px solid gray;\
		color				: #555;\
		padding				: 3px 7px;\
		cursor				: pointer;\
	}\
	#tabmenu LI span:hover {\
		background-color	: #DBFF6C;\
		color				: #003;\
	}\
	#tabmenu LI span:active {\
		color				: #000;\
	}\
	#tabmenu LI.selected span {\
		background-color	: #DBFF6C;\
		border-bottom		: 1px solid #fff;\
		border-top			: 2px solid #096;\
		color				: #003;\
		padding-top			: 5px;\
		position			: relative;\
		margin-top			: -5px;\
	}\
	#gfx-gui .tabs {\
		cursor				: default;\
		display				: inline-block;\
		height				: 400px !important;\
		max-height			: 400px !important;\
		min-height			: 400px !important;;\
		position			: relative;\
		width				: 380px;\
	}\
	#gfx-gui H1 {\
		color				: #777;\
		display				: inline-block;\
		font-size			: 22pt;\
		font-weight			: bold;\
	}\
	#gfx-gui FIELDSET {\
		border				: 1px solid gray;\
		display				: none;\
		height				: 380px !important;\
		margin				: 0 15px;\
		margin-bottom		: 4px;\
		min-height			: 380px !important;\
		min-width			: 360px;\
		padding				: 0 5px;\
		padding-top			: 5px;\
		position			: absolute;\
	}\
	#gfx-gui .tabs FIELDSET {\
		padding-bottom		: 3px;\
	}\
	#gfx-gui .tabs FIELDSET * {\
		font-family			: Arial,hevetica,sans-serif;\
		font-size			: 9pt;\
		font-weight			: normal;\
		height				: auto;\
		margin				: 0;\
		padding				: 0;\
		text-align			: left;\
		vertical-align		: top;\
		width				: auto;\
	}\
	#gfx-gui FIELDSET HR {\
		border				: none;\
		border-bottom		: 1px solid gray;\
		margin				: 6px 0 !important;\
	}\
	#gfx-gui FIELDSET SPAN.subtitle {\
		display				: inline-block;\
		font-size			: 12pt;\
		font-weight			: bold;\
		margin-bottom		: 5px;\
	}\
	#gfx-gui .about {\
		overflow-y			: auto;\
		text-align			: center !important;\
	}\
	#gfx-gui .about * {\
		background-color	: transparent;\
		text-shadow			: 0 1px 5px #555;\
		text-align			: center !important;\
	}\
	#gfx-gui .about h1,\
	#gfx-gui .about h2\
	{\
		display				: block;\
		font-weight			: bold;\
		color				: #000;\
	}\
	#gfx-gui .about h1 {\
		font-size			: 11pt;\
	}\
	#gfx-gui .about h2 {\
		font-size			: 10pt;\
	}\
	#gfx-gui .about h3 {\
		color				: #777;\
	}\
	#gfx-gui .btns {\
		display				: block;\
		height				: 11pt;\
		margin				: 3px 0 10px 0;\
		text-align			: center;\
		width				: 100%;\
	}\
	#gfx-gui .btns BUTTON {\
		text-align			: center;\
		width				: 80px;\
	}\
	#gfx-gui FIELDSET LEGEND {\
		color				: #005;\
		font-size			: 14pt !important;\
		font-weight			: bold !important;\
		margin-left			: 5px;\
		padding				: 0 1px;\
	}\
	#gfx-gui LABEL {\
		border				: 1px solid transparent;\
		color				: #333;\
		cursor				: pointer !important;\
		display				: block;\
		padding-left		: 2px;\
	}\
	#gfx-gui LABEL.sub {\
		font-size			: 8pt;\
		margin-left			: 20px;\
	}\
	#gfx-gui LABEL:hover {\
		color				: #000;\
		background-color	: #ffd;\
		border				: 1px solid #990;\
	}\
	#gfx-gui LABEL.disabled,\
	#gfx-gui LABEL.disabled:hover,\
	#gfx-gui LABEL.disabled * {\
		background-color	: #fff;\
		color				: #aaa;\
	}\
	#gfx-gui .services LABEL {\
		display				: inline-block;\
		min-width			: 115px;\
	}\
	#gfx-gui INPUT[type="checkbox"] {\
		display				: inline-block;\
		margin-right		: 5px;\
		margin-top			: 1px;\
	}\
	#gfx-gui INPUT[type="text"],\
	#gfx-gui SELECT {\
		max-height			: 21px;\
		padding-left		: 4px;\
	}\
	#gfx-gui INPUT[type="number"] {\
		cursor				: text;\
		font-weight			: bold;\
		padding-right		: 7px;\
		text-align			: right;\
		width				: 26px;\
	}\
	#gfx-gui OPTION {\
		display				: block;\
	}\
	#gfx-gui TABLE TR {\
		font-size			: 10pt;\
	}\
	#gfx-gui TABLE TD {\
		white-space			: nowrap;\
	}\
	.gfx-css-styles * {\
		font-size			: 8pt !important;\
	}\
	.gfx-css-styles INPUT {\
		font-size			: 8pt;\
		height				: 12pt;\
		min-width			: 65px;\
		padding				: 0 4px;\
	}\
	#gfx-gui FIELDSET BUTTON {\
		padding				: 0 4px !important;\
		font-weight			: bold !important;\
	}\
	#gfx-css-list {\
		margin-left			: 10px;\
		width				: 180px;\
	}\
	#gfx-gui .virtual-list-icon {\
		padding				: 1px 1px 0px 1px !important;\
		margin-left			: -6px !important;\
	}\
	#gfx-gui #virtual-list {\
		margin-top			: 2px;\
	}\
	#gfx-gui #virtual-list SPAN {\
		padding				: 2px;\
		padding-left		: 3px;\
		padding-right		: 5px;\
		text-align			: right;\
	}\
	#gfx-gui #colorpicker {\
		margin-top			: 2px;\
		margin-left			: -30px;\
	}\
	#gfx-gui #colorpicker SPAN {\
		margin-left			: 2px;\
	}\
	#gfx-gui #colorpicker IMG {\
		padding				: 7px;\
	}\
	#gfx-preview {\
		display				: inline-block;\
		position			: relative;\
		vertical-align		: top;\
		clear				: left;\
		width				: 380px;\
		max-width			: 380px;\
		overflow-x			: hidden;\
	}\
	#gfx-preview * {\
		border				: 1px solid transparent;\
	}\
	#gfx-preview FIELDSET {\
		background-color	: white;\
		'	+ CSS.cmn.body	+'\
		position			: relative !important;\
		border				: 2px solid #777;\
		display				: block;\
		margin				: 0;\
		padding				: 0;\
		margin-left			: 5px;\
		overflow			: auto;\
		min-height			: 386px !important;\
		height				: 386px !important;\
	}\
	#gfx-preview EM {\
		border-top			: 0 !important;\
		border-left			: 0 !important;\
		border-right		: 0 !important;\
	}\
	#gfx-preview UL {\
		font-size			: 10pt;\
		font-weight			: bold;\
		list-style-type		: none;\
		margin				: 0;\
		padding				: 0;\
	}\
	#gfx-preview LI {\
		display				: inline;\
		background-repeat	: no-repeat;\
		padding-left		: 18px;\
		margin				: 0;\
	}\
	#gfx-preview .res_box	{'	+ CSS.res.box	+';padding-right:20px;}\
	#gfx-preview .res_cnt	{'	+ CSS.res.cnt	+'}\
	#gfx-preview .res_tit	{'	+ CSS.res.tit	+';color:'+ DB.Color.link +';}\
	#gfx-preview .res_dsc	{'	+ CSS.res.dsc	+'}\
	#gfx-preview .res_hl 	{'	+ CSS.res.hl	+';\
		border-top:0 !important; border-left:0 !important; border-right:0 !important;}\
	#gfx-preview .res_url	{'	+ CSS.res.url	+'}\
	#gfx-preview .res_ref	{'	+ CSS.res.ref	+'}\
	#gfx-preview .res_ref A {\
		display 	: inline-block;\
	}\
	#gfx-preview .res_ref.repos {\
		float 		: right;\
		margin-top	: -30px;\
	}\
	#gfx-preview .res_ref.repos A {\
		display 	: block;\
	}\
	#gfx-preview .cmn_btns		{'	+ CSS.cmn.btns	+'}\
	#gfx-preview .cmn_btnsOv	{'	+ CSS.cmn.btnsOv+'}\
	#gfx-preview-tn {\
		background-color	: #aaa;\
		border				: 1px solid #559;\
		display				: inline-block;\
		margin-bottom		: 4px;\
		margin-right		: 6px;\
		margin-top			: 4px;\
		position			: relative;\
	}\
	#gfx-preview-tn {\
		display				: '+ (GFX.rThShots?'inline-block':'none') +';\
		min-height			: '+ parseInt(90*GFX.rThSize)	+'px;\
		min-width			: '+ parseInt(120*GFX.rThSize)	+'px;\
	}\
	#gfx-preview-tn { '+ (GFX.rThLft ? '\
		float				: left;\
	' : '\
		float				: right;\
		margin-left			: 5px;\
	') + '}\
	PV_hd_box {\
		display				: block;\
	}\
	#PV_res_box span {\
		display				: inline-block;\
		border				: 1px solid transparent;\
	}\
	#PV_res_box { min-height:'	+ parseInt(90*GFX.rThSize+20) +'px;}\
	#PV_res_cnt {\
		display				: '+ (GFX.rCounter ? 'inline-block' : 'none') +';\
	}\
	' + (isOpera ? '\
		#gfx-gui #tabmenu {\
			margin-top			: 61px;\
		}\
	' : (GFX.mzBrd ? '\
			#gfx-gui,\
			#gfx-gui .tabs FIELDSET,\
			#gfx-gui-dragger,\
			#gfx-preview .res_box,\
			#gfx-preview .cmn_btns,\
			#gfx-preview .cmn_btnsOv\
			{' + CSS.cmn.mzBrd + '}\
		' : '') + '\
		');

	var el = doc.createElement('style');
	el.type = 'text/css';
	el.innerHTML = STR;
	head.appendChild(el);
	STR = null;
};








// =================
// Global GFX Scripts
// =================
var GFX_Scripts = function(){
	var STR = '';
	//  GUI Settings Script
	//--------------------------------
	STR += '\
	var win = window, doc = document;\
	if (typeof($el) != "function")\
		var $el = function(el){\
			return doc.getElementById(el);\
		};\
	var cssOp = function(target){\
		var name = target.id.replace(/PV/,"CSS").replace(/_/g,".");\
		var list = $el("gfx-css-list");\
		var x = list.options.length;\
		while(x--){\
			if(list.options[x].id==name){\
				list.selectedIndex = x;\
				break;\
			}\
		}\
		setTabMnu(4);\
		return false;\
	};\
	var lastTarget=null;\
	var addList = function (target,options,style){\
		if(!target || !options) return;\
		var lst = $el("virtual-list");\
		if(lst){\
			doc.removeEventListener?doc.removeEventListener("click",closeList,false):doc.detachEvent("onclick",closeList);\
			lst.parentNode.removeChild(lst);\
			if(lastTarget == target) return;\
		}\
		lastTarget=target;\
		var input;\
		lst = doc.createElement("div");\
		lst.id = "virtual-list";\
		if(style) lst.setAttribute("style",style);\
		if(target.nodeName=="input"){\
			input = target;\
			lst.style.marginTop = input.offsetHeight + "px";\
		} else {\
			input = target.previousSibling;\
			lst.style.marginLeft =  -(input.offsetWidth+(window.opera?-4:-8)) + "px";\
		}\
		lst.style.minWidth = (input.offsetWidth+(window.opera?-9:1)) + "px";\
		var setValue = function(ev){\
			input.value = ev.target.innerHTML;\
			input.focus();input.blur();\
		};\
		var db = options.split(",");\
		var len = db.length;\
		for (var x=0; x < len; x++)\
		{\
			var p = doc.createElement("span");\
			p.innerHTML = db[x];\
			p.addEventListener("mousedown", setValue, false);\
			if(input.value==db[x]) p.className="selected";\
			lst.appendChild(p);\
		}\
		target.appendChild(lst);\
		window.setTimeout(function(){doc.addEventListener?doc.addEventListener("click",closeList,false):doc.attachEvent("onclick",closeList);},250);\
		return false;\
	};\
	var closeList = function() {\
		var lst = $el("virtual-list");\
		if (!lst) return;\
		doc.removeEventListener?doc.removeEventListener("click",closeList,false):doc.detachEvent("onclick",closeList);\
		lst.parentNode.removeChild(lst);\
		lastTarget=null;\
	};\
	var getColor = function(ev) {\
		var img = ev.target;\
		var cL = 0, cT = 0;\
		var p = img;\
		do {\
				cL += p.offsetLeft;\
				cT += p.offsetTop;\
		} while (p = p.offsetParent);\
		if(window.opera)\
		{\
			cL -= 22;\
			cT -= 30;\
		}\
		var x = ev.pageX - cL - 22;\
		var y = ev.pageY - cT - 42;\
		if(x<0) x = 0;\
		else if(x>100) x = 100;\
		if(y<0) y = 0;\
		else if(y>192) y = 192;\
		var Rmx = 0, Gmx = 0, Bmx = 0;\
		if(y <= 32) { Rmx = 255; Gmx = (y / 32) * 255; Bmx = 0;\
		} else if(y <= 64) {\
			y = y - 32; Rmx = 255 - (y / 32) * 255; Gmx = 255; Bmx = 0;\
		} else if(y <= 96) {\
			y = y - 64; Rmx = 0; Gmx = 255; Bmx = (y / 32) * 255;\
		} else if(y <= 128) {\
			y = y - 96; Rmx = 0; Gmx = 255 - (y / 32) * 255; Bmx = 255;\
		} else if(y <= 160) {\
			y = y - 128; Rmx = (y / 32) * 255; Gmx = 0; Bmx = 255;\
		} else {\
			y = y - 160; Rmx = 255; Gmx = 0; Bmx = 255 - (y / 32) * 255;\
		} \
		var r,g,b;\
		if(x <= 50) {\
			r = Math.abs(Math.floor(Rmx * x / 50));\
			g = Math.abs(Math.floor(Gmx * x / 50));\
			b = Math.abs(Math.floor(Bmx * x / 50));\
		} else {\
			x -= 50;\
			r = Math.abs(Math.floor(Rmx + (x / 50) * (255 - Rmx)));\
			g = Math.abs(Math.floor(Gmx + (x / 50) * (255 - Gmx)));\
			b = Math.abs(Math.floor(Bmx + (x / 50) * (255 - Bmx)));\
		};\
		var c = "#";\
		c += Math.floor(r/16).toString(16);\
		c += (r%16).toString(16);\
		c += Math.floor(g/16).toString(16);\
		c += (g%16).toString(16);\
		c += Math.floor(b/16).toString(16);\
		c += (b%16).toString(16);\
		return c.toUpperCase();\
	};\
	var addColorPicker = function(target) {\
		var cp = $el("colorpicker");\
		if(cp){\
			cp.parentNode.removeChild(cp);\
			if(lastTarget==target) return;\
		}\
		lastTarget = target;\
		cp = doc.createElement("div");\
		cp.id = "colorpicker";\
		var input;\
		if(target.nodeName=="input")\
		{\
			input = target;\
			cp.style.marginTop =  input.offsetHeight + "px";\
		} else {\
			input = target.previousSibling;\
			cp.style.marginLeft = "-100px";\
		}\
		prev = doc.createElement("span");\
		var img = doc.createElement("img");\
		img.src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGQAAADACAIAAAB9DVH7AAAABGdBTUEAALGPC/xhBQAAAAd0SU1FB9IIDwckH4KetsUAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAsdEVYdENyZWF0aW9uIFRpbWUAVGh1IDE1IEF1ZyAyMDAyIDE5OjM0OjU1ICsxMjAwuqQvdAAABmlJREFUeNrtneGJHEcUhIWw4QQS7P3TDycgcAICRyBwAgJHIByBUAbCEQhnIGcghyBnIIdwCzJIYIOFxPT8ebd1Va97unt2a6hfu7Nzs9PfvVdTM9tz/15ZrooeFz0pelr0rOh50Yuil0Wvi94UvS16V/S+6EPRTdH/UQ+vFv3weNGPTxb99HTRz88W/fJ80a8vFr16uei314t+f7Poj7eL/ny36K/3i/7+sOjm5qu+LffveaEX7mB9R+iK0ENCB0WPisAGHxSBHfu+6NS3Ew6Wl5MHa1qO1k8BoNa/DoBivmD+39DL3QdrNsSui5gNMjsmImaycmRNwtF1ENPycqUqNsEG1sHLSJ/16LS26481xctkTemzIj6VPgsAxezYnX7dPmtun7U2puY+S+qPoFHaZ+3BZ63jxgz7dj6rnV93zTqPPKvSZ4ENMnmWfdZ8PguMGzPsa2Gq5EjKRZ1nTeyzVhOcs/LNI9OclbfP2nmeJZ0SAp/VlyPXrJ4+C5yvM+diPX2W86wLyLOAmvusLYuXyRqbZzGlqnlk6uuG++yGQ8qZJPusnZN1evncSMegVlue6GCZrAw1n4riKyss6x2k8ZWoI6E+9JmsRmSB4w04AmUorszgE1n7FLS+VckRRsxk1ZEVjy7DEaAmIrZdzQL0SazdipjJ2sw6gBGQ+mOsYkzNqqxQ68fJCuWatRlZkQjwFsMRU8WYmhUrlMQaQMzdcHTNyvVHQITks4Bxl3pfRMw1awOypHYG1pHQyNUsaR1p57H/MllD86zKkYyMNEHDedbceVau1wBqbghJbTHXnZ1nTZNnSYiBxsQkCgxHOcTA9zJZffMsMEqAGulMkKlZoGNKiDnP2lWexUSUADpQs0AGm2t5Pjcc3Q1zASnIGGLmKdUscHFI8n3OsybOs1rd0CD5rIgYuAze8DYIkzX0RPpzazGIgVcY6O58xWSNJOuqkQ7Ery+kV+JNrolXTNY2ZHWg5nD6xzsAHwkWIHfDvmSBAy8BtYLwIAjgA6CLW17XYSYIiN+LYs1k1ZLFHHgwbpGaVjUrAsXgA6hZV3bNGu2zwOBUVjGpZsW31u3E4pVri7cUL5PVvhtWAgWqT85nRWokoECpQlXMZG1YsySgwMqAI6lR5oBKWnmTlSQrx0hcBzDCcBTXkf567QmgydpTniWhkXP5YJ0mYJqs7nmW1H0YNK4JgYwh169ds/qSxThdCSiGNaZmMecGElDOs/aQZwGgDsoFG2C4JI4YoFyzpjk3rGyUTJouicn7nWftIc+SfM1BuTzD+Cwm789dLnQ3HJ3BS6UqvsLco5CrWZVyN5wvdZDoGyLnWdOR9R8h5ia+fwgdg5hPMX+d+RYma0uyKjlab8cEIHwsOqY0iDWT1apmdShVEmKji5fJypHFHG/mdwuRGqbl5SbQ6saayZrSZzH9EUD38bTss87OZ0nFC1SxSp/VlzWT1dNnVSLW3Gcx+2OfNZ/P2q7lNfdZK7NNWDNZE/gsMJKVPst51mXnWasYIo6p3+HnWIvI22ftPM8CPltCjDklYPbn3yL7rD34LKnl5SbQYk4SnWddUp7V02cBlxf3ORYv+6w9+KwcR4zP8nVD+6ymiEm9Type9lmDuuEQ1hjomEbJ5Fl3enqTtTFZp5fznz3LZLUny7NnmaymZHn2LJPVlCzPnuWa1d1nXfTsWSarfTf07Fkma+OaddGzZ5msJFmePcs+a2951pnMnmWyuuZZFzR7lslqRpZnzzJZM+VZZzt7lsnqcW5Y2Sh3OXuWyeqRZ13i7Fkmq0cGf4mzZ5mswRHN2c6eZbJGHaztHtjaassmaz6ypAEETzg9Ek9TlX6805U+k9WMLHC8pQc2H08/epfBBzziPD7rvJIjhJjJqiULPBwecASokZ4JXVmzAH0Sa7cgZrI2tA5gBKT+GKsYU7MqK9T6capCuWZtSFYkArzFcMRUMaZmxQolsQYQczccXbNy/REQIfksYNyl3hcRc83agCypnYF1JDRyNUtaR9p55L9M1uA8q3IkIyNN0HCeNXeeles1gBppMhqmLea6s/OsafIsCTHQmI7KTCGAoxxi4HuZrL55FhglQI10JsjULNAxJcScZ+0qz2IiSgAdqFkgg821PJ8bju6GuYAUZAwx85RqFrg4JPk+51kT51mtbmiQfFZEDFwGb3YbhMkSli9l5MGTJ+WhBgAAAABJRU5ErkJggg==";\
		img.addEventListener("mousemove",function(ev){var c=getColor(ev); prev.style.background = c;},false);\
		var setValue = function(ev){\
			var c=getColor(ev);\
			target.style.backgroundColor = input.value = c;\
			input.focus();\
			input.blur();\
		};\
		img.addEventListener("mousedown", setValue, false);\
		img.addEventListener("mouseout", closeColorPicker, false);\
		cp.appendChild(img);\
		cp.appendChild(prev);\
		target.appendChild(cp);\
	};\
	var closeColorPicker = function() {\
		var cp = $el("colorpicker");\
		if(!cp) return;\
		cp.parentNode.removeChild(cp);\
		lastTarget = null;\
	};\
	var lastTabNum=0;\
	var setTabMnu = function(n,ev) {\
		if(ev) {\
			ev.stopPropagation();\
		}\
		var tb = $el("tabmenu");\
		tb.getElementsByTagName("li")[lastTabNum].className="";\
		var ltb = $el("gfx-tab-"+lastTabNum);\
		ltb.style.display="none";\
		tb.getElementsByTagName("li")[n].className="selected";\
		var ltb = $el("gfx-tab-"+n);\
		ltb.style.display="block";\
		lastTabNum = n;\
		if(n < 5) {\
			($el("gfx-preview")).style.display="inline-block";\
		} else {\
			($el("gfx-preview")).style.display="none";\
		}\
		return false;\
	};\
	var gmnu={current:null,last:null,isOver:false,isVisible:false,overID:null};\
	gmnu.qs = function(a) {\
		var c = window.encodeURIComponent&&(doc.forms[0].q||"").value;\
		if(c) a.href = a.href.replace( /([?&])q=[^&]*|$/, function(i,g){ return(g||"&")+"q="+encodeURIComponent(c) });\
	};\
	gmnu.open = function(event,id) {\
		event.cancelBubble = true;\
		var el = event.target;\
		gmnu.current = $el(id);\
		gmnu.isOver=true;\
		if(gmnu.last != null){\
			if(gmnu.last != gmnu.current && event.type != "click"){\
				gmnu.last.style.display = "none";\
			} else if(gmnu.last == gmnu.current && gmnu.isVisible) {\
				gmnu.isVisible = false;\
				gmnu.last.style.display = "none";\
				return;\
			}\
		}\
		gmnu.last = gmnu.current;\
		el.onmouseout = function(){gmnu.isOver=false;};\
		gmnu.current.onmouseover = function(){gmnu.isOver=true;};\
		gmnu.current.onmouseout = function(){gmnu.isOver=false;gmnu.close(400);};\
		doc.onclick = function(){gmnu.isOver=false;gmnu.close(0);};\
		var lft = 0;\
		do{\
			lft += el.offsetLeft;\
		} while(el=el.offsetParent);\
		gmnu.current.style.left = lft + "px";\
		gmnu.isOver = true;\
		window.clearTimeout(gmnu.overID);\
		gmnu.overID=window.setTimeout(function(){\
			if(!gmnu.isOver) return;\
			gmnu.current.style.display = "block";\
			gmnu.isVisible = true;\
		},150);\
	};\
	gmnu.close = function(timeout) {\
		window.clearTimeout(gmnu.overID);\
		var onTimeout = function() {\
			if(gmnu.isOver) return;\
			gmnu.isVisible = false;\
			doc.onclick = null;\
			if(gmnu.current != null){\
				gmnu.current.style.display = "none";\
				gmnu.current = null;\
			}\
			if(gmnu.last != null){\
				gmnu.last.style.display = "none";\
				gmnu.last = null;\
			}\
		};\
		if(timeout) gmnu.overID=window.setTimeout(onTimeout,400);\
		else onTimeout();\
	};\
	var onlyFrom = function (url,ev) {\
		if(ev) {\
			ev.preventDefault();\
			ev.stopPropagation();\
		}\
		var f = doc.forms[0];\
		var inNewTab =  ev.button == 1 || ev.ctrlKey;\
		if( inNewTab ) f.target = "_blank";\
		url = url.replace(RegExp("\\/$"),"");\
		if(f.q.value.indexOf(url) != -1){\
			var path = url.split("/");\
			if(path[1]) {\
				var len = path.length-1;\
				var last = len?(path[len]?-1:-2):-1;\
				path = path.slice(0,last).join("/");\
				f.q.value = f.q.value.replace(url, path);\
			} else {\
				f.q.value = f.q.value.replace(RegExp("\\-?site *: *" + url +"[^ $]*|$","i"),"");\
			}\
		} else {\
			f.q.value = f.q.value.replace(/ *site *: *[^ $]*|$/," site:"+url);\
		}\
		f.submit();\
		if( inNewTab ){\
			f.target = "";\
			f.q.value = f.q.value.replace(RegExp("\\-?site *: *" + url +"[^ $]*|$","i"),"");\
		}\
	};\
	';
	
	if (canRun.images) {
		// Image Preview
		//--------------------
		if(GFX.iPrev){
			STR += 'var iPrevRT=' + GFX.iPrevRT + ';';
			STR += '\
			var iPrevID;\
			var isPreview=false;\
			var isOverPreview=false;\
			function showImg(el,w,h,sz){\
				isOverPreview=false;\
				var ratio=0;\
				var thumb = el.className=="gfx_tn" ? el.getElementsByTagName("IMG")[0] : el.parentNode.parentNode.getElementsByTagName("IMG")[0];\
				var imgW,imgH;\
				if(w){\
					imgW = parseInt(w);\
					imgH = parseInt(h);\
				} else {\
				if(thumb.clientWidth > thumb.clientHeight){\
						imgW = 576;\
						imgH = parseInt(thumb.clientHeight*576/thumb.clientWidth);\
					}else{\
						imgW = parseInt(thumb.clientWidth*576/thumb.clientHeight);\
						imgH = 576;\
					}\
				}\
				var pos = {x:0,y:0};\
				var parent=el;\
				do{\
					pos.x += parent.offsetLeft;\
					pos.y += parent.offsetTop;\
				} while(parent=parent.offsetParent);\
				var scrollTop = doc.body.scrollTop||doc.documentElement.scrollTop;\
				var mrgW = parseInt( (thumb.parentNode.parentNode.offsetWidth - thumb.parentNode.offsetWidth)/2 );\
				var mxWidth = Math.max( pos.x, doc.body.clientWidth - pos.x - 20 );\
				var mxHeight = doc.body.clientHeight - 10;\
				if(imgW > mxWidth){var ratio = mxWidth/imgW; imgW = parseInt(imgW * ratio); imgH = parseInt(imgH * ratio);}\
				if(imgH > mxHeight){var ratio = mxHeight/imgH; imgW = parseInt(imgW * ratio); imgH = parseInt(imgH * ratio);}\
				var holder = $el("gfx-iprev");\
				if (!holder){\
					var holder = doc.createElement("div");\
					holder.id="gfx-iprev";\
					holder.onmouseout = function(){isOverPreview=false;hideImg();};\
					holder.onmouseover = function(){isOverPreview=true;};\
					doc.body.appendChild(holder);\
				} else {\
					while(holder.firstChild){\
						holder.removeChild(holder.firstChild);\
					}\
				}\
				holder.style.display="none";\
				holder.style.left = (( pos.x == mxWidth ? pos.x - imgW + 10 : pos.x - 10 ) + mrgW)+ "px";\
				var botPos = (scrollTop + mxHeight) - (pos.y + 20);\
				var offsetH = (mxHeight - botPos - imgH);\
				holder.style.bottom = (offsetH < 0 ? botPos + offsetH : botPos) + "px";\
				holder.style.width  = (imgW + 10) + "px";\
				holder.style.height = (imgH + 10) + "px";\
				var low = doc.createElement("IMG");\
				var img = doc.createElement("IMG");\
				img.onerror = function(){\
					holder.removeChild(img);\
					var sp = doc.createElement("span");\
					sp.className = "error";\
					sp.style.width  = imgW + "px";\
					sp.style.height = parseInt((imgH+30)/2) + "px";\
					sp.style.paddingTop = parseInt((imgH-30)/2) + "px";\
					sp.innerHTML = "ERROR 404";\
					holder.appendChild(sp);\
				};\
				img.onload = function(){\
					low.style.display = "none";\
					img.className = "high";\
				};\
				img.className = "high loading";\
				img.setAttribute("width",imgW);\
				img.setAttribute("height",imgH);\
				low.className = "low";\
				low.setAttribute("width",imgW);\
				low.setAttribute("height",imgH);\
				holder.appendChild( low );\
				holder.appendChild( img );\
				if(ratio){\
					var sp = doc.createElement("span");\
					sp.className = "ratio";\
					sp.innerHTML = parseInt( ratio * 100 ) + "%";\
					holder.appendChild(sp);\
				}\
				window.clearTimeout(iPrevID);\
				iPrevID = window.setTimeout(function(){\
					isPreview = true;\
					low.src = thumb.src;\
					img.src = thumb.src.substr(thumb.src.lastIndexOf("http://"));\
					holder.style.display = "block";\
				},iPrevRT);\
			};\
			function hideImg(timeout){\
				window.clearTimeout(iPrevID);\
				var onTimeout = function(){\
					if(isPreview && isOverPreview) return;\
					iPrevID=0;\
					isPreview=false;\
					var holder=$el("gfx-iprev");\
					if(!holder) return;\
					holder.style.display="none";\
					while(holder.firstChild){\
						holder.removeChild(holder.firstChild);\
					}\
				};\
				if(timeout) iPrevID=window.setTimeout(onTimeout, timeout);\
				else onTimeout();\
			};\
			if(window.opera) \
				window.addEventListener("mousewheel", function(){isOverPreview=0;hideImg();},false);\
			else \
				window.addEventListener("DOMMouseScroll", function(){isOverPreview=0;hideImg();},false);\
			';
		} // END ImagePreview
		if(GFX.iColor)
		{
			STR += '\
			var gooColorPicker=function(b){\
				b=b||window.event;\
				if(b.stopPropagation){ b.stopPropagation(); }\
				else{ b.cancelBubble=true; }\
				var a=doc.getElementById("sc-dropdown").style;\
				if(a.visibility=="visible"){\
					a.visibility="hidden";\
					doc.removeEventListener?doc.removeEventListener("click",gooColorPicker,false):doc.detachEvent("onclick",gooColorPicker);\
				}else{\
					a.visibility="visible";\
					doc.addEventListener?doc.addEventListener("click",gooColorPicker,false):doc.attachEvent("onclick",gooColorPicker);\
				}\
			};\
			';
		}
	}

	// Add JavaScript Object to the Body 
	var js = doc.createElement("script");
	js.setAttribute("type","text/javascript");
	js.innerHTML = STR;
	body.appendChild(js);
	STR = null;
};
// END GFX_Scripts function



var GFX_ChkLang = function(){
	LNG.blogsearch = LNG.blogs;
	LNG.intl = LNG.more + ' &raquo;';
	LNG.prdhp = LNG.products;
	LNG.search_in = LNG.search + ' ' + LNG.in_;
};

var GFX_ChkInfo = function(){
	INFO.newVer = (INFO.lastVer > SCRIPT_VER);
};






var doc = document; var win = window;

//MooTools, <http://mootools.net>, My Object Oriented (JavaScript) Tools. Copyright (c) 2006-2008 Valerio Proietti, <http://mad4milk.net>, MIT Style License.
var MooTools={version:"1.2.1"};
// Native
var Native=function(K){K=K||{};var A=K.name;var I=K.legacy;var B=K.protect; var C=K.implement;var H=K.generics;var F=K.initialize;var G=K.afterImplement||function(){};var D=F||I;H=H!==false;D.constructor=Native;D.$family={name:"native"}; if(I&&F){D.prototype=I.prototype;}/* Fix4GM */if (!D.prototype){D.prototype={};}/*-Fixed-*/D.prototype.constructor=D;if(A){var E=A.toLowerCase();D.prototype.$family={name:E};Native.typize(D,E);}var J=function(N,L,O,M){if(!B||M||!N.prototype[L]){N.prototype[L]=O; }if(H){Native.genericize(N,L,B);}G.call(N,L,O);return N;};D.alias=function(N,L,O){if(typeof N=="string"){if((N=this.prototype[N])){return J(this,L,N,O); }}for(var M in N){this.alias(M,N[M],L);}return this;};D.implement=function(M,L,O){if(typeof M=="string"){return J(this,M,L,O);}for(var N in M){J(this,N,M[N],L); }return this;};if(C){D.implement(C);}return D;};
Native.genericize=function(B,C,A){if((!A||!B[C])&&typeof B.prototype[C]=="function"){B[C]=function(){var D=Array.prototype.slice.call(arguments); return B.prototype[C].apply(D.shift(),D);};}};
Native.implement=function(D,C){for(var B=0,A=D.length;B<A;B++){D[B].implement(C);}};
Native.typize=function(A,B){if(!A.type){A.type=function(C){return($type(C)===B); };}};
(function(){var A={Array:Array,Date:Date,Function:Function,Number:Number,RegExp:RegExp,String:String};for(var G in A){new Native({name:G,initialize:A[G],protect:true}); }var D={"boolean":Boolean,"native":Native,object:Object};for(var C in D){Native.typize(D[C],C);}var F={Array:["concat","indexOf","join","lastIndexOf","pop","push","reverse","shift","slice","sort","splice","toString","unshift","valueOf"],String:["charAt","charCodeAt","concat","indexOf","lastIndexOf","match","replace","search","slice","split","substr","substring","toLowerCase","toUpperCase","valueOf"]}; for(var E in F){for(var B=F[E].length;B--;){/* Fix4GM */Native.genericize(this[E],F[E][B],true);/*-Fixed-*/}}})();
// Hash
var Hash=new Native({name:"Hash",initialize:function(A){if($type(A)=="hash"){A=$unlink(A.getClean()); }for(var B in A){this[B]=A[B];}return this;}});
Hash.implement({forEach:function(B,C){for(var A in this){if(this.hasOwnProperty(A)){B.call(C,this[A],A,this); }}},getClean:function(){var B={};for(var A in this){if(this.hasOwnProperty(A)){B[A]=this[A];}}return B;},getLength:function(){var B=0;for(var A in this){if(this.hasOwnProperty(A)){B++; }}return B;}});
Hash.alias("forEach","each");
Array.implement({forEach:function(C,D){for(var B=0,A=this.length;B<A;B++){C.call(D,this[B],B,this);}}});
Array.alias("forEach","each");
function $A(C){if(C.item){var D=[];for(var B=0,A=C.length;B<A;B++){D[B]=C[B];}return D;}return Array.prototype.slice.call(C);}
function $arguments(A){return function(){return arguments[A]; };}
function $chk(A){return !!(A||A===0);}
function $clear(A){clearTimeout(A);clearInterval(A);return null;}
function $defined(A){return(A!=undefined);}
function $each(C,B,D){var A=$type(C); ((A=="arguments"||A=="collection"||A=="array")?Array:Hash).each(C,B,D);}
function $empty(){}
function $extend(C,A){for(var B in (A||{})){C[B]=A[B];}return C;}
function $H(A){return new Hash(A);}
function $lambda(A){return(typeof A=="function")?A:function(){return A;};}
function $merge(){var E={};for(var D=0,A=arguments.length; D<A;D++){var B=arguments[D];if($type(B)!="object"){continue;}for(var C in B){var G=B[C],F=E[C];E[C]=(F&&$type(G)=="object"&&$type(F)=="object")?$merge(F,G):$unlink(G); }}return E;}
function $pick(){for(var B=0,A=arguments.length;B<A;B++){if(arguments[B]!=undefined){return arguments[B];}}return null;}
function $random(B,A){return Math.floor(Math.random()*(A-B+1)+B);}
function $splat(B){var A=$type(B);return(A)?((A!="array"&&A!="arguments")?[B]:B):[];}
var $time=Date.now||function(){return +new Date;};
function $try(){for(var B=0,A=arguments.length; B<A;B++){try{return arguments[B]();}catch(C){}}return null;}
function $type(A){if(A==undefined){return false;}if(A.$family){return(A.$family.name=="number"&&!isFinite(A))?false:A.$family.name; }if(A.nodeName){switch(A.nodeType){case 1:return"element";case 3:return(/\S/).test(A.nodeValue)?"textnode":"whitespace";}}else{if(typeof A.length=="number"){if(A.callee){return"arguments"; }else{if(A.item){return"collection";}}}}return typeof A;}
function $unlink(C){var B;switch($type(C)){case"object":B={};for(var E in C){B[E]=$unlink(C[E]); }break;case"hash":B=new Hash(C);break;case"array":B=[];for(var D=0,A=C.length;D<A;D++){B[D]=$unlink(C[D]);}break;default:return C;}return B;}
// Browser
var Browser=$merge({Engine:{name:"unknown",version:0},Platform:{name:(window.orientation!=undefined)?"ipod":(navigator.platform.match(/mac|win|linux/i)||["other"])[0].toLowerCase()},Features:{xpath:!!(doc.evaluate),air:!!(window.runtime),query:!!(doc.querySelector)},Plugins:{},Engines:{presto:function(){return(!window.opera)?false:((arguments.callee.caller)?960:((doc.getElementsByClassName)?950:925)); },trident:function(){return(!window.ActiveXObject)?false:((window.XMLHttpRequest)?5:4);},webkit:function(){return(navigator.taintEnabled)?false:((Browser.Features.xpath)?((Browser.Features.query)?525:420):419); },gecko:function(){return(doc.getBoxObjectFor==undefined)?false:((doc.getElementsByClassName)?19:18);}}},Browser||{});
Browser.Platform[Browser.Platform.name]=true;
Browser.detect=function(){for(var B in this.Engines){var A=this.Engines[B]();if(A){this.Engine={name:B,version:A};this.Engine[B]=this.Engine[B+A]=true; break;}}return{name:B,version:A};};
Browser.detect();
Browser.Plugins.Flash=(function(){var A=($try(function(){return navigator.plugins["Shockwave Flash"].description; },function(){return new ActiveXObject("ShockwaveFlash.ShockwaveFlash").GetVariable("$version");})||"0 r0").match(/\d+/g);return{version:parseInt(A[0]||0+"."+A[1]||0),build:parseInt(A[2]||0)};})();
function $exec(B){if(!B){return B;}if(window.execScript){window.execScript(B);}else{var A=doc.createElement("script");A.setAttribute("type","text/javascript"); A[(Browser.Engine.webkit&&Browser.Engine.version<420)?"innerText":"text"]=B;doc.head.appendChild(A);doc.head.removeChild(A);}return B;}
Native.UID=1;
var $uid=(Browser.Engine.trident)?function(A){return(A.uid||(A.uid=[Native.UID++]))[0];}:function(A){return A.uid||(A.uid=Native.UID++);};
// Window
var Window=new Native({name:"Window",legacy:(Browser.Engine.trident)?null:window.Window,initialize:function(A){$uid(A); if(!A.Element){A.Element=$empty;if(Browser.Engine.webkit){A.document.createElement("iframe");}A.Element.prototype=(Browser.Engine.webkit)?window["[[DOMElement.prototype]]"]:{}; }A.document.window=A;return $extend(A,Window.Prototype);},afterImplement:function(B,A){window[B]=Window.Prototype[B]=A;}});
Window.Prototype={$family:{name:"window"}};
new Window(window);
// Document
var Document=new Native({name:"Document",legacy:(Browser.Engine.trident)?null:window.Document,initialize:function(A){$uid(A);A.head=A.getElementsByTagName("head")[0]; A.html=A.getElementsByTagName("html")[0];if(Browser.Engine.trident&&Browser.Engine.version<=4){$try(function(){A.execCommand("BackgroundImageCache",false,true); });}if(Browser.Engine.trident){A.window.attachEvent("onunload",function(){A.window.detachEvent("onunload",arguments.callee);A.head=A.html=A.window=null; });}return $extend(A,Document.Prototype);},afterImplement:function(B,A){document[B]=Document.Prototype[B]=A;}});
Document.Prototype={$family:{name:"document"}};
new Document(document);
// Array ext
Array.implement({every:function(C,D){for(var B=0,A=this.length;B<A;B++){if(!C.call(D,this[B],B,this)){return false;}}return true; },filter:function(D,E){var C=[];for(var B=0,A=this.length;B<A;B++){if(D.call(E,this[B],B,this)){C.push(this[B]);}}return C;},clean:function(){return this.filter($defined); },indexOf:function(C,D){var A=this.length;for(var B=(D<0)?Math.max(0,A+D):D||0;B<A;B++){if(this[B]===C){return B;}}return -1;},map:function(D,E){var C=[]; for(var B=0,A=this.length;B<A;B++){C[B]=D.call(E,this[B],B,this);}return C;},some:function(C,D){for(var B=0,A=this.length;B<A;B++){if(C.call(D,this[B],B,this)){return true; }}return false;},associate:function(C){var D={},B=Math.min(this.length,C.length);for(var A=0;A<B;A++){D[C[A]]=this[A];}return D;},link:function(C){var A={}; for(var E=0,B=this.length;E<B;E++){for(var D in C){if(C[D](this[E])){A[D]=this[E];delete C[D];break;}}}return A;},contains:function(A,B){return this.indexOf(A,B)!=-1; },extend:function(C){for(var B=0,A=C.length;B<A;B++){this.push(C[B]);}return this;},getLast:function(){return(this.length)?this[this.length-1]:null;},getRandom:function(){return(this.length)?this[$random(0,this.length-1)]:null; },include:function(A){if(!this.contains(A)){this.push(A);}return this;},combine:function(C){for(var B=0,A=C.length;B<A;B++){this.include(C[B]);}return this; },erase:function(B){for(var A=this.length;A--;A){if(this[A]===B){this.splice(A,1);}}return this;},empty:function(){this.length=0;return this;},flatten:function(){var D=[]; for(var B=0,A=this.length;B<A;B++){var C=$type(this[B]);if(!C){continue;}D=D.concat((C=="array"||C=="collection"||C=="arguments")?Array.flatten(this[B]):this[B]); }return D;},hexToRgb:function(B){if(this.length!=3){return null;}var A=this.map(function(C){if(C.length==1){C+=C;}return C.toInt(16);});return(B)?A:"rgb("+A+")"; },rgbToHex:function(D){if(this.length<3){return null;}if(this.length==4&&this[3]==0&&!D){return"transparent";}var B=[];for(var A=0;A<3;A++){var C=(this[A]-0).toString(16); B.push((C.length==1)?"0"+C:C);}return(D)?B:"#"+B.join("");}});
// Function ext
Function.implement({extend:function(A){for(var B in A){this[B]=A[B];}return this;},create:function(B){var A=this; B=B||{};return function(D){var C=B.arguments;C=(C!=undefined)?$splat(C):Array.slice(arguments,(B.event)?1:0);if(B.event){C=[D||window.event].extend(C); }var E=function(){return A.apply(B.bind||null,C);};if(B.delay){return setTimeout(E,B.delay);}if(B.periodical){return setInterval(E,B.periodical);}if(B.attempt){return $try(E); }return E();};},run:function(A,B){return this.apply(B,$splat(A));},pass:function(A,B){return this.create({bind:B,arguments:A});},bind:function(B,A){return this.create({bind:B,arguments:A}); },bindWithEvent:function(B,A){return this.create({bind:B,arguments:A,event:true});},attempt:function(A,B){return this.create({bind:B,arguments:A,attempt:true})(); },delay:function(B,C,A){return this.create({bind:C,arguments:A,delay:B})();},periodical:function(C,B,A){return this.create({bind:B,arguments:A,periodical:C})(); }});
// Number ext
Number.implement({limit:function(B,A){return Math.min(A,Math.max(B,this));},round:function(A){A=Math.pow(10,A||0);return Math.round(this*A)/A;},times:function(B,C){for(var A=0; A<this;A++){B.call(C,A,this);}},toFloat:function(){return parseFloat(this);},toInt:function(A){return parseInt(this,A||10);}});Number.alias("times","each");
(function(B){var A={};B.each(function(C){if(!Number[C]){A[C]=function(){return Math[C].apply(null,[this].concat($A(arguments)));};}});Number.implement(A);})(["abs","acos","asin","atan","atan2","ceil","cos","exp","floor","log","max","min","pow","sin","sqrt","tan"]);
// String ext
String.implement({test:function(A,B){return((typeof A=="string")?new RegExp(A,B):A).test(this); },contains:function(A,B){return(B)?(B+this+B).indexOf(B+A+B)>-1:this.indexOf(A)>-1;},trim:function(){return this.replace(/^\s+|\s+$/g,"");},clean:function(){return this.replace(/\s+/g," ").trim(); },camelCase:function(){return this.replace(/-\D/g,function(A){return A.charAt(1).toUpperCase();});},hyphenate:function(){return this.replace(/[A-Z]/g,function(A){return("-"+A.charAt(0).toLowerCase()); });},capitalize:function(){return this.replace(/\b[a-z]/g,function(A){return A.toUpperCase();});},escapeRegExp:function(){return this.replace(/([-.*+?^${}()|[\]\/\\])/g,"\\$1"); },toInt:function(A){return parseInt(this,A||10);},toFloat:function(){return parseFloat(this);},hexToRgb:function(B){var A=this.match(/^#?(\w{1,2})(\w{1,2})(\w{1,2})$/); return(A)?A.slice(1).hexToRgb(B):null;},rgbToHex:function(B){var A=this.match(/\d{1,3}/g);return(A)?A.rgbToHex(B):null;},stripScripts:function(B){var A=""; var C=this.replace(/<script[^>]*>([\s\S]*?)<\/script>/gi,function(){A+=arguments[1]+"\n";return"";});if(B===true){$exec(A);}else{if($type(B)=="function"){B(A,C); }}return C;},substitute:function(A,B){return this.replace(B||(/\\?\{([^{}]+)\}/g),function(D,C){if(D.charAt(0)=="\\"){return D.slice(1);}return(A[C]!=undefined)?A[C]:""; });}});
// Hash ext
Hash.implement({has:Object.prototype.hasOwnProperty,keyOf:function(B){for(var A in this){if(this.hasOwnProperty(A)&&this[A]===B){return A;}}return null; },hasValue:function(A){return(Hash.keyOf(this,A)!==null);},extend:function(A){Hash.each(A,function(C,B){Hash.set(this,B,C);},this);return this;},combine:function(A){Hash.each(A,function(C,B){Hash.include(this,B,C); },this);return this;},erase:function(A){if(this.hasOwnProperty(A)){delete this[A];}return this;},get:function(A){return(this.hasOwnProperty(A))?this[A]:null; },set:function(A,B){if(!this[A]||this.hasOwnProperty(A)){this[A]=B;}return this;},empty:function(){Hash.each(this,function(B,A){delete this[A];},this); return this;},include:function(B,C){var A=this[B];if(A==undefined){this[B]=C;}return this;},map:function(B,C){var A=new Hash;Hash.each(this,function(E,D){A.set(D,B.call(C,E,D,this)); },this);return A;},filter:function(B,C){var A=new Hash;Hash.each(this,function(E,D){if(B.call(C,E,D,this)){A.set(D,E);}},this);return A;},every:function(B,C){for(var A in this){if(this.hasOwnProperty(A)&&!B.call(C,this[A],A)){return false; }}return true;},some:function(B,C){for(var A in this){if(this.hasOwnProperty(A)&&B.call(C,this[A],A)){return true;}}return false;},getKeys:function(){var A=[]; Hash.each(this,function(C,B){A.push(B);});return A;},getValues:function(){var A=[];Hash.each(this,function(B){A.push(B);});return A;},toQueryString:function(A){var B=[]; Hash.each(this,function(F,E){if(A){E=A+"["+E+"]";}var D;switch($type(F)){case"object":D=Hash.toQueryString(F,E);break;case"array":var C={};F.each(function(H,G){C[G]=H; });D=Hash.toQueryString(C,E);break;default:D=E+"="+encodeURIComponent(F);}if(F!=undefined){B.push(D);}});return B.join("&");}});
Hash.alias({keyOf:"indexOf",hasValue:"contains"});
// Event
var Event=new Native({name:"Event",initialize:function(A,F){F=F||window;var K=F.document;A=A||F.event;if(A.$extended){return A;}this.$extended=true;var J=A.type; var G=A.target||A.srcElement;while(G&&G.nodeType==3){G=G.parentNode;}if(J.test(/key/)){var B=A.which||A.keyCode;var M=Event.Keys.keyOf(B);if(J=="keydown"){var D=B-111; if(D>0&&D<13){M="f"+D;}}M=M||String.fromCharCode(B).toLowerCase();}else{if(J.match(/(click|mouse|menu)/i)){K=(!K.compatMode||K.compatMode=="CSS1Compat")?K.html:K.body; var I={x:A.pageX||A.clientX+K.scrollLeft,y:A.pageY||A.clientY+K.scrollTop};var C={x:(A.pageX)?A.pageX-F.pageXOffset:A.clientX,y:(A.pageY)?A.pageY-F.pageYOffset:A.clientY}; if(J.match(/DOMMouseScroll|mousewheel/)){var H=(A.wheelDelta)?A.wheelDelta/120:-(A.detail||0)/3;}var E=(A.which==3)||(A.button==2);var L=null;if(J.match(/over|out/)){switch(J){case"mouseover":L=A.relatedTarget||A.fromElement; break;case"mouseout":L=A.relatedTarget||A.toElement;}if(!(function(){while(L&&L.nodeType==3){L=L.parentNode;}return true;}).create({attempt:Browser.Engine.gecko})()){L=false; }}}}return $extend(this,{event:A,type:J,page:I,client:C,rightClick:E,wheel:H,relatedTarget:L,target:G,code:B,key:M,shift:A.shiftKey,control:A.ctrlKey,alt:A.altKey,meta:A.metaKey}); }});
Event.Keys=new Hash({enter:13,up:38,down:40,left:37,right:39,esc:27,space:32,backspace:8,tab:9,"delete":46});
Event.implement({stop:function(){return this.stopPropagation().preventDefault(); },stopPropagation:function(){if(this.event.stopPropagation){this.event.stopPropagation();}else{this.event.cancelBubble=true;}return this;},preventDefault:function(){if(this.event.preventDefault){this.event.preventDefault(); }else{this.event.returnValue=false;}return this;}});
// Class
var Class=new Native({name:"Class",initialize:function(B){B=B||{};var A=function(){for(var E in this){if($type(this[E])!="function"){this[E]=$unlink(this[E]); }}this.constructor=A;if(Class.prototyping){return this;}var D=(this.initialize)?this.initialize.apply(this,arguments):this;if(this.options&&this.options.initialize){this.options.initialize.call(this); }return D;};for(var C in Class.Mutators){if(!B[C]){continue;}B=Class.Mutators[C](B,B[C]);delete B[C];}$extend(A,this);A.constructor=Class;A.prototype=B; return A;}});
// Class.Mutators
Class.Mutators={Extends:function(C,A){Class.prototyping=A.prototype;var B=new A;delete B.parent;B=Class.inherit(B,C);delete Class.prototyping; return B;},Implements:function(A,B){$splat(B).each(function(C){Class.prototying=C;$extend(A,($type(C)=="class")?new C:C);delete Class.prototyping;});return A;}};
Class.extend({inherit:function(B,E){var A=arguments.callee.caller;for(var D in E){var C=E[D];var G=B[D];var F=$type(C);if(G&&F=="function"){if(C!=G){if(A){C.__parent=G; B[D]=C;}else{Class.override(B,D,C);}}}else{if(F=="object"){B[D]=$merge(G,C);}else{B[D]=C;}}}if(A){B.parent=function(){return arguments.callee.caller.__parent.apply(this,arguments); };}return B;},override:function(B,A,E){var D=Class.prototyping;if(D&&B[A]!=D[A]){D=null;}var C=function(){var F=this.parent;this.parent=D?D[A]:B[A];var G=E.apply(this,arguments); this.parent=F;return G;};B[A]=C;}});
Class.implement({implement:function(){var A=this.prototype;$each(arguments,function(B){Class.inherit(A,B);});return this;}});
// Chain
var Chain=new Class({$chain:[],chain:function(){this.$chain.extend(Array.flatten(arguments));return this;},callChain:function(){return(this.$chain.length)?this.$chain.shift().apply(this,arguments):false; },clearChain:function(){this.$chain.empty();return this;}});
// Events
var Events=new Class({$events:{},addEvent:function(C,B,A){C=Events.removeOn(C);if(B!=$empty){this.$events[C]=this.$events[C]||[]; this.$events[C].include(B);if(A){B.internal=true;}}return this;},addEvents:function(A){for(var B in A){this.addEvent(B,A[B]);}return this;},fireEvent:function(C,B,A){C=Events.removeOn(C); if(!this.$events||!this.$events[C]){return this;}this.$events[C].each(function(D){D.create({bind:this,delay:A,"arguments":B})();},this);return this;},removeEvent:function(B,A){B=Events.removeOn(B); if(!this.$events[B]){return this;}if(!A.internal){this.$events[B].erase(A);}return this;},removeEvents:function(C){if($type(C)=="object"){for(var D in C){this.removeEvent(D,C[D]); }return this;}if(C){C=Events.removeOn(C);}for(var D in this.$events){if(C&&C!=D){continue;}var B=this.$events[D];for(var A=B.length;A--;A){this.removeEvent(D,B[A]); }}return this;}});
Events.removeOn=function(A){return A.replace(/^on([A-Z])/,function(B,C){return C.toLowerCase();});};
// Options
var Options=new Class({setOptions:function(){this.options=$merge.run([this.options].extend(arguments)); if(!this.addEvent){return this;}for(var A in this.options){if($type(this.options[A])!="function"||!(/^on[A-Z]/).test(A)){continue;}this.addEvent(A,this.options[A]); delete this.options[A];}return this;}});
// Element
var Element=new Native({name:"Element",legacy:window.Element,initialize:function(A,B){var C=Element.Constructors.get(A); if(C){return C(B);}if(typeof A=="string"){return doc.newElement(A,B);}return $el(A).set(B);},afterImplement:function(A,B){Element.Prototype[A]=B;if(Array[A]){return ; }Elements.implement(A,function(){var C=[],G=true;for(var E=0,D=this.length;E<D;E++){var F=this[E][A].apply(this[E],arguments);C.push(F);if(G){G=($type(F)=="element"); }}return(G)?new Elements(C):C;});}});
Element.Prototype={$family:{name:"element"}};
Element.Constructors=new Hash;
// IFrame
var IFrame=new Native({name:"IFrame",generics:false,initialize:function(){var E=Array.link(arguments,{properties:Object.type,iframe:$defined}); var C=E.properties||{};var B=$el(E.iframe)||false;var D=C.onload||$empty;delete C.onload;C.id=C.name=$pick(C.id,C.name,B.id,B.name,"IFrame_"+$time());B=new Element(B||"iframe",C); var A=function(){var F=$try(function(){return B.contentWindow.location.host;});/* Fix4GM */if((F&&F==window.location.host)||!F){/*-Fixed- so with no host work */var G=new Window(B.contentWindow);new Document(B.contentWindow.document);/* Fix4GM */$extend(G.Element.prototype||(G.Element.prototype={}),Element.Prototype);/*-Fixed-*/}D.call(B.contentWindow,B.contentWindow.document);};(window.frames[C.id])?A():B.addListener("load",A);return B;}});
// Elements
var Elements=new Native({initialize:function(F,B){B=$extend({ddup:true,cash:true},B);F=F||[];if(B.ddup||B.cash){var G={},E=[];for(var C=0,A=F.length; C<A;C++){var D=$el.element(F[C],!B.cash);if(B.ddup){if(G[D.uid]){continue;}G[D.uid]=true;}E.push(D);}F=E;}return(B.cash)?$extend(F,this):F;}});
Elements.implement({filter:function(A,B){if(!A){return this; }return new Elements(Array.filter(this,(typeof A=="string")?function(C){return C.match(A);}:A,B));}});
Document.implement({newElement:function(A,B){if(Browser.Engine.trident&&B){["name","type","checked"].each(function(C){if(!B[C]){return ; }A+=" "+C+'="'+B[C]+'"';if(C!="checked"){delete B[C];}});A="<"+A+">";}return $el.element(this.createElement(A)).set(B);},newTextNode:function(A){return this.createTextNode(A); },getDocument:function(){/* Fix4GM */return $el(this);/*-Fixed-*/},getWindow:function(){/*return this.window;*/ /* Fix4GM */return (new Window(this.defaultView||this.parentWindow));/*-Fixed-*/}});
Window.implement({$el:function(B,C){if(B&&B.$family&&B.uid){return B;}var A=$type(B); return($el[A])?$el[A](B,C,/* Fix4GM */(new Document(this.document))/*-Fixed-*/):null;},$$:function(A){if(arguments.length==1&&typeof A=="string"){/*return this.document.getElements(A);*/ /* Fix4GM */return (new Document(this.document)).getElements(A);/*-Fixed-*/}var F=[];var C=Array.flatten(arguments); for(var D=0,B=C.length;D<B;D++){var E=C[D];/* Fix4GM */switch($type(E)){case"element":F.push(E);break;case"string":F.extend((new Document(this.document)).getElements(E,true));}/*-Fixed-*/}return new Elements(F); },getDocument:function(){/* Fix4GM */return (new Document(this.document));/*-Fixed-*/},getWindow:function(){return this;}});
$el.string=function(C,B,A){C=A.getElementById(C);return(C)?$el.element(C,B):null;};
$el.element=function(A,D){$uid(A);if(!D&&!A.$family&&!(/^object|embed$/i).test(A.tagName)){var B=Element.Prototype;for(var C in B){A[C]=B[C];}}return A;};
$el.object=function(B,C,A){if(B.toElement){return $el.element(B.toElement(A),C);}return null;};
$el.textnode=$el.whitespace=$el.window=$el.document=$arguments(0);
Native.implement([Element,Document],{getElement:function(A,B){return $el(this.getElements(A,true)[0]||null,B);},getElements:function(A,D){A=A.split(","); var C=[];var B=(A.length>1);A.each(function(E){var F=this.getElementsByTagName(E.trim());(B)?C.extend(F):C=F;},this);return new Elements(C,{ddup:B,cash:!D}); }});
(function(){var H={},F={};var I={input:"checked",option:"selected",textarea:(Browser.Engine.webkit&&Browser.Engine.version<420)?"innerHTML":"value"}; var C=function(L){return(F[L]||(F[L]={}));};var G=function(N,L){if(!N){return ;}var M=N.uid;if(Browser.Engine.trident){if(N.clearAttributes){var P=L&&N.cloneNode(false); N.clearAttributes();if(P){N.mergeAttributes(P);}}else{if(N.removeEvents){N.removeEvents();}}if((/object/i).test(N.tagName)){for(var O in N){if(typeof N[O]=="function"){N[O]=$empty; }}Element.dispose(N);}}if(!M){return ;}H[M]=F[M]=null;};var D=function(){Hash.each(H,G);if(Browser.Engine.trident){$A(doc.getElementsByTagName("object")).each(G); }if(window.CollectGarbage){CollectGarbage();}H=F=null;};var J=function(N,L,S,M,P,R){var O=N[S||L];var Q=[];while(O){if(O.nodeType==1&&(!M||Element.match(O,M))){if(!P){return $el(O,R); }Q.push(O);}O=O[L];}return(P)?new Elements(Q,{ddup:false,cash:!R}):null;};var E={html:"innerHTML","class":"className","for":"htmlFor",text:(Browser.Engine.trident||(Browser.Engine.webkit&&Browser.Engine.version<420))?"innerText":"textContent"}; var B=["compact","nowrap","ismap","declare","noshade","checked","disabled","readonly","multiple","selected","noresize","defer"];var K=["value","accessKey","cellPadding","cellSpacing","colSpan","frameBorder","maxLength","readOnly","rowSpan","tabIndex","useMap"]; Hash.extend(E,B.associate(B));Hash.extend(E,K.associate(K.map(String.toLowerCase)));var A={before:function(M,L){if(L.parentNode){L.parentNode.insertBefore(M,L); }},after:function(M,L){if(!L.parentNode){return ;}var N=L.nextSibling;(N)?L.parentNode.insertBefore(M,N):L.parentNode.appendChild(M);},bottom:function(M,L){L.appendChild(M); },top:function(M,L){var N=L.firstChild;(N)?L.insertBefore(M,N):L.appendChild(M);}};A.inside=A.bottom;Hash.each(A,function(L,M){M=M.capitalize();Element.implement("inject"+M,function(N){L(this,$el(N,true)); return this;});Element.implement("grab"+M,function(N){L($el(N,true),this);return this;});});Element.implement({set:function(O,M){switch($type(O)){case"object":for(var N in O){this.set(N,O[N]); }break;case"string":var L=Element.Properties.get(O);(L&&L.set)?L.set.apply(this,Array.slice(arguments,1)):this.setProperty(O,M);}return this;},get:function(M){var L=Element.Properties.get(M); return(L&&L.get)?L.get.apply(this,Array.slice(arguments,1)):this.getProperty(M);},erase:function(M){var L=Element.Properties.get(M);(L&&L.erase)?L.erase.apply(this):this.removeProperty(M); return this;},setProperty:function(M,N){var L=E[M];if(N==undefined){return this.removeProperty(M);}if(L&&B[M]){N=!!N;}(L)?this[L]=N:this.setAttribute(M,""+N); return this;},setProperties:function(L){for(var M in L){this.setProperty(M,L[M]);}return this;},getProperty:function(M){var L=E[M];var N=(L)?this[L]:this.getAttribute(M,2); return(B[M])?!!N:(L)?N:N||null;},getProperties:function(){var L=$A(arguments);return L.map(this.getProperty,this).associate(L);},removeProperty:function(M){var L=E[M]; (L)?this[L]=(L&&B[M])?false:"":this.removeAttribute(M);return this;},removeProperties:function(){Array.each(arguments,this.removeProperty,this);return this; },hasClass:function(L){return this.className.contains(L," ");},addClass:function(L){if(!this.hasClass(L)){this.className=(this.className+" "+L).clean(); }return this;},removeClass:function(L){this.className=this.className.replace(new RegExp("(^|\\s)"+L+"(?:\\s|$)"),"$1");return this;},toggleClass:function(L){return this.hasClass(L)?this.removeClass(L):this.addClass(L); },adopt:function(){Array.flatten(arguments).each(function(L){L=$el(L,true);if(L){this.appendChild(L);}},this);return this;},appendText:function(M,L){/*return this.grab(this.getDocument().newTextNode(M),L);*/ /* Fix4GM */return this.grab((new Document(this.getDocument())).newTextNode(M),L);/*-Fixed-*/},grab:function(M,L){A[L||"bottom"]($el(M,true),this);return this;},inject:function(M,L){A[L||"bottom"](this,$el(M,true));return this;},replaces:function(L){L=$el(L,true); L.parentNode.replaceChild(this,L);return this;},wraps:function(M,L){M=$el(M,true);return this.replaces(M).grab(M,L);},getPrevious:function(L,M){return J(this,"previousSibling",null,L,false,M); },getAllPrevious:function(L,M){return J(this,"previousSibling",null,L,true,M);},getNext:function(L,M){return J(this,"nextSibling",null,L,false,M);},getAllNext:function(L,M){return J(this,"nextSibling",null,L,true,M); },getFirst:function(L,M){return J(this,"nextSibling","firstChild",L,false,M);},getLast:function(L,M){return J(this,"previousSibling","lastChild",L,false,M); },getParent:function(L,M){return J(this,"parentNode",null,L,false,M);},getParents:function(L,M){return J(this,"parentNode",null,L,true,M);},getChildren:function(L,M){return J(this,"nextSibling","firstChild",L,true,M); },getWindow:function(){/*return this.ownerDocument.window;*/ /* Fix4GM */return (new Window(this.ownerDocument.defaultView||this.ownerDocument.parentWindow));/*-Fixed-*/},getDocument:function(){/* Fix4GM */return new Document(this.ownerDocument);/*-Fixed-*/},getElementById:function(O,N){var M=this.ownerDocument.getElementById(O); if(!M){return null;}for(var L=M.parentNode;L!=this;L=L.parentNode){if(!L){return null;}}return $el.element(M,N);},getSelected:function(){return new Elements($A(this.options).filter(function(L){return L.selected; }));},getComputedStyle:function(M){/* Fix4GM */var $this=$el(this);if($this.currentStyle){return $this.currentStyle[M.camelCase()];}var L=$this.getDocument().defaultView.getComputedStyle($this,null);/*-Fixed-*/ return(L)?L.getPropertyValue([M.hyphenate()]):null;},toQueryString:function(){var L=[];this.getElements("input, select, textarea",true).each(function(M){if(!M.name||M.disabled){return ; }var N=(M.tagName.toLowerCase()=="select")?Element.getSelected(M).map(function(O){return O.value;}):((M.type=="radio"||M.type=="checkbox")&&!M.checked)?null:M.value; $splat(N).each(function(O){if(typeof O!="undefined"){L.push(M.name+"="+encodeURIComponent(O));}});});return L.join("&");},clone:function(O,L){O=O!==false; var R=this.cloneNode(O);var N=function(V,U){if(!L){V.removeAttribute("id");}if(Browser.Engine.trident){V.clearAttributes();V.mergeAttributes(U);V.removeAttribute("uid"); if(V.options){var W=V.options,S=U.options;for(var T=W.length;T--;){W[T].selected=S[T].selected;}}}var X=I[U.tagName.toLowerCase()];if(X&&U[X]){V[X]=U[X]; }};if(O){var P=R.getElementsByTagName("*"),Q=this.getElementsByTagName("*");for(var M=P.length;M--;){N(P[M],Q[M]);}}N(R,this);return $el(R);},destroy:function(){Element.empty(this); Element.dispose(this);G(this,true);return null;},empty:function(){$A(this.childNodes).each(function(L){Element.destroy(L);});return this;},dispose:function(){return(this.parentNode)?this.parentNode.removeChild(this):this; },hasChild:function(L){L=$el(L,true);if(!L){return false;}if(Browser.Engine.webkit&&Browser.Engine.version<420){return $A(this.getElementsByTagName(L.tagName)).contains(L); }return(this.contains)?(this!=L&&this.contains(L)):!!(this.compareDocumentPosition(L)&16);},match:function(L){return(!L||(L==this)||(Element.get(this,"tag")==L)); }});Native.implement([Element,Window,Document],{addListener:function(O,N){if(O=="unload"){var L=N,M=this;N=function(){M.removeListener("unload",N);L(); };}else{H[this.uid]=this;}if(this.addEventListener){this.addEventListener(O,N,false);}else{this.attachEvent("on"+O,N);}return this;},removeListener:function(M,L){if(this.removeEventListener){this.removeEventListener(M,L,false); }else{this.detachEvent("on"+M,L);}return this;},retrieve:function(M,L){var O=C(this.uid),N=O[M];if(L!=undefined&&N==undefined){N=O[M]=L;}return $pick(N); },store:function(M,L){var N=C(this.uid);N[M]=L;return this;},eliminate:function(L){var M=C(this.uid);delete M[L];return this;}});window.addListener("unload",D); })();
Element.Properties=new Hash;
Element.Properties.style={set:function(A){this.style.cssText=A;},get:function(){return this.style.cssText;},erase:function(){this.style.cssText=""; }};
Element.Properties.tag={get:function(){return this.tagName.toLowerCase();}};
Element.Properties.html=(function(){var C=doc.createElement("div"); var A={table:[1,"<table>","</table>"],select:[1,"<select>","</select>"],tbody:[2,"<table><tbody>","</tbody></table>"],tr:[3,"<table><tbody><tr>","</tr></tbody></table>"]}; A.thead=A.tfoot=A.tbody;var B={set:function(){var E=Array.flatten(arguments).join("");var F=Browser.Engine.trident&&A[this.get("tag")];if(F){var G=C;G.innerHTML=F[1]+E+F[2]; for(var D=F[0];D--;){G=G.firstChild;}this.empty().adopt(G.childNodes);}else{this.innerHTML=E;}}};B.erase=B.set;return B;})();
if(Browser.Engine.webkit&&Browser.Engine.version<420){Element.Properties.text={get:function(){if(this.innerText){return this.innerText; }var A=this.ownerDocument.newElement("div",{html:this.innerHTML}).inject(this.ownerDocument.body);var B=A.innerText;A.destroy();return B;}};}
// Element.Event
Element.Properties.events={set:function(A){this.addEvents(A);}};
Native.implement([Element,Window,Document],{addEvent:function(E,G){var H=this.retrieve("events",{});H[E]=H[E]||{keys:[],values:[]};if(H[E].keys.contains(G)){return this; }H[E].keys.push(G);var F=E,A=Element.Events.get(E),C=G,I=this;if(A){if(A.onAdd){A.onAdd.call(this,G);}if(A.condition){C=function(J){if(A.condition.call(this,J)){return G.call(this,J); }return true;};}F=A.base||F;}var D=function(){return G.call(I);};var B=Element.NativeEvents[F];if(B){if(B==2){D=function(J){J=new Event(J,I.getWindow()); if(C.call(I,J)===false){J.stop();}};}this.addListener(F,D);}H[E].values.push(D);return this;},removeEvent:function(C,B){var A=this.retrieve("events");if(!A||!A[C]){return this; }var F=A[C].keys.indexOf(B);if(F==-1){return this;}A[C].keys.splice(F,1);var E=A[C].values.splice(F,1)[0];var D=Element.Events.get(C);if(D){if(D.onRemove){D.onRemove.call(this,B); }C=D.base||C;}return(Element.NativeEvents[C])?this.removeListener(C,E):this;},addEvents:function(A){for(var B in A){this.addEvent(B,A[B]);}return this; },removeEvents:function(A){if($type(A)=="object"){for(var C in A){this.removeEvent(C,A[C]);}return this;}var B=this.retrieve("events");if(!B){return this; }if(!A){for(var C in B){this.removeEvents(C);}this.eliminate("events");}else{if(B[A]){while(B[A].keys[0]){this.removeEvent(A,B[A].keys[0]);}B[A]=null;}}return this; },fireEvent:function(D,B,A){var C=this.retrieve("events");if(!C||!C[D]){return this;}C[D].keys.each(function(E){E.create({bind:this,delay:A,"arguments":B})(); },this);return this;},cloneEvents:function(D,A){D=$el(D);var C=D.retrieve("events");if(!C){return this;}if(!A){for(var B in C){this.cloneEvents(D,B);}}else{if(C[A]){C[A].keys.each(function(E){this.addEvent(A,E); },this);}}return this;}});
Element.NativeEvents={click:2,dblclick:2,mouseup:2,mousedown:2,contextmenu:2,mousewheel:2,DOMMouseScroll:2,mouseover:2,mouseout:2,mousemove:2,selectstart:2,selectend:2,keydown:2,keypress:2,keyup:2,focus:2,blur:2,change:2,reset:2,select:2,submit:2,load:1,unload:1,beforeunload:2,resize:1,move:1,DOMContentLoaded:1,readystatechange:1,error:1,abort:1,scroll:1};
(function(){var A=function(B){var C=B.relatedTarget;if(C==undefined){return true;}if(C===false){return false;}return($type(this)!="document"&&C!=this&&C.prefix!="xul"&&!this.hasChild(C)); };Element.Events=new Hash({mouseenter:{base:"mouseover",condition:A},mouseleave:{base:"mouseout",condition:A},mousewheel:{base:(Browser.Engine.gecko)?"DOMMouseScroll":"mousewheel"}}); })();
Element.Properties.styles={set:function(A){this.setStyles(A);}};
Element.Properties.opacity={set:function(A,B){if(!B){if(A==0){if(this.style.visibility!="hidden"){this.style.visibility="hidden"; }}else{if(this.style.visibility!="visible"){this.style.visibility="visible";}}}if(!this.currentStyle||!this.currentStyle.hasLayout){this.style.zoom=1;}if(Browser.Engine.trident){this.style.filter=(A==1)?"":"alpha(opacity="+A*100+")"; }this.style.opacity=A;this.store("opacity",A);},get:function(){return this.retrieve("opacity",1);}};
Element.implement({setOpacity:function(A){return this.set("opacity",A,true); },getOpacity:function(){return this.get("opacity");},setStyle:function(B,A){switch(B){case"opacity":return this.set("opacity",parseFloat(A));case"float":B=(Browser.Engine.trident)?"styleFloat":"cssFloat"; }B=B.camelCase();if($type(A)!="string"){var C=(Element.Styles.get(B)||"@").split(" ");A=$splat(A).map(function(E,D){if(!C[D]){return"";}return($type(E)=="number")?C[D].replace("@",Math.round(E)):E; }).join(" ");}else{if(A==String(Number(A))){A=Math.round(A);}}this.style[B]=A;return this;},getStyle:function(G){switch(G){case"opacity":return this.get("opacity"); case"float":G=(Browser.Engine.trident)?"styleFloat":"cssFloat";}G=G.camelCase();var A=this.style[G];if(!$chk(A)){A=[];for(var F in Element.ShortStyles){if(G!=F){continue; }for(var E in Element.ShortStyles[F]){A.push(this.getStyle(E));}return A.join(" ");}A=this.getComputedStyle(G);}if(A){A=String(A);var C=A.match(/rgba?\([\d\s,]+\)/); if(C){A=A.replace(C[0],C[0].rgbToHex());}}if(Browser.Engine.presto||(Browser.Engine.trident&&!$chk(parseInt(A)))){if(G.test(/^(height|width)$/)){var B=(G=="width")?["left","right"]:["top","bottom"],D=0; B.each(function(H){D+=this.getStyle("border-"+H+"-width").toInt()+this.getStyle("padding-"+H).toInt();},this);return this["offset"+G.capitalize()]-D+"px"; }if((Browser.Engine.presto)&&String(A).test("px")){return A;}if(G.test(/(border(.+)Width|margin|padding)/)){return"0px";}}return A;},setStyles:function(B){for(var A in B){this.setStyle(A,B[A]); }return this;},getStyles:function(){var A={};Array.each(arguments,function(B){A[B]=this.getStyle(B);},this);return A;}});
// Element.Style
Element.Styles=new Hash({left:"@px",top:"@px",bottom:"@px",right:"@px",width:"@px",height:"@px",maxWidth:"@px",maxHeight:"@px",minWidth:"@px",minHeight:"@px",backgroundColor:"rgb(@, @, @)",backgroundPosition:"@px @px",color:"rgb(@, @, @)",fontSize:"@px",letterSpacing:"@px",lineHeight:"@px",clip:"rect(@px @px @px @px)",margin:"@px @px @px @px",padding:"@px @px @px @px",border:"@px @ rgb(@, @, @) @px @ rgb(@, @, @) @px @ rgb(@, @, @)",borderWidth:"@px @px @px @px",borderStyle:"@ @ @ @",borderColor:"rgb(@, @, @) rgb(@, @, @) rgb(@, @, @) rgb(@, @, @)",zIndex:"@",zoom:"@",fontWeight:"@",textIndent:"@px",opacity:"@"});
Element.ShortStyles={margin:{},padding:{},border:{},borderWidth:{},borderStyle:{},borderColor:{}};
["Top","Right","Bottom","Left"].each(function(G){var F=Element.ShortStyles; var B=Element.Styles;["margin","padding"].each(function(H){var I=H+G;F[H][I]=B[I]="@px";});var E="border"+G;F.border[E]=B[E]="@px @ rgb(@, @, @)";var D=E+"Width",A=E+"Style",C=E+"Color"; F[E]={};F.borderWidth[D]=F[E][D]=B[D]="@px";F.borderStyle[A]=F[E][A]=B[A]="@";F.borderColor[C]=F[E][C]=B[C]="rgb(@, @, @)";});
(function(){Element.implement({scrollTo:function(H,I){if(B(this)){this.getWindow().scrollTo(H,I); }else{this.scrollLeft=H;this.scrollTop=I;}return this;},getSize:function(){if(B(this)){return this.getWindow().getSize();}return{x:this.offsetWidth,y:this.offsetHeight}; },getScrollSize:function(){if(B(this)){return this.getWindow().getScrollSize();}return{x:this.scrollWidth,y:this.scrollHeight};},getScroll:function(){if(B(this)){return this.getWindow().getScroll(); }return{x:this.scrollLeft,y:this.scrollTop};},getScrolls:function(){var I=this,H={x:0,y:0};while(I&&!B(I)){H.x+=I.scrollLeft;H.y+=I.scrollTop;I=I.parentNode; }return H;},getOffsetParent:function(){var H=this;if(B(H)){return null;}if(!Browser.Engine.trident){return H.offsetParent;}while((H=H.parentNode)&&!B(H)){if(D(H,"position")!="static"){return H; }}return null;},getOffsets:function(){if(Browser.Engine.trident){var L=this.getBoundingClientRect(),J=/* Fix4GM */$el(this).getDocument().documentElement;/*-Fixed-*/return{x:L.left+J.scrollLeft-J.clientLeft,y:L.top+J.scrollTop-J.clientTop}; }var I=this,H={x:0,y:0};if(B(this)){return H;}while(I&&!B(I)){H.x+=I.offsetLeft;H.y+=I.offsetTop;if(Browser.Engine.gecko){if(!F(I)){H.x+=C(I);H.y+=G(I); }var K=I.parentNode;if(K&&D(K,"overflow")!="visible"){H.x+=C(K);H.y+=G(K);}}else{if(I!=this&&Browser.Engine.webkit){H.x+=C(I);H.y+=G(I);}}I=I.offsetParent; }if(Browser.Engine.gecko&&!F(this)){H.x-=C(this);H.y-=G(this);}return H;},getPosition:function(K){if(B(this)){return{x:0,y:0};}var L=this.getOffsets(),I=this.getScrolls(); var H={x:L.x-I.x,y:L.y-I.y};var J=(K&&(K=$el(K)))?K.getPosition():{x:0,y:0};return{x:H.x-J.x,y:H.y-J.y};},getCoordinates:function(J){if(B(this)){return this.getWindow().getCoordinates(); }var H=this.getPosition(J),I=this.getSize();var K={left:H.x,top:H.y,width:I.x,height:I.y};K.right=K.left+K.width;K.bottom=K.top+K.height;return K;},computePosition:function(H){return{left:H.x-E(this,"margin-left"),top:H.y-E(this,"margin-top")}; },position:function(H){return this.setStyles(this.computePosition(H));}});Native.implement([Document,Window],{getSize:function(){var I=this.getWindow(); if(Browser.Engine.presto||Browser.Engine.webkit){return{x:I.innerWidth,y:I.innerHeight};}var H=A(this);return{x:H.clientWidth,y:H.clientHeight};},getScroll:function(){var I=this.getWindow(); var H=A(this);return{x:I.pageXOffset||H.scrollLeft,y:I.pageYOffset||H.scrollTop};},getScrollSize:function(){var I=A(this);var H=this.getSize();return{x:Math.max(I.scrollWidth,H.x),y:Math.max(I.scrollHeight,H.y)}; },getPosition:function(){return{x:0,y:0};},getCoordinates:function(){var H=this.getSize();return{top:0,left:0,bottom:H.y,right:H.x,height:H.y,width:H.x}; }});var D=Element.getComputedStyle;function E(H,I){return D(H,I).toInt()||0;}function F(H){return D(H,"-moz-box-sizing")=="border-box";}function G(H){return E(H,"border-top-width"); }function C(H){return E(H,"border-left-width");}function B(H){return(/^(?:body|html)$/i).test(H.tagName);}function A(H){var I=H.getDocument();return(!I.compatMode||I.compatMode=="CSS1Compat")?I.html:I.body; }})();

Native.implement([Window,Document,Element],{getHeight:function(){return this.getSize().y;},getWidth:function(){return this.getSize().x;},getScrollTop:function(){return this.getScroll().y; },getScrollLeft:function(){return this.getScroll().x;},getScrollHeight:function(){return this.getScrollSize().y;},getScrollWidth:function(){return this.getScrollSize().x; },getTop:function(){return this.getPosition().y;},getLeft:function(){return this.getPosition().x;}});
Native.implement([Document,Element],{getElements:function(H,G){H=H.split(","); var C,E={};for(var D=0,B=H.length;D<B;D++){var A=H[D],F=Selectors.Utils.search(this,A,E);if(D!=0&&F.item){F=$A(F);}C=(D==0)?F:(C.item)?$A(C).concat(F):C.concat(F); }return new Elements(C,{ddup:(H.length>1),cash:!G});}});
Element.implement({match:function(B){if(!B||(B==this)){return true;}var D=Selectors.Utils.parseTagAndID(B); var A=D[0],E=D[1];if(!Selectors.Filters.byID(this,E)||!Selectors.Filters.byTag(this,A)){return false;}var C=Selectors.Utils.parseSelector(B);return(C)?Selectors.Utils.filter(this,C,{}):true; }});
var Selectors={Cache:{nth:{},parsed:{}}};
// Selectors.RegExps
Selectors.RegExps={id:(/#([\w-]+)/),tag:(/^(\w+|\*)/),quick:(/^(\w+|\*)$/),splitter:(/\s*([+>~\s])\s*([a-zA-Z#.*:\[])/g),combined:(/\.([\w-]+)|\[(\w+)(?:([!*^$~|]?=)(["']?)([^\4]*?)\4)?\]|:([\w-]+)(?:\(["']?(.*?)?["']?\)|$)/g)};
// Selectors.Utils
Selectors.Utils={chk:function(B,C){if(!C){return true;}var A=$uid(B);if(!C[A]){return C[A]=true;}return false;},parseNthArgument:function(F){if(Selectors.Cache.nth[F]){return Selectors.Cache.nth[F]; }var C=F.match(/^([+-]?\d*)?([a-z]+)?([+-]?\d*)?$/);if(!C){return false;}var E=parseInt(C[1]);var B=(E||E===0)?E:1;var D=C[2]||false;var A=parseInt(C[3])||0; if(B!=0){A--;while(A<1){A+=B;}while(A>=B){A-=B;}}else{B=A;D="index";}switch(D){case"n":C={a:B,b:A,special:"n"};break;case"odd":C={a:2,b:0,special:"n"}; break;case"even":C={a:2,b:1,special:"n"};break;case"first":C={a:0,special:"index"};break;case"last":C={special:"last-child"};break;case"only":C={special:"only-child"}; break;default:C={a:(B-1),special:"index"};}return Selectors.Cache.nth[F]=C;},parseSelector:function(E){if(Selectors.Cache.parsed[E]){return Selectors.Cache.parsed[E]; }var D,H={classes:[],pseudos:[],attributes:[]};while((D=Selectors.RegExps.combined.exec(E))){var I=D[1],G=D[2],F=D[3],B=D[5],C=D[6],J=D[7];if(I){H.classes.push(I); }else{if(C){var A=Selectors.Pseudo.get(C);if(A){H.pseudos.push({parser:A,argument:J});}else{H.attributes.push({name:C,operator:"=",value:J});}}else{if(G){H.attributes.push({name:G,operator:F,value:B}); }}}}if(!H.classes.length){delete H.classes;}if(!H.attributes.length){delete H.attributes;}if(!H.pseudos.length){delete H.pseudos;}if(!H.classes&&!H.attributes&&!H.pseudos){H=null; }return Selectors.Cache.parsed[E]=H;},parseTagAndID:function(B){var A=B.match(Selectors.RegExps.tag);var C=B.match(Selectors.RegExps.id);return[(A)?A[1]:"*",(C)?C[1]:false]; },filter:function(F,C,E){var D;if(C.classes){for(D=C.classes.length;D--;D){var G=C.classes[D];if(!Selectors.Filters.byClass(F,G)){return false;}}}if(C.attributes){for(D=C.attributes.length; D--;D){var B=C.attributes[D];if(!Selectors.Filters.byAttribute(F,B.name,B.operator,B.value)){return false;}}}if(C.pseudos){for(D=C.pseudos.length;D--;D){var A=C.pseudos[D]; if(!Selectors.Filters.byPseudo(F,A.parser,A.argument,E)){return false;}}}return true;},getByTagAndID:function(B,A,D){if(D){var C=(B.getElementById)?B.getElementById(D,true):Element.getElementById(B,D,true); return(C&&Selectors.Filters.byTag(C,A))?[C]:[];}else{return B.getElementsByTagName(A);}},search:function(I,H,N){var B=[];var C=H.trim().replace(Selectors.RegExps.splitter,function(Y,X,W){B.push(X); return":)"+W;}).split(":)");var J,E,U;for(var T=0,P=C.length;T<P;T++){var S=C[T];if(T==0&&Selectors.RegExps.quick.test(S)){J=I.getElementsByTagName(S); continue;}var A=B[T-1];var K=Selectors.Utils.parseTagAndID(S);var V=K[0],L=K[1];if(T==0){J=Selectors.Utils.getByTagAndID(I,V,L);}else{var D={},G=[];for(var R=0,Q=J.length; R<Q;R++){G=Selectors.Getters[A](G,J[R],V,L,D);}J=G;}var F=Selectors.Utils.parseSelector(S);if(F){E=[];for(var O=0,M=J.length;O<M;O++){U=J[O];if(Selectors.Utils.filter(U,F,N)){E.push(U); }}J=E;}}return J;}};
// Selectors.Getters
Selectors.Getters={" ":function(H,G,I,A,E){var D=Selectors.Utils.getByTagAndID(G,I,A);for(var C=0,B=D.length;C<B;C++){var F=D[C];if(Selectors.Utils.chk(F,E)){H.push(F); }}return H;},">":function(H,G,I,A,F){var C=Selectors.Utils.getByTagAndID(G,I,A);for(var E=0,D=C.length;E<D;E++){var B=C[E];if(B.parentNode==G&&Selectors.Utils.chk(B,F)){H.push(B); }}return H;},"+":function(C,B,A,E,D){while((B=B.nextSibling)){if(B.nodeType==1){if(Selectors.Utils.chk(B,D)&&Selectors.Filters.byTag(B,A)&&Selectors.Filters.byID(B,E)){C.push(B); }break;}}return C;},"~":function(C,B,A,E,D){while((B=B.nextSibling)){if(B.nodeType==1){if(!Selectors.Utils.chk(B,D)){break;}if(Selectors.Filters.byTag(B,A)&&Selectors.Filters.byID(B,E)){C.push(B); }}}return C;}};
// Selectors.Filters
Selectors.Filters={byTag:function(B,A){return(A=="*"||(B.tagName&&B.tagName.toLowerCase()==A));},byID:function(A,B){return(!B||(A.id&&A.id==B)); },byClass:function(B,A){return(B.className&&B.className.contains(A," "));},byPseudo:function(A,D,C,B){return D.call(A,C,B);},byAttribute:function(C,D,B,E){var A=Element.prototype.getProperty.call(C,D); if(!A){return(B=="!=");}if(!B||E==undefined){return true;}switch(B){case"=":return(A==E);case"*=":return(A.contains(E));case"^=":return(A.substr(0,E.length)==E); case"$=":return(A.substr(A.length-E.length)==E);case"!=":return(A!=E);case"~=":return A.contains(E," ");case"|=":return A.contains(E,"-");}return false; }};
// Selectors.Pseudo
Selectors.Pseudo=new Hash({checked:function(){return this.checked;},empty:function(){return !(this.innerText||this.textContent||"").length;},not:function(A){return !Element.match(this,A); },contains:function(A){return(this.innerText||this.textContent||"").contains(A);},"first-child":function(){return Selectors.Pseudo.index.call(this,0);},"last-child":function(){var A=this; while((A=A.nextSibling)){if(A.nodeType==1){return false;}}return true;},"only-child":function(){var B=this;while((B=B.previousSibling)){if(B.nodeType==1){return false; }}var A=this;while((A=A.nextSibling)){if(A.nodeType==1){return false;}}return true;},"nth-child":function(G,E){G=(G==undefined)?"n":G;var C=Selectors.Utils.parseNthArgument(G); if(C.special!="n"){return Selectors.Pseudo[C.special].call(this,C.a,E);}var F=0;E.positions=E.positions||{};var D=$uid(this);if(!E.positions[D]){var B=this; while((B=B.previousSibling)){if(B.nodeType!=1){continue;}F++;var A=E.positions[$uid(B)];if(A!=undefined){F=A+F;break;}}E.positions[D]=F;}return(E.positions[D]%C.a==C.b); },index:function(A){var B=this,C=0;while((B=B.previousSibling)){if(B.nodeType==1&&++C>A){return false;}}return(C==A);},even:function(B,A){return Selectors.Pseudo["nth-child"].call(this,"2n+1",A); },odd:function(B,A){return Selectors.Pseudo["nth-child"].call(this,"2n",A);}});
// Element.Event.domready
Element.Events.domready={onAdd:function(A){if(Browser.loaded){A.call(this); }}};
(function(){var B=function(){if(Browser.loaded){return ;}Browser.loaded=true;window.fireEvent("domready");doc.fireEvent("domready");};if(Browser.Engine.trident){var A=doc.createElement("div"); (function(){($try(function(){A.doScroll("left");return $el(A).inject(doc.body).set("html","temp").dispose();}))?B():arguments.callee.delay(50);})(); }else{if(Browser.Engine.webkit&&Browser.Engine.version<525){(function(){(["loaded","complete"].contains(document.readyState))?B():arguments.callee.delay(50); })();}else{window.addEvent("load",B);doc.addEvent("DOMContentLoaded",B);}}})();
Element.alias("grab","append");
// JSON
var JSON=new Hash({$specialChars:{"\b":"\\b","\t":"\\t","\n":"\\n","\f":"\\f","\r":"\\r",'"':'\\"',"\\":"\\\\"},$replaceChars:function(A){return JSON.$specialChars[A]||"\\u00"+Math.floor(A.charCodeAt()/16).toString(16)+(A.charCodeAt()%16).toString(16); },encode:function(B){switch($type(B)){case"string":return'"'+B.replace(/[\x00-\x1f\\"]/g,JSON.$replaceChars)+'"';case"array":return"["+String(B.map(JSON.encode).filter($defined))+"]"; case"object":case"hash":var A=[];Hash.each(B,function(E,D){var C=JSON.encode(E);if(C){A.push(JSON.encode(D)+":"+C);}});return"{"+A+"}";case"number":case"boolean":return String(B); case false:return"null";}return null;},decode:function(string,secure){if($type(string)!="string"||!string.length){return null;}if(secure&&!(/^[,:{}\[\]0-9.\-+Eaeflnr-u \n\r\t]*$/).test(string.replace(/\\./g,"@").replace(/"[^"\\\n\r]*"/g,""))){return null; }return eval("("+string+")");}});
Native.implement([Hash,Array,String,Number],{toJSON:function(){return JSON.encode(this);}});
// Swiff
var Swiff=new Class({Implements:[Options],options:{id:null,height:1,width:1,container:null,properties:{},params:{quality:"high",allowScriptAccess:"always",wMode:"transparent",swLiveConnect:true},callBacks:{},vars:{}},toElement:function(){return this.object; },initialize:function(L,M){this.instance="Swiff_"+$time();this.setOptions(M);M=this.options;var B=this.id=M.id||this.instance;var A=$el(M.container);Swiff.CallBacks[this.instance]={}; var E=M.params,G=M.vars,F=M.callBacks;var H=$extend({height:M.height,width:M.width},M.properties);var K=this;for(var D in F){Swiff.CallBacks[this.instance][D]=(function(N){return function(){return N.apply(K.object,arguments); };})(F[D]);G[D]="Swiff.CallBacks."+this.instance+"."+D;}E.flashVars=Hash.toQueryString(G);if(Browser.Engine.trident){H.classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"; E.movie=L;}else{H.type="application/x-shockwave-flash";H.data=L;}var J='<object id="'+B+'"';for(var I in H){J+=" "+I+'="'+H[I]+'"';}J+=">";for(var C in E){if(E[C]){J+='<param name="'+C+'" value="'+E[C]+'" />'; }}J+="</object>";this.object=((A)?A.empty():new Element("div")).set("html",J).firstChild;},replaces:function(A){A=$el(A,true);A.parentNode.replaceChild(this.toElement(),A); return this;},inject:function(A){$el(A,true).appendChild(this.toElement());return this;},remote:function(){return Swiff.remote.apply(Swiff,[this.toElement()].extend(arguments)); }});
Swiff.CallBacks={};
Swiff.remote=function(obj,fn){var rs=obj.CallFunction('<invoke name="'+fn+'" returntype="javascript">'+__flash__argumentsToXML(arguments,2)+"</invoke>"); return eval(rs);};
// FX
var Fx=new Class({Implements:[Chain,Events,Options],options:{fps:50,unit:false,duration:500,link:"ignore"},initialize:function(A){this.subject=this.subject||this; this.setOptions(A);this.options.duration=Fx.Durations[this.options.duration]||this.options.duration.toInt();var B=this.options.wait;if(B===false){this.options.link="cancel"; }},getTransition:function(){return function(A){return -(Math.cos(Math.PI*A)-1)/2;};},step:function(){var A=$time();if(A<this.time+this.options.duration){var B=this.transition((A-this.time)/this.options.duration); this.set(this.compute(this.from,this.to,B));}else{this.set(this.compute(this.from,this.to,1));this.complete();}},set:function(A){return A;},compute:function(C,B,A){return Fx.compute(C,B,A); },check:function(A){if(!this.timer){return true;}switch(this.options.link){case"cancel":this.cancel();return true;case"chain":this.chain(A.bind(this,Array.slice(arguments,1))); return false;}return false;},start:function(B,A){if(!this.check(arguments.callee,B,A)){return this;}this.from=B;this.to=A;this.time=0;this.transition=this.getTransition(); this.startTimer();this.onStart();return this;},complete:function(){if(this.stopTimer()){this.onComplete();}return this;},cancel:function(){if(this.stopTimer()){this.onCancel(); }return this;},onStart:function(){this.fireEvent("start",this.subject);},onComplete:function(){this.fireEvent("complete",this.subject);if(!this.callChain()){this.fireEvent("chainComplete",this.subject); }},onCancel:function(){this.fireEvent("cancel",this.subject).clearChain();},pause:function(){this.stopTimer();return this;},resume:function(){this.startTimer(); return this;},stopTimer:function(){if(!this.timer){return false;}this.time=$time()-this.time;this.timer=$clear(this.timer);return true;},startTimer:function(){if(this.timer){return false; }this.time=$time()-this.time;this.timer=this.step.periodical(Math.round(1000/this.options.fps),this);return true;}});
Fx.compute=function(C,B,A){return(B-C)*A+C; };
Fx.Durations={"short":250,normal:500,"long":1000};
// Fx.CSS
Fx.CSS=new Class({Extends:Fx,prepare:function(D,E,B){B=$splat(B);var C=B[1];if(!$chk(C)){B[1]=B[0]; B[0]=D.getStyle(E);}var A=B.map(this.parse);return{from:A[0],to:A[1]};},parse:function(A){A=$lambda(A)();A=(typeof A=="string")?A.split(" "):$splat(A); return A.map(function(C){C=String(C);var B=false;Fx.CSS.Parsers.each(function(F,E){if(B){return ;}var D=F.parse(C);if($chk(D)){B={value:D,parser:F};}}); B=B||{value:C,parser:Fx.CSS.Parsers.String};return B;});},compute:function(D,C,B){var A=[];(Math.min(D.length,C.length)).times(function(E){A.push({value:D[E].parser.compute(D[E].value,C[E].value,B),parser:D[E].parser}); });A.$family={name:"fx:css:value"};return A;},serve:function(C,B){if($type(C)!="fx:css:value"){C=this.parse(C);}var A=[];C.each(function(D){A=A.concat(D.parser.serve(D.value,B)); });return A;},render:function(A,D,C,B){A.setStyle(D,this.serve(C,B));},search:function(A){if(Fx.CSS.Cache[A]){return Fx.CSS.Cache[A];}var B={};Array.each(doc.styleSheets,function(E,D){var C=E.href; if(C&&C.contains("://")&&!C.contains(doc.domain)){return ;}var F=E.rules||E.cssRules;Array.each(F,function(I,G){if(!I.style){return ;}var H=(I.selectorText)?I.selectorText.replace(/^\w+/,function(J){return J.toLowerCase(); }):null;if(!H||!H.test("^"+A+"$")){return ;}Element.Styles.each(function(K,J){if(!I.style[J]||Element.ShortStyles[J]){return ;}K=String(I.style[J]);B[J]=(K.test(/^rgb/))?K.rgbToHex():K; });});});return Fx.CSS.Cache[A]=B;}});Fx.CSS.Cache={};
Fx.CSS.Parsers=new Hash({Color:{parse:function(A){if(A.match(/^#[0-9a-f]{3,6}$/i)){return A.hexToRgb(true); }return((A=A.match(/(\d+),\s*(\d+),\s*(\d+)/)))?[A[1],A[2],A[3]]:false;},compute:function(C,B,A){return C.map(function(E,D){return Math.round(Fx.compute(C[D],B[D],A)); });},serve:function(A){return A.map(Number);}},Number:{parse:parseFloat,compute:Fx.compute,serve:function(B,A){return(A)?B+A:B;}},String:{parse:$lambda(false),compute:$arguments(1),serve:$arguments(0)}});
// FX.Tween
Fx.Tween=new Class({Extends:Fx.CSS,initialize:function(B,A){this.element=this.subject=$el(B);this.parent(A);},set:function(B,A){if(arguments.length==1){A=B; B=this.property||this.options.property;}this.render(this.element,B,A,this.options.unit);return this;},start:function(C,E,D){if(!this.check(arguments.callee,C,E,D)){return this; }var B=Array.flatten(arguments);this.property=this.options.property||B.shift();var A=this.prepare(this.element,this.property,B);return this.parent(A.from,A.to); }});
Element.Properties.tween={set:function(A){var B=this.retrieve("tween");if(B){B.cancel();}return this.eliminate("tween").store("tween:options",$extend({link:"cancel"},A)); },get:function(A){if(A||!this.retrieve("tween")){if(A||!this.retrieve("tween:options")){this.set("tween",A);}this.store("tween",new Fx.Tween(this,this.retrieve("tween:options"))); }return this.retrieve("tween");}};
Element.implement({tween:function(A,C,B){this.get("tween").start(arguments);return this;},fade:function(C){var E=this.get("tween"),D="opacity",A; C=$pick(C,"toggle");switch(C){case"in":E.start(D,1);break;case"out":E.start(D,0);break;case"show":E.set(D,1);break;case"hide":E.set(D,0);break;case"toggle":var B=this.retrieve("fade:flag",this.get("opacity")==1); E.start(D,(B)?0:1);this.store("fade:flag",!B);A=true;break;default:E.start(D,arguments);}if(!A){this.eliminate("fade:flag");}return this;},highlight:function(C,A){if(!A){A=this.retrieve("highlight:original",this.getStyle("background-color")); A=(A=="transparent")?"#fff":A;}var B=this.get("tween");B.start("background-color",C||"#ffff88",A).chain(function(){this.setStyle("background-color",this.retrieve("highlight:original")); B.callChain();}.bind(this));return this;}});
// FX.Morph
Fx.Morph=new Class({Extends:Fx.CSS,initialize:function(B,A){this.element=this.subject=$el(B);this.parent(A);},set:function(A){if(typeof A=="string"){A=this.search(A); }for(var B in A){this.render(this.element,B,A[B],this.options.unit);}return this;},compute:function(E,D,C){var A={};for(var B in E){A[B]=this.parent(E[B],D[B],C); }return A;},start:function(B){if(!this.check(arguments.callee,B)){return this;}if(typeof B=="string"){B=this.search(B);}var E={},D={};for(var C in B){var A=this.prepare(this.element,C,B[C]); E[C]=A.from;D[C]=A.to;}return this.parent(E,D);}});
Element.Properties.morph={set:function(A){var B=this.retrieve("morph");if(B){B.cancel();}return this.eliminate("morph").store("morph:options",$extend({link:"cancel"},A)); },get:function(A){if(A||!this.retrieve("morph")){if(A||!this.retrieve("morph:options")){this.set("morph",A);}this.store("morph",new Fx.Morph(this,this.retrieve("morph:options"))); }return this.retrieve("morph");}};Element.implement({morph:function(A){this.get("morph").start(A);return this;}});
// FX.Transition
Fx.Transition=function(B,A){A=$splat(A);return $extend(B,{easeIn:function(C){return B(C,A);},easeOut:function(C){return 1-B(1-C,A);},easeInOut:function(C){return(C<=0.5)?B(2*C,A)/2:(2-B(2*(1-C),A))/2; }});};
Fx.Transitions=new Hash({linear:$arguments(0)});
Fx.Transitions.extend=function(A){for(var B in A){Fx.Transitions[B]=new Fx.Transition(A[B]);}};
Fx.Transitions.extend({Pow:function(B,A){return Math.pow(B,A[0]||6); },Expo:function(A){return Math.pow(2,8*(A-1));},Circ:function(A){return 1-Math.sin(Math.acos(A));},Sine:function(A){return 1-Math.sin((1-A)*Math.PI/2); },Back:function(B,A){A=A[0]||1.618;return Math.pow(B,2)*((A+1)*B-A);},Bounce:function(D){var C;for(var B=0,A=1;1;B+=A,A/=2){if(D>=(7-4*B)/11){C=A*A-Math.pow((11-6*B-11*D)/4,2); break;}}return C;},Elastic:function(B,A){return Math.pow(2,10*--B)*Math.cos(20*B*Math.PI*(A[0]||1)/3);}});["Quad","Cubic","Quart","Quint"].each(function(B,A){Fx.Transitions[B]=new Fx.Transition(function(C){return Math.pow(C,[A+2]); });});
Fx.implement({getTransition:function(){var A=this.options.transition||Fx.Transitions.Sine.easeInOut; if(typeof A=="string"){var B=A.split(":");A=Fx.Transitions;A=A[B[0]]||A[B[0].capitalize()];if(B[1]){A=A["ease"+B[1].capitalize()+(B[2]?B[2].capitalize():"")]; }}return A;}});

/* MooTools, My Object Oriented Javascript Tools. Copyright (c) 2006-2009 Valerio Proietti, <http://mad4milk.net>, MIT Style License. */ 
(function(){var B={wait:function(C){return this.chain(function(){this.callChain.delay($pick(C,500),this); }.bind(this));}};Chain.implement(B);if(window.Fx){Fx.implement(B);["Css","Tween","Elements"].each(function(C){if(Fx[C]){Fx[C].implement(B);}});}try{Element.implement({chains:function(C){$splat($pick(C,["tween","morph","reveal"])).each(function(D){var D=this.get(D); if(!D){return ;}D.setOptions({link:"chain"});},this);return this;},pauseFx:function(D,C){this.chains(C).get($pick(C,"tween")).wait(D);return this;}});}catch(A){}})(); (function(){var C={"[\xa0\u2002\u2003\u2009]":" ","\xb7":"*","[\u2018\u2019]":"'","[\u201c\u201d]":'"',"\u2026":"...","\u2013":"-","\u2014":"--","\uFFFD":"&raquo;"}; String.implement({repeat:function(D){return new Array(D+1).join(this); },pad:function(E,G,D){if(this.length>=E){return this;}G=G||" ";var F=G.repeat(E-this.length).substr(0,E-this.length);if(!D||D=="right"){return this+F;}if(D=="left"){return F+this; }return F.substr(0,(F.length/2).floor())+this+F.substr(0,(F.length/2).ceil());},stripTags:function(){return this.replace(/<\/?[^>]+>/gi,"");},tidy:function(){var D=this.toString(); $each(C,function(F,E){D=D.replace(new RegExp(E,"g"),F);});return D;}});})();Element.implement({measure:function(C){var B=this.expose();var A=C.apply(this);B();return A;},expose:function(){if(this.getStyle("display")!="none"){return $empty; }var B={};var A={visibility:"hidden",display:"block",position:"absolute"};$each(A,function(D,C){B[C]=this.style[C]||"";},this);this.setStyles(A);return(function(){this.setStyles(B); }).bind(this);},getDimensions:function(A){A=$merge({computeSize:false},A);var D={};var C=function(F,E){return(E.computeSize)?F.getComputedSize(E):F.getSize(); };if(this.getStyle("display")=="none"){D=this.measure(function(){return C(this,A);});}else{try{D=C(this,A);}catch(B){}}return $chk(D.x)?$extend(D,{width:D.x,height:D.y}):$extend(D,{x:D.width,y:D.height}); },getComputedSize:function(A){A=$merge({styles:["padding","border"],plains:{height:["top","bottom"],width:["left","right"]},mode:"both"},A);var C={width:0,height:0}; switch(A.mode){case"vertical":delete C.width;delete A.plains.width;break;case"horizontal":delete C.height;delete A.plains.height;break;}var B=[];$each(A.plains,function(G,F){G.each(function(H){A.styles.each(function(I){B.push((I=="border")?I+"-"+H+"-width":I+"-"+H); });});});var E=this.getStyles.apply(this,B);var D=[];$each(A.plains,function(G,F){C["total"+F.capitalize()]=0;C["computed"+F.capitalize()]=0;G.each(function(H){C["computed"+H.capitalize()]=0; B.each(function(J,I){if(J.test(H)){E[J]=E[J].toInt();if(isNaN(E[J])){E[J]=0;}C["total"+F.capitalize()]=C["total"+F.capitalize()]+E[J];C["computed"+H.capitalize()]=C["computed"+H.capitalize()]+E[J]; }if(J.test(H)&&F!=J&&(J.test("border")||J.test("padding"))&&!D.contains(J)){D.push(J);C["computed"+F.capitalize()]=C["computed"+F.capitalize()]-E[J];}}); });});["Width","Height"].each(function(G){var F=G.toLowerCase();if(!$chk(C[F])){return ;}C[F]=C[F]+this["offset"+G]+C["computed"+G];C["total"+G]=C[F]+C["total"+G]; delete C["computed"+G];},this);return $extend(E,C);}});(function(){var A=false;window.addEvent("domready",function(){var B=new Element("div").setStyles({position:"fixed",top:0,right:0}).inject(doc.body); A=(B.offsetTop===0);B.dispose();});Element.implement({pin:function(C){if(this.getStyle("display")=="none"){return ;}if(C!==false){var D=this.getPosition(); if(!this.retrieve("pinned")){var F={top:D.y-window.getScroll().y,left:D.x-window.getScroll().x};if(A){this.setStyle("position","fixed").setStyles(F);}else{this.store("pinnedByJS",true); this.setStyles({position:"absolute",top:D.y,left:D.x});this.store("scrollFixer",(function(){if(this.retrieve("pinned")){this.setStyles({top:F.top.toInt()+window.getScroll().y,left:F.left.toInt()+window.getScroll().x}); }}).bind(this));window.addEvent("scroll",this.retrieve("scrollFixer"));}this.store("pinned",true);}}else{var E;if(!Browser.Engine.trident){if(this.getParent().getComputedStyle("position")!="static"){E=this.getParent(); }else{E=this.getParent().getOffsetParent();}}var D=this.getPosition(E);this.store("pinned",false);var B;if(A&&!this.retrieve("pinnedByJS")){B={top:D.y+window.getScroll().y,left:D.x+window.getScroll().x}; }else{this.store("pinnedByJS",false);window.removeEvent("scroll",this.retrieve("scrollFixer"));B={top:D.y,left:D.x};}this.setStyles($merge(B,{position:"absolute"})); }return this.addClass("isPinned");},unpin:function(){return this.pin(false).removeClass("isPinned");},togglepin:function(){this.pin(!this.retrieve("pinned")); }});})();Element.implement({position:function(Q){$each(Q||{},function(S,R){if(!$defined(S)){delete Q[R];}});Q=$merge({relativeTo:doc.body,position:{x:"center",y:"center"},edge:false,offset:{x:0,y:0},returnPos:false,relFixedPosition:false,ignoreMargins:false,allowNegative:false},Q); var A={x:0,y:0};var G=false;var B=this.measure(function(){return $el(this.getOffsetParent());});if(B&&B!=this.getDocument().body){A=B.measure(function(){return this.getPosition();});G=true;Q.offset.x=Q.offset.x-A.x;Q.offset.y=Q.offset.y-A.y;}var P=function(R){if($type(R)!="string"){return R;}R=R.toLowerCase();var S={};if(R.test("left")){S.x="left"; }else{if(R.test("right")){S.x="right";}else{S.x="center";}}if(R.test("upper")||R.test("top")){S.y="top";}else{if(R.test("bottom")){S.y="bottom";}else{S.y="center"; }}return S;};Q.edge=P(Q.edge);Q.position=P(Q.position);if(!Q.edge){if(Q.position.x=="center"&&Q.position.y=="center"){Q.edge={x:"center",y:"center"};}else{Q.edge={x:"left",y:"top"}; }}this.setStyle("position","absolute");var O=$el(Q.relativeTo)||doc.body;var H=O==doc.body?window.getScroll():O.getPosition();var N=H.y;var F=H.x; if(Browser.Engine.trident){var K=O.getScrolls();N+=K.y;F+=K.x;}var I=this.getDimensions({computeSize:true,styles:["padding","border","margin"]});if(Q.ignoreMargins){Q.offset.x=Q.offset.x-I["margin-left"]; Q.offset.y=Q.offset.y-I["margin-top"];}var M={};var C=Q.offset.y;var D=Q.offset.x;var J=window.getSize();switch(Q.position.x){case"left":M.x=F+D;break; case"right":M.x=F+D+O.offsetWidth;break;default:M.x=F+((O==doc.body?J.x:O.offsetWidth)/2)+D;break;}switch(Q.position.y){case"top":M.y=N+C;break;case"bottom":M.y=N+C+O.offsetHeight; break;default:M.y=N+((O==doc.body?J.y:O.offsetHeight)/2)+C;break;}if(Q.edge){var L={};switch(Q.edge.x){case"left":L.x=0;break;case"right":L.x=-I.x-I.computedRight-I.computedLeft; break;default:L.x=-(I.x/2);break;}switch(Q.edge.y){case"top":L.y=0;break;case"bottom":L.y=-I.y-I.computedTop-I.computedBottom;break;default:L.y=-(I.y/2); break;}M.x=M.x+L.x;M.y=M.y+L.y;}M={left:((M.x>=0||G||Q.allowNegative)?M.x:0).toInt(),top:((M.y>=0||G||Q.allowNegative)?M.y:0).toInt()};if(O.getStyle("position")=="fixed"||Q.relFixedPosition){var E=window.getScroll(); M.top=M.top.toInt()+E.y;M.left=M.left.toInt()+E.x;}if(Q.returnPos){return M;}else{this.setStyles(M);}return this;}});(function(){var A=Element.prototype.setPosition; Element.implement({setPosition:function(B){if($defined(B.x)||$defined(B.y)){return A.apply(this,arguments);}return this.position.apply(this,arguments);}});});Element.implement({isDisplayed:function(){return this.getStyle("display")!="none";},toggle:function(){return this[this.isDisplayed()?"hide":"show"](); },hide:function(){var B;try{if("none"!=this.getStyle("display")){B=this.getStyle("display");}}catch(A){}return this.store("originalDisplay",B||"block").setStyle("display","none"); },show:function(A){return this.setStyle("display",A||this.retrieve("originalDisplay")||"block");},swapClass:function(A,B){return this.removeClass(A).addClass(B); }});
// FX.Slider
Fx.Slide=new Class({Extends:Fx,options:{mode:"vertical"},initialize:function(B,A){this.addEvent("complete",function(){this.open=(this.wrapper["offset"+this.layout.capitalize()]!=0); if(this.open&&Browser.Engine.webkit419){this.element.dispose().inject(this.wrapper);}},true);this.element=this.subject=$el(B);this.parent(A);var C=this.element.retrieve("wrapper"); this.wrapper=C||new Element("div",{styles:$extend(this.element.getStyles("margin","position"),{overflow:"hidden"})}).wraps(this.element);this.element.store("wrapper",this.wrapper).setStyle("margin",0); this.now=[];this.open=true;},vertical:function(){this.margin="margin-top";this.layout="height";this.offset=this.element.offsetHeight;},horizontal:function(){this.margin="margin-left"; this.layout="width";this.offset=this.element.offsetWidth;},set:function(A){this.element.setStyle(this.margin,A[0]);this.wrapper.setStyle(this.layout,A[1]); return this;},compute:function(E,D,C){var B=[];var A=2;A.times(function(F){B[F]=Fx.compute(E[F],D[F],C);});return B;},start:function(B,E){if(!this.check(arguments.callee,B,E)){return this; }this[E||this.options.mode]();var D=this.element.getStyle(this.margin).toInt();var C=this.wrapper.getStyle(this.layout).toInt();var A=[[D,C],[0,this.offset]]; var G=[[D,C],[-this.offset,0]];var F;switch(B){case"in":F=A;break;case"out":F=G;break;case"toggle":F=(this.wrapper["offset"+this.layout.capitalize()]==0)?A:G; }return this.parent(F[0],F[1]);},slideIn:function(A){return this.start("in",A);},slideOut:function(A){return this.start("out",A);},hide:function(A){this[A||this.options.mode](); this.open=false;return this.set([-this.offset,0]);},show:function(A){this[A||this.options.mode]();this.open=true;return this.set([0,this.offset]);},toggle:function(A){return this.start("toggle",A); }});
Element.Properties.slide={set:function(B){var A=this.retrieve("slide");if(A){A.cancel();}return this.eliminate("slide").store("slide:options",$extend({link:"cancel"},B)); },get:function(A){if(A||!this.retrieve("slide")){if(A||!this.retrieve("slide:options")){this.set("slide",A);}this.store("slide",new Fx.Slide(this,this.retrieve("slide:options"))); }return this.retrieve("slide");}};
Element.implement({slide:function(D,E){D=D||"toggle";var B=this.get("slide"),A;switch(D){case"hide":B.hide(E);break;case"show":B.show(E); break;case"toggle":var C=this.retrieve("slide:flag",B.open);B[(C)?"slideOut":"slideIn"](E);this.store("slide:flag",!C);A=true;break;default:B.start(D,E); }if(!A){this.eliminate("slide:flag");}return this;}});
// FX.Scroll
Fx.Scroll=new Class({Extends:Fx,options:{offset:{x:0,y:0},wheelStops:true},initialize:function(B,A){this.element=this.subject=$el(B); this.parent(A);var D=this.cancel.bind(this,false);if($type(this.element)!="element"){this.element=$el(this.element.getDocument().body);}var C=this.element; if(this.options.wheelStops){this.addEvent("start",function(){C.addEvent("mousewheel",D);},true);this.addEvent("complete",function(){C.removeEvent("mousewheel",D); },true);}},set:function(){var A=Array.flatten(arguments);this.element.scrollTo(A[0],A[1]);},compute:function(E,D,C){var B=[];var A=2;A.times(function(F){B.push(Fx.compute(E[F],D[F],C)); });return B;},start:function(C,H){if(!this.check(arguments.callee,C,H)){return this;}var E=this.element.getSize(),F=this.element.getScrollSize();var B=this.element.getScroll(),D={x:C,y:H}; for(var G in D){var A=F[G]-E[G];if($chk(D[G])){D[G]=($type(D[G])=="number")?D[G].limit(0,A):A;}else{D[G]=B[G];}D[G]+=this.options.offset[G];}return this.parent([B.x,B.y],[D.x,D.y]); },toTop:function(){return this.start(false,0);},toLeft:function(){return this.start(0,false);},toRight:function(){return this.start("right",false);},toBottom:function(){return this.start(false,"bottom"); },toElement:function(B){var A=$el(B).getPosition(this.element);return this.start(A.x,A.y);}});
// SmoothScroll
var SmoothScroll=new Class({Extends:Fx.Scroll,initialize:function(options,context){context=context||document;var doc=context.getDocument(),win=context.getWindow();this.parent(doc, options);this.links=(this.options.links)?$$(this.options.links):$$(doc.links);var location=win.location.href.match(/^[^#]*/)[0]+'#';this.links.each(function(link){if (link.href.indexOf(location)!=0){return;}var anchor=link.href.substr(location.length);if (anchor&&$el(anchor)){this.useLink(link, anchor);}},this);if (!Browser.Engine.webkit419){this.addEvent('complete', function(){win.location.hash=this.anchor;},true);}},useLink:function(link,anchor){link.addEvent('click',function(event){this.anchor=anchor;this.toElement(anchor);event.stop();}.bind(this));}});
// FX.Elements
Fx.Elements=new Class({Extends:Fx.CSS,initialize:function(B,A){this.elements=this.subject=$$(B); this.parent(A);},compute:function(G,H,I){var C={};for(var D in G){var A=G[D],E=H[D],F=C[D]={};for(var B in A){F[B]=this.parent(A[B],E[B],I);}}return C; },set:function(B){for(var C in B){var A=B[C];for(var D in A){this.render(this.elements[C],D,A[D],this.options.unit);}}return this;},start:function(C){if(!this.check(arguments.callee,C)){return this; }var H={},I={};for(var D in C){var F=C[D],A=H[D]={},G=I[D]={};for(var B in F){var E=this.prepare(this.elements[D],B,F[B]);A[B]=E.from;G[B]=E.to;}}return this.parent(H,I); }});
// Accordion
var Accordion=new Class({Extends:Fx.Elements,options:{display:0,show:false,height:true,width:false,opacity:true,fixedHeight:false,fixedWidth:false,wait:false,alwaysHide:false},initialize:function(){var C=Array.link(arguments,{container:Element.type,options:Object.type,togglers:$defined,elements:$defined}); this.parent(C.elements,C.options);this.togglers=$$(C.togglers);this.container=$el(C.container);this.previous=-1;if(this.options.alwaysHide){this.options.wait=true; }if($chk(this.options.show)){this.options.display=false;this.previous=this.options.show;}if(this.options.start){this.options.display=false;this.options.show=false; }this.effects={};if(this.options.opacity){this.effects.opacity="fullOpacity";}if(this.options.width){this.effects.width=this.options.fixedWidth?"fullWidth":"offsetWidth"; }if(this.options.height){this.effects.height=this.options.fixedHeight?"fullHeight":"scrollHeight";}for(var B=0,A=this.togglers.length;B<A;B++){this.addSection(this.togglers[B],this.elements[B]); }this.elements.each(function(E,D){if(this.options.show===D){this.fireEvent("active",[this.togglers[D],E]);}else{for(var F in this.effects){E.setStyle(F,0); }}},this);if($chk(this.options.display)){this.display(this.options.display);}},addSection:function(E,C,G){E=$el(E);C=$el(C);var F=this.togglers.contains(E); var B=this.togglers.length;this.togglers.include(E);this.elements.include(C);if(B&&(!F||G)){G=$pick(G,B-1);E.inject(this.togglers[G],"before");C.inject(E,"after"); }else{if(this.container&&!F){E.inject(this.container);C.inject(this.container);}}var A=this.togglers.indexOf(E);E.addEvent("click",this.display.bind(this,A)); if(this.options.height){C.setStyles({"padding-top":0,"border-top":"none","padding-bottom":0,"border-bottom":"none"});}if(this.options.width){C.setStyles({"padding-left":0,"border-left":"none","padding-right":0,"border-right":"none"}); }C.fullOpacity=1;if(this.options.fixedWidth){C.fullWidth=this.options.fixedWidth;}if(this.options.fixedHeight){C.fullHeight=this.options.fixedHeight;}C.setStyle("overflow","hidden"); if(!F){for(var D in this.effects){C.setStyle(D,0);}}return this;},display:function(A){A=($type(A)=="element")?this.elements.indexOf(A):A;if((this.timer&&this.options.wait)||(A===this.previous&&!this.options.alwaysHide)){return this; }this.previous=A;var B={};this.elements.each(function(E,D){B[D]={};var C=(D!=A)||(this.options.alwaysHide&&(E.offsetHeight>0));this.fireEvent(C?"background":"active",[this.togglers[D],E]); for(var F in this.effects){B[D][F]=C?0:E[this.effects[F]];}},this);return this.start(B);}});
// Drag
//----------
var Drag=new Class({Implements:[Events,Options],options:{snap:6,unit:"px",grid:false,style:true,limit:false,handle:false,invert:false,preventDefault:false,modifiers:{x:"left",y:"top"}},initialize:function(){var B=Array.link(arguments,{options:Object.type,element:$defined}); this.element=$el(B.element);this.document=this.element.getDocument();this.setOptions(B.options||{});var A=$type(this.options.handle);this.handles=(A=="array"||A=="collection")?$$(this.options.handle):$el(this.options.handle)||this.element; this.mouse={now:{},pos:{}};this.value={start:{},now:{}};this.selection=(Browser.Engine.trident)?"selectstart":"mousedown";this.bound={start:this.start.bind(this),check:this.check.bind(this),drag:this.drag.bind(this),stop:this.stop.bind(this),cancel:this.cancel.bind(this),eventStop:$lambda(false)}; this.attach();},attach:function(){this.handles.addEvent("mousedown",this.bound.start);return this;},detach:function(){this.handles.removeEvent("mousedown",this.bound.start); return this;},start:function(C){if(this.options.preventDefault){C.preventDefault();}this.fireEvent("beforeStart",this.element);this.mouse.start=C.page; var A=this.options.limit;this.limit={x:[],y:[]};for(var D in this.options.modifiers){if(!this.options.modifiers[D]){continue;}if(this.options.style){this.value.now[D]=this.element.getStyle(this.options.modifiers[D]).toInt(); }else{this.value.now[D]=this.element[this.options.modifiers[D]];}if(this.options.invert){this.value.now[D]*=-1;}this.mouse.pos[D]=C.page[D]-this.value.now[D]; if(A&&A[D]){for(var B=2;B--;B){if($chk(A[D][B])){this.limit[D][B]=$lambda(A[D][B])();}}}}if($type(this.options.grid)=="number"){this.options.grid={x:this.options.grid,y:this.options.grid}; }this.document.addEvents({mousemove:this.bound.check,mouseup:this.bound.cancel});this.document.addEvent(this.selection,this.bound.eventStop);},check:function(A){if(this.options.preventDefault){A.preventDefault(); }var B=Math.round(Math.sqrt(Math.pow(A.page.x-this.mouse.start.x,2)+Math.pow(A.page.y-this.mouse.start.y,2)));if(B>this.options.snap){this.cancel();this.document.addEvents({mousemove:this.bound.drag,mouseup:this.bound.stop}); this.fireEvent("start",this.element).fireEvent("snap",this.element);}},drag:function(A){if(this.options.preventDefault){A.preventDefault();}this.mouse.now=A.page; for(var B in this.options.modifiers){if(!this.options.modifiers[B]){continue;}this.value.now[B]=this.mouse.now[B]-this.mouse.pos[B];if(this.options.invert){this.value.now[B]*=-1; }if(this.options.limit&&this.limit[B]){if($chk(this.limit[B][1])&&(this.value.now[B]>this.limit[B][1])){this.value.now[B]=this.limit[B][1];}else{if($chk(this.limit[B][0])&&(this.value.now[B]<this.limit[B][0])){this.value.now[B]=this.limit[B][0]; }}}if(this.options.grid[B]){this.value.now[B]-=(this.value.now[B]%this.options.grid[B]);}if(this.options.style){this.element.setStyle(this.options.modifiers[B],this.value.now[B]+this.options.unit); }else{this.element[this.options.modifiers[B]]=this.value.now[B];}}this.fireEvent("drag",this.element);},cancel:function(A){this.document.removeEvent("mousemove",this.bound.check); this.document.removeEvent("mouseup",this.bound.cancel);if(A){this.document.removeEvent(this.selection,this.bound.eventStop);this.fireEvent("cancel",this.element); }},stop:function(A){this.document.removeEvent(this.selection,this.bound.eventStop);this.document.removeEvent("mousemove",this.bound.drag);this.document.removeEvent("mouseup",this.bound.stop); if(A){this.fireEvent("complete",this.element);}}});
Element.implement({makeResizable:function(A){return new Drag(this,$merge({modifiers:{x:"width",y:"height"}},A)); }});
// Drag.Move
Drag.Move=new Class({Extends:Drag,options:{droppables:[],container:false},initialize:function(C,B){this.parent(C,B);this.droppables=$$(this.options.droppables); this.container=$el(this.options.container);if(this.container&&$type(this.container)!="element"){this.container=$el(this.container.getDocument().body);}C=this.element; var D=C.getStyle("position");var A=(D!="static")?D:"absolute";if(C.getStyle("left")=="auto"||C.getStyle("top")=="auto"){C.position(C.getPosition(C.offsetParent)); }C.setStyle("position",A);this.addEvent("start",function(){this.checkDroppables();},true);},start:function(B){if(this.container){var D=this.element,J=this.container,E=J.getCoordinates(D.offsetParent),F={},A={}; ["top","right","bottom","left"].each(function(K){F[K]=J.getStyle("padding-"+K).toInt();A[K]=D.getStyle("margin-"+K).toInt();},this);var C=D.offsetWidth+A.left+A.right,I=D.offsetHeight+A.top+A.bottom; var H=[E.left+F.left,E.right-F.right-C];var G=[E.top+F.top,E.bottom-F.bottom-I];this.options.limit={x:H,y:G};}this.parent(B);},checkAgainst:function(B){B=B.getCoordinates(); var A=this.mouse.now;return(A.x>B.left&&A.x<B.right&&A.y<B.bottom&&A.y>B.top);},checkDroppables:function(){var A=this.droppables.filter(this.checkAgainst,this).getLast(); if(this.overed!=A){if(this.overed){this.fireEvent("leave",[this.element,this.overed]);}if(A){this.overed=A;this.fireEvent("enter",[this.element,A]);}else{this.overed=null; }}},drag:function(A){this.parent(A);if(this.droppables.length){this.checkDroppables();}},stop:function(A){this.checkDroppables();this.fireEvent("drop",[this.element,this.overed]); this.overed=null;return this.parent(A);}});
Element.implement({makeDraggable:function(A){return new Drag.Move(this,A);}});
// Slider
var Slider=new Class({Implements:[Events,Options],options:{onTick:function(A){if(this.options.snap){A=this.toPosition(this.step);}this.knob.setStyle(this.property,A); },snap:false,offset:0,range:false,wheel:false,steps:100,mode:"horizontal"},initialize:function(E,A,D){this.setOptions(D);this.element=$el(E);this.knob=$el(A); this.previousChange=this.previousEnd=this.step=-1;this.element.addEvent("mousedown",this.clickedElement.bind(this));if(this.options.wheel){this.element.addEvent("mousewheel",this.scrolledElement.bindWithEvent(this)); }var F,B={},C={x:false,y:false};switch(this.options.mode){case"vertical":this.axis="y";this.property="top";F="offsetHeight";break;case"horizontal":this.axis="x"; this.property="left";F="offsetWidth";}this.half=this.knob[F]/2;this.full=this.element[F]-this.knob[F]+(this.options.offset*2);this.min=$chk(this.options.range[0])?this.options.range[0]:0; this.max=$chk(this.options.range[1])?this.options.range[1]:this.options.steps;this.range=this.max-this.min;this.steps=this.options.steps||this.full;this.stepSize=Math.abs(this.range)/this.steps; this.stepWidth=this.stepSize*this.full/Math.abs(this.range);this.knob.setStyle("position","relative").setStyle(this.property,-this.options.offset);C[this.axis]=this.property; B[this.axis]=[-this.options.offset,this.full-this.options.offset];this.drag=new Drag(this.knob,{snap:0,limit:B,modifiers:C,onDrag:this.draggedKnob.bind(this),onStart:this.draggedKnob.bind(this),onComplete:function(){this.draggedKnob(); this.end();}.bind(this)});if(this.options.snap){this.drag.options.grid=Math.ceil(this.stepWidth);this.drag.options.limit[this.axis][1]=this.full;}},set:function(A){if(!((this.range>0)^(A<this.min))){A=this.min; }if(!((this.range>0)^(A>this.max))){A=this.max;}this.step=Math.round(A);this.checkStep();this.end();this.fireEvent("tick",this.toPosition(this.step));return this; },clickedElement:function(C){var B=this.range<0?-1:1;var A=C.page[this.axis]-this.element.getPosition()[this.axis]-this.half;A=A.limit(-this.options.offset,this.full-this.options.offset); this.step=Math.round(this.min+B*this.toStep(A));this.checkStep();this.end();this.fireEvent("tick",A);},scrolledElement:function(A){var B=(this.options.mode=="horizontal")?(A.wheel<0):(A.wheel>0); this.set(B?this.step-this.stepSize:this.step+this.stepSize);A.stop();},draggedKnob:function(){var B=this.range<0?-1:1;var A=this.drag.value.now[this.axis]; A=A.limit(-this.options.offset,this.full-this.options.offset);this.step=Math.round(this.min+B*this.toStep(A));this.checkStep();},checkStep:function(){if(this.previousChange!=this.step){this.previousChange=this.step; this.fireEvent("change",this.step);}},end:function(){if(this.previousEnd!==this.step){this.previousEnd=this.step;this.fireEvent("complete",this.step+""); }},toStep:function(A){var B=(A+this.options.offset)*this.stepSize/this.full*this.steps;return this.options.steps?Math.round(B-=B%this.stepSize):B;},toPosition:function(A){return(this.full*Math.abs(this.min-A))/(this.steps*this.stepSize)-this.options.offset; }});
// Sortables
var Sortables=new Class({Implements:[Events,Options],options:{snap:4,opacity:1,clone:false,revert:false,handle:false,constrain:false},initialize:function(A,B){this.setOptions(B); this.elements=[];this.lists=[];this.idle=true;this.addLists($$($el(A)||A));if(!this.options.clone){this.options.revert=false;}if(this.options.revert){this.effect=new Fx.Morph(null,$merge({duration:250,link:"cancel"},this.options.revert)); }},attach:function(){this.addLists(this.lists);return this;},detach:function(){this.lists=this.removeLists(this.lists);return this;},addItems:function(){Array.flatten(arguments).each(function(A){this.elements.push(A); var B=A.retrieve("sortables:start",this.start.bindWithEvent(this,A));(this.options.handle?A.getElement(this.options.handle)||A:A).addEvent("mousedown",B); },this);return this;},addLists:function(){Array.flatten(arguments).each(function(A){this.lists.push(A);this.addItems(A.getChildren());},this);return this; },removeItems:function(){var A=[];Array.flatten(arguments).each(function(B){A.push(B);this.elements.erase(B);var C=B.retrieve("sortables:start");(this.options.handle?B.getElement(this.options.handle)||B:B).removeEvent("mousedown",C); },this);return $$(A);},removeLists:function(){var A=[];Array.flatten(arguments).each(function(B){A.push(B);this.lists.erase(B);this.removeItems(B.getChildren()); },this);return $$(A);},getClone:function(B,A){if(!this.options.clone){return new Element("div").inject(doc.body);}if($type(this.options.clone)=="function"){return this.options.clone.call(this,B,A,this.list); }return A.clone(true).setStyles({margin:"0px",position:"absolute",visibility:"hidden",width:A.getStyle("width")}).inject(this.list).position(A.getPosition(A.getOffsetParent())); },getDroppables:function(){var A=this.list.getChildren();if(!this.options.constrain){A=this.lists.concat(A).erase(this.list);}return A.erase(this.clone).erase(this.element); },insert:function(C,B){var A="inside";if(this.lists.contains(B)){this.list=B;this.drag.droppables=this.getDroppables();}else{A=this.element.getAllPrevious().contains(B)?"before":"after"; }this.element.inject(B,A);this.fireEvent("sort",[this.element,this.clone]);},start:function(B,A){if(!this.idle){return ;}this.idle=false;this.element=A; this.opacity=A.get("opacity");this.list=A.getParent();this.clone=this.getClone(B,A);this.drag=new Drag.Move(this.clone,{snap:this.options.snap,container:this.options.constrain&&this.element.getParent(),droppables:this.getDroppables(),onSnap:function(){B.stop(); this.clone.setStyle("visibility","visible");this.element.set("opacity",this.options.opacity||0);this.fireEvent("start",[this.element,this.clone]);}.bind(this),onEnter:this.insert.bind(this),onCancel:this.reset.bind(this),onComplete:this.end.bind(this)}); this.clone.inject(this.element,"before");this.drag.start(B);},end:function(){this.drag.detach();this.element.set("opacity",this.opacity);if(this.effect){var A=this.element.getStyles("width","height"); var B=this.clone.computePosition(this.element.getPosition(this.clone.offsetParent));this.effect.element=this.clone;this.effect.start({top:B.top,left:B.left,width:A.width,height:A.height,opacity:0.25}).chain(this.reset.bind(this)); }else{this.reset();}},reset:function(){this.idle=true;this.clone.destroy();this.fireEvent("complete",this.element);},serialize:function(){var C=Array.link(arguments,{modifier:Function.type,index:$defined}); var B=this.lists.map(function(D){return D.getChildren().map(C.modifier||function(E){return E.get("id");},this);},this);var A=C.index;if(this.lists.length==1){A=0; }return $chk(A)&&A>=0&&A<this.lists.length?B[A]:B;}});
// Color
//---------
var Color=new Native({initialize:function(B,C){if(arguments.length>=3){C="rgb";B=Array.slice(arguments,0,3); }else{if(typeof B=="string"){if(B.match(/rgb/)){B=B.rgbToHex().hexToRgb(true);}else{if(B.match(/hsb/)){B=B.hsbToRgb();}else{B=B.hexToRgb(true);}}}}C=C||"rgb"; switch(C){case"hsb":var A=B;B=B.hsbToRgb();B.hsb=A;break;case"hex":B=B.hexToRgb(true);break;}B.rgb=B.slice(0,3);B.hsb=B.hsb||B.rgbToHsb();B.hex=B.rgbToHex(); return $extend(B,this);}});
Color.implement({mix:function(){var A=Array.slice(arguments);var C=($type(A.getLast())=="number")?A.pop():50;var B=this.slice(); A.each(function(D){D=new Color(D);for(var E=0;E<3;E++){B[E]=Math.round((B[E]/100*(100-C))+(D[E]/100*C));}});return new Color(B,"rgb");},invert:function(){return new Color(this.map(function(A){return 255-A; }));},setHue:function(A){return new Color([A,this.hsb[1],this.hsb[2]],"hsb");},setSaturation:function(A){return new Color([this.hsb[0],A,this.hsb[2]],"hsb"); },setBrightness:function(A){return new Color([this.hsb[0],this.hsb[1],A],"hsb");}});
function $RGB(C,B,A){return new Color([C,B,A],"rgb");}function $HSB(C,B,A){return new Color([C,B,A],"hsb"); }function $HEX(A){return new Color(A,"hex");}Array.implement({rgbToHsb:function(){var B=this[0],C=this[1],J=this[2];var G,F,H;var I=Math.max(B,C,J),E=Math.min(B,C,J); var K=I-E;H=I/255;F=(I!=0)?K/I:0;if(F==0){G=0;}else{var D=(I-B)/K;var A=(I-C)/K;var L=(I-J)/K;if(B==I){G=L-A;}else{if(C==I){G=2+D-L;}else{G=4+A-D;}}G/=6; if(G<0){G++;}}return[Math.round(G*360),Math.round(F*100),Math.round(H*100)];},hsbToRgb:function(){var C=Math.round(this[2]/100*255);if(this[1]==0){return[C,C,C]; }else{var A=this[0]%360;var E=A%60;var F=Math.round((this[2]*(100-this[1]))/10000*255);var D=Math.round((this[2]*(6000-this[1]*E))/600000*255);var B=Math.round((this[2]*(6000-this[1]*(60-E)))/600000*255); switch(Math.floor(A/60)){case 0:return[C,B,F];case 1:return[D,C,F];case 2:return[F,C,B];case 3:return[F,D,C];case 4:return[B,F,C];case 5:return[C,F,D]; }}return false;}});String.implement({rgbToHsb:function(){var A=this.match(/\d{1,3}/g);return(A)?hsb.rgbToHsb():null;},hsbToRgb:function(){var A=this.match(/\d{1,3}/g); return(A)?A.hsbToRgb():null;}});
// Scroller
var Scroller=new Class({Implements:[Events,Options],options:{area:20,velocity:1,onChange:function(A,B){this.element.scrollTo(A,B);}},initialize:function(B,A){this.setOptions(A); this.element=$el(B);this.listener=($type(this.element)!="element")?$el(this.element.getDocument().body):this.element;this.timer=null;this.coord=this.getCoords.bind(this); },start:function(){this.listener.addEvent("mousemove",this.coord);},stop:function(){this.listener.removeEvent("mousemove",this.coord);this.timer=$clear(this.timer); },getCoords:function(A){this.page=(this.listener.get("tag")=="body")?A.client:A.page;if(!this.timer){this.timer=this.scroll.periodical(50,this);}},scroll:function(){var B=this.element.getSize(),A=this.element.getScroll(),E=this.element.getPosition(),D={x:0,y:0}; for(var C in this.page){if(this.page[C]<(this.options.area+E[C])&&A[C]!=0){D[C]=(this.page[C]-this.options.area-E[C])*this.options.velocity;}else{if(this.page[C]+this.options.area>(B[C]+E[C])&&B[C]+B[C]!=A[C]){D[C]=(this.page[C]-B[C]+this.options.area-E[C])*this.options.velocity; }}}if(D.y||D.x){this.fireEvent("change",[A.x+D.x,A.y+D.y]);}}});

// MooTools Extras
Element.implement({'hover':function(fnA,fnB){this.addEvents({'mouseenter':function(e){fnA.attempt(e,this);},'mouseleave':function(e){fnB.attempt(e,this);}})}});

/**
 * Sexy Alert Box v1.2  - for mootools 1.2
  * @author Eduardo D. Sada - http://www.coders.me/web-js-html/javascript/sexy-alert-box
 * @copyright (c) 2009 Eduardo D. Sada (www.coders.me)
 * @license MIT - http://es.wikipedia.org/wiki/Licencia_MIT
 */

var SexyAlertBox=new Class({Implements:[Chain],getOptions:function(){return{name:'SABox',zIndex:65555,onReturn:false,onReturnFunction:$empty,BoxStyles:{'width':500},OverlayStyles:{'background-color':'#000','opacity':0.7},showDuration:200,showEffect:Fx.Transitions.linear,closeDuration:100,closeEffect:Fx.Transitions.linear,moveDuration:500,moveEffect:Fx.Transitions.Back.easeOut,onShowStart:$empty,onShowComplete:$empty,onCloseStart:$empty,onCloseComplete:function(a){this.options.onReturnFunction(this.options.onReturn)}.bind(this)}},initialize:function(b){this.i=0;this.setOptions(this.getOptions(),b);this.Overlay=new Element('div',{'id':'BoxOverlay','styles':{'display':'none','position':'absolute','top':'0','left':'0','opacity':0.01,'z-index':this.options.zIndex,'background-color':this.options.OverlayStyles['background-color'],'background-image':this.options.OverlayStyles['background-image'],'background-repeat':this.options.OverlayStyles['background-repeat'],'height':window.getScrollHeight()+'px','width':window.getScrollWidth()+'px'}});this.Content=new Element('div',{'id':this.options.name+'-BoxContenedor'});this.Contenedor=new Element('div',{'id':this.options.name+'-BoxContent'}).adopt(this.Content);this.InBox=new Element('div',{'id':this.options.name+'-InBox'}).adopt(this.Contenedor);this.Box=new Element('div',{'id':this.options.name+'-Box','styles':{'display':'none','z-index':this.options.zIndex+2,'position':'absolute','top':'0','left':'0','width':this.options.BoxStyles['width']+'px'}}).adopt(this.InBox);this.Overlay.injectInside(doc.body);this.Box.injectInside(doc.body);this.preloadImages();window.addEvent('resize',function(){if(this.options.display==1){this.Overlay.setStyles({'height':window.getScrollHeight()+'px','width':window.getScrollWidth()+'px'});this.replaceBox()}}.bind(this));this.Box.addEvent('keydown',function(a){if(a.key=='esc'){this.options.onReturn=false;this.display(0)}}.bind(this));window.addEvent('scroll',this.replaceBox.bind(this))},preloadImages:function(){var a=new Array(2);a[0]=new Image();a[1]=new Image();a[2]=new Image();a[0].src=this.Box.getStyle('background-image').replace(new RegExp("url\\('?([^']*)'?\\)",'gi'),"$1");a[1].src=this.InBox.getStyle('background-image').replace(new RegExp("url\\('?([^']*)'?\\)",'gi'),"$1");a[2].src=this.Contenedor.getStyle('background-image').replace(new RegExp("url\\('?([^']*)'?\\)",'gi'),"$1")},togFlashObjects:function(a){var b=new Array("embed","iframe","object");for(y=0;y<b.length;y++){var c=doc.getElementsByTagName(b[y]);for(i=0;i<c.length;i++){c[i].style.visibility=a}}},display:function(a){if(this.Transition)this.Transition.cancel();if(this.options.display==0&&a!=0||a==1){if(Browser.Engine.trident4)$$('select','object','embed').each(function(node){node.style.visibility='hidden'});this.togFlashObjects('hidden');this.Overlay.setStyle('display','block');this.options.display=1;this.fireEvent('onShowStart',[this.Overlay]);if(this.options.showDuration){this.Transition=new Fx.Tween(this.Overlay,{property:'opacity',duration:this.options.showDuration,transition:this.options.showEffect,onComplete:function(){this.showBox();}.bind(this)}).start(this.options.OverlayStyles['opacity'])}else{this.Overlay.setStyle('opacity',this.options.OverlayStyles['opacity']);this.showBox();}}else{if(Browser.Engine.trident4)$$('select','object','embed').each(function(node){node.style.visibility='visible'});this.togFlashObjects('visible');this.queue.delay(500,this);this.Box.setStyles({'display':'none','top':0});this.Content.empty();this.options.display=0;this.fireEvent('onCloseStart',[this.Overlay]);if(this.Transition&&this.i==1){this.Transition=new Fx.Tween(this.Overlay,{property:'opacity',duration:this.options.closeDuration,transition:this.options.closeEffect,onComplete:function(){this.fireEvent('onCloseComplete',[this.Overlay])}.bind(this)}).start(0)}else if(this.i==1){this.Overlay.setStyles({'display':'none','opacity':0.01,});this.fireEvent('onCloseComplete',[this.Overlay]);}}},showBox:function(){sizes=window.getSize();scrollito=window.getScroll();this.Box.setStyles({'display':'block','left':(scrollito.x+(sizes.x-this.options.BoxStyles['width'])/2).toInt()});this.replaceBox();this.fireEvent('onShowComplete',[this.Overlay])},replaceBox:function(){if(this.options.display==1){sizes=window.getSize();scrollito=window.getScroll();if(this.MoveBox)this.MoveBox.cancel();this.MoveBox=new Fx.Morph(this.Box,{duration:this.options.moveDuration,transition:this.options.moveEffect}).start({'left':(scrollito.x+(sizes.x-this.options.BoxStyles['width'])/2).toInt(),'top':(scrollito.y+(sizes.y-this.Box.offsetHeight)/2).toInt()});this.focusin.delay(this.options.moveDuration,this)}},focusin:function(){if($chk($el('BoxAlertBtnOk'))){$el('BoxAlertBtnOk').focus()}else if($chk($el('BoxPromptInput'))){$el('BoxPromptInput').focus()}else if($chk($el('BoxConfirmBtnOk'))){$el('BoxConfirmBtnOk').focus()}},queue:function(){this.i--;this.callChain()},messageBox:function(a,b,c,d){this.chain(function(){c=$extend({'textBoxBtnOk':'OK','textBoxBtnCancel':'Cancelar','textBoxInputPrompt':null,'password':false,'onComplete':$empty},c||{});this.options.onReturnFunction=c.onComplete;this.ContenedorBotones=new Element('div',{'id':this.options.name+'-Buttons'});if(a=='alert'||a=='info'||a=='error'){this.AlertBtnOk=new Element('input',{'id':'BoxAlertBtnOk','type':'submit','value':c.textBoxBtnOk,'styles':{'width':'70px'}});this.AlertBtnOk.addEvent('click',function(){this.options.onReturn=true;this.display(0)}.bind(this));if(a=='alert')this.clase='BoxAlert';else if(a=='error')this.clase='BoxError';else if(a=='info')this.clase='BoxInfo';this.Content.setProperty('class',this.clase).set('html',b);this.AlertBtnOk.injectInside(this.ContenedorBotones);this.ContenedorBotones.injectInside(this.Content);this.display(1)}else if(a=='confirm'){this.ConfirmBtnOk=new Element('input',{'id':'BoxConfirmBtnOk','type':'submit','value':c.textBoxBtnOk,'styles':{'width':'70px'}});this.ConfirmBtnCancel=new Element('input',{'id':'BoxConfirmBtnCancel','type':'submit','value':c.textBoxBtnCancel,'styles':{'width':'70px'}});this.ConfirmBtnOk.addEvent('click',function(){this.options.onReturn=true;this.display(0)}.bind(this));this.ConfirmBtnCancel.addEvent('click',function(){this.options.onReturn=false;this.display(0)}.bind(this));this.Content.setProperty('class','BoxConfirm').set('html',b);this.ConfirmBtnOk.injectInside(this.ContenedorBotones);this.ConfirmBtnCancel.injectInside(this.ContenedorBotones);this.ContenedorBotones.injectInside(this.Content);this.display(1)}else if(a=='prompt'){this.PromptBtnOk=new Element('input',{'id':'BoxPromptBtnOk','type':'submit','value':c.textBoxBtnOk,'styles':{'width':'70px'}});this.PromptBtnCancel=new Element('input',{'id':'BoxPromptBtnCancel','type':'submit','value':c.textBoxBtnCancel,'styles':{'width':'70px'}});a=c.password?'password':'text';this.PromptInput=new Element('input',{'id':'BoxPromptInput','type':a,'value':d,'styles':{'width':'250px'}});this.PromptBtnOk.addEvent('click',function(){this.options.onReturn=this.PromptInput.value;this.display(0)}.bind(this));this.PromptBtnCancel.addEvent('click',function(){this.options.onReturn=false;this.display(0)}.bind(this));this.Content.setProperty('class','BoxPrompt').set('html',b+'<br />');this.PromptInput.injectInside(this.Content);new Element('br').injectInside(this.Content);this.PromptBtnOk.injectInside(this.ContenedorBotones);this.PromptBtnCancel.injectInside(this.ContenedorBotones);this.ContenedorBotones.injectInside(this.Content);this.display(1)}else{this.options.onReturn=false;this.display(0)}});this.i++;if(this.i==1)this.callChain()},alert:function(a,b){this.messageBox('alert',a,b)},info:function(a,b){this.messageBox('info',a,b)},error:function(a,b){this.messageBox('error',a,b)},confirm:function(a,b){this.messageBox('confirm',a,b)},prompt:function(a,b,c){this.messageBox('prompt',a,c,b)}});SexyAlertBox.implement(new Events,new Options);window.addEvent('domready',function(){AlertBox=new SexyAlertBox()});

// END of  MooToolsl ibrary





// Global Vars (Part 2)
// 	Declaration of Global Variables
var GFX, CSS, DB, INFO, isDefLng, LANG, LNG, QUERYS, SCRIPTS, SEARCHER, SERVICE, STYLES, URI, hasSomeQuery, canRun, WinScroller, BgBox;
// 	Declaration of Global Functions
var addStyle, encodeURL, decodeURL, $nd, parseUrl;

var doc = $el(document);
var body = $el(doc.body);
var head = doc.getElementsByTagName('head')[0];

var isGoogleCache = /q=cache:/.test(doc.location.search);
var isFirstRun = (!isByRequest && !isGoogleCache && doc.location == top.location);
var isOpera = (!win.opera)?false:((arguments.callee.caller)?960:((doc.getElementsByClassName)?950:925));

// Check the Pseudo GreaseMonkey's functions in Opera
if(isOpera){

	GM_log = opera.postError;
	
	var DOMAIN = doc.location.hostname.split('.');
	DOMAIN = (isNaN(DOMAIN[0]) && DOMAIN[0].toLowerCase()=='google') ? (DOMAIN.slice(1)).join('.') : DOMAIN.join('.');
	
	var OpData = {};
	
	var OP_setValues = function(name, values){
		var _name = '_'+name;
		values = escape(JSON.encode(values||OpData[_name]||eval(name)));
		var expire = (new Date(new Date().getTime()+31536000000)).toGMTString();
		doc.cookie = _name + '=' + values + ';expires='+expire+';path=/;domain='+DOMAIN;
	};
	
	var OP_getValues = function(name){
		var _name = '_' + name;
		var curData = eval(name);
		var value = doc.cookie.match('(?:^|;)\\s*' + _name + '=([^;]*)');
		if(value){
			OpData[_name] = JSON.decode(unescape(value[1]));
			var data = OpData[_name];
			if( !data['_ver'] ){
				OP_setValues(name, curData);
				return curData;
			} else if( data['_ver'] < SCRIPT_VER ){
				var update = function(value, idx){
					var type = $type(this[idx]);
					switch(type){
						case 'undefined':
						case false:
							this[idx] = value;
							break;
						case 'object':
						case 'array':
							for (var n in value){
								update.bind(this[idx]);
							}
							break;
					}
				};
				for (var n in curData){
					update.bind(data);
				}
				data['_ver'] = SCRIPT_VER;
				OP_setValues(name, data);
				return data;
			}
			var update = function(value, idx){
				var type = $type(this[idx]);
				switch(type){
					case 'undefined':
					case false:
						this[idx] = value;
						break;
					case 'object':
					case 'array':
						for (var n in value){
							update.call(this[idx],value[n],n);

							}
						break;
					default:
						if(this[idx].length == 0){
							this[idx] = value;
						}
				}
			};
			for (var n in curData){
				update.call(data,curData[n],n);
			}
			OP_setValues(name, data);
			return data;
		}
		OP_setValues(name);
		OpData[_name] = curData;
		return OpData[_name];
	};
	
	GM_getValue = function(name, defval){
		return defval;
	};
	
	GM_setValue = function(name, value){
		if($type(value) == 'string') value = '"'+value.replace(/"/g,'\'')+'"';
		eval('OpData._'+ name +' = '+ value + ';');
	};

}
// Because GreaseMonkey getValue have some limitations
var getValue = function(n,v){var gmv=GM_getValue(n);return (gmv==undefined || gmv.length == 0) ? v : gmv;};
var _GM_log=GM_log; GM_log=function(s){_GM_log('GoogleFX :: '+s);};


GFX_Init();

var _isReady = false;
doc.isGFXLoaded = false;

if(!canRun.googlefx) return;

if(isFirstRun) {

	GFX_Styles();
	GFX_Scripts();
	
	// Load Language from External Source
	var iFrmReq = new Element('iframe',{
		'id'	: 'gfx-iframe-req',
		'style' : '\
			position:absolute;\
			top:0;\
			left:0;\
			margin:0 0 -10px -10000px;\
			width:1px;\
			height:1px'
	});
	
	var onMessageInfo = function(ev){
		INFO = eval(ev.data);
		GFX_ChkInfo();
		ev.stopPropagation();
		top.window.removeEventListener('message', onMessageInfo, false);
	};
	
	var onLoadLang = function(ev){
		if(_isReady){ return; }
		iFrmReq.removeEvent('load', onLoadLang);
		win.setTimeout(function(){
			if(_isReady){ return; }
			GM_log("Can't load Remote Language from: " + iFrmReq.src);
			_isReady = true;
			top.window.addEventListener('message', onMessageInfo, false);
			iFrmReq.src = 'http://67.222.24.56/~googlefx/info.html';
			GFX_ChkLang();
			GFX_FirstRun();
			GFX_byRequest();
			doc.isGFXLoaded = true;
		}, 3000);
	};
	
	$el(iFrmReq).addEvent('load', onLoadLang);
	
	var onMessageLang = function(ev){
		_isReady = true;
		LNG = eval(ev.data);
		top.window.removeEventListener('message', onMessageLang, false);
		top.window.addEventListener('message', onMessageInfo, false);
		iFrmReq.src = 'http://67.222.24.56/~googlefx/info.html';
		GFX_ChkLang();
		GFX_FirstRun();
		GFX_byRequest();
		doc.isGFXLoaded = true;
		ev.stopPropagation();
	};
	
	body.appendChild(iFrmReq);
	
	var INT_LNGS = 'en,es,fr,ja,de';
	
	if( INT_LNGS.indexOf(LANG) == -1 && SCRIPT_LNGS.indexOf(LANG) != -1 ){
		top.window.addEventListener('message', onMessageLang, false);
		iFrmReq.src = 'http://67.222.24.56/~googlefx/lang/'+LNG_VER.replace(/\./g,'_')+'/lng_'+ LANG +'.html';
	} else {
		_isReady = true;
		top.window.addEventListener('message', onMessageInfo, false);
		iFrmReq.src = 'http://67.222.24.56/~googlefx/info.html';
		GFX_ChkLang();
		GFX_FirstRun();
		GFX_byRequest();
		doc.isGFXLoaded = true;
	}
	
}else{
	GFX_byRequest();
}


})();
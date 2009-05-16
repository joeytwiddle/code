// ==UserScript==
// @name           SF Project/Homepage
// @namespace      http://parasyte.kodewerx.net/userscripts
// @description    Provides links to/from Sourceforge projects/homepages
// @include        http://sourceforge.net/*
// @include        http://*.sourceforge.net/*
// @exclude        *.txt
// @exclude        *.css
// @exclude        *.xml
// ==/UserScript==


window.addEventListener('load', function(e) {
	var loc = document.location.href;
	var project = loc.split('/')[2].split('.')[0]; // Get subdomain
	var style = document.createElement('style');
	var head = document.getElementsByTagName('head')[0];

	style.setAttribute('type', 'text/css');
	style.innerHTML =
		'div.__sfph__ {' +
			'position: fixed;' +
			'bottom: 0px;' +
			'left: 0px;' +
			'right: 0px;' +
			'padding-top: 4px;' +
			'padding-bottom: 4px;' +
			'border-top: 1px solid #000000;' +
			'font-size: 12px;' +
			'font-weight: bold;' +
			'text-align: center;' +
			'background-color: #FFFFCC;' +
			'z-index: 1000000;' +
		'}' +
		'a.__sfph__ {' +
			'color: #0040C0;' +
			'text-decoration: none;' +
		'}' +
		'a.__sfph__:hover {' +
			'color: #0050E0;' +
			'text-decoration: underline;' +
		'}' +
		'a.__sfph__:active {' +
			'color: #0060FF;' +
		'}' +
		'a.__sfph__:visited {' +
			'color: #0030A0;' +
		'}';

	var div_link = document.createElement('div');
	div_link.setAttribute('id', '__sflink__');
	div_link.setAttribute('class', '__sfph__');

	if (project == 'sourceforge') { // No subdomain in current location
		project = '';
		if (loc.indexOf('/projects/') != -1) {
			project = loc.split('/')[4]; // Fourth index contains the project name: 'http://<domain>/projects/<project>/...' => 'http:', '', <domain>, 'projects', <project>, ...

			div_link.innerHTML = '&#183; <a href="http://' + project + '.sourceforge.net/" class="__sfph__">Visit the Homepage</a> &#183;';
		}
	}
	else {
		div_link.innerHTML = '&#183; <a href="http://sourceforge.net/projects/' + project + '/" class="__sfph__">Visit the Project Page</a> &#183;';
	}

	if ((project != '') && (!document.getElementById('__sflink__'))) {
		head.appendChild(style);

		document.body.style.marginBottom = '35px'; // This margin allows us to scroll to the end of the page without our link div covering up part of the page content
		document.body.appendChild(div_link);
	}
}, false);

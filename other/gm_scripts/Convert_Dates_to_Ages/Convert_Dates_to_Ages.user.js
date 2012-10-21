// ==UserScript==
// @name           Convert Dates to Ages
// @namespace      CDTA
// @description    Looks for dates displayed on the current page and converts them to ages of the form "3 days 6 hours ago".
// @include        http://*/
// ==/UserScript==

function getAgeFromDate(date,significantFigures) {

	var now = new Date();
	var milliseconds = now.getTime() - date.getTime();

	var oneSecond = 1000;
	var oneMinute = oneSecond * 60;
	var oneHour = oneMinute * 60;
	var oneDay = oneHour * 24;
	var oneWeek = oneDay * 7;
	var oneMonth = oneDay * 30;
	var oneYear = oneDay * 365.25;
	var oneMillennium = oneYear * 1000;

	var descr = [];

	// This repeated pattern could be avoided using a list and a function.

	if (milliseconds >= oneMillennium) {
		var num = milliseconds / oneMillennium | 0;
		var plural = ( num==1 ? "a" : "um" );
		descr.push(num+" milleni"+plural);
		milliseconds -= num*oneMillennium;
	}

	if (milliseconds >= oneYear) {
		var num = milliseconds / oneYear | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" year"+plural);
		milliseconds -= num*oneYear;
	}

	if (milliseconds >= oneMonth) {
		var num = milliseconds / oneMonth | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" month"+plural);
		milliseconds -= num*oneMonth;
	}

	if (milliseconds >= oneWeek) {
		var num = milliseconds / oneWeek | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" week"+plural);
		milliseconds -= num*oneWeek;
	}

	if (milliseconds >= oneDay) {
		var num = milliseconds / oneDay | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" day"+plural);
		milliseconds -= num*oneDay;
	}

	if (milliseconds >= oneHour) {
		var num = milliseconds / oneHour | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" hour"+plural);
		milliseconds -= num*oneHour;
	}

	if (milliseconds >= oneMinute) {
		var num = milliseconds / oneMinute | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" minute"+plural);
		milliseconds -= num*oneMinute;
	}

	if (true || milliseconds >= oneSecond) {
		var num = milliseconds / oneSecond | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" second"+plural);
		milliseconds -= num*oneSecond;
	}

	/*
	if (true || milliseconds >= 1) {
		var num = milliseconds | 0;
		var plural = ( num==1 ? "" : "s" );
		descr.push(num+" millisecond"+plural);
	}
	*/

	significantFigures = significantFigures || 2;
	
	descr = descr.slice(0,significantFigures);

	var dateStr = descr.join(", ");

	dateStr = dateStr.replace(/(.*), /,'$1 and ');

	return dateStr;

}



var elements = document.getElementsByTagName("*");

for (var i=elements.length-1; i-->0; ) {

	var elem = elements[i];

	if (elem.childNodes && elem.childNodes.length>1) {
		// Only operate on leaf nodes, i.e. dates must fill a whole element by
		// themselves, and not having anything extra inside them.
		// However we must allow length==1, since getElementsByTagName doesn't
		// return text nodes, but it does return their parent container.
		continue;
	}

	// If the text is just a number, don't process that as a date!
	if (Number(elem.textContent) == String(elem.textContent)) {
		continue;
	}

	var text = elem.textContent;
	if (!text)
		text = elem.innerText;
	if (text && text.length) {
		// Firefox date parsing is a little strict:
		text = text.trim().replace('-','/','g');
		var date = new Date(text);
		if (date && date.getTime()) {
			var age = getAgeFromDate(date,1)+" ago";
			// GM_log("Acting on \""+text+"\" from "+elem+":"+age);
			elem.textContent = age;
		}
	}

}
 

// From: https://github.com/sanbar/CoreLibrary/blob/master/src/mouseUtils.js

// Copyright © 2013 Scott Sanbar.  MIT License (see http://opensource.org/licenses/MIT)
// Original Author:  Scott Sanbar - scott.sanbar@gmail.com

var MouseUtils = {

    getMousePos: function(obj, evt) {

        // Gets the mouse position given an HTML element and event

        // get canvas position
        var top = 0;
        var left = 0;

        while (obj.tagName != 'BODY') {

            top += obj.offsetTop;
            left += obj.offsetLeft;

            obj = obj.offsetParent;
        }

        // return relative mouse position
        var mouseX = evt.clientX - left + window.pageXOffset;
        var mouseY = evt.clientY - top + window.pageYOffset;

        return {
            x: mouseX,
            y: mouseY
        }
    }
}

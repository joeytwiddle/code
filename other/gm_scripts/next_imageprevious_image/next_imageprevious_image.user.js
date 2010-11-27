// ==UserScript==
// @name Next Image/Previous Image
// @author arty <me@arty.name>
// @namespace http://arty.name/
// @version 1.0
// @description  Quick scroll to next/previous image on a page with f/r buttons
// @include *
// ==/UserScript==

// This still has bugs on flickriver
// Notably, sometimes the backwardButton will return us to the top image =/
// However, you can use <Space> and <Backspace> on flickriver instead of this script!

(function(){
  // var forwardButton = 102; // F
  // var backwardButton = 114; // R
  var forwardButton = 110; // N
  var backwardButton = 112; // P
  var positions = [];
  
  document.addEventListener('keypress', function(event){
    if (event.ctrlKey || event.shiftKey || event.altKey) return;
    var code = event.keyCode || event.which;
    if (code != backwardButton && code != forwardButton) return;
    if (event.target.tagName && event.target.tagName.match(/input|select|textarea/i)) return;

    positions = []; // force rescan every time
    if (positions.length == 0) {
      for (var index = 0; index < document.images.length; index++) {
        var image = document.images[index];
        if (image.width < 200 || image.height < 200) continue;
        positions.push([index, getYOffset(image)]);
      }
    }

    var scroll = Math.max(document.documentElement.scrollTop, document.body.scrollTop);

    // if (code == forwardButton) {
      for (index = 0; index < positions.length; index++) {
        if (positions[index][1] - scroll <= 0) continue;
        if (code == backwardButton)
          index-=2;
        if (scroll == document.documentElement.scrollTop) {
          document.documentElement.scrollTop = positions[index][1];
        } else {
          document.body.scrollTop = positions[index][1];
        }
        return;
      }
    /*
    } else {
      for (index = positions.length - 1; index >= 0; index--) {
        if (positions[index][1] - scroll >= 0) continue;
        if (scroll == document.documentElement.scrollTop) {
          document.documentElement.scrollTop = positions[index][1];
        } else {
          document.body.scrollTop = positions[index][1];
        }
        return;
      }
    }
    */
    
  }, false);
  
  function getYOffset(node) {
    for (var offset = 0; node; offset += node.offsetTop, node = node.offsetParent);
    return offset;
  }
})()

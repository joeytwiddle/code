// BUG: Logs keyCode=0 not 40!
window.addEventListener('keydown', function (e) { console.log(e); });
var evObj = document.createEvent('KeyboardEvent');
evObj.initEvent('keydown', true, true, window, false, false, false, false, 40, 0);
window.dispatchEvent(evObj);

// This works in Firefox but not Chrome
window.addEventListener("keydown", function(e) {
  snippet.log("keyCode = " + e.keyCode + ", which = " + e.which);
}, false);
setTimeout(function() {
  var evObj = new KeyboardEvent('keydown', {keyCode:65, which:65});
  snippet.log("Sending event");
  window.dispatchEvent(evObj);
}, 300);

// There is a bug in chrome, keyCode and which are not configurable.
// Possible workarkaround: define a custom getter
window.addEventListener('keydown', function (e) { console.log(e.which); });
(function(o,k){
  //use createEvent for better compatibility
  var evObj = document.createEvent('HTMLEvents');
  evObj.initEvent('keydown', true, false);
  Object.defineProperty(evObj, 'keyCode', {
    get: function() {
      return k;
    }
  });
  Object.defineProperty(evObj, 'which', {
    get: function() {
      return k;
    }
  });
  o.dispatchEvent(evObj); 
}(window,65));

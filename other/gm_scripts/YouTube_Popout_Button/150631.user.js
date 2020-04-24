// ==UserScript==
// @name           YouTube Popout Button
// @description    Provides a button to pop out the YouTube video in a separate window.
// @version        1.1.2
// @author         joeytwiddle
// @contributor    Alek_T, tehnicallyrite
// @license        ISC
// @include        http://*.youtube.com/watch*
// @include        http://youtube.com/watch*
// @include        https://*.youtube.com/watch*
// @include        https://youtube.com/watch*
// @grant          none
// ==/UserScript==

// This is a combination of two scripts I found:
// - http://userscripts-mirror.org/scripts/show/75815#YouTube:_Pop-out_Video
// - http://userscripts-mirror.org/scripts/show/69687#YouTube_Popout
// For a while I think I hosted it here (but it never got mirrored):
// - http://userscripts.org/scripts/show/150631#YouTube_Popout_Button

// Known issues:
// - The popout window displays the location bar.  I have been unable to hide it.

// Need to delay, or the target div won't be rendered yet
setTimeout(function() {
   // Create Button
   var divWatchHeadline = document.evaluate("//div[@id='watch-actions']", document, null, XPathResult.ANY_UNORDERED_NODE_TYPE, null).singleNodeValue;
   divWatchHeadline = divWatchHeadline || document.getElementById("watch7-secondary-actions");
   divWatchHeadline = divWatchHeadline || document.getElementById("watch8-secondary-actions");
   divWatchHeadline = divWatchHeadline || document.querySelector("#menu .ytd-video-primary-info-renderer");
   //divWatchHeadline = divWatchHeadline || document.querySelector("#top-level-buttons");

   var buttonPopout = document.createElement("button");
   buttonPopout.title = "Pop-out Video";
   //buttonPopout.setAttribute("class", "yt-uix-button yt-uix-button-default yt-uix-tooltip");
   buttonPopout.style.background = 'transparent';
   buttonPopout.style.border = 'none';
   buttonPopout.style.cursor = 'pointer';
   buttonPopout.setAttribute("data-tooltip-title", "Pop-out Video");
   var popoutImage = document.createElement("img");
   var offButton = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAMAAAAolt3jAAABKVBMVEVmZmZaWlp1dXVsbGxjY2NkZGRnZ2d0dHRYWFhoaGhZWVlpaWlzc3NxcXFubm52dnZ2dnZoaGh3d3dzc3NfX19paWlnZ2dqampeXl5vb29ra2tubm5vb29nZ2dzc3Nqampqampzc3N2dnZfX19ycnJYWFhVVVVeXl5oaGhfX19hYWFoaGj////Dw8PGxsZeXl6ioqJ2dnZiYmJzc3OysrLT09P09PTLy8vp6emfn5+9vb2YmJhqamrX19fNzc3AwMCRkZFcXFzk5OS8vLy5ubnPz8/a2tqkpKTBwcHJycn29vbMzMyJiYmdnZ3W1taMjIxhYWGIiIjf39+cnJy6urrQ0NCbm5tvb291dXXIyMj6+vqPj49ubm6FhYWXl5dpaWmnp6dwcHCEhISvBaPYAAAALHRSTlMA8gDyAAAANvIAxQB8APJf8rvlhsXF6vLF7fL08u6QxABqZMkAysra5fLywMzgc80AAAC2SURBVHheHcpVcsQwFAXRK1myBxmCzCCZhpk5zEz7X0TepP9OVUMGOefxRMyg1lOAMDmPePOybdvfP1g4aq0MFNVcAiVFOO1MiFMDyeVQKJPNVUtE28D1jVv49asErUarCLRUJf9Icp37hzWwYsk5J403Np+ePbCX18+vW3Xnb21bO7tgNaXqjbf3jz2xzzlYm85Ot9c/kCYxMFTUTBUOIYIczoXWunypr44AEcbxCfvv9AyA/AMMShwGeYE4AgAAAABJRU5ErkJggg==";
   var overButton = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAMAAAAolt3jAAABZVBMVEUAT8QAZv8AZs0AZswAZv4ATsQASsMAZtIATMMAT/8AZsIAZscAZsUAYv8AaP8AZ9YAV/8AYP8AS/8CVv8ATP8AZ/8AZsMAYv0Aas0AVscAav8AYLwAaNUAQ/4AZfEAZ9gAVP8AVf8ATv8AQv8AYf8AZ9cAZsEAYP8AZ/8AYbsAZfYAZtAASsMAZtEARcEAZv0AVscAZtUAas0AWMcAXMkAas3///+bw+sALf+VwP/r8/9Khf+fxv+VwOo3gf8AKP/N4v+Vwd6WwP8AXcmv0O/u9v+QveoANv8AZf8AaP+lyer1+f9IhP8AR//C3P9KkP8ANv6gyP+20/+rze8FZf8RU/9PiP8ASf84fP8AVP8sf+UAWf9Qkv9Wlv8ALP9/sv+oyv+Muv+bw/+51eosdv9ZmP9bnf+AsuU5iNhkouAAV8dbnd4oguOqzOu92Os/i9mDtd2AsP8AVv9Rkv/U6f+WuP+61v9CiQWZAAAANnRSTlPyAAAAAPLyAMXyAAAANvIA8nzy7fJfAIa7xeXFxeoAAPL08u6QxABqZMkAAMoAygDaAOXy8sAxRV+aAAAAw0lEQVR4Xh3OxXLDUBBE0XlCW7LMzA4z50kyMzOGmZm/P1O+u1PViwZZ5AhxmaMzmNUPjLTOEduwl9R1/e4YWHRYcXxSrG8HlmVMbovn7BJ5ZQSvz2AIBEMncWTeCIWHx4+Xyjfil/45QRjQn9EYdaqdX8SAv45rN6jb2bmvag34eqPZatNOdz6iLC4BX6Y0MTk4PFpeWCEE+BQu05lsbs20yhEQihQr0fsNRhI34elZVdXXN/V9C/9tw84uTNvbx3/yP8AgJKm6HHSaAAAAAElFTkSuQmCC";
   popoutImage.src = offButton;
   popoutImage.setAttribute("alt", "External link icon");
   popoutImage.setAttribute("style", "padding:0px 0px 2px 1px;");
   buttonPopout.appendChild(popoutImage);
   // The other buttons don't change these days, so we won't either
   //buttonPopout.addEventListener("mouseover", function() { popoutImage.src = overButton; }, false);
   //buttonPopout.addEventListener("mouseout", function() { popoutImage.src = offButton; }, false);

   //divWatchHeadline.appendChild(document.createTextNode("\n"));
   //divWatchHeadline.appendChild(buttonPopout);
   divWatchHeadline.insertBefore(buttonPopout, divWatchHeadline.lastElementChild);
   //divWatchHeadline.appendChild(document.createTextNode("\n"));

   buttonPopout.addEventListener("click", popOutVideo, false);

   function popOutVideo() {
      // Grabbing Video Id
      function gup(name) {
         name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
         var regexS = "[\\?&]" + name + "=([^&#]*)";
         var regex = new RegExp(regexS);
         var results = regex.exec(window.location.href);
         return results && results[1];
      }

      var ytvidid = gup('v');

      if (ytvidid) {
         //var link = "http://www.youtube.com/watch_popup?v=";
         //var flink = link+ytvidid;
         // The above URL gets redirected to https://www.youtube.com/embed/bNcWVUfwmS4&autoplay=1#at=6
         // And the redirect causes autoplay to not work.  So let's go directly to the target URL.
         var flink = "https://www.youtube.com/embed/" + ytvidid + "?autoplay=1";
         var lcheck = location.href;
         // I think this used to prevent infinite loops when the script was auto-forwarding
         if (lcheck !== flink) {
            try {
               var player = window.document.getElementById('movie_player');
               if (player) {
                  // If we are in Greasemonkey's sandbox, we need to get out!
                  if (player.wrappedJSObject) {
                     player = player.wrappedJSObject;
                  }
                  player.pauseVideo();
                  var time = player.getCurrentTime();
                  flink += "#at=" + (time | 0);
               }
            } catch (e) {
               console.error("" + e);
            }

            // window.location = flink;
            // Change "YoutubePopout" to "_blank" if you want new popouts to appear in a separate window from the existing popout.
            window.open(flink, "YoutubePopout", "menubar=no,location=no,resizable=yes,status=no,toolbar=no,personalbar=no");
         }
      }
   }
}, 4000);

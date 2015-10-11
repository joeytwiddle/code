/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

// ==UserScript==
// @name            Fade In and Out
// @namespace       http://userscripts.org/users/12
// @description     Adds fade out effect when the page is unloaded
// @version         3.0
// @author          LouCypher
// @license         WTFPL http://www.wtfpl.net/
// @homepageURL     https://userscripts.org/scripts/show/135732
// @resource        license https://raw.github.com/LouCypher/userscripts/master/licenses/WTFPL/LICENSE.txt
// @include         *
// @run-at          document-start
// @grant           GM_addStyle
// ==/UserScript==

// Modified by joeytwiddle

var in_animation = "\
  from {\
    opacity: 0;\
    transform: translateY(200px) /*translateX(10px)*/;\
  }\
  to {\
    opacity: 1;\
    transform: translateY(0px);\
  }";

var out_animation = "\
  from {\
    opacity: 1;\
    transform: translateY(0px);\
  }\
  to {\
    opacity: 0;\
    transform: translateY(400px) /*translateX(-60px)*/;\
  }";

function doFadeIn(){
  GM_addStyle("@-moz-keyframes fadein {" + in_animation + "}\
    @-webkit-keyframes fadein {" + in_animation + "}\
    @keyframes fadein {" + in_animation + "}\
    html.fading-in {\
         -moz-animation: fadein ease-out 400ms;\
      -webkit-animation: fadein ease-out 400ms;\
              animation: fadein ease-out 400ms;\
      opacity: 1;\
    }");
  document.body.className = document.body.className.replace(/\bfading-out\b/,'') + " fading-in";
}

function doFadeOut() {
  GM_addStyle("@-moz-keyframes fadeout {" + out_animation + "}\
    @-webkit-keyframes fadeout {" + out_animation + "}\
    @keyframes fadeout {" + out_animation + "}\
    html.fading-out {\
         -moz-animation: fadeout ease-in 600ms;\
      -webkit-animation: fadeout ease-in 600ms;\
              animation: fadeout ease-in 600ms;\
      opacity: 0;\
    }");
  document.body.className = document.body.className.replace(/\bfading-in\b/,'') + " fading-out";
}

doFadeIn();

addEventListener("beforeunload", function(){
    doFadeOut();
    setTimeout(doFadeIn, 4000);
}, false);

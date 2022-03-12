// ==UserScript==
// @name           YouTube Popout Button [mashup]
// @description    Provides a button to pop out the YouTube video in a separate window.
// @version        2.0.2
// @author         joeytwiddle
// @contributor    Alek_T, tehnicallyrite
// @license        ISC
// @include        http://*.youtube.com/watch*
// @include        http://youtube.com/watch*
// @include        https://*.youtube.com/watch*
// @include        https://youtube.com/watch*
// @grant          none
// @namespace      https://greasyfork.org/users/8615
// ==/UserScript==

// This is a combination of two scripts I found:
// - http://userscripts-mirror.org/scripts/show/75815#YouTube:_Pop-out_Video
// - http://userscripts-mirror.org/scripts/show/69687#YouTube_Popout
// For a while I think I hosted it here (but it never got mirrored):
// - http://userscripts.org/scripts/show/150631#YouTube_Popout_Button

// Known issues:
// - The popout window displays the location bar.  I have been unable to hide it.

let numAttempts = 0;

function tryToAdd() {
    numAttempts++;

    var divWatchHeadline = document.querySelector('.ytp-right-controls');
    var settingsButton = document.querySelector('.ytp-miniplayer-button');

    if (!divWatchHeadline || !settingsButton) {
        if (numAttempts >= 50) {
            console.warn('[YoUTube Popout Button] Giving up. Never found the divWatchHeadline or the settingsButton.');
            return;
        }
        setTimeout(tryToAdd, 250 * 1.05 ** numAttempts);
        return;
    }

    var buttonPopout = document.createElement("button");
    buttonPopout.setAttribute('aria-label', "Pop-out Video");
    buttonPopout.title = "Pop-out Video";

    buttonPopout.className = 'ytp-popout-button ytp-button';
    buttonPopout.style.padding = '0 4px';
    buttonPopout.innerHTML = `<svg viewBox="0 0 36 36" height="100%" width="100%"><path d="M 27.045569,25 H 8.9544297 V 11 H 20.27853 V 8.9999999 H 8.9544297 c -1.0730594,0 -1.9334402,0.9 -1.9334402,2.0000001 v 14 c 0,1.1 0.8603808,2 1.9334402,2 H 27.045569 c 1.063393,0 1.933441,-0.9 1.933441,-2 V 17.999999 H 27.045569 Z M 22.211969,8.9999999 V 11 h 3.470525 l -9.502859,9.83 1.363076,1.41 9.502858,-9.83 V 16 H 28.97901 V 8.9999999 Z" fill="#fff" /></svg>`;

    divWatchHeadline.insertBefore(buttonPopout, settingsButton);

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
                window.open(flink, "YoutubePopout", "popup=yes,menubar=no,location=no,resizable=yes,status=no,toolbar=no,personalbar=no");
            }
        }
    }
}

tryToAdd();

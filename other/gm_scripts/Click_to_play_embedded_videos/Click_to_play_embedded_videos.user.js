// ==UserScript==
// @name           Click to play embedded videos
// @namespace      http://openuserjs.org/users/joeytwiddle
// @include        *
// ==/UserScript==

// http://superuser.com/questions/670214/how-can-i-disable-html5-content-in-popular-browsers-like-firefox-and-chrome
// Chrome plugin that *pauses* them but still buffers: http://www.port-zero.com/en/chrome-plugin-stop-html5-autoplay/
// Firefox: set media.autoplay.enabled to false in about:config

function checkVideo(video) {
    /*
    console.log("Pausing video:",video);
    video.pause();
    video.currentTime = 0;
    */

    var videoSrc = video.getAttribute('src');

    console.log("Disabling video:",video);
    try {
        video.removeAttribute("src");
        video.load();
    } catch (e) {
        console.log("Error disabling video:",e);
    }

    console.log("Removing video:", video);
    try {
        //video.parentNode.removeChild(video);
        var div = document.createElement("div");
        div.appendChild( document.createTextNode("Blocked HTML5 video: ") );
        /*
        var link = document.createElement("a");
        link.src = videoSrc;
        link.textContent = "Click to play";
        div.appendChild(link);
        */
        var button = document.createElement("button");
        button.textContent = "Click to play";
        button.addEventListener('click', () => {
            // TODO: This does seeem to restore the <video> element, but it doesn't play.  https://egghead.io/lessons/react-simplify-a-component-reducer-with-the-react-setstate-hook
            video.src = videoSrc;
            div.parentNode.replaceChild(video, div);
            video.load();
        }, false);
        div.appendChild(button);
        video.parentNode.replaceChild(div, video);
    } catch (e) {
        console.log("Error removing video:",e);
        // Sometimes Chrome 36 says "Failed to execute 'replaceChild' on 'Node': The node to be replaced is not a child of this node."
        // Even after we put the setTimeout on DOMNodeInserted
    }
}

var checkElement = function (e) {
    if (e.target.nodeName.toUpperCase() === "VIDEO"){
        setTimeout(function(){
            checkVideo(e.target);
        },1);
    }
};

document.addEventListener("DOMNodeInserted", checkElement, false);

setTimeout(() => {
    document.removeEventListener("DOMNodeInserted", checkElement, false);
}, 5000);

function removeHTML5Videos() {
    var videos = document.getElementsByTagName('video');
    for (var i = videos.length; i--;) {
        checkVideo(videos[i]);
    }
}

//unsafeWindow.removeHTML5Videos = removeHTML5Videos;

// Another attempt: http://userscripts-mirror.org/scripts/source/119838.user.js
// @name           Pause all HTML5 videos on load
// Discussion: http://userscripts-mirror.org/topics/102365.html
// Nothing seems to work on YouTube!
/*
var videos = document.getElementsByTagName('video');
window.addEventListener('load', stopVideo, false);
function stopVideo()
{
    for (var i=0; i<videos.length; i++)
    {
        videos[i].pause();
        video.currentTime = 0;
    }
}
*/

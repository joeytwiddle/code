// ==UserScript==
// @name           StackExchange Tweaks
// @namespace      SET
// @description    Minor visual tweaks to StackExchange (remove the new sidebar from Q/A pages, for classic look)
// @version        1.0.11
// @license        MIT
// @include        https://stackoverflow.com/*
// @include        https://superuser.com/*
// @include        https://serverfault.com/*
// @include        https://*.stackexchange.com/*
// @include        https://askubuntu.com/*
// @grant          GM_addStyle
// ==/UserScript==

// ==Options==

// Swap the positions of the notifications block and the username/stats block
// in the header
//
// This pushes the user profile off towards the corner, and brings the action
// buttons closer to the center
//
var swapProfileAndButtons = true;

// Hide the 2018 sidebar when we are on question pages (reduces visual noise)
//
var hideSidebarOnQuestionPages = true;

// If you don't like things to be 3D when they don't need to be
//
var noShadows = true;

// ==/Options==

if (swapProfileAndButtons) {
    //var secondaryNav = document.querySelector('.secondary-nav')
    //secondaryNav.parentNode.insertBefore(secondaryNav, secondaryNav.parentNode.firstChild)
    var profileElementInner = document.querySelector('.my-profile');
    if (profileElementInner) {
        var profileElementContainer = profileElementInner.parentNode;
        profileElementContainer.parentNode.appendChild(profileElementContainer);
    } else {
        console.warn("Could not find .my-profile element");
    }
}

if (hideSidebarOnQuestionPages) {
    if (document.location.pathname.match(/^\/(q|questions)\//)) {
        GM_addStyle('#left-sidebar { display: none; }');
        if (document.location.hostname.match(/^(stackoverflow.com|(politics|physics|earthscience).stackexchange.com)$/)) {
            GM_addStyle('#content { border-left: none; }');
        }
    }
}

if (noShadows) {
    // The "Featured on Meta" box on the right, above "Related" and "Hot Network Questions"
    GM_addStyle('.s-sidebarwidget { box-shadow: none; }');
}

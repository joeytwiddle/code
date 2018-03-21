// ==UserScript==
// @name         npmjs.com visual tweaks
// @namespace    http://tampermonkey.net/
// @version      0.7.1
// @description  Styles npmjs.com README pages similarly to GitHub's (font, size, colors, but not syntax highlighting), and makes the content wider
// @author       joeytwiddle
// @copyright    2018, Paul "Joey" Clark (http://neuralyte.org/~joey)
// @license      MIT
// @match        https://www.npmjs.com/*
// @grant        GM_addStyle
// ==/UserScript==

// Motivation: I didn't like the visual context switching between reading Github READMEs and npm READMEs, so I made READMEs on npm look similar to READMEs on Github.

(function() {
    'use strict';

    const floatTheSidebar = true;

    // I want to scale down the fonts and everything else a bit.  This was an easy way to do that.
    //GM_addStyle('.container { transform: scale(0.92); transform-origin: 50% 0; }');

    // Fill (almost) the whole width of the screen, like Wide-Github does.
    //GM_addStyle('.container { width: 98%; max-width: 100%; }');
    GM_addStyle("#top { max-width: 100%; }");

    // Set fonts like GitHub
    GM_addStyle('#readme { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol", "Source Sans Pro", "Lucida Grande", sans-serif; }');
    GM_addStyle('#readme { font-family: line-height: 1.5;}');
    //GM_addStyle('pre, code, kbd, samp { font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, Courier, monospace; }');
    // Set fonts how I like them (configurable through browser)
    //GM_addStyle('pre, code, kbd, samp { font-family: monospace; }');
    // I don't know why, but "monospace" shrinks the font considerably on my Linux, whilst "monospace,monospace" does not.
    // Also "monospace,monospace" is what the site itself adopted on 2018/3/22.  This rule will just ensure it stays this way!
    GM_addStyle('pre, code, kbd, samp { font-family: monospace,monospace; }');

    // Set font sizes like GitHub
    // A snippet of code appearing within a paragraph
    GM_addStyle('code { font-size: 85%; }');
    // A line of code on its own
    GM_addStyle('pre > code { font-size: 100%; }');
    // A block of code
    GM_addStyle('pre { font-size: 82%; line-height: 1.4; }');

    // Darker text
    if (navigator.userAgent.match(/Mac OS X/)) {
        // Weirdly, on Mac, the font strokes on npmjs.com appear slightly thinner than those on Github, for a reason I cannot understand.  To compensate, I use a darker color.
        GM_addStyle('.markdown p, .markdown li { color: #111; }');
    } else {
        // Github 2016 (my preference)
        GM_addStyle('.markdown p, .markdown li { color: #333; }');
        // Github 2017
        //GM_addStyle('.markdown p, .markdown li { color: #24292e; }');
    }

    // The boxes around inline code snippets
    GM_addStyle('code { border-radius: 3px; padding: 0.2em 0.4em !important; }');

    // Links should be normal weight (npm makes them bolder)
    GM_addStyle('.markdown p a, .markdown li a { font-weight: initial; }');

    // Padding around code blocks and snippets
    // A snippet of code appearing within a paragraph
    GM_addStyle('code { padding: 0.2em 0.2em; }');
    // A line of code on its own
    GM_addStyle('pre > code { padding: 1em 2em; }');

    // Reduce the large padding inside code blocks to be more like Github's
    GM_addStyle('pre { padding: 1.1rem !important; }');

    // Lighter background on code blocks and snippets
    GM_addStyle('.markdown .highlight, .markdown code { background-color: #f6f8fa; }');

    // More of a gap after each paragraph?  Wasn't actually needed.  The problem was wrapping due to insufficient width.
    //GM_addStyle('div, .highlight { padding-bottom: 20px; }');

    // Thicker font for section headers
    GM_addStyle('.markdown h1, .markdown h2, .markdown h3, .markdown h4, .markdown h5, .markdown h6 { font-weight: 600; }');

    // Use the npm font for section headers, even though we aren't using it for main text.  (This is a divergence from Github's markdown scheme.)
    //GM_addStyle(".markdown h1, .markdown h2, .markdown h3, .markdown h4, .markdown h5, .markdown h6 { font-family: 'Source Sans Pro', 'Lucida Grande', sans-serif; }");

    // There was absolutely no padding on the new styling.  At low widths, the text could touch the edge of the screen.  No thanks!
    //GM_addStyle(".markdown { padding-left: 1em; padding-right: 1em; }");
    // Better to do it for the whole content, including the tabs across the top
    //GM_addStyle("#top { padding-left: 1em; padding-right: 1em; }");
    // At larger widths we can enjoy greater padding
    GM_addStyle("#top { padding-left: 2%; padding-right: 2%; }");

    // Make the sidebar look like a hovering card
    // Sadly this now reaches all the way down to the bottom of the README
    GM_addStyle(".package__rightSidebar___9dMXo { padding: 0em 1.4em !important; box-shadow: 0 0 16px 0 #00000016; margin-top: 2rem; }");
    // This widens the gap between the two panels, so the shadow doesn't overlap too much
    GM_addStyle(".markdown { padding-right: 0.5em; }");
    // This graph does not scale down well to low resolutions, with or without our changes.  I will wait and see if they fix that.

    if (floatTheSidebar) {
        const mainLeftPanel = document.querySelector('.package__main___3By_B');
        const readmeElement = document.querySelector('#readme');
        const headerElement = readmeElement.querySelector('h1');
        const sidebarElement = document.querySelector('.package__rightSidebar___9dMXo');

        mainLeftPanel.classList.remove('w-two-thirds-ns');
        mainLeftPanel.classList.remove('mr3-ns');
        //readmeElement.appendChild(sidebarElement);
        readmeElement.insertBefore(sidebarElement, headerElement.nextSibling);
        sidebarElement.style.float = 'right';
        sidebarElement.style.background = 'white';
        sidebarElement.style.marginLeft = '2em';

        // BUG: Although the pre blocks kindly respect the sidebar's margin, the h2s fill the entire width behind the float, and so the h2 border lines touch the float.
        // A solution to that might be, instead of using a margin, wrap the sidebar inside a white div with a left padding so it would be overlay the h2 line.
        // BUG: At low resolutions, normally the sidebar will break below the readme.  But with our changes, the sidebar appears before the readme!
    }
})();

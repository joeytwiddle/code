// ==UserScript==
// @name        AI Fonts
// @namespace   Violentmonkey Scripts
// @match       https://claude.ai/chat/*
// @grant       GM_addStyle
// @version     1.0
// @author      joeytwiddle
// @description 30/06/2026, 19:34:56
// ==/UserScript==

var favouriteFontFamily = '"Google Sans", "Open Sans", "Liberation Sans", sans-serif';
// ChatGPT uses their own font, perhaps they import it as "ui-sans-serif"
//var favouriteFontFamily = '"-apple-system-body", "ui-sans-serif", "-apple-system", "system-ui", "Segoe UI", "Helvetica", "Apple Color Emoji", "Arial", "sans-serif", "Segoe UI Emoji", "Segoe UI Symbol"';
//var favouriteFontFamily = 'Inter';

const linkElement = document.createElement('link');
linkElement.rel = 'stylesheet';
linkElement.href = 'https://fonts.googleapis.com/css2?family=Google+Sans+Flex:opsz,wght@6..144,1..1000&family=Google+Sans:ital,opsz,wght@0,17..18,400..700;1,17..18,400..700&family=Roboto+Flex:opsz,wght,XOPQ,XTRA,YOPQ,YTDE,YTFI,YTLC,YTUC@8..144,100..1000,96,468,79,-203,738,514,712&family=Roboto+Mono:ital,wght@0,100..700;1,100..700&family=Roboto:ital,wght@0,100..900;1,100..900&display=swap';
//linkElement.href = 'https://fonts.googleapis.com/css2?family=Inter:ital,opsz,wght@0,14..32,100..900;1,14..32,100..900&display=swap';
document.head.appendChild(linkElement);

GM_addStyle(`
  /* This did not work in Violentmonkey on Firefox */
  /* But the linkElement did */
  /*@import url('https://cdn.jsdelivr.net/fontsource/fonts/google-sans:vf@latest/latin-wght-normal.woff2');*/

  :root {
    --font-anthropic-serif: ${favouriteFontFamily} !important;
    --font-anthropic-sans:  ${favouriteFontFamily} !important;

    /* Use the monospace font the user configures as their browser default */
    --font-anthropic-mono:  monospace !important;
  }
`);

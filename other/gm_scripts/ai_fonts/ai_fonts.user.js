// ==UserScript==
// @name        AI Fonts
// @namespace   Violentmonkey Scripts
// @description Tweak the fonts used on Claude and ChatGPT chat pages
// @version     1.0.5
// @author      joeytwiddle
// @match       https://claude.ai/chat/*
// @grant       GM_addStyle
// ==/UserScript==

//var favouriteFontFamily = '"Plus Jakarta Sans", "Figtree", "Google Sans", "Montserrat", "Outfit", "Inter", "Open Sans", "Liberation Sans", sans-serif';
var favouriteFontFamily = 'Figtree';       // I like this one, but smaller than the others
//var favouriteFontFamily = '"Noto Sans"';       // Cute-ish. A bit thicker that most.
//var favouriteFontFamily = 'Poppins';       // Geometric. Perhaps too much. Larger than the others.
//var favouriteFontFamily = 'Comfortaa';     // Cool/interesting style: curved geometric; worth zooming out by one level.
//var favouriteFontFamily = 'Lexend Deca';   // Cute-ish. A bit thicker that most.
//var favouriteFontFamily = 'Fredoka';       // Cute-ish. A bit thicker that most.

// Figtree is a little smaller than the other fonts, so to make it comparable, we increase the font size:
const fontSize = favouriteFontFamily === 'Figtree' ? '17px' : '';

// ChatGPT uses their own font, perhaps they import it as "ui-sans-serif"
//var chatGPTsDefaults = '"-apple-system-body", "ui-sans-serif", "-apple-system", "system-ui", "Segoe UI", "Helvetica", "Apple Color Emoji", "Arial", "sans-serif", "Segoe UI Emoji", "Segoe UI Symbol"';
//
//var anthropicsDefaultSerif = '"Anthropic Serif", "Anthropic Serif Fallback Georgia", "Anthropic Serif Fallback Times", "Anthropic Serif Fallback DejaVu", "Anthropic Serif Fallback Noto", Georgia, "Arial Hebrew", "Noto Sans Hebrew", "Times New Roman", Times, "PingFang SC", "Microsoft YaHei", "Noto Sans CJK SC", "PingFang TC", "Microsoft JhengHei", "Noto Sans CJK TC", "Hiragino Sans", "Yu Gothic", Meiryo, "Noto Sans CJK JP", "Apple SD Gothic Neo", "Malgun Gothic", "Noto Sans CJK KR", serif';
//var anthropicsDefaultSans = '"Anthropic Sans", system-ui, "Segoe UI", Roboto, Helvetica, Arial, sans-serif';

const linkElement = document.createElement('link');
linkElement.rel = 'stylesheet';
//linkElement.href = 'https://fonts.googleapis.com/css2?family=Google+Sans+Flex:opsz,wght@6..144,1..1000&family=Google+Sans:ital,opsz,wght@0,17..18,400..700;1,17..18,400..700&family=Roboto+Flex:opsz,wght,XOPQ,XTRA,YOPQ,YTDE,YTFI,YTLC,YTUC@8..144,100..1000,96,468,79,-203,738,514,712&family=Roboto+Mono:ital,wght@0,100..700;1,100..700&family=Roboto:ital,wght@0,100..900;1,100..900&display=swap';
//linkElement.href = 'https://fonts.googleapis.com/css2?family=Figtree:ital,wght@0,300..900;1,300..900&display=swap';
//linkElement.href = 'https://fonts.googleapis.com/css2?family=Montserrat:ital,wght@0,100..900;1,100..900&display=swap';
//linkElement.href = 'https://fonts.googleapis.com/css2?family=Plus+Jakarta+Sans:ital,wght@0,200..800;1,200..800&display=swap';
//linkElement.href = 'https://fonts.googleapis.com/css2?family=Outfit:wght@100..900&display=swap';
//linkElement.href = 'https://fonts.googleapis.com/css2?family=Inter:ital,opsz,wght@0,14..32,100..900;1,14..32,100..900&display=swap';
// Works for Roboto
//linkElement.href = `https://fonts.googleapis.com/css2?family=${favouriteFontFamily}:wght@100..900&display=swap`;
// Works for Figtree
//linkElement.href = `https://fonts.googleapis.com/css2?family=${favouriteFontFamily}:ital,wght@0,300..900;1,300..900&display=swap`;
// Works for both, same as requesting `wght@400` but DOES NOT yet support fonts with spaces in them
linkElement.href = `https://fonts.googleapis.com/css2?family=${favouriteFontFamily}&display=swap`;
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

  /* If you want to change the font weight, there is no universal defintion, but you can try this. */
  /* These are in order: prompt textbox and past prompts, Claude's response, sidebar */
  @layer utilities {
    [data-theme="claude"][data-mode="dark"] .font-large,
    [data-theme="claude"][data-mode="dark"] .font-claude-response-body,
    [data-theme="claude"][data-mode="dark"] .font-base {
      font-weight: 400 !important;
    }
  }

  @layer utilities {
    .font-claude-response-body {
      line-height: 1.6rem !important;
    }
  }

  ${fontSize ? `
    @layer utilities {
      .font-claude-response-body {
        font-size: ${fontSize};
      }
    }
  ` : ''}
`);

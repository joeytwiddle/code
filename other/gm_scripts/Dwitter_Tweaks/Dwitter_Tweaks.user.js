// ==UserScript==
// @name         Dwitter Tweaks
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      0.1.1
// @description  Tweak Dwitter with extreme dark theme
// @author       joeytwiddle
// @license      ISC
// @match        https://beta.dwitter.net/*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
  'use strict';

  // In theory I like the dark background.
  // But in practice is causes my retinas to open, which then get overwhelmed by the bright white of the default canvas!
  // So a somewhat lighter dark theme can help to keep my eyes prepared.

  const hue = 200;
  const sat = 4;

  // 6, 11, 8
  // 9, 13, 11
  // 13, 19, 11

  GM_addStyle(`
    /*
    :root {
      --pageBackgroundColor: #0b0b0b;
      --mainBackgroundColor: #1b1b1b;
    }
    */
    .App {
      background: hsl(${hue}, ${sat}%, 6%) !important;
    }
    .App > header, .card {
      background: hsl(${hue}, ${sat}%, 11%);
    }
    /* Code textarea */
    .card .mb-3+div,
    input.form-control, input.form-control[readonly],
    /* "Theme challenge of the month" */
    .card.text-center.mb-3.px-3 > div {
      background: hsl(${hue}, ${sat}%, 8%) !important;
    }
    input.form-control, input.form-control[readonly] {
      border: 0;
    }
    .btn-primary, .btn-secondary,
	.btn-primary.disabled, .btn-primary:disabled,
	.btn-secondary.disabled, .btn-secondary:disabled {
      background-color: #123a66;
      border-color: #123a66;
    }
    .shadow-primary {
      box-shadow: none !important;
    }
  `);
})();

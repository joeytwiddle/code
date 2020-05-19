// ==UserScript==
// @name         Dwitter Tweaks
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      0.1
// @description  Tweak Dwitter with extreme dark theme
// @author       joeytwiddle
// @license      ISC
// @match        https://beta.dwitter.net/*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
  'use strict';

  GM_addStyle(`
    /*
    :root {
      --pageBackgroundColor: #0b0b0b;
      --mainBackgroundColor: #1b1b1b;
    }
    */
    .App {
      background: #0b0b0b !important;
    }
    .App > header, .card {
      background: #1b1b1b;
    }
    /* Code textarea */
    .card .mb-3+div,
    input.form-control, input.form-control[readonly],
    /* "Theme challenge of the month" */
    .card.text-center.mb-3.px-3 > div {
      background: #141414 !important;
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

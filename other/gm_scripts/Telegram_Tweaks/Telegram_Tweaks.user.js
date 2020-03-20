// ==UserScript==
// @name         Telegram Tweaks
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      0.0.1
// @description  Make Telegram Web nicer to use (layout, design)
// @author       joeytwiddle
// @match        https://web.telegram.org/*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
  'use strict';

  const useFullWidth = true;

  const alignMyselfToRight = true;

  const makeTextBubbles = true;

  if (useFullWidth) {
    GM_addStyle(`
      .tg_head_split {
        max-width: none;
      }
      .im_page_wrap {
        max-width: none;
      }
      .im_message_wrap {
        max-width: none;
      }
    `);
  }

  if (alignMyselfToRight) {
    GM_addStyle(`
      @media (min-width: 900px) {
        .im_content_message_wrap {
          max-width: 80%;
          /* This allows the divs to shrink to the width of their content */
          display: table;
        }
        .im_message_out {
          margin-left: auto;
        }
      }
    `);
  }

  if (makeTextBubbles) {
    GM_addStyle(`
      .im_content_message_wrap {
          /* background-color: #cef; */
          background-color: hsl(203, 60%, 92%);
          /* margin: 4px; */
          border-radius: 12px;
          padding: 4px;
      }
    `);
  }
})();

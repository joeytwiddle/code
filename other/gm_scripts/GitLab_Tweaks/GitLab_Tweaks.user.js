// ==UserScript==
// @name         GitLab Tweaks
// @namespace    GLT
// @version      0.0.2
// @description  Improve visual design of GitLab, to reduce visual and cognitive strain, and increase zen mindfulness
// @author       joeytwiddle
// @copyright    2018, Paul "Joey" Clark (http://neuralyte.org/~joey)
// @license      MIT
// @match        https://gitlab.com/*
// @run-at       document-start
// @grant        GM_addStyle
// ==/UserScript==

(function() {
  'use strict';

  // Some whitespace around the central content, to enhance zen state
  const padTheCentralColumn = true;

  const padTheHeader = true;

  const padTheContentBlocks = true;

  // Don't let the textarea grow too tall.  (If it does, it's likely the submit button will be pushed offscreen!)
  const limitHeightOfTextarea = true;

  const showScrollbarOnIssueSidebar = true;

  const smallScrollbars = true;


  // Notes:
  // We put `body` at the start of every selector to increase its specificity.
  // That allows us to add our CSS early, but still have it override the site's CSS.

  if (padTheCentralColumn) {
    GM_addStyle(`
      body .content-wrapper .container-fluid {
        padding: 0 40px;
      }
      body .alert-wrapper {
        padding-top: 10px;
      }
    `);
  }

  if (padTheHeader) {
    GM_addStyle(`
      body .breadcrumbs-container {
        padding-bottom: 30px;
      }
      body .detail-page-header {
        margin-bottom: 14px;
      }
    `);
  }

  // Instead of adding whitespace, we can take the existing grey lines, and turn them into whitespace by making them thicker and transparent
  if (padTheContentBlocks) {
    GM_addStyle(`
      body .content-block {
        border-bottom: 20px solid transparent;
      }
    `);
  }

  if (limitHeightOfTextarea) {
    GM_addStyle(`
      body textarea.js-gfm-input {
        max-height: 60vh !important;
        max-height: calc(100vh - 400px) !important;
      }
    `);
  }

  if (showScrollbarOnIssueSidebar) {
    GM_addStyle(`
      body .right-sidebar .issuable-sidebar {
        width: calc(100% - 2px);
      }
    `);
  }

  if (smallScrollbars) {
    // The body here is used to avoid modifying the outermost page scrollbar.
    // Remove 'body *' if you want these to apply to the page scrollbar too.
    GM_addStyle(`
      body *::-webkit-scrollbar {
        width: 8px;
        background: #eee;
      }

      body *::-webkit-scrollbar-thumb {
        background: #ccc;
        border-radius: 10px;
      }

      /*
      body *::-webkit-scrollbar-track {
        box-shadow: inset 0 0 3px white;
        border-radius: 10px;
      }
      */
    `);
  }

  /*
  // This is good for increasing specificity, but it gets added too late, causing a relayout.
  document.addEventListener('DOMContentLoaded', () => {
    document.body.classList.add('vistweaks');
  });
  */
})();

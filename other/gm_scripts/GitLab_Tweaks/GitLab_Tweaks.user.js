// ==UserScript==
// @name         GitLab Tweaks
// @namespace    GLT
// @version      0.0.1
// @description  Improve visual design of GitLab, to reduce visual and cognitive strain
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

  // Don't let the textarea grow too tall.  (If it does, it's likely the submit button will be pushed offscreen!)
  const limitHeightOfTextarea = true;

  const showScrollbarOnIssueSidebar = true;

  const smallScrollbars = true;


  if (padTheCentralColumn) {
    GM_addStyle(`
      .vistweaks .content-wrapper .container-fluid {
        padding: 0 40px;
      }
      .vistweaks .alert-wrapper {
        padding-top: 10px;
      }
    `);
  }

  if (limitHeightOfTextarea) {
    GM_addStyle(`
      .vistweaks textarea.js-gfm-input {
        max-height: 60vh !important;
        max-height: calc(100vh - 400px) !important;
      }
    `);
  }

  if (showScrollbarOnIssueSidebar) {
    GM_addStyle(`
      .vistweaks .right-sidebar .issuable-sidebar {
        width: calc(100% - 2px);
      }
    `);
  }

  if (smallScrollbars) {
    // Remove 'body *' if you want these to apply to the outermost page scrollbar too
    GM_addStyle(`
      body.vistweaks *::-webkit-scrollbar {
        width: 8px;
        background: #eee;
      }

      body.vistweaks *::-webkit-scrollbar-thumb {
        background: #ccc;
        border-radius: 10px;
      }

      /*
      body.vistweaks *::-webkit-scrollbar-track {
        box-shadow: inset 0 0 3px white;
        border-radius: 10px;
      }
      */
    `);
  }

  // DOMNodeInserted might trigger sooner than DOMContentLoaded (as soon as the <body> appears, without waiting for the rest of the document)
  const addVistweaks = () => {
    document.body.classList.add('vistweaks');
    document.removeEventListener('DOMNodeInserted', addVistweaks);
  };
  document.addEventListener('DOMNodeInserted', addVistweaks);
})();

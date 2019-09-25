// ==UserScript==
// @name         Google Lucky Bang
// @namespace    https://greasyfork.org/en/users/8615-joeytwiddle
// @version      1.0.0
// @description  Add a ! to the end of your google search to perform a lucky search
// @author       joeytwiddle
// @license      ISC
// This is supposed to be all google search sites.  Copied from faviconizegoogle.user.js
// TODO This currently acts on pages which do not support lucky search (e.g. Google Images).  Ideally we would activate only on pages where we can actually help!
// @include      /https?:\/\/((www\.)?|encrypted\.)google\.[a-z]{2,3}(\.[a-z]{2})?\/(search|webhp|\?gws_rd|\?gfe_rd)?.*/
// @grant        none
// ==/UserScript==

(function() {
  'use strict';

  const searchParams = {};
  document.location.search.slice(1).split('&').forEach(pair => {
    const [key, value] = pair.split('=');
    searchParams[decodeURIComponent(key)] = decodeURIComponent(value);
  });

  if (searchParams.q && searchParams.q.slice(-1) === '!') {
    const newQuery = searchParams.q.slice(0, -1);

    console.log("Doing lucky search for", newQuery);

    // Example lucky URL: https://www.google.com/search?sxsrf=ACYBGNS3YpGqRlxp6MfsZ-8oYSGTyllGOA%3A1569379745064&source=hp&ei=odWKXeO0Af_B3LUPoNaDuAQ&q=test%21&btnI=I%27m+Feeling+Lucky&oq=test%21&gs_l=psy-ab.3..0i131j0l9.9511.10195..14336...0.0..0.69.340.6......0....1..gws-wiz.....10..35i362i39j35i39j0i67j0i131i67.HtcsjjxXywM

    //const newUrl = `${document.location.origin}/search?q=${encodeURIComponent(newQuery)}&btnI=I%27m+Feeling+Lucky`;
    const { origin, pathname, search } = document.location;
    const newUrl = `${origin}${pathname}${search.replace(/\bq=[^&]*/, `q=${encodeURIComponent(newQuery)}`)}&btnI=I%27m+Feeling+Lucky`;

    //document.location.replace(newUrl);
    document.location.assign(newUrl);
  }
})();

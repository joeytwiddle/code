// ==UserScript==
// @name         Engineers.SG Tweaks
// @namespace    https://github.com/joeytwiddle/code/blob/master/other/gm_scripts/engineers.sg_tweaks/engineers.sg_tweaks.user.js
// @version      1.0
// @description  Small visual improvements to the engineers.sg website
// @author       joeytwiddle
// @match        https://engineers.sg/events
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';

    const removeLeadingZeros = true;
    const displayEventsAsCards = true;
    const compressEventBoxes = true;
    // Useful if one of the groups annoys you, so you want to hide it
    // (Can also be used if you are looking for groups to add to accept/rejectlists, and want to hide the ones you have already handled)
    const clickToHideGroup = false;

    if (removeLeadingZeros) {
        // Remove leading '0' from times.  E.g. '06:00pm' -> '6:00pm'
        Array.from(document.querySelectorAll('.event-item > div > p > strong')).forEach(elem => {
            elem.textContent = elem.textContent.replace(/0(\d:\d\d [AP]M)/i, '$1');
        });
    }

    if (displayEventsAsCards) {
        // More like Material Design
        GM_addStyle(`

            /* Make some space for the shadows we add below */
            .collection.events-list {
                padding-left: 2em;
                padding-right: 2em;
                margin-left: -2em;
                margin-right: -2em;
            }

            .events-list .event-item {
                /* border: 1px solid #0001; */
                margin: 1.2em 0;
                box-shadow: 0 0px 6px #0001;
            }

            /* Don't make the headers wider than the cards (it looks weird) */
            .events-list .date-marker {
                margin: 0;
            }

        `);
    }

    if (compressEventBoxes) {
        // Put some data on the right of the card, so it can become shorter
        // As you can see there is a lot of CSS here.  This is a bit fragile.
        GM_addStyle(`
            .events-list .event-item > div {
                position: relative;
            }

            /* Put the date and time in the top-right */
            .events-list .event-item > div > p:nth-child(3) {
                position: absolute;
                top: 0;
                right: 1em;
                margin: 0;
            }

            /* Put the event location below that */
            /*
            .events-list .event-item > div > p:nth-child(4) {
                position: absolute;
                top: 2.5em;
                right: 1em;
                margin: 0;
            }
            */

            /* THe header must leave space for the date and time */
            .events-list .event-item h5 {
                margin-right: 10em;
            }

            /* Put the join button in the bottom-right */
            .events-list .event-item > div > a {
                position: absolute;
                bottom: 0.5em;
                right: 1em;
            }

            .events-list .event-item {
                min-height: 8.7em;
            }
        `);
    }

    if (clickToHideGroup) {
        const groupNameElements = document.querySelectorAll('.event-item blockquote');
        for (const groupNameElem of groupNameElements) {
            groupNameElem.style.cursor = 'not-allowed';
            groupNameElem.addEventListener('click', function() {
                const groupName = this.textContent;
                const allOccurrences = Array.from(document.querySelectorAll('.event-item blockquote')).filter(elem => elem.textContent === groupName);
                for (const occurrence of allOccurrences) {
                    const container = occurrence.parentNode.parentNode;
                    container.parentNode.removeChild(container);
                }
            });
        }
    }
})();

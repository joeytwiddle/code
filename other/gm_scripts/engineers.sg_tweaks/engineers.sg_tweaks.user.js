// ==UserScript==
// @name         Engineers.SG Tweaks
// @namespace    https://github.com/joeytwiddle/code/blob/master/other/gm_scripts/engineers.sg_tweaks/engineers.sg_tweaks.user.js
// @version      1.1.0
// @description  Small visual improvements to the engineers.sg website
// @author       joeytwiddle
// @match        https://engineers.sg/*
// @grant        GM_addStyle
// ==/UserScript==

(function() {
    'use strict';

    // Options for events page
    const removeLeadingZeros = true;
    const displayEventsAsCards = false;
    const compressEventBoxes = true;
    // Useful if one of the groups annoys you, so you want to hide it
    // (Can also be used if you are looking for groups to add to accept/rejectlists, and want to hide the ones you have already handled)
    const clickToHideGroup = true;

    // Options for organisations page
    const increaseSpacingBetweenOrganisations = false;
    const addBordersAroundOrganisations = true;

    const onEventsPage = document.location.pathname.startsWith('/events');
    const onOrganisationsPage = document.location.pathname.startsWith('/organizations');

    if (onEventsPage && removeLeadingZeros) {
        // Remove leading '0' from times.  E.g. '06:00pm' -> '6:00pm'
        Array.from(document.querySelectorAll('.event-item > div > p > strong')).forEach(elem => {
            elem.textContent = elem.textContent.replace(/0(\d:\d\d [AP]M)/i, '$1');
        });
    }

    if (onEventsPage && displayEventsAsCards) {
        // More like Material Design
        GM_addStyle(`

            /* Make some space for the shadows we add below */
            .collection.events-list {
                padding-left: 2em;
                padding-right: 2em;
                margin-left: -2em;
                margin-right: -2em;
            }

            / Add a shadow behind each event */
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

    if (onEventsPage && compressEventBoxes) {
        // Move some data to the right of the card, so it can become shorter.
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

    if (onEventsPage && clickToHideGroup) {
        // If the user clicks on the group's name, we will hide all events from that group
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

    if (onOrganisationsPage && increaseSpacingBetweenOrganisations) {
        GM_addStyle(`
            .container > .row > .col.s6 {
                margin-top: 25px;
                margin-bottom: 25px;
            }
        `);
    }

    if (onOrganisationsPage && addBordersAroundOrganisations) {
        GM_addStyle(`
            .container > .row > .col.s6 {
                //box-shadow: 0 0px 6px #0001;
            }
            .container > .row,
            .container > .row > .col.s6 {
                border: 1px solid #eee;
            }
            .container > .row > .col.s6 {
                padding: 12px;
            }
            h5.truncate {
                text-align: center;
            }
            .thumbnail {
                margin: auto;
            }
        `);
    }
})();

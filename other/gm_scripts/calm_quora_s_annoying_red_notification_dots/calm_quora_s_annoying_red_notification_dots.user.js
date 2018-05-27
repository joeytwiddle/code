// ==UserScript==
// @name         Calm Quora's annoying red notification dots
// @namespace    joeytwiddle
// @version      1.0.7
// @license      MIT
// @description  The red notifications on Quora are too glaring, appear too frequently, and do not go away easily enough.  Let's make them grey so they aren't such a bother.  Also the popups and the adverts can take a hike.
// @author       joeytwiddle
// @match        https://www.quora.com/*
// @grant        GM_addStyle
// @run-at       document-start
// ==/UserScript==

var makeRedNotificationsGrey = true;
var makeTheHeaderGray = false;
var showPopups = false;

var deemphasiseAds = true;

var openLinksInThisWindow = true;

// I find the answers on Quora sort of blur into each other
// To address that, this increases the spacing between them
var increaseSeparationOfAnswers1 = true;

// And this indents each answer, but leaves the user's profile picture sticking out
var increaseSeparationOfAnswers2 = true;

function afterPageLoad (callback) {
    setTimeout(callback, 4000);
}

if (makeRedNotificationsGrey) {
    // Make the red notification dots grey instead
    GM_addStyle('.SiteHeaderBadge, .WriteNavbadge, .red_badge { background: #ddd !important; background-color: #ddd !important; color: #444 !important; transform: scale(0.8); opacity: 0.5; }');
}

if (!showPopups) {
    GM_addStyle('.Growl { display: none !important; }');
}

// Just make the whole damn header grey!
// As requested by Keeni: https://greasyfork.org/en/forum/discussion/37380/x
if (makeTheHeaderGray) {
    GM_addStyle('.SiteHeader, .questions_to_answer_icon { filter: saturate(0%); }');
}

if (deemphasiseAds) {
    afterPageLoad(function () {
        // Quora's advertisements appear visually a lot like the answers, so it is easy to read an advert and get confused because it's talking about a completely tangential topic.
        // For some reason, I decided not to remove the adverts completely, but to de-emphasise them and differentiate them.
        // The padding is needed so that the text doesn't appear to close to the edge of the red box.
        // A hint of Quora red:
        //var cssForAdverts = { backgroundColor: 'hsl(3, 25%, 90%)', opacity: 0.3, padding: '1em' };
        // Just a light grey
        var cssForAdverts = { backgroundColor: '#f2f2f2', opacity: 0.5, padding: '1em', border: '1px solid #bbb' };
        // No background, just de-emphasise.  The padding is not strictly necessary but it can help to differentiate.
        //var cssForAdverts = { opacity: 0.3, padding: '1em' };
        //var cssForAdverts = { opacity: 0.1, padding: '0em' };
        //jQuery('.advertiser_endpoint').closest('.outer_content_box').css(cssForAdverts)
        // jQuery isn't always loaded at this point of time, so let's use DOM instead.
        Array.from(document.querySelectorAll('.advertiser_endpoint')).map(ad => ad.closest('.outer_content_box')).forEach(elem => Object.assign(elem.style, cssForAdverts));
        // On the "Home" feed page
        Array.from(document.querySelectorAll('.AdStory')).forEach(elem => Object.assign(elem.style, cssForAdverts));
    });
}

// Beautify avatars
GM_addStyle('.feed_item_answer_user .user { font-weight: 600; }');
GM_addStyle('.NameCredential, .IdentityCredential, .UserCredential { font-style: italic; font-size: 85%; }');

if (increaseSeparationOfAnswers1) {
    // Instead of answers cramped together with horizontal rules separating them, put some nice big whitespace between each answer
    //GM_addStyle('.NewGridQuestionPage .AnswerBase { border-top: none; padding: 50px 0px 50px; }');

    // Or keep the lines close to the top of each answer
    GM_addStyle('.NewGridQuestionPage .AnswerBase { margin: 40px 0px; }');
    // This gives the same spacing to the adverts
    GM_addStyle('.content_area { margin: 40px 0px; }');
}

if (increaseSeparationOfAnswers2) {
    GM_addStyle('.Answer { padding-left: 45px; }');
    GM_addStyle('.ContentHeader { margin-left: -47px; }');
}

if (openLinksInThisWindow) {
    afterPageLoad(function () {
        document.body.addEventListener('click', function (evt) {
            var link = evt.target.closest('a');
            if (link && link.getAttribute('target') === '_blank') {
                link.setAttribute('target', '');
            }
        });
    });
}

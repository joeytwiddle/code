// ==UserScript==
// @name           URLs Need Titles
// @namespace      UNT
// @description    When you paste a URL to a friend, it is useful if it contains the title of the page.  This script adds these missing titles for common websites using # part of URL.  In other words, it turns non-semantic URLs into semantic URLs!
// @version        1.2.2
// @include        http://*/*
// @include        https://github.com/*/*
// ==/UserScript==

var overwriteExistingHash = true;

var rules = [

    /* An example rule:

    {
        hostMatch: "youtube.TLD",   All subdomains will be accepted, e.g.
                                    somewhere.youtube.com.  TLD will match one
                                    or two toplevel domain elements, e.g.
                                    "youtube.ath.cx" or "youtube.net".

        pathMatch: "/watch",        This regex will be wrapped with ^..$ so use
                                    .* at either end for wildcards.  Note that
                                    path does not include the search part of
                                    the URL, "?q=squash&type=image".  Filtering
                                    on this part of the URL may be added later.
                                    If left blank all paths will be accepted.

        getTitle: function(){...}   A function which returns a sensible title
                                    for this page.  If left blank will just
                                    grab document.title.
    }

    */

    {
        hostMatch: "youtube.TLD",
        pathMatch: "/watch",
        getTitle: function(){
            return document.title.replace(/^YouTube - /,'');
        }
    },

    {
        hostMatch: "xkcd.TLD",
        pathMatch: ".*[0-9]+/",
        getTitle: function(){ return (""+document.title).replace(/^[^:]*: /,'','g'); }
    },

    {
        hostMatch: "imdb.TLD",
        pathMatch: ".*title.*",
        getTitle: function(){ return (""+document.title).replace(/ - IMDb/,'','g'); }
    },

    {
        hostMatch: "pouet.net",
        pathMatch: ".*"
    },

    {
        hostMatch: "userscripts.org",
        pathMatch: "/scripts/show/.*",
        getTitle: function(){ return document.title.replace(" for Greasemonkey",''); }
    },

    {
        hostMatch: "bbc.co.uk",
        pathMatch: "/news/.*",
        getTitle: function(){ return document.title.replace(/BBC News - /,''); }
    },

    {
        hostMatch: "imgur.com",
        pathMatch: "/gallery/.*",
        getTitle: function(){ return document.title.replace(/ - Imgur/,''); }
    },

    {
        hostMatch: "9gag.com",
        pathMatch: "/gag/.*",
        getTitle: function(){ return document.title.replace(/9GAG - /,''); }
    },

    {
        hostMatch: "unrealadmin.org",
        getTitle: function(){ return document.title.replace(/ - The Unreal Admins Page .*/,''); }
    },

    {
        hostMatch: "vim.org",
        getTitle: function(){
            // When viewing user profile page
            if (document.location.pathname.indexOf("/profile.php") >= 0) {
                var username = document.evaluate("//td[string(.)='user name']/following-sibling::*",document,null,6,null).snapshotItem(0).textContent; // maybe I should start using jquery
                return username+"'s profile";
            }
            // Otherwise (good when viewing a script page)
            return document.title.replace(/ - .*/,'');
        }
    },

    {
        hostMatch: "github.com",
        pathMatch: "/[^/]*/[^/]*",
        getTitle: function(){
            // This is a naughty hack.  It doesn't return a title to put in the URL.
            // Instead it grabs the repository description from the page, and puts that in the title*.
            // This is useful when bookmarking a git repository, because the name alone is often not very descriptive.
            var repoDescriptionElems = document.getElementsByClassName("repository-description");
            if (repoDescriptionElems) {
                var repoDescription = repoDescriptionElems[0].textContent.trim();
                // For a while their titles were "<author_name>/<repo_name> <weird-dot> GitHub" but right now they just have the repo path.
                document.title = document.title.replace(/(\s+[^ ]*\s+GitHub|)$/, " - "+repoDescription+"$1");
            }
            return null;
        }
    }


];

function check() {
    if (document.location.hash && !overwriteExistingHash) {
        return;
    }
    rules.forEach(checkRule);
}

function checkRule(rule) {
    var hostRegexp = '(^|\\.)' + rule.hostMatch.replace(/\.TLD$/, "(\\.[^.]*$|\\.[^.]*\\.[^.]*$)") + '$';
    if (document.location.host.match(hostRegexp)) {
        if (rule.pathMatch) {
            var pathRegexp = '^' + rule.pathMatch + '$';
            if (!document.location.pathname.match(pathRegexp)) {
                return false;
            }
        }
        var newTitle = ( rule.getTitle ? rule.getTitle() : document.title );
        if (newTitle == '' || newTitle == null) {
            console.log("Failed to get new title for "+document.location+" from "+document.title);
        }
        setTitle(newTitle);
    }
}

function setTitle(title) {
    if (title) {
        // "_"s paste better into IRC, since " "s become "%20"s which are hard to read.  The second and third parts trim "_"s and newlines from the start and end of the string.
        title = title.replace(/ /g,'_').replace(/^[\r\n_]*/,'').replace(/[\r\n_]*$/,'');
        var strippedHref = document.location.href.replace(/#.*/,'');
        document.location.replace(strippedHref + '#' + title); // Does not alter browser history
        // document.location.hash = title; // Crashes Chrome less often
    }
}


// 2010/11: Waiting a bit can prevent crashing (e.g. YouTube in Chrome).
setTimeout(check,5000);

// vim: ts=4 sw=4 expandtab

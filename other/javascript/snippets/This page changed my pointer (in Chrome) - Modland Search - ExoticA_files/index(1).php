/* css placed here will affect all users (no matter what skin) */

/* disable displaying of "Main Page" heading */

body.page-Main_Page h1.firstHeading { display: none; }

/* homepage stuff */

.homepage_box_left {
  padding: 4px;
  margin: 0 4px 4px 0;
}

.homepage_box_right {
  padding: 4px;
  margin: 0 0 4px 4px;
}

.homepage_box_left h2 {
  border: 0;
  margin: 0;
  color: black;
  padding: 4px;
  clear: both;
}

.homepage_box_left .right,.homepage_box_right .right {
  text-align: right;
  clear: both;
}

.homepage_box_right h2 {
  border: 0;
  margin: 0;
  color: black;
  padding: 4px;
  clear: both;
}

#quick_links {
  float: right;
  width: 50%;
}

#quick_links ul {
  margin-left: 2em;
}

div#latest_news {
  border: 1px #c0c0c0 solid;
}

div#latest_news h2 {
  background-color: #c0c0c0;
}

div#latest_news h3 {
  font-size: 120%;
  margin: 0 0 0 0;
  border-bottom: 1px #c0c0c0 solid;
}

div#latest_news img {
  margin: .5em .5em 0 0;
}

div#latest_news h4 {
  font-size: 110%;
  margin-bottom: 0;
}

div#last_pages {
  border: 1px #c0c0c0 solid;
}

div#last_pages h2 {
  background-color: #c0c0c0;
}

div#daily_dumps {
  border: 1px #c0c0c0 solid;
}

div#daily_dumps h2 {
  background-color: #c0c0c0;
}

div#music {
  border: 1px #ffa2e8 solid;
}

div#music h2 {
  background-color: #ffa2e8;
}

div#galleries {
  border: 1px #a2abff solid;
}

div#galleries h2 {
  background-color: #a2abff;
}

div#demoscene {
  border: 1px #a2ffb6 solid;
}

div#demoscene h2 {
  background-color: #a2ffb6;
}

div#media {
  border: 1px #d0ffa2 solid;
}

div#media h2 {
  background-color: #d0ffa2;
}

div#other_areas {
  border: 1px #ffe6a2 solid;
}

div#other_areas h2 {
  background-color: #ffe6a2;
}


/* Infobox template style */

.infobox {
   border: 1px solid #aaaaaa;
   background-color: #f9f9f9;
   color: black;
   margin-bottom: 0.5em;
   margin-left: 1em;
   padding: 0.2em;
   float: right;
   clear: right;
   text-align: left;
   font-size: 90%;
}

.infobox .image {
  margin: 0;
  padding: 0;
}
.infobox td,
.infobox th {
  vertical-align: top;
  padding: 3px;
}
.infobox th {
  background-color: #ccccff;
}
.infobox caption {
   font-size: larger;
   margin-left: inherit;
}

.infobox .large {
   font-size: 110%;
}

.infobox.bordered {
   border-collapse: collapse;
}
.infobox.bordered td,
.infobox.bordered th {
  border: 1px solid #aaaaaa;
}
.infobox.bordered .borderless td,
.infobox.bordered .borderless th {
  border: 0;
}

.infobox .left {
  text-align: left;
}
.infobox .right {
  text-align: right;
}

.infobox .center {
  text-align: center;
}

/* information table style */

.infotable {
  font-size: 100%;
  border-collapse: collapse;
  border: 1px black solid;
  padding: .5em;
}

.infotable caption {
  border: 2px #aaa solid;
  border-bottom: 0;
  margin-left: 16px;
  margin-right: 16px;
  padding: 2px;
  font-size: 110%;
  font-weight: bold;
}

.infotable th {
  background-color: #ccccff;
}

.infotable th, .infotable td {
  padding: 4px;
}

.infotable table, .infotable th, .infotable td {
  border: 1px solid #aaa;
}

.infotable .left {
  text-align: left;
}

.infotable .right {
  text-align: right;
}

.infotable .l {
  text-align: left;
}

.infotable .r {
  text-align: right;
}

.infotable .center {
  text-align: center;
}

/* file table (for tables with tree display) */

.filetable {
  border-collapse: collapse;
  border: 1px black solid;
  line-height: 1em;
}

.filetable image {
  width: 1.4em;
  height: 1.4em;
}

.filetable th {
  border-bottom: 1px black solid;
  padding: .5em;
  background-color: #ccf;
}

.filetable .o {
  background-color: #fff;
}

.filetable .e {
  background-color: #eef;
}

.filetable .dir {
  font-weight: bold;
  height: 1.4em;
}

.filetable .r {
  text-align: right;
}

.filetable td {
  border-right: 1px black solid;
  padding: 0 .5em;
  white-space: nowrap;
}


/* gallery styles */
.captiongallery {
  margin: auto;
}

.captiongallery td {
  vertical-align: top;
}

/* message boxes */

.messagebox {
    border: 1px solid #aaa;
    background-color: #f9f9f9;
    width: 80%;
    margin: 0 auto 1em auto;
    padding: .2em;
    text-align: center;
}

.messagebox img {
  vertical-align: middle;
}

.messagebox.protected {
  background-color: #f9c9c9;
  border: 1px solid #800;
}

.messagebox.stub {
  background-color: #c9c9f9;
  border: 1px solid #008;
}

.messagebox.historical {
  background-color: #f9f9c9;
  border: 1px solid #880;
}

.messagebox.copyright {
  background-color: #f9c9c9;
  border: 1px solid #800;
}

.messagebox.copyright img {
  float: left;
  display: block;
}

.messagebox.warn {
  background-color: #f9f9c9;
  border: 1px solid #880;
}

/* taming definition lists */

/* inline lists */

div.list_inline dt {
  float: left;
  width: 5em;
}

div.list_inline dd {
  margin-left: 5em;
}

div.list_inline dl dl dt {
  float: left;
  width: 5em;
}

div.list_inline dl dl dd {
  margin-left: 5em;
}

/* gallery style */

table.gallery {
  margin: auto;
}

div.gallerytext {
  text-align: center;
}

/* "quote" styles */

div.quote {
  position: relative;
  border: 1px #cccccc solid;
  background-color: #eeeeee;
  padding: 1em;
  margin-top: 0.5em; /* aligns with floated thumbs */
  _padding-bottom: 1em;
  margin-bottom: 0.5em;
}

div.quote span.quote_left, div.quote span.quote_right {
  font-family: "Times New Roman", serif;
  font-size: 300%;
  font-weight: bold;
  line-height: .75em;
  position: absolute;
}

div.quote span.quote_left {
  left: .25em;
  top: .25em;
}

div.quote span.quote_right {
  right: .25em;
  bottom: .5em;
  /* ie6 */
  _bottom: .75em;
}

div.quote div {
  margin-left: 2em;
  margin-right: 2em;
}

div.quote span.quote_author {
 float: right;
 margin-right: 2em;
 margin-top: 1em;
 font-size: 90%;
}

/* lost in translation gallery */

.lit_gallery {
  text-align: center;
  font-size: 90%;
}

/* Allow limiting of which header levels are shown in a TOC; <div class="toclimit-3">, for
   instance, will limit to showing ==headings== and ===headings=== but no further (as long as
   there are no =headings= on the page, which there shouldn't be according to the MoS). */
.toclimit-2 .toclevel-2 {display:none;}
.toclimit-3 .toclevel-3 {display:none;}
.toclimit-4 .toclevel-4 {display:none;}
.toclimit-5 .toclevel-5 {display:none;}
.toclimit-6 .toclevel-6 {display:none;}
.toclimit-7 .toclevel-7 {display:none;}
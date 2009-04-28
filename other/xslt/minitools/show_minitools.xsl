<?xml version="1.0"?> 
<?xml-stylesheet type="text/xsl" href="show_minitools.xsl"?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:yaslt="http://www.mod-xslt2.com/ns/1.0"
	xmlns:exslt="http://exslt.org/common"
	>
	<!--
	extension-element-prefixes="exsl"
	-->

	<!--
	<xsl:import href="../lib/exslt.org/exsl/exsl.xsl"/>
	-->
	<!--
	<xsl:import href="../xhtmlCommon.xsl" />
	-->

	<!-- TODO: The Javascript should be in a separate file and included. -->

	<!-- Load relevant CGI variables. -->
	<xsl:variable name="mode">
		<xsl:choose>
			<xsl:when test="element-available('yaslt:value-of')">
				<yaslt:value-of select="$GET[mode]" />
			</xsl:when>
			<xsl:otherwise></xsl:otherwise>
		</xsl:choose>
	</xsl:variable>


	<xsl:output method="xml"/>

	<xsl:include href="../xmlverbatim.xsl"/>

	<xsl:template match="/">
		<!-- srcUrl is preset as 'http://hwi.ath.cx/xslt/src/minitools/show_minitools.html -->
		<xsl:variable name="xsltURL" value="'http://hwi.ath.cx/xslt/src/minitools/show_minitools.xsl'"/>
		<xsl:variable name="pageUrl" select="//URL/@url"/>

		<!-- <P>Document 1 has <xsl:value-of select="count(//*)"/> nodes.</P> -->
		<xsl:variable name="tidyPageUrl" select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',$pageUrl)"/>
		<xsl:variable name="thePage" select="document($tidyPageUrl)"/>
		<!-- <P>Document 2 has <xsl:value-of select="count($thePage//*)"/> nodes.</P> -->

		<HTML>
			<HEAD>
				<TITLE>Joey's Minitools/Bookmarklets</TITLE>
				<style type="text/css">
					a:link {
						text-decoration: underline;
						color: #0000bb;
					}
					a:visited {
						text-decoration: underline;
						color: #bb00bb;
					}
					a:hover {
						text-decoration: underline;
						color: #0000dd;
					}
				</style>
				<!-- Keep viewer safe from infinte alert trap! -->
				<SCRIPT type='text/javascript' src='http://hwi.ath.cx/javascript/alert_watcher.user.js'></SCRIPT>
			</HEAD>
			<BODY bgcolor="white">

				<H1>Joey's Minitools/Bookmarklets</H1>

				<P>
					My Konqueror Bookmarklets from <A><xsl:attribute name="href"><xsl:value-of select="//URL/@url"/></xsl:attribute>
						<!-- <xsl:value-of select="substring-after-last(//URL/@url,'/')"/></A> -->
						minitools.xml</A>
					<!-- via <A><xsl:attribute name="href"><xsl:value-of select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',$pageUrl)"/></xsl:attribute>Tidied</A> -->
					<!-- via <A><xsl:attribute name="href"><xsl:value-of select="'http://hwi.ath.cx/cgi-bin/joey/tidy?url='"/><xsl:value-of select="$pageUrl"/></xsl:attribute>this tidied copy</A> -->
					arranged into HTML by
					<!-- TODO: very odd - $xsltURL kept coming out as $srcUrl O_o. -->
					<!-- by <A><xsl:attribute name="href" select="$xsltURL"/>this XSLT</A>. -->
					<!-- <A><xsl:attribute name="href" select="'http://hwi.ath.cx/xslt/src/minitools/show_minitools.xsl'"/>this XSLT</A>. -->
					<A href="http://hwi.ath.cx/xslt/src/minitools/show_minitools.xsl">this XSLT</A>
					.
				</P>

				<P>Bookmarklets are a bit like Bookmarks, but instead of taking you to a web page, they perform useful actions on the page you are viewing.</P>
				<P>Click on a bookmarklet below to try it out. (Reload the page or navigate back if you don't like the results!)</P>
				<P>To copy a bookmarklet so you can use it later on any web page, drag the link to your Bookmarks Toolbar, or right-click and bookmark it.</P>

				<TABLE cellpadding='20'><TR>
				<TD id='minitoolsBlock' width="60%" bgcolor="#eeffdd">
					<H2>Minitools</H2>
					<!-- The reasons for the xsl:sort's is that we need to reverse the order.  I store my minitools in reverse order because Konqueror displays them in reverse order! -->
					<!-- Show the toplevel (no subfolder) bookmarklets: -->
					<!-- <xsl:apply-templates select="($thePage)"/> -->
					<xsl:apply-templates select="$thePage/xbel"/>
					<!--
					<xsl:for-each select="$thePage/*/folder">
						<xsl:sort select="position()" data-type="number" order="descending"/>
						<xsl:apply-templates select="."/>
					</xsl:for-each>
					-->
				</TD>

				<TD width="1%" valign='top' bgcolor='#ddeeff'>

					<P align='right'><nobr>[ <A id='scriptStuffToggle' href='javascript:(function()%7B
						var s = document.getElementById("scriptStuff");
						s.style.display = ( s.style.display=="" ? "none" : "");
						document.getElementById("scriptStuffToggle").innerHTML = ( s.style.display=="" ? "Hide" : "Show" );
						%7D)();'>Show Code</A> ]</nobr></P>

					<DIV id='scriptStuff' style='display:none'>

						<H3>Bookmarklet Viewer / Editor</H3>

						<INPUT id='scriptLabel1' type="text" name="text" value="" size='50'/>
						<font size='-1'><P id='scriptDescription1' style='background-color:#eeeebb;padding:10px;border:solid;border-width:1;border-color:black;'>Description</P></font>
						<TEXTAREA id='scriptTextArea1' cols='50' rows='20'></TEXTAREA>
						<P align="right"><FONT size='+2'><A id="scriptTestLink1" target="_self" href="javascript:">Test It!</A></FONT></P>

						<BR/>
						<BR/>
						<BR/>

						<P>Advanced <A target="_self" href="javascript:(function()%7Bdocument.getElementById('scriptStuff').innerHTML=''; document.getElementById('bmlbldriframe').innerHTML = '&lt;IFRAME width=&quot;100%&quot; height=&quot;600&quot; src=&quot;http://subsimple.com/bookmarklets/jsbuilder.htm&quot;&gt;&lt;/IFRAME&gt;';%7D)();">Bookmarklet Builder</A> - can format the code for easy reading.</P>

						<!-- <P>Advanced <A target="_blank" href="http://subsimple.com/bookmarklets/jsbuilder.htm">Bookmarklet Builder</A> - can format the code for easy reading.</P> -->
						<!-- <IFRAME width="100%" height="300" src="http://subsimple.com/bookmarklets/jsbuilder.htm"></IFRAME> -->
						<DIV id='bmlbldriframe'/>
						<P align='right'><A target="_blank" href="http://subsimple.com/bookmarklets/jsbuilder.htm">Popup</A></P>

					</DIV>

					<!--
						DOH this is running when ANY element under the TD is clicked.
					<SCRIPT type='text/javascript'>
						function toggleScriptStuff(evt) {
							var s = document.getElementById("scriptStuff");
							s.style.display = ( s.style.display=="" ? "none" : "");
							s.parentNode.width = ( s.style.display=="" ? "8px" : "50%");
						}
						document.getElementById('scriptStuff').parentNode.onclick = toggleScriptStuff;
					</SCRIPT>
					-->

				</TD>

				</TR></TABLE>

				<!-- Not working well!
				<script language="JavaScript" type="text/javascript" src="http://hwi.ath.cx/javascript/modifyDOM.js"></script>
				<script language="JavaScript" type="text/javascript"> addFoldsToBlockQuotes(); </script>
				-->

				<SCRIPT type="text/javascript">
					document.write("Javascript reports "+document.links.length+" links on this page so far.&lt;BR&gt;");
					for (i=0;i&lt;document.links;i++) {
					}
				</SCRIPT>

				<P>NOTE: Not all of these Bookmarklets work in Firefox.  They are my Konqueror Bookmarklets.</P>

				<H3>Importing</H3>
				<BLOCKQUOTE>
					There is an easy way to import all these bookmarklets into Firefox in one shot, and it goes like this:
					<BLOCKQUOTE>
						<!-- TODO: This still leaves some unwated links around.  Really we should make the left row of the table the only thing in the document. -->
						<LI>0) <A href="javascript: (function()%7B
								for (i=document.links.length-1;i&gt;=0;i--) %7B
									/*if (document.links[i].href.substring(0,11)!='javascript:') %7B
										document.links[i].parentNode.removeChild(document.links[i]);
									%7D*/
									var link = document.links[i];
									var target = document.getElementById('minitoolsBlock');
									var parent = link.parentNode;
									while (parent &amp;&amp; parent!=target) %7B
										parent = parent.parentNode;
									%7D
									if (parent != target &amp;&amp; link.id!='scriptTestLink1') %7B
										link.parentNode.removeChild(link);
									%7D
								%7D
							%7D)();">Click Here</A> to remove the non-bookmarklet links from the page.</LI>
						<LI>1) Save this page as a temporary file on your system, e.g. <TT>to_import.html</TT>.</LI>
						<LI>2) In Firefox, go to Bookmarks -&gt; Organise Bookmarks, then select Import, and import the file.</LI>
						<LI>3) Look for the imported folders &quot;Minitools&quot; at the bottom of your Bookmarks Menu, and move them wherever you want.  (The import produces many depth-1 folders, not a proper tree - too bad.)  I prefer to put a folder of Bookmarklets in my Bookmarks Toolbar, so they drop down as a menu.)</LI>
						<LI><FONT size='+0'>4) You may wish to delete those few imported bookmarks which are links on this page but not bookmarklets!</FONT></LI>
					</BLOCKQUOTE>
					<P><FONT size='-1'>Implementation notes: It appears links inside a BLOCKQUOTE headed by an H3 get imported into a folder with the name of the H3, but these folders are not imported into a tree, they are all placed top-level.</FONT></P>
				</BLOCKQUOTE>

				<H3>TODO</H3>
				<BLOCKQUOTE>
					<LI>We have not yet brought the comments out of the minitools.  We may be able to present them so that Firefox will import them as descriptions.</LI>
					<LI>Add Javascript which allows viewers to View (done) or Edit the Javascript of each Bookmarklet.</LI>
					<LI>Make the Importing and See Also sections appear in the top-right of the page!</LI>
				</BLOCKQUOTE>

				<P align='right'>
					You can find other Bookmarklets at
					<A href="http://hwi.ath.cx/joeys_bookmarklets.html">My bookmarklet development page</A> and on <A href="http://hwi.ath.cx/xslt/live/jumpgate/jumpgate.xml">Jumpgate 2</A>.
				</P>

					<SCRIPT type="text/javascript">

						var scriptLabel = document.getElementById('scriptLabel1');
						var scriptTextArea = document.getElementById('scriptTextArea1');
						var scriptTestLink = document.getElementById('scriptTestLink1');

						var hasBeenEdited = false;

						function showBookmarklet(evt) {
							if (hasBeenEdited) {
								return;
							}
							var linkNode = evt.srcElement; /*moz?*/
							if (!linkNode) { linkNode = evt.currentTarget; } /*not moz?*/
							// alert('linkNode='+linkNode);
							// scriptTextArea.innerHTML = unescape(""+linkNode.href);
							scriptTextArea.value = unescape(""+linkNode.href).replace('\t','  ');
							scriptLabel.value = ""+linkNode.text;
							// scriptLabel.innerHTML = escape(""+linkNode);
							// document.getElementById('textBox').value = escape(""+linkNode);
							scriptTestLink.href = linkNode.href;
							document.getElementById('scriptDescription1').innerHTML = linkNode.title;
						}
						for (i=0;i&lt;document.links.length;i++) {
							if (document.links[i].href.substring(0,11) == 'javascript:') {
								document.links[i].onmouseover = showBookmarklet;
							}
						}
						// document.write("Added "+document.links.length+" events.<BR/>");

						function updateScript() {
							scriptTestLink.href = (scriptTextArea.value); // .replace(/^javascript%25%3A/,'javascript:');
							hasBeenEdited = (scriptTextArea.value != "");
						}
						// scriptTextArea.onchange = updateScript;
						scriptTextArea.onkeypress = updateScript;

					</SCRIPT>

			</BODY>
		</HTML>

	</xsl:template>

	<xsl:template match="folder|xbel">
		<H3><xsl:value-of select="title"/></H3>
		<BLOCKQUOTE>
			<!-- <xsl:apply-templates select=".//bookmark"/> -->
			<xsl:for-each select="./folder | ./bookmark">
				<xsl:sort select="position()" data-type="number" order="descending"/>
				<xsl:apply-templates select="."/>
			</xsl:for-each>
		</BLOCKQUOTE>
	</xsl:template>

	<xsl:template match="bookmark">
		<A>
			<xsl:attribute name="id"><xsl:value-of select="position()"/></xsl:attribute>
			<xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
			<xsl:attribute name="title"><xsl:value-of select="string(desc)"/></xsl:attribute>
			<xsl:value-of select="string(title)"/>
		</A>
		<BR/>

		<!-- Content of TEXTAREA should be converted to HTML. -->
		<!-- <xsl:value-of select="@href"/> -->
		<!-- <xsl:value-of select="translate(@href,';',';   ')"/> -->
		<!--
		<BR/>
		<TEXTAREA cols="60" rows="3">
			<xsl:value-of select="@href"/>
		</TEXTAREA>
		<BR/>
		-->
		<!-- <xsl:copy-of select="string(@href)"/> -->
		<!-- <A href="javascript:edit();">Edit</A> -->
	</xsl:template>

</xsl:stylesheet>


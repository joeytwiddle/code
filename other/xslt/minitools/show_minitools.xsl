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
				<TITLE>Minitools</TITLE>
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
			</HEAD>
			<BODY bgcolor="white">

				<H1>Minitools</H1>

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

				<TABLE cellpadding='20'><TR>
				<TD bgcolor="#ffffdd">
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

				<TD valign='top' bgcolor='#ddddff'>
					<div id='scriptLabel1'>Scriptname</div>
					<TEXTAREA id='scriptTextArea1' cols='40' rows='15'>javascript: </TEXTAREA>
					<BR/>

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
						<LI>1) Save this page as a temporary file on your system, e.g. <TT>to_import.html</TT>.</LI>
						<LI>2) In Firefox, go to Bookmarks -&gt; Organise Bookmarks, then select Import, and import the file.</LI>
						<LI>3) Look for the imported Bookmarklets at the bottom of your Bookmarks Menu, and move them wherever you want.  (I prefer to keep my Bookmarklets in my Bookmarks Toolbar.)</LI>
						<LI><FONT size='-1'>4) You may wish to delete those few imported bookmarks which are links on this page but not bookmarklets!</FONT></LI>
					</BLOCKQUOTE>
					<P><FONT size='-1'>Implementation notes: It appears links inside a BLOCKQUOTE headed by an H3 get imported into a folder with the name of the H3, but these folders are not imported into a tree, they are all placed top-level.</FONT></P>
				</BLOCKQUOTE>

				<H3>TODO</H3>
				<BLOCKQUOTE>
					<LI>Add Javascript which allows viewers to View or Edit the Javascript of each Bookmarklet.</LI>
					<LI>Make the Importing and See Also sections appear in the top-right of the page!</LI>
				</BLOCKQUOTE>

				<P align='right'>
					You can find other Bookmarklets at
					<A href="http://hwi.ath.cx/joeys_bookmarklets.html">My bookmarklet development page</A> and on <A href="http://hwi.ath.cx/xslt/live/jumpgate/jumpgate.xml">Jumpgate 2</A>.
				</P>

					<SCRIPT type="text/javascript">

						var scriptLabel = document.getElementById('scriptLabel1');
						var scriptTextArea = document.getElementById('scriptTextArea1');
						// alert('scriptLabel='+scriptLabel);
						// alert('scriptTextArea='+scriptTextArea);

						function showBookmarklet(evt) {
							var linkNode = evt.srcElement; /*moz?*/
							if (!linkNode) { linkNode = evt.currentTarget; } /*not moz?*/
							alert('linkNode='+linkNode);
							scriptLabel.innerHTML = escape(""+linkNode);
							scriptTextArea.innerHTML = escape(""+linkNode);
						}

						for (i=0;i&lt;document.links;i++) {
							document.links[i].onmouseover = showBookmarklet;
						}
						document.write("Added "+document.links.length+" events.<BR/>");

					</SCRIPT>

					<!--
					<A href="javascript:
						for (i=0;i&lt;document.links;i++) {
							document.links[i].onmouseover = showBookmarklet;
						}
						alert(&quot;Added &quot;+document.links.length+&quot; events.&lt;BR/&gt;&quot;);
					">Test</A>
					-->

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
			<xsl:value-of select="string(.)"/>
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


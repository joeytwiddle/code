<?xml version="1.0"?> 
<?xml-stylesheet type="text/xsl" href="jumpgate.xsl"?> 
<xsl:stylesheet version="1.0"
	extension-element-prefixes="yaslt"
	xmlns:yaslt="http://www.mod-xslt2.com/ns/1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!--
	TODO:
	It should be possible to create a bookmarklet for each jumpentry,
	so that the user can be browsing the web, then highlight some text, then run the bookmarklet to search on it.  =)
-->

<xsl:import href="../xhtmlCommon.xsl" />
<xsl:import href="formscraper.xsl" />

<xsl:output method="html"/>

<xsl:include href="../xmlverbatim.xsl"/>

	<xsl:template match="/">
		<xsl:apply-templates select="//JumpgateData"/>
	</xsl:template>

	<xsl:template match="JumpgateData">

		<H1>Jumpgate 2</H1>

		A portal to search engines on the Web.

		<P/>

		Wizard: <A href="formscraper.xsl?mode=addEntry">Create a new entry for the jumpgate from an existing website</A> (not finished; you must add the entry manually!)

		<P/>

		Choose a Category:
		<select name="categoryMenu">
			<xsl:for-each select="//Category/.">
				<option>
					<xsl:attribute name="name"><xsl:value-of select="."/></xsl:attribute>
					<xsl:value-of select="."/>
				</option>
			</xsl:for-each>
		</select>
		(not working)

		<P/>

		<xsl:for-each select="JumpgateEntry">
			<table width="100%" bgcolor="#000000"><tr><td>
			<table width="100%" cellspacing="5" bgcolor="#ffffff">
			<tr>
			<td>
				<font size="+1"><xsl:value-of select="@name"/></font>
			</td>
			<td align="right">
				<font size="-1"><xsl:value-of select="Description"/></font>
			</td>
			</tr>
			</table>
			<table><tr><td></td></tr></table>
			<table width="100%" cellspacing="5" bgcolor="#ffffff">
			<tr>
			<td colspan="2">
				<form>
					<xsl:attribute name="action"><xsl:value-of select="HowToAccess/Form/@action"/></xsl:attribute>
					<xsl:if test="HowToAccess/Form/@method">
						<xsl:attribute name="method"><xsl:value-of select="HowToAccess/Form/@method"/></xsl:attribute>
					</xsl:if>
					<!--
					<xsl:for-each select=".//input">
						<xsl:copy-of select="."/>
					</xsl:for-each>
					-->
					<xsl:for-each select="HowToAccess/Form/ExposedParameters/*">
						<xsl:copy-of select="."/>
					</xsl:for-each>
					<xsl:for-each select="HowToAccess/Form/HiddenParameters/*">
						<xsl:copy-of select="."/>
					</xsl:for-each>

					<!-- If the form does not have its own (possible named) submit button, then we add a default one. -->
					<xsl:if test="not(HowToAccess/Form//*[name(.)='input'][@type='submit'])">
						<DIV align="right">
							<INPUT type="submit" value="Search &gt;"/>
						</DIV>
					</xsl:if>

				</form>
			</td>
			</tr>
			<tr>
			<td align="right">
				<xsl:apply-templates select="HowToAccess"/>
			</td>
			</tr>
			<!--
			<tr>
			<td align="right">
				<font size="-1"><xsl:value-of select="Description"/></font>
			</td>
			</tr>
			-->
			</table>
			</td></tr></table>
			<P/>
		</xsl:for-each>

		Bookmarklets: Select some text on a webpage, and invoke a Bookmarklet to quickly search for that text on the Bookmarklet's search engine.
		To install a Bookmarklet for use when browsing any web page, right click on the link below and add it to your Bookmarks/Favourites/Minitools.

		<P/>

		TODO:
		<TL>
		<LI>Bookmarklets are a little premature because we have not yet defined which of the exposed parameters is the primary.</LI>
		<LI>Also parameter values need to be CGI encoded, which is why Google doesn't work.</LI>
		<LI>During development, JumpgateEntries should be stored in terms of their original settings, and manipulated at runtime (but would be nice to cache somehow).</LI>
		<LI>The Wizard should also become a JumpgateEntry editor.  Maybe the Wizard could even handle versioning...?</LI>
		</TL>
		<P/>
		DONE:
		<TL>
		<LI>Also they should popup the search in a new window rather than replace the current webpage. </LI>
		</TL>

		<P/>

	</xsl:template>

	<!-- Bookmarklet "function" -->
	<xsl:template match="HowToAccess">
		<xsl:value-of select='string("Bookmarklet: ")'/>
		<xsl:element name="A">
			<xsl:attribute name="href">
				<xsl:value-of select='string("javascript: var searchStr = document.getSelection(); ")'/>
				<xsl:value-of select='string("if (searchStr == &#39;&#39;) { searchStr = prompt(&#39;Search for what&#63;&#39;,&#39;&#39;); } ")'/>
				<xsl:value-of select='string("if (searchStr != null) { ")'/>
					<xsl:value-of select='string("var searchUrl = &#39;")'/>
					<xsl:value-of select='./Form/@action'/>
					<xsl:value-of select='string("&#63;")'/>
					<xsl:for-each select="./Form/ExposedParameters/*[1]"> <!-- There can be only one! -->
						<xsl:value-of select='@name'/>
						<xsl:value-of select='string("=&#39;+searchStr+&#39;&amp;")'/>
					</xsl:for-each>
					<xsl:for-each select="./Form/ExposedParameters/*[position()&gt;1] | ./Form/HiddenParameters/*">
						<xsl:value-of select='@name'/>
						<xsl:value-of select='string("=")'/>
						<xsl:value-of select='@value'/>
						<xsl:if test='not(boolean(position()=last()))'>
							<xsl:value-of select='string("&amp;")'/>
						</xsl:if>
					</xsl:for-each>
					<xsl:value-of select='string("&#39;; ")'/>
					<!-- <xsl:value-of select='string("document.location = searchUrl;")'/> -->
					<xsl:value-of select='string("window.open(searchUrl,&#39;")'/>
					<xsl:value-of select='@name'/>
					<xsl:value-of select='string("&#39;,&#39;location=yes,status=yes,menubar=yes,scrollbars=yes,resizable=yes,width=800,height=600&#39;);")'/>
				<xsl:value-of select='string(" }")'/>
			</xsl:attribute>
			<xsl:value-of select='../@name'/>
			<xsl:value-of select='string(" Quick Search")'/>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>

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

		<A href="formscraper.xsl?mode=addEntry">Add a jumpgate entry</A>

		<select name="categoryMenu">
			<xsl:for-each select="//Category/.">
				<option>
					<xsl:attribute name="name"><xsl:value-of select="."/></xsl:attribute>
					<xsl:value-of select="."/>
				</option>
			</xsl:for-each>
		</select>

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
				</form>
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
	</xsl:template>

</xsl:stylesheet>

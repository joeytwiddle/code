<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
	extension-element-prefixes="yaslt"
	xmlns:yaslt="http://www.mod-xslt2.com/ns/1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:import href="../xhtmlCommon.xsl" />

<xsl:variable name="mode">
	<xsl:choose>
		<xsl:when test="element-available('yaslt:value-of')">
			<yaslt:value-of select="$GET[mode]" />
		</xsl:when>
		<xsl:otherwise> addEntry </xsl:otherwise>
	</xsl:choose>
</xsl:variable>

<xsl:output method="html"/>

<xsl:include href="../xmlverbatim.xsl"/>

	<xsl:template match="/">
		<xsl:apply-templates select="//JumpgateData"/>
	</xsl:template>

	<xsl:template match="JumpgateData">
		<select name="categoryMenu">
			<xsl:for-each select="//Category/.">
				<option>
					<xsl:attribute name="name"><xsl:value-of select="."/></xsl:attribute>
					<xsl:attribute name="value"><xsl:value-of select="."/></xsl:attribute>
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

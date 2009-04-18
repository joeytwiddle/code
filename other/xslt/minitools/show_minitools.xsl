<?xml version="1.0"?> 
<?xml-stylesheet type="text/xsl" href="jumpgate.xsl"?>
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
<!-- <xsl:import href="formscraper.xsl"/> -->

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

						<xsl:variable name="statsPre">
							<xsl:apply-templates select="//URL"/>
							<xsl:apply-templates name="postProc" select="//banana"/>
							<xsl:apply-templates select="//banana"/>
						</xsl:variable>
						<xsl:variable name="statsPreNodeSet" select="exslt:node-set($statsPre)"/>
						<!--
						<xsl:for-each select="exslt:node-set($statsPre)//*" sort-by="number(./@ScorePerHour)">
						<xsl:for-each select="$statsPreNodeSet//*" sort-by="number(./@ScorePerHour)">
						-->

						<xsl:variable name="srcUrl" value="'http://hwi.ath.cx/xslt/src/minitools/show_minitools.xsl'"/>

						<xsl:variable name="pageUrl" select="//URL/@url"/>

						<P>
							Generating from <A><xsl:attribute name="href"><xsl:value-of select="//URL/@url"/></xsl:attribute><xsl:value-of select="//URL/@url"/></A>
							via <A><xsl:attribute name="href"><xsl:value-of select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',$pageUrl)"/></xsl:attribute>this tidied copy</A>
							<!-- via <A><xsl:attribute name="href"><xsl:value-of select="'http://hwi.ath.cx/cgi-bin/joey/tidy?url='"/><xsl:value-of select="$pageUrl"/></xsl:attribute>this tidied copy</A> -->
							using <A><xsl:attribute name="href" select="$srcUrl"/>this XSLT</A>.
						</P>
						<P>Document 1 has <xsl:value-of select="count(//*)"/> nodes.</P>

						<xsl:variable name="tidyPageUrl" select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',$pageUrl)"/>
						<xsl:variable name="thePage" select="document($tidyPageUrl)"/>

						<P>Document 2 has <xsl:value-of select="count($thePage//*)"/> nodes.</P>

						<xsl:for-each select="$thePage//bookmark">
							<xsl:sort select="position()" data-type="number" order="descending"/>
							<P><A><xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute><xsl:value-of select="string(.)"/></A></P>
						</xsl:for-each>

					</P>
			</BODY></HTML>
	</xsl:template>

	<xsl:template match="URL">

			MATCH

	</xsl:template>

	<xsl:template name="postProc" match="banana">
		Hello Mr. Post-proc here.  I have got this.
	</xsl:template>

</xsl:stylesheet>

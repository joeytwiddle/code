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
		<xsl:variable name="srcUrl" value="'http://hwi.ath.cx/xslt/src/minitools/show_minitools.xsl'"/>
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
					Konqueror Minitools from <A><xsl:attribute name="href"><xsl:value-of select="//URL/@url"/></xsl:attribute>
						<!-- <xsl:value-of select="substring-before(//URL/@url,'/')"/></A> -->
						<xsl:value-of select="(//URL/@url)"/></A>
					<!-- via <A><xsl:attribute name="href"><xsl:value-of select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',$pageUrl)"/></xsl:attribute>Tidied</A> -->
					<!-- via <A><xsl:attribute name="href"><xsl:value-of select="'http://hwi.ath.cx/cgi-bin/joey/tidy?url='"/><xsl:value-of select="$pageUrl"/></xsl:attribute>this tidied copy</A> -->
					formatted into HTML by <A><xsl:attribute name="href" select="$srcUrl"/>this XSLT</A>.
				</P>

				<TABLE>

					<!-- <xsl:apply-templates select="$thePage//bookmark"/> -->
					<!-- We need to reverse the order.  I store my minitools in reverse order because Konqueror displays them in reverse order! -->
					<xsl:for-each select="$thePage//bookmark">
						<xsl:sort select="position()" data-type="number" order="descending"/>
						<xsl:apply-templates select="."/>
					</xsl:for-each>

				</TABLE>

				<SCRIPT type="text/javascript">
					document.write("There are "+document.links.length+" links on this page.&lt;BR&gt;");
					for (i=0;i&lt;document.links;i++) {
					}
					document.write("Script finished.");
				</SCRIPT>

			</BODY>
		</HTML>

	</xsl:template>

	<xsl:template match="bookmark">
		<TR>
			<TD>
				<A>
					<xsl:attribute name="id"><xsl:value-of select="position()"/></xsl:attribute>
					<xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute>
					<xsl:value-of select="string(.)"/>
				</A>
			</TD>
			<!-- Content of TEXTAREA should be converted to HTML. -->
			<TD>
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
			</TD>
		</TR>
	</xsl:template>

</xsl:stylesheet>


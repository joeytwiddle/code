<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:import href="../xhtmlCommon.xsl" />

<!--
	Well this was going to be the stylesheet that renders the jumpgate from its XML bits.
	But currently it is instead prototyping the ability to automatically steal forms off other webpages.
-->

<!-- Dont' forget to:
mkdir /tmp/mod-xslt
chown www-data:www-data /tmp/mod-xslt
!! -->

<xsl:include href="../xmlverbatim.xsl"/>

<xsl:output method="html"/>

	<xsl:template match="/">
		<html>
			<body>
				<xsl:apply-templates select="*"/>

				<script language="JavaScript" type="text/javascript" src="http://hwi.ath.cx/javascript/modifyDOM.js"></script>

				<script language="JavaScript" type="text/javascript"> <xsl:value-of select="string('&lt;!--')"/>
				addFoldsToBlockQuotes();
				// <xsl:value-of select="string('--&gt;')"/> </script>
			
			</body>
		</html>
	</xsl:template>

	<xsl:template match="CopyFormFromPage">

		<h2><xsl:value-of select="@name"/></h2>

		<blockquote>

			<xsl:variable name="pageUrl" select="@page"/>
			<xsl:variable name="tidyPageUrl" select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',string($pageUrl))"/>

			<xsl:variable name="thePage" select="document($tidyPageUrl)"/>
			<!--
			<xsl:variable name="thePage" select="document($tidyPageUrl)"/>
			<xsl:variable name="thePage" select="document(concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',string($pageUrl)))"/>
			-->
				<!--
				<xsl:import href="$pageUrl"/>
				<xsl:include href="$pageUrl"/>
				<xsl:import href="../fractal.xml"/>
				<xsl:include href="../fractal.xml"/>
			</xsl:variable>
				-->

			<P>
			OK I have loaded <xsl:value-of select="$pageUrl"/>, and tidied it.
			<BR/>
			<!--
			It has <xsl:value-of select="count($thePage//form)"/> forms.
			-->
			It has <xsl:value-of select="count($thePage//*[name(.)='form'])"/> forms.
			</P>

			<!-- <xsl:value-of select="$thePage"/> -->

			<!--
			<BR/><xsl:apply-templates select="$thePage//*[@color='red']" mode="xmlverb"/>
			<BR/><xsl:apply-templates select="$thePage//*[@size]" mode="xmlverb"/>
			-->
			<!--
			<BR/><xsl:apply-templates select="$thePage//xsl:form" mode="xmlverb"/>
			<BR/><xsl:apply-templates select="$thePage//xml:form" mode="xmlverb"/>
			<BR/><xsl:apply-templates select="$thePage//xhtml:form" mode="xmlverb"/>
			<BR/><xsl:apply-templates select="$thePage//html:form" mode="xmlverb"/>
			-->

			<P>
				Here are the forms:
				<blockquote>
					<xsl:apply-templates select="$thePage//*[name(.)='form']" mode="xmlverb"/>
				</blockquote>
			</P>
			<P>
				And here are their juicy bits:
				<blockquote>
					<xsl:apply-templates select="$thePage//*[name(.)='form']/@action" mode="xmlverb"/>
					<BR/>
					<xsl:apply-templates select="$thePage//*[name(.)='form']//*[name(.)='input' or name(.)='select' or name(.)='button']" mode="xmlverb"/>
				</blockquote>
			</P>

		</blockquote>

		<P/>

	</xsl:template>

</xsl:stylesheet>

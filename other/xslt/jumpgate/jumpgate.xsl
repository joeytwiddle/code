<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
	extension-element-prefixes="yaslt"
	xmlns:yaslt="http://www.mod-xslt2.com/ns/1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:import href="../xhtmlCommon.xsl" />

<!-- <xsl:param name="mode" select="'default'"/> -->
<xsl:variable name="mode">
	<yaslt:value-of select="$GET[mode]" />
</xsl:variable>
<!--
<xsl:param name="mode" select="'demo'"/>
-->
<!--
<?modxslt-param name="mode" value="default"/>
-->

<!-- TODO: there must be a proper way to do this! -->
<xsl:variable name="baseUrl" select="'jumpgate.xml'"/>

<!--
	Well this was going to be the stylesheet that renders the jumpgate from its XML bits.
	But currently it is instead prototyping the ability to automatically steal forms off other webpages.
-->

<!-- Dont' forget to:
mkdir /tmp/mod-xslt
chown www-data:www-data /tmp/mod-xslt
Hmm well i did move it to /tmp in Apache config, but that's not entirely secure is it?!  I mean, ppl could read the tmpfiles if nothing else.
!! -->

<xsl:include href="../xmlverbatim.xsl"/>

<xsl:output method="html"/>

	<xsl:template match="/">
		<html>
			<body>

				<!-- MODE=<xsl:value-of select="$mode"/> -->

				<xsl:if test="$mode='default' or $mode=''">

					<A>
						<xsl:attribute name="href"><xsl:value-of select="$baseUrl"/>?mode=addEntry</xsl:attribute>
						Add a jumpgate entry
					</A>

					<A>
						<xsl:attribute name="href"><xsl:value-of select="$baseUrl"/>?mode=demo</xsl:attribute>
						Show the demo
					</A>

				</xsl:if>

				<xsl:if test="$mode='addEntry'">
					<xsl:variable name="urlToScrape">
						<yaslt:value-of select="$GET[urlToScrape]" />
					</xsl:variable>
					<xsl:if test="boolean(1)">
						<FORM action="">
							Enter the webpage which contains your service's search form:
							<BR/>
							<INPUT type="hidden" name="mode" value="addEntry"/>
							<INPUT name="urlToScrape" value="http://" size="30" maxlength="9999"/>
						</FORM>
					</xsl:if>
					<xsl:apply-templates select="'hello'" mode="xmlverb"/>
					URL=<xsl:value-of select="$urlToScrape"/>
					<xsl:if test="boolean(1)">
						<xsl:apply-templates select="$urlToScrape" mode="xmlverb"/>
						<xsl:variable name="todo">
							<CopyFormFromPage>
								<xsl:attribute name="page"><xsl:value-of select="$urlToScrape"/></xsl:attribute>
							</CopyFormFromPage>
						</xsl:variable>
						<!--
							<xsl:copy-of select="concat('&lt;CopyFormFromPage page=&quot;',$urlToScrape,'&quot; name=&quot;NoNameYet&quot;/&gt;')"/>
							<xsl:value-of select="concat('&lt;CopyFormFromPage page=&quot;',$urlToScrape,'&quot; name=&quot;NoNameYet&quot;/&gt;')"/>
							<CopyFormFromPage page="$urlToScrape" name="NoNameYet"/>
							<xsl:copy>
							<xsl:element name="CopyFormFromPage">
								<xsl:attribute name="page"><xsl:value-of select="$urlToScrape"/></xsl:attribute>
							</xsl:element>
							</xsl:copy>
						-->
						TODO=<xsl:value-of select="$todo"/>
						<xsl:apply-templates select="$todo" mode="xmlverb"/>
						<xsl:apply-templates select="$todo"/>
						<xsl:call-template name="ScrapeForm" select="$todo/node()"/>
						<!-- ... -->
					</xsl:if>
				</xsl:if>

				<xsl:if test="$mode='demo'">

					<xsl:apply-templates select="*"/>

					<script language="JavaScript" type="text/javascript" src="http://hwi.ath.cx/javascript/modifyDOM.js"></script>
					<script language="JavaScript" type="text/javascript"> <xsl:value-of select="string('&lt;!--')"/>
					addFoldsToBlockQuotes();
					// <xsl:value-of select="string('--&gt;')"/> </script>

				</xsl:if>

			</body>
		</html>
	</xsl:template>

	<xsl:template name="ScrapeForm" match="CopyFormFromPage">

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

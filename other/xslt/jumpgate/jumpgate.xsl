<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
	extension-element-prefixes="yaslt"
	xmlns:yaslt="http://www.mod-xslt2.com/ns/1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:import href="../xhtmlCommon.xsl" />

<!--
<xsl:variable name="mode">
	<xsl:choose>
		<xsl:when test="element-available('yaslt:value-of')">
			<yaslt:value-of select="$GET[mode]" />
		</xsl:when>
		<xsl:otherwise> addEntry </xsl:otherwise>
	</xsl:choose>
</xsl:variable>
-->
<!--
<xsl:param name="mode" value="addEntry"/>
-->
<xsl:variable name="mode">addEntry</xsl:variable>
<!--
<?modxslt-param name="mode" value="default"/>
-->
<!-- <xsl:param name="mode" select="'default'"/> -->

<xsl:output method="html"/>

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

	<xsl:template match="/">
		<html>
			<body>

				MODE=<xsl:value-of select="$mode"/>=<P/>

				<xsl:variable name="color">red</xsl:variable>
				<FONT COLOR="{$color}">Red Text</FONT>

				<xsl:if test="$color='red'">
					success
				</xsl:if>

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
					<!--
					<xsl:variable name="urlToScrape">
						<xsl:choose>
							<xsl:when test="element-available('yaslt:value-of')">
								<yaslt:value-of select="$GET[urlToScrape]" />
							</xsl:when>
							<xsl:otherwise> http://www.google.com/ </xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					-->
					<xsl:variable name="urlToScrape">http://www.google.com/</xsl:variable>
					URL=<xsl:value-of select="$urlToScrape"/>
					<xsl:if test="boolean(1)">
						<FORM action="">
							Enter the webpage which contains your service's search form:
							<BR/>
							<INPUT type="hidden" name="mode" value="addEntry"/>
							<INPUT name="urlToScrape" value="http://" size="30" maxlength="9999"/>
						</FORM>
					</xsl:if>
					<xsl:if test="boolean(1)">
						<!--
						<xsl:apply-templates select="'hello'" mode="xmlverb"/>
						<xsl:apply-templates select="{$urlToScrape}" mode="xmlverb"/>
						-->
						<xsl:if test="boolean(1)">
							<!--
							<xsl:variable name="todo">
								<CopyFormFromPage>
									<xsl:attribute name="page"><xsl:value-of select="$urlToScrape"/></xsl:attribute>
								</CopyFormFromPage>
							</xsl:variable>
								<xsl:copy-of select="concat('&lt;CopyFormFromPage page=&quot;',$urlToScrape,'&quot; name=&quot;NoNameYet&quot;/&gt;')"/>
								<xsl:value-of select="concat('&lt;CopyFormFromPage page=&quot;',$urlToScrape,'&quot; name=&quot;NoNameYet&quot;/&gt;')"/>
								<CopyFormFromPage page="$urlToScrape" name="NoNameYet"/>
								<xsl:copy>
								<xsl:element name="CopyFormFromPage">
									<xsl:attribute name="page"><xsl:value-of select="$urlToScrape"/></xsl:attribute>
								</xsl:element>
								</xsl:copy>
							TODO=<xsl:value-of select="$todo"/>
							<xsl:apply-templates select="$todo" mode="xmlverb"/>
							<xsl:apply-templates select="$todo"/>
							<xsl:call-template name="ScrapeForm" select="$todo/node()"/>
							-->
							<xsl:call-template name="AddEntry_SelectForm">
								<!--
								<xsl:with-param name="pageUrl" select="string($urlToScrape)"/>
								-->
								<xsl:with-param name="pageUrl" select="string('http://www.google.com/')"/>
							</xsl:call-template>
							<!-- ... -->
						</xsl:if>
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

	<xsl:template name="AddEntry_SelectForm">
		<xsl:param name="pageUrl"/>

		<xsl:variable name="forms">
			<xsl:call-template name="ScrapeFormsFromUrl">
				<xsl:with-param name="pageUrl" select="$pageUrl"/>
			</xsl:call-template>
		</xsl:variable>

		<P>
		OK I have loaded <xsl:value-of select="$pageUrl"/>, and tidied it.
		<BR/>
		It has <xsl:value-of select="count($forms)"/> forms.
		<!--
		It has <xsl:value-of select="count($forms//*[name(.)='form'])"/> forms.
		-->
		<xsl:value-of select="$forms"/>
		<!--
		<xsl:copy-of select="$forms//*[name(.)='form']"/>
		-->
		<!--
		<xsl:for-each select="$forms//*[name(.)='form']">
		-->
		<xsl:for-each select="$forms/.">
			<BR/>MARK
		</xsl:for-each>
		<!--
		<xsl:variable name="mozForms">
			<xsl:copy>
			<xsl:value-of select="$forms"/>
			</xsl:copy>
			<xsl:copy-of select="$forms"/>
		</xsl:variable>
		<xsl:value-of select="$mozForms/."/>
		-->
		</P>

		<P/>
		VALUE-OF(forms):<BR/>
		<xsl:value-of select="$forms"/>
		<P/>

		<!-- mod_xslt complains "Invalid type" here, presumably cannot iterate null result! -->
		<!--
		<xsl:for-each select="$forms//*[name(.)='input']">
			WIKKED <P/>
		</xsl:for-each>
		-->

		<!--
		<xsl:apply-templates select="$forms"/>
		<xsl:apply-templates select="string($forms)" mode="xmlverb"/>
		<xsl:apply-templates select="." mode="xmlverb"/>
		-->

	</xsl:template>

	<xsl:template name="ScrapeFormsFromUrl">
		<xsl:param name="pageUrl"/>
		<xsl:variable name="tidyPageUrl" select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',string($pageUrl))"/>

		<xsl:variable name="thePage" select="document($tidyPageUrl)"/>

		<!--
		<xsl:copy>
		<xsl:copy-of select="$thePage//*[name(.)='form']"/>
		</xsl:copy>
		-->

		<!--
		<ScrapingResults>
			<xsl:value-of select="$thePage//*[name(.)='form']"/>
		</ScrapingResults>
		-->

		<xsl:element name="ScrapingResults">
			<xsl:copy-of select="$thePage"/>
			<!--
			<xsl:copy-of select="$thePage//*[name(.)='form']"/>
			-->
			<!--
			<xsl:value-of select="$thePage//*[name(.)='form']"/>
			<xsl:element name="Inside"/>
			-->
		</xsl:element>

		<xsl:for-each select="$thePage//*[name(.)='input']">
			INPUT <P/>
		</xsl:for-each>

<!--
		<P>
		OK I have loaded <xsl:value-of select="$pageUrl"/>, and tidied it.
		<BR/>
		It has <xsl:value-of select="count($thePage//*[name(.)='form'])"/> forms.
		</P>
-->

		<!--
		<xsl:for-each select="$thePage//*[name(.)='form']">

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

		</xsl:for-each>
		-->

	</xsl:template>

	<xsl:template match="form|FORM">
		Got a form with action=<xsl:value-of select="@action"/>
		<P/>
	</xsl:template>

</xsl:stylesheet>

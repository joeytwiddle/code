<?xml version="1.0"?> 
<?xml-stylesheet type="text/xsl" href="formscraper.xsl"?> 
<xsl:stylesheet version="1.0"
	extension-element-prefixes="yaslt"
	xmlns:yaslt="http://www.mod-xslt2.com/ns/1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:import href="../xhtmlCommon.xsl" />
<xsl:include href="../xmlverbatim.xsl"/>

<xsl:variable name="mode">
	<xsl:choose>
		<xsl:when test="element-available('yaslt:value-of')">
			<yaslt:value-of select="$GET[mode]" />
		</xsl:when>
		<xsl:otherwise></xsl:otherwise>
	</xsl:choose>
</xsl:variable>
<!--
<xsl:variable name="mode">addEntry</xsl:variable>
-->
<!--
<xsl:param name="mode" value="addEntry"/>
-->
<!--
<?modxslt-param name="mode" value="default"/>
-->
<!-- <xsl:param name="mode" select="'default'"/> -->

<xsl:output method="html"/>

<!-- TODO: there must be a proper way to do this! -->
<xsl:variable name="baseUrl" select="'formscraper.xsl'"/>

<!--
	Well this was going to be the stylesheet that renders the jumpgate from its XML bits.
	But currently it is instead prototyping the ability to automatically steal forms off other webpages.
-->

<!-- Dont' forget to:
mkdir /tmp/mod-xslt
chown www-data:www-data /tmp/mod-xslt
Hmm well i did move it to /tmp in Apache config, but that's not entirely secure is it?!  I mean, ppl could read the tmpfiles if nothing else.
!! -->

	<xsl:template match="/">
		<html>
			<body>

				<!--
				MODE=<xsl:value-of select="$mode"/>=<P/>
				<xsl:variable name="color">red</xsl:variable>
				<FONT COLOR="{$color}">Red Text</FONT>
				<xsl:if test="$color='red'">
					success
				</xsl:if>
				-->

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

				<xsl:variable name="urlToScrape">
					<xsl:choose>
						<xsl:when test="element-available('yaslt:value-of')">
							<yaslt:value-of select="$GET[urlToScrape]" />
						</xsl:when>
						<xsl:otherwise></xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<!--
				<xsl:if test="$mode='addEntry' or $mode='addEntryProgress'">
				-->
				<xsl:if test="$mode='addEntry'">
					<!--
					<xsl:variable name="urlToScrape">http://www.google.com/</xsl:variable>
					URL=<xsl:value-of select="$urlToScrape"/>
					-->
					<xsl:if test="$urlToScrape=''">
						<!-- Links won't accept action="" ! -->
						<!-- JavaWebBrowser/HttpUnit won't accept action="#" ! -->
						<FORM action="">
							<!-- <xsl:attribute name="action"><xsl:value-of select="$baseUrl"/></xsl:attribute> -->
							Enter the webpage which contains your service's search form:
							<BR/>
							<INPUT type="hidden" name="mode" value="addEntryProgress"/>
							<INPUT type="text" name="urlToScrape" value="http://" size="30" maxlength="9999"/>
							<INPUT type="submit" value="Next"/>
							<!--
							<INPUT type="submit" onclick="document.forms[0].submit()" value="Next"/>
							<A href="javascript:document.forms[0].submit();">Next</A>
							<A href="#" onclick="document.forms[0].submit();">Next</A>
							-->
						</FORM>
					</xsl:if>
				</xsl:if>
				<xsl:if test="$mode='addEntryProgress'">
					<!--
					<xsl:if test="$urlToScrape!='' or $mode='addEntryProgress'">
					-->
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
								<xsl:with-param name="pageUrl" select="string($urlToScrape)"/>
								<!--
								<xsl:with-param name="pageUrl" select="string('http://www.google.com/')"/>
								-->
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

		<xsl:choose>
			<xsl:when test="not(starts-with($pageUrl,'http://'))">
				Since the pageUrl did not start with http://, for security reasons I cannot process your so-called URL.
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="tidyPageUrl" select="concat('http://hwi.ath.cx/cgi-bin/joey/tidy?url=',string($pageUrl))"/>

				<xsl:variable name="thePage" select="document($tidyPageUrl)"/>

				<xsl:variable name="addEntryStage">
					<xsl:choose>
						<xsl:when test="element-available('yaslt:value-of')">
							<yaslt:value-of select="$GET[addEntryStage]" />
						</xsl:when>
						<xsl:otherwise> addEntry </xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name="urlToScrape">
					<xsl:choose>
						<xsl:when test="element-available('yaslt:value-of')">
							<yaslt:value-of select="$GET[urlToScrape]" />
						</xsl:when>
						<xsl:otherwise></xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:if test="$addEntryStage=''">
					<form action="#">
						Please select the form you wish to use:
						<input type="hidden" name="mode" value="addEntryProgress"/>
						<input type="hidden" name="addEntryStage" value="2formChosen"/>
						<INPUT type="hidden" name="urlToScrape">
							<xsl:attribute name="value"><xsl:value-of select="$urlToScrape"/></xsl:attribute>
						</INPUT>
						<table border="1">
							<xsl:for-each select="$thePage//*[name(.)='form']">
								<tr>
								<td>Form #<xsl:value-of select="string(position())"/></td>
								<td><xsl:copy-of select="./*"/></td>
								<!--
								<td><xsl:apply-templates select="." mode="xmlverb"/></td>
								-->
								<td>
									<!--
									<input type="radio" value="This one" onclick="javascript:alert('u clicked me');"/>
									-->
									<input type="radio" name="selectedFormNum">
										<xsl:attribute name="value"><xsl:value-of select="string(position())"/></xsl:attribute>
									</input>
								</td>
								</tr>
							</xsl:for-each>
							<tr><td colspan="3" align="right">
								<input type="submit" value="Next"/>
							</td></tr>
						</table>
					</form>
				</xsl:if>

				<xsl:variable name="selectedFormNum">
					<xsl:choose>
						<xsl:when test="element-available('yaslt:value-of')">
							<yaslt:value-of select="$GET[selectedFormNum]" />
						</xsl:when>
						<xsl:otherwise> addEntry </xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:if test="$addEntryStage='2formChosen'">
					<xsl:variable name="form" select="($thePage//*[name(.)='form'])[number($selectedFormNum)]"/>
					<form action="#">
						<input type="hidden" name="mode" value="addEntryProgress"/>
						<input type="hidden" name="addEntryStage" value="3fieldChosen"/>
						<INPUT type="hidden" name="urlToScrape">
							<xsl:attribute name="value"><xsl:value-of select="$urlToScrape"/></xsl:attribute>
						</INPUT>
						<input type="hidden" name="selectedFormNum">
							<xsl:attribute name="value"><xsl:value-of select="$selectedFormNum"/></xsl:attribute>
						</input>
						Now please select which of the fields is the one you wish to search on:
						<table>
							<xsl:for-each select="$form//*[name(.)='input'] | $form//*[name(.)='select']">
								<tr>
									<td><xsl:value-of select="@name"/>=<xsl:value-of select="@value"/></td>
									<td><xsl:copy-of select="."/></td>
									<td>
										<input type="radio" name="selectedInputName">
											<xsl:attribute name="value"><xsl:value-of select="@name"/></xsl:attribute>
											<!-- TODO: fails -->
											<xsl:if test="name(.)='input' and ./@type='text'">
												<xsl:attribute name="selected">true</xsl:attribute>
											</xsl:if>
										</input>
									</td>
								</tr>
							</xsl:for-each>
							<tr><td colspan="3" align="right">
								<input type="submit" value="Next"/>
							</td></tr>
						</table>
					</form>
				</xsl:if>

				<xsl:if test="$addEntryStage='3fieldChosen'">
					<xsl:variable name="selectedInputName">
						<xsl:choose>
							<xsl:when test="element-available('yaslt:value-of')">
								<yaslt:value-of select="$GET[selectedInputName]" />
							</xsl:when>
							<xsl:otherwise> todo:moz </xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name="entryName">nameTODO</xsl:variable>
					<xsl:variable name="entryDescription">descriptionTODO</xsl:variable>
					<xsl:variable name="entryCategory">categoryTODO</xsl:variable>
					<!--
					<xsl:variable name="entryName">
						<xsl:choose>
							<xsl:when test="element-available('yaslt:value-of')">
								<yaslt:value-of select="$GET[entryName]" />
							</xsl:when>
							<xsl:otherwise> todo:moz </xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name="entryDescription">
						<xsl:choose>
							<xsl:when test="element-available('yaslt:value-of')">
								<yaslt:value-of select="$GET[entryDescription]" />
							</xsl:when>
							<xsl:otherwise> todo:moz </xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name="entryCategory">
						<xsl:choose>
							<xsl:when test="element-available('yaslt:value-of')">
								<yaslt:value-of select="$GET[entryCategory]" />
							</xsl:when>
							<xsl:otherwise> todo:moz </xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					-->
					<xsl:variable name="form" select="($thePage//*[name(.)='form'])[number($selectedFormNum)]"/>
					<!--
					<xsl:apply-templates select="$form/." mode="xmlverb"/>
					-->
					This is the data you will need to add to the jumpgate:
					<xsl:variable name="newEntry">
					<!--
					<xsl:copy mode="xmlverb">
						<xsl:element name="JumpgateEntry">
					-->
						<JumpgateEntry>
							<xsl:attribute name="name"><xsl:value-of select="$entryName"/></xsl:attribute>
							<Description><xsl:value-of select="$entryDescription"/></Description>
							<Category><xsl:value-of select="$entryCategory"/></Category>
							<!--
							<MainParameter name=&quot;<xsl:value-of select="$selectedInputName"/>&quot;/&gt;
							-->
							<HowToAccess>
								<Form>
									<!-- TODO: this action URL needs to be made absolute if it isn't already (relative to urlToScrape). -->
									<xsl:attribute name="action"><xsl:value-of select="$form/@action"/></xsl:attribute>
									<HiddenParameters>
										<!-- TODO: need to turn them all into hidden input elements -->
										<!-- That includes type=radio but also textarea, select, and option tags! -->
										<!-- NO!!  Better to retain all info and allow Jumpgate to hide or do whatever as-and-when.  Leave flexibility until the other end?! -->
										<!-- Do we want to keep Javascript in?  Do we want to keep their original layout of elements? -->
										<xsl:apply-templates select="$form//*[@name != string($selectedInputName)][name(.)='input' or name(.)='select' or name(.)='textarea']"/>
									</HiddenParameters>
									<ExposedParameters>
										<xsl:apply-templates select="$form//*[@name = string($selectedInputName)][name(.)='input' or name(.)='select' or name(.)='textarea']"/>
									</ExposedParameters>
								</Form>
							</HowToAccess>
						</JumpgateEntry>
					<!--
						</xsl:element>
					</xsl:copy>
					-->
					</xsl:variable>
					<blockquote>
						<xsl:apply-templates select="string($newEntry)" mode="xmlverb"/>
						<P/>
						<xsl:apply-templates select="$newEntry" mode="xmlverb"/>
						<P/>
						<xsl:copy-of select="$newEntry" mode="xmlverb"/>
						<P/>
						<font size="-1">
							<textarea cols="80" rows="20">
								<xsl:copy-of select="$newEntry"/>
							</textarea>
						</font>
					</blockquote>
					<!--
					<form action="">
						<input type="hidden" name="mode" value="addEntry"/>
						<input type="hidden" name="addEntryStage" value="3fieldChosen"/>
						<input type="hidden" name="selectedFormNum">
							<xsl:attribute name="value"><xsl:value-of select="$selectedFormNum"/></xsl:attribute>
						</input>
						Now please select which of the fields is the one you wish to search on:
						<table>
							<xsl:for-each select="$form//*[name(.)='input'] | $form//*[name(.)='select']">
								<tr>
									<td><xsl:value-of select="@name"/>=<xsl:value-of select="@value"/></td>
									<td><xsl:copy-of select="."/></td>
									<td>
										<input type="radio" name="selectedInputName">
											<xsl:attribute name="value"><xsl:value-of select="@name"/></xsl:attribute>
											<xsl:if test="name(.)='input' and ./@type='text'">
												<xsl:attribute name="selected">true</xsl:attribute>
											</xsl:if>
										</input>
									</td>
								</tr>
							</xsl:for-each>
							<tr><td colspan="3" align="right">
								<input type="submit" value="Next"/>
							</td></tr>
						</table>
					</form>
					-->
				</xsl:if>

			</xsl:otherwise>
		</xsl:choose>
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

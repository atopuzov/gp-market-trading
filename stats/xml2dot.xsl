<?xml version="1.0" encoding="ISO-8859-1"?>
<!--
Author: William Candillon <wcandillon@php.net>, (http://wcandillon.blogspot.com/)
License: GPL (http://www.opensource.org/licenses/gpl-license.php)

This stylesheet generate a graph in dot format from an xml tree.
Modified for use with OpenBeagle by Aleksandar Topuzovic <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>
-->
<!DOCTYPE stylesheet [
<!ENTITY xsl "http://www.w3.org/1999/XSL/Transform">
<!ENTITY tab "    ">
<!ENTITY newline "
">
<!ENTITY indent "    ">
]>
<xsl:stylesheet xmlns="&xsl;" xmlns:xsl="&xsl;" version="1.0">
	<xsl:output method="text" indent="no"/>
	<xsl:template match="*">
		<xsl:if test="not(parent::*)">
			<xsl:text>graph Tree {&newline;&tab;</xsl:text>
		</xsl:if>
		<xsl:if test="parent::*">
			<xsl:call-template name="generateRelation">
				<xsl:with-param name="parent" select="parent::*"/>
				<xsl:with-param name="child" select="."/>
			</xsl:call-template>
			<xsl:call-template name="generateLabel">
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
		</xsl:if>
		<xsl:for-each select="child::*">
			<xsl:call-template name="generateRelation">
				<xsl:with-param name="parent" select="parent::*"/>
				<xsl:with-param name="child" select="."/>
			</xsl:call-template>
			<xsl:call-template name="generateLabel">
				<xsl:with-param name="node" select="."/>
			</xsl:call-template>
			<!-- Root node -->
			<xsl:if test="parent::*[not(parent::*)]">
				<xsl:call-template name="generateLabel">
					<xsl:with-param name="node" select="parent::*"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:apply-templates select="child::*"/>
		</xsl:for-each>
		<xsl:if test="not(parent::*)">
			<xsl:text>&newline;}</xsl:text>
		</xsl:if>
	</xsl:template>
	<!-- Generates the label -->
	<xsl:template name="generateLabel">
		<xsl:param name="node"/>
		<xsl:value-of select="generate-id($node)"/>
		<xsl:text> [label="</xsl:text>
		<xsl:value-of select="local-name($node)"/>
		<!-- Check if the value attribute exist -->
		<xsl:if test="@value">
			<xsl:text> = </xsl:text>
			<xsl:value-of select="@value"/>
		</xsl:if>
		<xsl:text>"];&newline;&tab;</xsl:text>
	</xsl:template>
	<!-- Generates the relation -->	
	<xsl:template name="generateRelation">
		<xsl:param name="parent"/>
		<xsl:param name="child"/>
		<xsl:value-of select="generate-id($parent)"/>
		<xsl:text> -- </xsl:text>
		<xsl:value-of select="generate-id($child)"/>
		<xsl:text>;&newline;&tab;</xsl:text>
	</xsl:template>
</xsl:stylesheet>

"""
	The Dictionary Lookup for XChat!
	================================
	This looks up and prints the definition 
	to a word as found on dictionary.com

	This is no where near extensive, as it
	uses only one of the definitions given, 
	and there is also very little error checking.

	Use with care: you could be spammed/flooded.

	To use, simply (in XChat) type, or have someone
	else type, !define <word>


	License
	-------
	Copyright (C) 2005 Jens Taylor

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
"""

import xchat, time
import re
from sys import exit
from httplib import HTTPConnection
from urllib import urlencode
from thread import start_new_thread

__module_name__ = "Dict" 
__module_version__ = "0.2" 
__module_description__ = "Dictionary Lookup"

lastcommand = 0
wait = 60
active = 1
prevent_loop = 0

def me_say(word, word_eol, userdata):
	"""
	Fixes the definition coming before your inquiry when YOU make a call to !define
	"""
	global lastcommand, timeout, active, prevent_loop
	if active == 1 and time.time() - lastcommand > wait and prevent_loop == 0:
		if word[1][0:7] == "!define":
			destination = xchat.get_context()
			start_new_thread(get_word, (word, destination))
			prevent_loop = 1
			destination.command("SAY " + word[1])
			prevent_loop = 0
			return xchat.EAT_ALL

def they_say(word, word_eol, userdata):
	"""
	Prints their message before your definition (only a problem on your screen, now fixed)
	"""
	global lastcommand, timeout, active, prevent_loop
	if active == 1 and time.time() - lastcommand > wait and prevent_loop == 0:
		if word[1][0:7] == "!define":
			destination = xchat.get_context()
			start_new_thread(get_word, (word, destination))
			prevent_loop = 1
			destination.emit_print("Channel Message", word[0], word[1], "@")
			prevent_loop = 0
			return xchat.EAT_ALL

def get_word(word, destination):
	"""
	If the person said !define, pull the word out they want defined
	"""
	if word[1][8:] != '':
		lastcommand = time.time()
		definition = connect(word[1][8:])
		destination.command("SAY -\0033" + word[1][8:] + "\003-")
		destination.command("SAY " + definition)
		try:
			definition[439]
			destination.command("SAY \0037For more info, search http://dictionary.reference.com?q="+test)
		except:
			return
	else:
		destination.command("SAY \0034Sorry, you need to input a word!")

def connect(word):
	"""
	Make the connection and get that code!
	"""
	conn = HTTPConnection("dictionary.reference.com")
	try:
		conn.request("GET", "/search?q="+word)
	except:
		return "\0034Sorry, error connecting to dictionary server!"
	response = conn.getresponse()
	data = response.read()

	if data.find("<!-- begin wn -->") == -1:
		return "\0034Sorry, \0033" + word + " \0034not found!"
	else:
		text = get_def(data)
	conn.close()
	return text[0:440]


def get_def(data):
	"""
	Strip out all the useless web code, leaving us with a nice definition
	"""
	data = data.replace("\n", " ")
	re_def = re.compile("^(.*)<!-- begin wn -->(.*)<p>(?P<definition>.*)</p>(.*)<!-- end wn -->(.*)")
	fulldef = re_def.search(data)
	re_strip = re.compile("\[(syn|ant).*?</a>\]")
	strip = re_strip.sub('', fulldef.group("definition"))
	return strip

def set_vars(word, word_eol, userdata):
	"""
	Change the way the script behaves. You can set it on or off, and also change the timeout period
	"""
	global wait, active, lastcommand
	if word[1][0:4] == "time":
		try:
			wait = int(word[2])
			xchat.prnt("Timeout changed to "+str(wait)+" seconds")
		except:
			xchat.prnt("Sorry, you need to input a time in seconds")
	elif word[1][0:3] == "off":
		if active == 1:
			active = 0
			xchat.prnt("Module turned off!")
		else:
			xchat.prnt("Module already disabled!")
	elif word[1][0:2] == "on":
		if active == 0:
			active = 1
			lastcommand = 0
			xchat.prnt("Module turned on!")
		else:
			xchat.prnt("Module already enabled!")
	return xchat.EAT_ALL

def unloading(userdata):
	xchat.prnt("Unloading Dict module...")

# These are all XChat specific functions
xchat.hook_print("Channel Msg Hilight", they_say)
xchat.hook_print("Channel Message", they_say)
xchat.hook_print("Private Message to Dialog", they_say)
xchat.hook_print("Your Message", me_say)
xchat.hook_command("dict", set_vars)
xchat.hook_unload(unloading)

# Print success on load
xchat.prnt("Dict loaded successfully.")

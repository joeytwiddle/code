# Simple Python pounce plugin. Waits for a user to join a channel, and sends 
# them a message. You'll have to be in the channel the whole time, and it 
# won't save unsent pounces when you close xchat. Messages are only sent once.
#
# Tested on XChat 2.0.9, Python 2.3.4.
# Email dufflebunk@dufflebunk.homeip.net if there are issues

import xchat

__module_name__ = "Pounce"
__module_version__ = "0.2"
__module_description__ = "Allow messages to be sent to people when they join a channel"

NetList={}

def pounce_cb(word, word_eol, userdata, NetList=NetList):
	if len(word) == 2 and word[1] == "list" :
		print "Pounces ready for pouncing:"
		for  netName in NetList.keys():
			net=NetList[netName]
			for chanName in net.keys():
				chan=net[chanName]
				for nickName in chan.keys():
					nick=chan[nickName]
					print "On "+netName+" in "+chanName+" for "+nickName+":"
					for msg in nick:
						print msg
		return xchat.EAT_XCHAT
	if len(word) < 3:
		print("Invalid invocation of pounce. Try /help POUNCE.")
		return xchat.EAT_XCHAT
	
	channel=xchat.get_info("channel")
	network=xchat.get_info("network")
	if network is None:
		network=xchat.get_info("host")
	
	nick=word[1]
	message=word_eol[2]
	if not NetList.has_key(network):
		NetList[network]={}
	netMsgs=NetList[network]
	if not netMsgs.has_key(channel):
		netMsgs[channel]={}
	chanMsgs=netMsgs[channel]
	if not chanMsgs.has_key(nick):
		nickMsgs=[]
		chanMsgs[nick]=nickMsgs
	
	nickMsgs=chanMsgs[nick]
	nickMsgs.append(message)
	print "Messages waiting for "+nick+" on "+network+" in "+channel+": "
	print nickMsgs

	return xchat.EAT_XCHAT

def pounce_join_cb(word, word_eol, userdata):
	
	network=xchat.get_info("network")
	channel=word[1]
	nickTxt=word[0]

	if not NetList.has_key(network):
		return xchat.EAT_NONE
	net=NetList[network]

	if not net.has_key(channel):
		return xchat.EAT_NONE
	chan=net[channel]

	if not chan.has_key(nickTxt):
		return xchat.EAT_NONE
	nick=chan[nickTxt]

	for msg in nick:
		xchat.command("msg "+nickTxt+" (Pounce message): "+msg)
	chan.pop(nickTxt)
	
	return xchat.EAT_NONE

xchat.hook_command("POUNCE", pounce_cb, help="/POUNCE <nick> <message> Sends a <message> to the <nick> as soon as they join the <channel>. /POUNCE list will list all waiting messages.")
xchat.hook_print("Join", pounce_join_cb)

# ChangeLog
# 0.2 :
# - Added /pounce list
# - Add case where the network isn't named, the hostname is used
#
# 0.1 :
# Basic functionality

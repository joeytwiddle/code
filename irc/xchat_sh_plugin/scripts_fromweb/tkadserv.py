#!/usr/bin/python

############################################################
# Config options
# Also check near the bottom of this file for much more 
# advanced configuration...  (inside of "run()")
############################################################
# configure this in your ~/.tkadserv.conf

############################################################
# The actual script
############################################################

import xchat

program_name = "tkadserv"
program_version = "0.3"
__module_name__ = "TK_adserv" 
__module_version__ = program_version
__module_description__ = "Responds to triggers, per channel." 
 
print "%s %s initializing..." % (__module_name__, __module_version__)

debug = 1
def dprint(text):
  if debug: print text

class trigger:
  def __init__(self, trig, response, mode="notice"):
    self.trig = trig.lower()
    self.response = response
    self.mode = mode
    if not self.mode:
      self.mode = "notice"

class channel:
  def __init__(self, name):
    self.name = name.lower()
    self.triggers = {}

  def make_trigger(self, trig, response, mode=None):
    t = trigger(trig, response, mode)
    self.add_trigger(t)

  def add_trigger(self, t):
    self.triggers[t.trig] = t

class adserv:
  def __init__(self, cfg):
    self.cfg = cfg
    self.privmsg_hook = None
    self.control_hook = None
    self.channels = {}

  def add_channel(self, chan):
    self.channels[chan.name] = chan

  def run(self):
    # xchat.nickcmp(nick1, nick2)
    self.control_hook = xchat.hook_command("adserv", self.adserv_control)
    self.enable()

  def enable(self, *args):
    dprint("enable()")
    if not self.privmsg_hook:
      self.privmsg_hook = xchat.hook_server("PRIVMSG", self.responder)

  def disable(self, *args):
    dprint("disable()")
    if self.privmsg_hook:
      xchat.unhook(self.privmsg_hook)
      self.privmsg_hook = None

  def stop(self, *args):
    self.disable()
    xchat.unhook(self.control_hook)

  def responder(self, word, word_eol, userdata):
    #dprint("responder(%s, %s, %s)" % (word, word_eol, userdata))
    if word[1] != "PRIVMSG":
      return

    user = word[0][1:]
    channel = word[2].lower()
    foo = user.index("!")
    nick = user[:foo]
    try:
      req = word_eol[3][1:].lower()
    except:
      return

    if self.channels.has_key(channel):
      if self.channels[channel].triggers.has_key(req):
	t = self.channels[channel].triggers[req]
	#dprint("Trigger found.")
	#dprint("nick(%s) channel(%s) server(%s)" % (nick, channel, ""))
	xchat.command("%s %s %s" % (t.mode, nick, t.response))

  def adserv_control(self, word, word_eol, userdata):
    #dprint("adserv_control(%s, %s, %s)" % (word, word_eol, userdata))

    if word[1] == "help":
      print "%s %s: %s" % (__module_name__, __module_version__, __module_description__)
      print "Usage:"
      print "\t/adserv on \tEnable adserv"
      print "\t/adserv off \tDisable adserv"
      print "\t/adserv list \tShow all channels and triggers"

    elif word[1] == "on":
      self.enable()

    elif word[1] == "off":
      self.disable()

    elif word[1] == "list":
      for channel in self.channels.keys():
	c = self.channels[channel]
	for trigger in c.triggers.keys():
	  t = c.triggers[trigger]
	  print "\t%s: %s" % (channel, t.trig)

############################################################
# Config class stuff...
############################################################
class config:
  def __init__(self, name=program_name):
    import os
    try:
      home = os.environ["HOME"]
    except:
      home = "/tmp"

    self._name = name
    self._filenames = [
	"/etc/%s/%s.conf" % (self._name, self._name),
	"%s/.%s.conf" % (home, self._name),
	"%s/.%src" % (home, self._name),
	"%s/.%s" % (home, self._name),
	]
    self._namespace = {}

  def load(self):
    import os.path
    for name in self._filenames:
      if os.path.exists(name):
        self.parse(name)

    for k in self._namespace.keys():
      if k[0] != "_":
	setattr(self, k, self._namespace[k])

  def parse(self, filename):
    #print "parsing %s..." % filename
    try:
      execfile(filename, self._namespace)
    except:
      print "Error in config file %s" % filename
      import traceback, sys
      foo = traceback.format_exception(sys.exc_type,
                                       sys.exc_value,
                                       sys.exc_traceback)
      for item in foo:
        item = item[:-1]
        print item
      print "Press Enter to continue..."
      raw_input()

  def show(self):
    for k in self._namespace.keys():
      if k[0] != "_":
	print "%s: %s" % (k, self._namespace[k])

############################################################
# Start the adserv..  define channels and triggers...
############################################################
def run():
  cfg = config(program_name)
  cfg.load()
  a = adserv(cfg)

  adtext = "%s%s - %s %s" % (cfg.color1, cfg.adtext, __module_name__, __module_version__)
  adtext = adtext.replace("[", "%s[%s" % (cfg.color2, cfg.color3))
  adtext = adtext.replace("]", "%s]%s" % (cfg.color2, cfg.color1))
  t1 = trigger("!list", adtext)
  t2 = trigger("!list %s" % (cfg.mynick), adtext)

  for chan in cfg.listen_channels:
    c = channel(chan)
    c.add_trigger(t1)
    c.add_trigger(t2)
    a.add_channel(c)

  a.run()


run()
print "%s %s initializing...  Done." % (__module_name__, __module_version__)


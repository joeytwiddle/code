# XChat notification area icon plugin 
# Copyright (C) 2005 Ross Burton <ross@burtonini.com> 
# 
#   This library is free software; you can redistribute it and/or modify it 
#   under the terms of the GNU Lesser General Public License as published by the 
#   Free Software Foundation; either version 2 of the License, or (at your 
#   option) any later version. 
# 
#   This library is distributed in the hope that it will be useful, but WITHOUT 
#   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
#   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License 
#   for more details. 
# 
# Magically shows an icon in the notification area when someone talks to you. 
# Note that the icon doesn't appear if the event happens in the active tab. This 
# may change in the future, I'm not sure. 
# 
# Contribution by Bas van der Lei <basvdlei@gmail.com> 
# * decent icon, use the xchat theme icon 
# * tooltips describing happened 
 
__module_name__ = "notify" 
__module_version__ = "0.2" 
__module_description__ = "Notification area integration plugin" 
 
import xchat 
xchat.prnt(__module_description__ + " loaded") 
 
import pygtk; pygtk.require("2.0") 
import gtk, egg.trayicon 
 
icon = egg.trayicon.TrayIcon("XChat") 
image = gtk.Image() 
image.set_from_icon_name("xchat", gtk.ICON_SIZE_SMALL_TOOLBAR) 
icon.add(image) 
tips = gtk.Tooltips() 
#gtk.icon_theme_get_default().lookup_icon("stock_dialog-error", 24, 0) 
 
focus_context = None 
notify_context = None 
 
def same_context(a, b): 
    if a is None or b is None: return False 
    return a.get_info("host") == b.get_info("host") and a.get_info("channel") == b.get_info("channel") 
 
def focus_cb(word, word_eol, userdata): 
    global focus_context, notify_context 
    focus_context = xchat.get_context() 
    if same_context (notify_context, focus_context): 
        icon.hide() 
        tips.disable() 
    return xchat.EAT_NONE 
 
def highlight_cb(word, word_eol, userdata): 
    global focus_context, notify_context 
    if not same_context(focus_context, xchat.get_context()): 
        tips.set_tip(icon, ("[%s]: %s"%(word[0], word[1]))[0:79]) 
        tips.enable() 
        icon.show_all() 
        notify_context = xchat.get_context() 
    return xchat.EAT_NONE 
 
def private_cb(word, word_eol, userdata): 
    global focus_context, notify_context 
    if not same_context(focus_context, xchat.get_context()): 
        tips.set_tip(icon, ("[%s]: %s"%(word[0], word[1]))[0:79]) 
        tips.enable() 
        icon.show_all() 
        notify_context = xchat.get_context() 
    return xchat.EAT_NONE 
 
xchat.hook_print("Focus Tab", focus_cb) 
xchat.hook_print("Channel Action Hilight", highlight_cb) 
xchat.hook_print("Channel Msg Hilight", highlight_cb) 
xchat.hook_print("Private Message", private_cb) 
xchat.hook_print("Private Message to Dialog", private_cb)

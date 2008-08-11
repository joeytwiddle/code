package org.neuralyte.irc;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.sql.Date;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;
import java.util.Map.Entry;

import org.jibble.pircbot.Colors;
import org.jibble.pircbot.User;
import org.omg.CORBA.ShortSeqHelper;
import org.omg.PortableServer.Servant;

/**
 * Commands accepted by IRCBot (but only by private message):
 * !stdout - Globally toggle LogBot logging to standard out.
 * !logall - Globally toggle Pirc default logging.
 * !status - Sends back a summary of ALL the bots.
 */

/**
 * TODO: support for joining alternative servers if the main one is not reachable.
 */

public class IRCBot extends LogBot {

    // final File userHome = new File(java.util.prefs.Preferences.userRoot().absolutePath());
    public final static File userHome = new File(System.getProperty("user.home"));

    public static File prefsDir = new File(userHome+"/.ircbot");
    public static File logDir = new File(userHome+"/.xchat2.utb0t/logs");

    public static boolean logAll = false;
    public static float floodDelay = 3.0f; // 4.5f;
    
    public static java.util.List<IRCBot> allBots = new Vector();

    public String pluginDir = userHome+"/.xchat2.utb0t/plugin";

    // Per bot:
    
    public String server = "pictor.vm.bytemark.co.uk";
    // public String server = "irc.quakenet.org";
    
    final File configDir,channelsFile,performFile;
    
    public static void main(String[] args) {
        
        /*
        IRCBot bot = new IRCBot("utb0t");
        bot.connect();
        */
        
        /*
        IRCBot bot2 = new IRCBot("utbot");
        bot2.connect();
        
        IRCBot bot3 = new IRCBot("ut99bot");
        bot3.connect();
        */

        //// TODO PROBLEM: bot4 will try to use the same channels file as bot1
        // IRCBot bot4 = new IRCBot("utb0t");
        // bot4.server = "irc.utchat.com";
        // bot4.connect();
    
        for (final File file : prefsDir.listFiles()) {
            if (file.isDirectory() && file.getName().contains("-")) {
                // During startup, if the first bot cannot connect (e.g. server down or "Your username is invalid",
                // then loadBot() never returns.  So to ensure the later bots will definitely also be started,
                // we call loadBot() in a separate thread (but wait 10 seconds before going on to start the next bot).
                new Thread() {
                    public void run() {
                        loadBot(file);
                    }
                }.start();
                justSleep(10.0);
            }
        }
        
    }
    
    static void loadBot(File configDir) {
        final String server = configDir.getName().replaceAll("-.*", "");
        final String botName = configDir.getName().replaceAll("^[^-]*-", "");
        final IRCBot bot = new IRCBot(configDir,botName,server);
        allBots.add(bot);
        bot.doConnect();
    }

    /*
    public IRCBot() {
        this("IRCBot",defaultServer);
    }
    */
    
    public IRCBot(File configDir, String name, String server) {
        super(name, logDir);
        setName(name);
        setFinger("whoAreYou");
        setVersion("utb0t-O.o");
        setLogin(""+(char)((int)'a' + 26*Math.random())+(int)(Math.random()*10000));
        setAutoNickChange(true);
        this.configDir = configDir;
        this.channelsFile = new File(configDir+"/channels");
        this.performFile = new File(configDir+"/perform");
        this.server = server;
    }
    
    private void doConnect() {
        setVerbose(true);
        int sleepTime = 30;
        while (true) {
            try {
                connect(server);
                break;
            } catch (final Exception e) {
                e.printStackTrace(System.err);
                mylog("Sleeping for "+sleepTime+" seconds.");
                justSleep(sleepTime);
                sleepTime = sleepTime * 2;
                /*
                if (e instanceof org.jibble.pircbot.NickAlreadyInUseException) {
                    String[] alternativeNames = { "utb0t", "utb1t", "ut99bot", "ut100bot", "ut1bot" };
                    String newName = alternativeNames[ (int)(Math.random()*alternativeNames.length) ];
                    setName(newName);
                }
                */
            }
        }
        // joinChannel("#testing");
        doDefaultPerform();
        joinDefaultChannels();
        setVerbose(false);
    }
    
    void doDefaultPerform() {
        if (performFile.exists()) {
            final String[] commands = readLinesFromFile(performFile);
            for (int i=0;i<commands.length;i++) {
                String command = commands[i];
                if (command.startsWith("/"))
                    command = command.substring(1);
                command = command.replaceAll("\\$me", getNick());
                // sendRawLineViaQueue(command);
                floodProtect();
                sendSlashAction("", "/"+command);
                justSleep(3.0);
            }
        } else {
            mylog("No perform file: "+performFile);
        }
    }
    
    void joinDefaultChannels() {
        if (channelsFile.exists()) {
            final String[] channels = readLinesFromFile(channelsFile);
            for (int i=0;i<channels.length;i++) {
                // floodProtect();
                joinChannel(channels[i]);
            }
        } else {
            mylog("No channels file: "+channelsFile);
        }
    }
    
    public static String[] readLinesFromFile(File file) {
        try {
            final Vector<String> result = new Vector<String>();
            final BufferedReader in = new BufferedReader(new FileReader(file));
            String line;
            while (true) {
                line = in.readLine();
                if (line == null)
                    break;
                result.add(line);
            }
            return result.toArray(new String[0]);
        } catch (final Exception e) {
            e.printStackTrace(System.err);
            return null;
        }
    }
    
    @Override
    public void onMessage(String channel, String sender, String login, String hostname, String message) {
        super.onMessage(channel, sender, login, hostname, message);
        checkMessage(channel, sender, message, login, hostname);
    }

    @Override
    public void onPrivateMessage(String sender, String login, String hostname, String message) {

        if (message.equals("!logall")) {
            logAll = !logAll;
            sendNotice(sender, "logAll = "+logAll);
        }
        
        if (message.equals("!status")) {
            for (IRCBot b : allBots) {
                String botReport = b.getBotReport();
                floodProtect();
                sendNotice(sender, botReport);
            }
        }
        
        if (message.equals("!flooddelay")) {
            sendNotice(sender, "Current flood delay = " + floodDelay);
        }
        if (message.startsWith("!flooddelay ")) {
            floodDelay = new Float(message.substring(message.indexOf(" ")+1));
            sendNotice(sender, "New flood delay = " + floodDelay);
        }
        
        super.onPrivateMessage(sender, login, hostname, message);
        checkMessage(sender, sender, message, login, hostname);
    }

    /** @param channel may be a channel or a nick **/
    private void checkMessage(final String channel, final String sender, final String message, final String login, final String hostname) {
        
        char firstChar = ' ';
        if (message.length()>0)
            firstChar = message.charAt(0);
        
        if (firstChar=='!' || firstChar=='.') {

            mylog("Caught: ["+channel+"] <"+sender+"> "+message);

            // We do this in a new thread because if we wait for an auth, then we must unblock, or we won't get it!
            new Thread() {
                public void run() {
                    runShellCommand(channel, sender, message, login, hostname);
                }
            }.start();
            
        } else {
            // Ignore this line
            // mylog("Ignoring: ["+source+"] <"+sender+"> "+message);
        }
    }

    private void runShellCommand(final String channel, String sender, String message, String login, String hostname) {
        try {

            final String command = "/bin/bash /home/joey/j/jsh "+pluginDir+"/xchat_sh_handler.sh "+message.substring(1);
            // @todo Instead of server, we could try getServer().
            final File topDirFile = new File(pluginDir);
            
            // mylog("-sh- Calling: "+command);

            //// Pass environment variables in one big String:
            // command = "env NETWORK="+server+" env SERVER="+server+" env NICK="+ sender + " env CHANNEL="+channel+" " + command;
            // final Process process = Runtime.getRuntime().exec(command, null, topDirFile);
            
            //// Add environment variables to map, then collect as String[].
            // //We cannot modify System.getEnv(), so we create a new map.
            Map<String, String> envMap = new HashMap<String, String>( System.getenv() ); 
            envMap.put("NETWORK",server);
            envMap.put("SERVER",server);
            envMap.put("NICK",sender);
            envMap.put("CHANNEL",channel);
            envMap.put("LOGIN",login);
            envMap.put("HOST",hostname);
            envMap.put("MYNICK",getNick());
            envMap.put("AUTH",getAuth(server,sender /*,login,hostname*/));
            envMap.put("USERS",getUsersString(channel,false));
            envMap.put("USERMODES",getUsersString(channel,true));
            User senderU = getUser(channel,sender);
            envMap.put("HASOP",(senderU != null && senderU.isOp()?"@":""));
            envMap.put("HASVOICE",(senderU != null && senderU.hasVoice()?"+":""));
            // Iterator<Entry<String, String>> envVars = ;
            Vector<String> outList = new Vector<String>();
            for (Entry<String,String> entry : envMap.entrySet()) {
                outList.add(entry.getKey() + "=" + entry.getValue());
            }
            String[] env = (String[])outList.toArray(new String[0]);
            
            mylog("Calling: " + command);

            final Process process = Runtime.getRuntime().exec(command, env, topDirFile);

            /** @todo WARNING possible BUG:
             * I seem to remember having to fix this for FuseJshFS.
             * If the process sends alot to stderr, the stderr buffer can fill, and the process will block.
             * In this case, nothing will ever come through stdout, and we won't reach the end of stdout either, we'll be stuck here (and won't even see the errors from stderr).
             * The solution is to read each of the response streams in a separate thread.
             */

            final LogBot me = this;
            
            final Thread stdoutThread = new Thread() {
                @Override
                public void run() {

                    try {

                    BufferedReader in = new BufferedReader(new InputStreamReader(process.getInputStream()));
                    String line;
                    while (true) {
                        line = in.readLine();
                        if (line == null)
                            break;
                        floodProtect();
                        if (line.startsWith("/")) {
                            sendSlashAction(channel, line);
                        } else {
                            mylog(">>>> "+channel+" :: "+line);
                            // We want to call our LogBot.onMessage(), not our IRCBot.onMessage(), and we are inside an anonymous class!
                            IRCBot.super.onMessage(channel, getNick(), getLogin(), "hostname123", line);
                            sendMessage(channel, line);
                        }
                    }
                    
                    mylog("Completed: "+command);
                    
                    } catch (Exception e) {
                        e.printStackTrace(System.err);
                    }

                }
            };
            
            final Thread stderrThread = new Thread() {
                @Override
                public void run() {

                    try {

                        BufferedReader inErr = new BufferedReader(new InputStreamReader(process.getErrorStream()));
                        String line;
                        int n = 0;
                        while (true) {
                            line = inErr.readLine();
                            if (line == null)
                                break;
                            mylog(""+(n>0?"!!!!":"....")+" "+line); // @todo Consider sending this to stderr
                            n++;
                        }

                    } catch (Exception e) {
                        e.printStackTrace(System.err);
                    }

                }
            };
            
            stdoutThread.start();
            stderrThread.start();
            
            // stdoutThread.wait();
            // stderrThread.wait();
            //// We could try process.wait();

        } catch (final Exception e) {
            e.printStackTrace(System.err);
        }
    }
    
    public String getUsersString(String channel, boolean withModes) {
        if (!channel.startsWith(""))
            return "";
        User[] users = getUsers(channel);
        String out = "";
        for (int i=0;i<users.length;i++) {
            String toAdd = users[i].getNick();
            if (withModes) {
              if (users[i].isOp())
                  toAdd = "@"+toAdd;
              else if (users[i].hasVoice())
                  toAdd = "+"+toAdd;
            }
            out += toAdd;
            if (i < users.length-1)
                out += " ";
        }
        return out;
    }

    public User getUser(String channel, String nick) {
        if (!channel.startsWith(""))
            return null;
        User[] users = getUsers(channel);
        for (int i=0;i<users.length;i++) {
            if (users[i].getNick().equalsIgnoreCase(nick))
                return users[i];
        }
        return null;
    }

    public String getBotReport() {
        String channelReport = getChannels().length + " channels on " + getServer() + ": ";
        boolean first = true;
        for (String c : getChannels()) {
            if (!first)
                channelReport += " :: ";
            first = false;
            channelReport += "[" + c +"] "+getUsers(c).length+" users";
        }
        channelReport = channelReport.substring(0,channelReport.length());

        java.util.List<String> newChannels = new java.util.Vector<String>();
        newChannels.addAll(Arrays.asList(getChannels()));
        String[] configChannels = readLinesFromFile(channelsFile);
        newChannels.removeAll(Arrays.asList(configChannels));
        for (String l : readLinesFromFile(performFile)) {
            String[] words = l.split(" ");
            newChannels.removeAll(Arrays.asList(words));
        }
        if (newChannels.size()>0)
            channelReport += " // New channels are: "+newChannels;

        return "{"+getName()+"} " + channelReport;
    }
    
    @Override
    public void onDisconnect() {
        // super.onDisconnect();
        mylog("Disconnected!  Sleeping for 60 seconds.");
        justSleep(60.0);
        doConnect();
    }
    
    private void sendSlashAction(String source, String line) {
        mylog(":::: "+line);
        final String com = line.replaceAll(" .*","").toLowerCase();
        String rest = line.replaceAll("^[^ ]* ",""); 
        final String firstArg = rest.replaceAll(" .*","");
        if (rest.indexOf(' ')>=0) {
            rest = rest.replaceAll("^[^ ]* ","");
        } else {
            rest = "";
        }
        if (com.equals("/mode")) {
            final String channel = firstArg;
            final String mode = rest;
            setMode(channel, mode);
        } else if (com.equals("/join")) {
            final String channel= firstArg;
            final String key = rest;
            if (key == "")
                joinChannel(channel);
            else
                joinChannel(channel,key);
        } else if (com.equals("/notice")) {
            final String target = firstArg;
            final String message = rest;
            super.onNotice(getNick(), getLogin(), "hostname123", target, message); // For LogBot
            sendNotice(target,message);
        } else if (com.equals("/msg")) {
            final String target = firstArg;
            final String message = rest;
            super.onMessage(target, getNick(), getLogin(), "hostname123", message); // For LogBot 
            sendMessage(target,message);
        } else if (com.equals("/me")) {
            final String target = source;
            final String message = firstArg + " " + rest;
            super.onAction(getNick(), getLogin(), "hostname123", target, message); // For LogBot 
            sendAction(target,message);
        } else if (com.equals("/invite")) { // Maybe worked before anyway
            final String nick = firstArg;
            final String channel = rest;
            super.onInvite(nick, getNick(), getLogin(), "hostname123", channel); // For LogBot
            sendInvite(nick, channel);
        } else if (com.equals("/voice")) {
            // Actually I found if I tried to voice more than one person at a time, I would end up changing various channel modes (depending on the chars in the nicks being "voiced").  So now I voice users one on each line.  ;p
            super.onVoice(source, getNick(), getLogin(), "hostname123", firstArg+" "+rest);
            voice(source, firstArg + " " + rest);
        } else {
            // We warn that this command might not work raw:
            mylog("XXXX "+line);
            //// Although in fact /WHOIS /PART and /NICK work fine this way.
            //// TODO: But really we should use:
            // changeNick(newNick);
            if (source.equals(""))
                source = DEFAULT_TARGET;
            
            super.append(BRICK, source, "--- " + getNick()+ " does "+line); // For LogBot
            sendRawLineViaQueue(line.substring(1));
        }
    }

    
    
    // statics make it work across all bots at once (maybe not desirable),
    // but also (more importantly) across different threads of one bot.
    // TODO: however, it doesn't check the *first* response it sends, so if the bot is
    // flooded with requests, it may still flood responses!
    // One easy way to solve this, is to put all flood checking just before instead of just after the call to send each message.
    static volatile int floodCount = 0;
    static volatile long lastFloodTime = 0;
    final static String FLOOD_LOCK = "FLOOD_LOCK";
    /** floodProtect() should be called once before every message is sent. **/
    void floodProtect() {
        if (new java.util.Date().getTime() > lastFloodTime + 10000) // we guess we are marked as non-flooding now, so can go fast again
            floodCount=0;
        /* if (floodCount>3) {
            // mylog("[flood protection!] sleeping 5 seconds");
            justSleep(floodDelay); // 5.0 was fine, testing 4.5 now
        }*/
        while (true) {
            synchronized (FLOOD_LOCK) {
                // mylog("Count = "+floodCount+" and Time since last action = " + (new java.util.Date().getTime() - lastFloodTime)/1000f + "s");
                // if (floodCount<3 || new java.util.Date().getTime() >= lastFloodTime+floodDelay*1000) { // For some reason this always succeeded! O_o
                if (floodCount<3 || (new java.util.Date().getTime() - lastFloodTime)/1000f > floodDelay) {
                    floodCount++;
                    lastFloodTime = new java.util.Date().getTime();
                    // mylog("Unblocking!");
                    break;
                }
            }
            justSleep(0.5);
        }
    }

    
    
    public void mylog(String txt) {
        txt = Colors.removeFormattingAndColors(txt);
        final String whereStr = ( getWhereStr().equals("log") ? "." : "#" ); // getWhereStr();
        final String dateStr = new java.text.SimpleDateFormat("kk:mm:ss").format(new java.util.Date());
        final String[] serverBits = getServer().split("\\.");
        String shortServer = ""; 
        for (String serverBit : serverBits) {
            if (!serverBit.equals("irc")) {
                shortServer = serverBit; break; 
            }
        }
        final String threadStr = padLeft(""+Thread.currentThread().getId(),'0',3);
        final String who = getName() + "@" + shortServer + ":" + threadStr;
        final String output = dateStr+" ["+who+"] "+ whereStr+" "+ txt;
        System.out.println(output);
    }

    public static int longestWhereStr = 0; 
    private String getWhereStr() {
        final StackTraceElement where = new Throwable().getStackTrace()[2];
        // String whereStr = where.getClassName().replaceAll(".*\\.","")+"."+where.getMethodName()+"("+ /* where.getFileName()+":"+where.getLineNumber()+ */ ")";
        final String whereStr = where.getMethodName();
//        if (whereStr.length() > longestWhereStr)
//            longestWhereStr = whereStr.length();
//        whereStr = padLeft(whereStr,longestWhereStr);
        return whereStr;
    }
    
    @Override
    public void log(String line) {
        // Stop Pirc's normal logging once we are actually connected, unless we have logAll set.
        if (getChannels().length>0 && !logAll)
            return;
        // NEVER log ping/pong lines.
        if (line.startsWith("PING") || line.startsWith(">>>PONG") || line.startsWith(">>>PING") || line.contains(" PONG "))
            return;
        mylog(line);
    }
    
    
    
    public static void justSleep(double seconds) {
        try { Thread.sleep( (int)(1000*seconds) ); } catch (final Exception e) { }
    }

    
    
    static final Map<String,String> authDB = new Hashtable();
    static final Map<String,Long> authDateDB = new Hashtable();
    
    public void clearOldAuthData() {
        while ( removeOneOldAuthRecord() ) {
        }
    }

    private boolean removeOneOldAuthRecord() {
        // We want to remove out-of-date auths for security.
        // Although hopefully these will be caught by onNickChange() onQuit() and onPart().
        // But we also need to remove old cached auths now and then, just to free up memory.
        long timeNow = new java.util.Date().getTime();
        for (String key : authDateDB.keySet()) {
            Long recordedDate = authDateDB.get(key);
            String recordedAuth = authDB.get(key);
            long age = timeNow - recordedDate;
            if (age > 1000 * 60 * 50 // Auth data remains associated with nick for max 50 minutes.  This should be secure, since we intercept messages about the user changing nick or leaving.
                || ( recordedAuth.equals("") && age > 1000 * 60 * 2) // User having no auth is cached for max 2 minutes.
            ) {
                authDB.remove(key);
                authDateDB.remove(key);
                return true; // We go to the top of the while loop; this should avoid any problems with removing whilst iterating.
            }
        }
        return false; // No more need to be removed.
    }

    public String getAuth(String server, String sender /*, String login, String hostname*/) {
        clearOldAuthData();
        // String key = server + ":" + login + "@" + hostname;
        String key = server + ":" + sender;
        String result = authDB.get(key);
        if (result == null) {
            sendSlashAction("", "/whois "+sender);
            mylog("Waiting for auth for "+sender);
            for (int i=0;i<10 && result==null;i++) {
                justSleep(1.0);
                result = authDB.get(key);
            }
            mylog("Got auth for "+sender+": "+result);
        }
        if (result == null)
            result = "";
        return result;
    }

    @Override
    public void onNickChange(String oldNick, String login, String hostname, String newNick) {
        super.onNickChange(oldNick, login, hostname, newNick);
        String key = server + ":" + oldNick;
        authDB.remove(key);
        authDateDB.remove(key);
        // @todo: If they did have an auth with the old nick, add it to the DB with their new nick.
    }
    
    @Override
    public void onQuit(String sourceNick, String sourceLogin, String sourceHostname, String reason) {
        super.onQuit(sourceNick, sourceLogin, sourceHostname, reason);
        String key = server + ":" + sourceNick;
        authDB.remove(key);
        authDateDB.remove(key);
    }

    
    
    @Override
    public void onPart(String channel, String sender, String login, String hostname) {
        super.onPart(channel, sender, login, hostname);
        // If this is the last channel we know them on, then we will probably lose track of them now,
        // so we should remove their cached auth data.
        String key = server + ":" + sender;
        authDB.remove(key);
        authDateDB.remove(key);
    }

    @Override
    protected void onServerResponse(int code, String response) {
        super.onServerResponse(code, response);
        if (code == 330 && response.endsWith(" :is authed as")) {
            String[] split = response.split(" ");
            String nick = split[1];
            String auth = split[2];
            String key = server + ":" + nick;
            authDB.put(key,auth);
            authDateDB.put(key,new java.util.Date().getTime());
            mylog("Received auth for "+nick+": "+auth);
        } else if (code == 318 && response.endsWith(" :End of /WHOIS list.")) {
            // We have reached end of whois list.
            String[] split = response.split(" ");
            String nick = split[1];
            String key = server + ":" + nick;
            if (authDB.get(key) == null) {
                // And we still don't have their auth, so they must be not authed.
                // We give them AUTH="", so that getAuth() will stop waiting for their auth.
                authDB.put(key, "");
                authDateDB.put(key,new java.util.Date().getTime());
                mylog("Received response: "+nick+" is not authed.");
            }
        }
    }

}

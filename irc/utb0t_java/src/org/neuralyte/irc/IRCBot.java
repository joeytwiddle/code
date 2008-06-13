package org.neuralyte.irc;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.util.Vector;

import org.jibble.pircbot.Colors;
import org.jibble.pircbot.PircBot;

public class IRCBot extends LogBot {

    // final File userHome = new File(java.util.prefs.Preferences.userRoot().absolutePath());
    public final static File userHome = new File(System.getProperty("user.home"));

    public static File prefsDir = new File(userHome+"/.ircbot");
    public static File logDir = new File(userHome+"/.xchat2.utb0t/logs");

    public String pluginDir = userHome+"/.xchat2.utb0t/plugin";
    
    public String server = "pictor.vm.bytemark.co.uk";
    // public String server = "irc.quakenet.org";

    File configDir;
    
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
    
        for (File file : prefsDir.listFiles()) {
            if (file.isDirectory() && file.getName().contains("-")) {
                loadBot(file);
            }
        }
        
    }
    
    static void loadBot(File configDir) {
        String server = configDir.getName().replaceAll("-.*", "");
        String botName = configDir.getName().replaceAll("^[^-]*-", "");
        IRCBot bot = new IRCBot(configDir,botName,server);
        bot.doConnect();
    }

    /*
    public IRCBot() {
        this("IRCBot",defaultServer);
    }
    */
    
    public IRCBot(File configDir, String name, String server) {
        super(name, logDir, "");
        setName(name);
        setAutoNickChange(true);
        this.configDir = configDir;
        this.server = server;
    }
    
    private void doConnect() {
        setVerbose(true);
        int sleepTime = 30;
        while (true) {
            try {
                connect(server);
                break;
            } catch (Exception e) {
                e.printStackTrace(System.err);
                mylog("Sleeping for "+sleepTime+" seconds.");
                justSleep((double)sleepTime);
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
        File performFile = new File(configDir+"/perform");
        if (performFile.exists()) {
            String[] commands = readLinesFromFile(performFile);
            for (int i=0;i<commands.length;i++) {
                String command = commands[i];
                if (command.startsWith("/"))
                    command = command.substring(1);
                command = command.replaceAll("\\$me", getNick());
                // sendRawLineViaQueue(command);
                sendSlashAction("", "/"+command);
                justSleep(3.0);
                floodProtect();
            }
        } else {
            mylog("No perform file: "+performFile);
        }
    }
    
    void joinDefaultChannels() {
        File channelsFile = new File(configDir+"/channels");
        if (channelsFile.exists()) {
            String[] channels = readLinesFromFile(channelsFile);
            for (int i=0;i<channels.length;i++) {
                joinChannel(channels[i]);
                // floodProtect();
            }
        } else {
            mylog("No channels file: "+channelsFile);
        }
    }
    
    public static String[] readLinesFromFile(File file) {
        try {
            Vector<String> result = new Vector<String>();
            BufferedReader in = new BufferedReader(new FileReader(file));
            String line;
            while (true) {
                line = in.readLine();
                if (line == null)
                    break;
                result.add(line);
            }
            return (String[])result.toArray(new String[0]);
        } catch (Exception e) {
            e.printStackTrace(System.err);
            return null;
        }
    }
    
    public void onMessage(String channel, String sender, String login, String hostname, String message) {
        super.onMessage(channel, sender, login, hostname, message);
        checkMessage(channel, sender, message);
    }

    public void onPrivateMessage(String sender, String login, String hostname, String message) {
        super.onPrivateMessage(sender, login, hostname, message);
        checkMessage(sender, sender, message);
    }

    /** @param channel may be a channel or a nick **/
    private void checkMessage(final String channel, String sender, String message) {
        
        char firstChar = ' ';
        if (message.length()>0)
            firstChar = message.charAt(0);
        
        if (firstChar=='!' || firstChar=='.') {

            mylog("Caught: ["+channel+"] <"+sender+"> "+message);

            try {

                String command = "/bin/bash /home/joey/j/jsh "+pluginDir+"/xchat_sh_handler.sh "+message.substring(1);
                command = "env NETWORK="+server+" env SERVER="+server+" env NICK="+ sender + " env CHANNEL="+channel+" " + command;
                // @todo Instead of server, we could try getServer().
                File topDirFile = new File(pluginDir);
                
                // mylog("-sh- Calling: "+command);
                
                final Process process = Runtime.getRuntime().exec(command, null, topDirFile);

                /** @todo WARNING possible BUG:
                 * I seem to remember having to fix this for FuseJshFS.
                 * If the process sends alot to stderr, the stderr buffer can fill, and the process will block.
                 * In this case, nothing will ever come through stdout, and we won't reach the end of stdout either, we'll be stuck here (and won't even see the errors from stderr).
                 * The solution is to read each of the response streams in a separate thread.
                 */

                final LogBot me = this;
                
                Thread stdoutThread = new Thread() {
                    public void run() {

                        try {

                        BufferedReader in = new BufferedReader(new InputStreamReader(process.getInputStream()));
                        String line;
                        while (true) {
                            line = in.readLine();
                            if (line == null)
                                break;
                            if (line.startsWith("/")) {
                                sendSlashAction(channel, line);
                            } else {
                                mylog(">>>> "+channel+" :: "+line);
                                // We want to call our LogBot.onMessage(), not our IRCBot.onMessage(), and we are inside an anonymous class!
                                IRCBot.super.onMessage(channel, getNick(), getLogin(), "hostname123", line);
                                sendMessage(channel, line);
                            }
                            floodProtect();
                        }
                        
                        } catch (Exception e) {
                            e.printStackTrace(System.err);
                        }

                    }
                };
                
                Thread stderrThread = new Thread() {
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

            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
            
        } else {
            // Ignore this line
            // mylog("Ignoring: ["+source+"] <"+sender+"> "+message);
        }
    }
    
    public void onDisconnect() {
        // super.onDisconnect();
        mylog("Disconnected!  Sleeping for 60 seconds.");
        justSleep(60.0);
        doConnect();
    }
    

    private void sendSlashAction(String source, String line) {
        mylog(":::: "+line);
        String com = line.replaceAll(" .*","").toLowerCase();
        String rest = line.replaceAll("^[^ ]* ",""); 
        String firstArg = rest.replaceAll(" .*","");
        rest = rest.replaceAll("^[^ ]* ",""); 
        if (com.equals("/mode")) {
            String channel= firstArg;
            String mode = rest;
            setMode(channel, mode);
        } else if (com.equals("/join")) {
            String channel= firstArg;
            String key = rest;
            if (key == "")
                joinChannel(channel);
            else
                joinChannel(channel,key);
        } else if (com.equals("/notice")) {
            String target = firstArg;
            String message = rest;
            super.onNotice(getNick(), getLogin(), "hostname123", target, message); // For LogBot
            sendNotice(target,message);
        } else if (com.equals("/msg")) {
            String target = firstArg;
            String message = rest;
            super.onMessage(target, getNick(), getLogin(), "hostname123", message); // For LogBot 
            sendMessage(target,message);
        } else if (com.equals("/me")) {
            String target = source;
            String message = firstArg + " " + rest;
            super.onAction(getNick(), getLogin(), "hostname123", target, message); // For LogBot 
            sendAction(target,message);
        } else if (com.equals("/invite")) { // Maybe worked before anyway
            String nick = firstArg;
            String channel = rest;
            super.onInvite(nick, getNick(), getLogin(), "hostname123", channel); // For LogBot
            sendInvite(nick, channel);
        } else {
            mylog("XXXX "+line);
            super.append(BRICK, source, "! " + getNick()+ " does "+line); // For LogBot
            sendRawLineViaQueue(line.substring(1));
        }
    }

    // statics make it work across all bots at once
    static int floodCount = 0;
    static long lastFloodTime = 0;
    void floodProtect() {
        long time = new java.util.Date().getTime();
        if (time > lastFloodTime + 10000)
            floodCount=0;
        if (floodCount>3) {
            // mylog("[flood protection!] sleeping 5 seconds");
            justSleep(5.0);
        }
        floodCount++;
        lastFloodTime = time;
    }
    
    public void mylog(String txt) {
        txt = Colors.removeFormattingAndColors(txt);
        String whereStr = ( getWhereStr().equals("log") ? "." : "#" ); // getWhereStr();
        String dateStr = new java.text.SimpleDateFormat("kk:mm:ss").format(new java.util.Date());;
        String output = dateStr+" ["+getName()+"] "+ whereStr+" "+ txt;
        System.out.println(output);
    }

    public static int longestWhereStr = 0; 
    private String getWhereStr() {
        StackTraceElement where = new Throwable().getStackTrace()[2];
        // String whereStr = where.getClassName().replaceAll(".*\\.","")+"."+where.getMethodName()+"("+ /* where.getFileName()+":"+where.getLineNumber()+ */ ")";
        String whereStr = where.getMethodName();
//        if (whereStr.length() > longestWhereStr)
//            longestWhereStr = whereStr.length();
//        whereStr = padLeft(whereStr,longestWhereStr);
        return whereStr;
    }
    
    public void log(String line) {
        if (getChannels().length>0) // Stop Pirc's normal logging once we are actually connected
            return;
        if (line.startsWith("PING") || line.startsWith(">>>PONG") || line.startsWith(">>>PING") || line.contains(" PONG "))
            return;
        mylog(line);
    }
    
    public void justSleep(double seconds) {
        try { Thread.sleep( (int)(1000*seconds) ); } catch (Exception e) { }
    }

}

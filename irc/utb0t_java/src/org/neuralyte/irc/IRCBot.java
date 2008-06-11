package org.neuralyte.irc;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.util.Vector;

import org.jibble.pircbot.PircBot;

import com.sun.java_cup.internal.emit;

public class IRCBot extends PircBot {

    // final File userHome = new File(java.util.prefs.Preferences.userRoot().absolutePath());
    final File userHome = new File(System.getProperty("user.home"));

    public File prefsDir = new File(userHome+"/.ircbot");
    public String pluginDir = userHome+"/.xchat2.utb0t/plugin";
    
    public String server = "pictor.vm.bytemark.co.uk";
    // public String server = "irc.quakenet.org";

    public static void main(String[] args) {
        
        IRCBot bot = new IRCBot("utb0t");
        bot.connect();
        
        IRCBot bot2 = new IRCBot("utbot");
        bot2.connect();
        
        IRCBot bot3 = new IRCBot("ut99bot");
        bot3.connect();

        //// TODO PROBLEM: bot4 will try to use the same channels file as bot1
        // IRCBot bot4 = new IRCBot("utb0t");
        // bot4.server = "irc.utchat.com";
        // bot4.connect();
        
    }

    public IRCBot() {
        setName("IRCBot");
    }
    
    public IRCBot(String name) {
        setName(name);
    }
    
    private void connect() {
        setVerbose(true);
        while (true) {
            try {
                connect(server);
                break;
            } catch (Exception e) {
                e.printStackTrace(System.err);
                mylog("Sleeping for 30 seconds.");
                try { Thread.currentThread().sleep(30); } catch (Exception e2) { }
                if (e instanceof org.jibble.pircbot.NickAlreadyInUseException) {
                    String[] alternativeNames = { "utb0t", "utb1t", "ut99bot", "ut100bot", "ut1bot" };
                    String newName = alternativeNames[ (int)(Math.random()*alternativeNames.length) ];
                    setName(newName);
                }
            }
        }
        // joinChannel("#testing");
        doDefaultPerform();
        joinDefaultChannels();
        setVerbose(false);
    }
    
    void doDefaultPerform() {
        File performFile = new File(prefsDir+"/"+getName()+".perform");
        if (performFile.exists()) {
            String[] commands = readLinesFromFile(performFile);
            for (int i=0;i<commands.length;i++) {
                String command = commands[i];
                if (command.startsWith("/"))
                    command = command.substring(1);
                // sendRawLineViaQueue(command);
                sendSlashAction("", "/"+command);
                // floodProtect();
            }
        } else {
            mylog("No perform file: "+performFile);
        }
    }
    
    void joinDefaultChannels() {
        File channelsFile = new File(prefsDir+"/"+getName()+".channels");
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
        checkMessage(channel, sender, message);
    }

    protected void onPrivateMessage(String sender, String login, String hostname, String message) {
        checkMessage(sender, sender, message);
    }

    /** @param source may be a channel or a nick **/
    private void checkMessage(final String source, String sender, String message) {
        
        char firstChar = ' ';
        if (message.length()>0)
            firstChar = message.charAt(0);
        
        if (firstChar=='!' || firstChar=='.') {

            mylog("Caught: ["+source+"] <"+sender+"> "+message);

            try {

                String command = "/bin/bash /home/joey/j/jsh "+pluginDir+"/xchat_sh_handler.sh "+message.substring(1);
                command = "env NETWORK="+server+" env SERVER="+server+" env NICK="+ sender + " env CHANNEL="+source+" " + command;
                File topDirFile = new File(pluginDir);
                
                mylog("Calling: "+command);
                
                final Process process = Runtime.getRuntime().exec(command, null, topDirFile);

                /** @todo WARNING possible BUG:
                 * I seem to remember having to fix this for FuseJshFS.
                 * If the process sends alot to stderr, the stderr buffer can fill, and the process will block.
                 * In this case, nothing will ever come through stdout, and we won't reach the end of stdout either, we'll be stuck here (and won't even see the errors from stderr).
                 * The solution is to read each of the response streams in a separate thread.
                 */
                
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
                                sendSlashAction(source, line);
                            } else {
                                mylog("  >> "+source+" >> "+line);
                                sendMessage(source, line);
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
                                mylog((n>0?" [!!!] ":"")+line);
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
    
    protected void onDisconnect() {
        mylog("Disconnected!  Sleeping for 60 seconds.");
        try { Thread.currentThread().sleep(60); } catch (Exception e2) { }
        connect();
    }
    

    private void sendSlashAction(String source, String line) {
        mylog("  :: "+line);
        String com = line.replaceAll(" .*","").toLowerCase();
        String rest = line.replaceAll("^[^ ]* ",""); 
        String firstArg = rest.replaceAll(" .*","");
        rest = rest.replaceAll("^[^ ]* ",""); 
        if (com.equals("/mode")) {
            String channel= firstArg; String mode = rest;
            setMode(channel, mode);
        } else if (com.equals("/notice")) {
            String target = firstArg; String message = rest;
            sendNotice(target,message);
        } else if (com.equals("/msg")) {
            String target = firstArg; String message = rest;
            sendMessage(target,message);
        } else if (com.equals("/me")) {
            String message = firstArg + " " + rest;
            sendAction(source,message);
        } else {
            mylog(" [XXX] Trying unknown: "+line);
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
            try { Thread.currentThread().sleep(4000); } catch (Exception e) { } // todo: test 3.5 and 3.0 on quakenet
        }
        floodCount++;
        lastFloodTime = time;
    }
    
    public static int longestWhereStr = 0; 
    public void mylog(String txt) {
//        StackTraceElement where = new Throwable().getStackTrace()[1];
//        String whereStr = where.getClassName().replaceAll(".*\\.","")+"."+where.getMethodName()+"("+ /* where.getFileName()+":"+where.getLineNumber()+ */ ")";
//        if (whereStr.length() > longestWhereStr)
//            longestWhereStr = whereStr.length();
//        whereStr = padLeft(whereStr,longestWhereStr);
        String dateStr = new java.text.SimpleDateFormat("kk:mm").format(new java.util.Date());;
        String output = dateStr+" ["+getName()+"] "+ /* whereStr+" "+ */ txt;
        System.out.println(output);
    }
    
    public static String padLeft(String str, int desiredLength) {
        if (desiredLength > str.length()) {
            StringBuffer left = new StringBuffer();
            for (int i=0;i<desiredLength - str.length();i++) {
                left.append(' ');
            }
            return left + str;
        } else {
            return str;
        }
    }

}

package org.neuralyte.irc;

import java.util.*;
import java.util.regex.*;
import java.io.*;
import java.text.SimpleDateFormat;
import org.jibble.pircbot.*;

/**
 * BUGS:
 * - global events, such as nick changes, and players leaving the network, are not logged to all channels where they are present, as might be expected
 * - events sent by any subclasses of this class cannot be logged, since we cannot override sendMessage() etc. 
 * @author joey
 *
 */

public class LogBot extends PircBot {

    private static final Pattern urlPattern = Pattern.compile("(?i:\\b((http|https|ftp|irc)://[^\\s]+))");
    private static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd");
    private static final SimpleDateFormat TIME_FORMAT = new SimpleDateFormat("HH:mm:ss");
    
    public static final String GREEN = "irc-green";
    public static final String BLACK = "irc-black";
    public static final String BROWN = "irc-brown";
    public static final String NAVY = "irc-navy";
    public static final String BRICK = "irc-brick";
    public static final String RED = "irc-red";
    
    public static final String DEFAULT_TARGET = "NETWORK";
    
    public static boolean logToStdOut = false;
    
    public LogBot(String name, File outDir, String joinMessage) {
        setName(name);
        setVerbose(true);
        this.outDir = outDir;
        if (!outDir.exists()) {
            outDir.mkdirs();
        }
        this.joinMessage = joinMessage;
    }
    
    public void append(String color, String channel, String line) {
        line = Colors.removeFormattingAndColors(line);

        // Right justify nicks:
        if (line.startsWith("<")) {
            String nick = line.substring(0, line.indexOf(" "));
            if (nick.endsWith(">")) {
                String rest = line.substring(line.indexOf(" ")+1);
                line = rightJustifyNick(nick) + " " + rest;
            }
        }
        
        Date now = new Date();
        String date = DATE_FORMAT.format(now);
        String time = TIME_FORMAT.format(now);

        if (logToStdOut)
            System.out.println(date+" "+time+" ["+channel+"] "+line);

        // File file = new File(outDir, date + ".log");
        String fileName = getServer()+ "-" + channel + ".log";
        File file = new File(outDir, fileName);
        try {
            // BufferedWriter writer = getWriterForChannel(channel); 
            BufferedWriter writer = new BufferedWriter(new FileWriter(file, true));
            // String entry = "<span class=\"irc-date\">[" + time + "]</span> <span class=\"" + color + "\">" + line + "</span><br />";
            // String entry = "[" + time + "] ["+ channel +"] " + line;

            String entry = date + " " + time + " : "+ line;
            writer.write(entry);
            writer.newLine();
            writer.flush();
            writer.close();
        }
        catch (IOException e) {
            System.out.println("Could not write to log: " + e);
        }
    }
    
    public void onAction(String sender, String login, String hostname, String target, String action) {
        append(BRICK, target, "* " + sender + " " + action);
    }
    
    public void onJoin(String channel, String sender, String login, String hostname) {
        append(GREEN, channel, "* " + sender + " (" + login + "@" + hostname + ") has joined " + channel);
        /*
        if (sender.equals(getNick())) {
            sendNotice(channel, joinMessage);
        }
        else {
            sendNotice(sender, joinMessage);
        }
        */
    }
    
    public void onMessage(String channel, String sender, String login, String hostname, String message) {
        append(BLACK, channel, "<" + sender + "> " + message);
        
        if (message.equals("!togglestdout"))
            logToStdOut = ! logToStdOut;
        
        message = message.toLowerCase();
        if (message.startsWith(getNick().toLowerCase()) && message.indexOf("help") > 0) {
            sendMessage(channel, joinMessage);
        }
    }
    
    public void onMode(String channel, String sourceNick, String sourceLogin, String sourceHostname, String mode) {
        append(GREEN, channel, "* " + sourceNick + " sets mode " + mode);
    }
    
    public void onNickChange(String oldNick, String login, String hostname, String newNick) {
        append(GREEN, DEFAULT_TARGET, "* " + oldNick + " is now known as " + newNick);
    }
    
    public void onNotice(String sourceNick, String sourceLogin, String sourceHostname, String target, String notice) {
        append(BROWN, target, "-" + sourceNick + "- " + notice);
    }
    
    public void onPart(String channel, String sender, String login, String hostname) {
        append(GREEN, channel, "* " + sender + " (" + login + "@" + hostname + ") has left " + channel);
    }
    
    public void onPing(String sourceNick, String sourceLogin, String sourceHostname, String target, String pingValue) {
        append(RED, target, "[" + sourceNick + " PING]");
    }
    
    public void onPrivateMessage(String sender, String login, String hostname, String message) {
         append(BLACK, sender, "<- *" + sender + "* " + message);
    }
    
    public void onQuit(String sourceNick, String sourceLogin, String sourceHostname, String reason) {
        append(NAVY, DEFAULT_TARGET, "* " + sourceNick + " (" + sourceLogin + "@" + sourceHostname + ") Quit (" + reason + ")");
    }
    
    public void onTime(String sourceNick, String sourceLogin, String sourceHostname, String target) {
        append(RED, target, "[" + sourceNick + " TIME]");
    }
    
    public void onTopic(String channel, String topic, String setBy, long date, boolean changed) {
        if (changed) {
            append(GREEN, channel, "* " + setBy + " changes topic to '" + topic + "'");
        }
        else {
            append(GREEN, channel, "* Topic is '" + topic + "'");
            append(GREEN, channel, "* Set by " + setBy + " on " + new Date(date));
        }
    }
    
    public void onVersion(String sourceNick, String sourceLogin, String sourceHostname, String target) {
        append(RED, target, "[" + sourceNick + " VERSION]");
    }
    
    public void onKick(String channel, String kickerNick, String kickerLogin, String kickerHostname, String recipientNick, String reason) {
        append(GREEN, channel, "* " + recipientNick + " was kicked from " + channel + " by " + kickerNick);
        /*
        if (recipientNick.equalsIgnoreCase(getNick())) {
            joinChannel(channel);
        }
        */
    }
    
    public void onDisconnect() {
        append(NAVY, DEFAULT_TARGET, "* Disconnected.");
        /*
        while (!isConnected()) {
            try {
                reconnect();
            }
            catch (Exception e) {
                try {
                    Thread.sleep(10000);
                }
                catch (Exception anye) {
                    // Do nothing.
                }
            }
        }
        */
    }
    
    /*
    public static void copy(File source, File target) throws IOException {
        BufferedInputStream input = new BufferedInputStream(new FileInputStream(source));
        BufferedOutputStream output = new BufferedOutputStream(new FileOutputStream(target));
        int bytesRead = 0;
        byte[] buffer = new byte[1024];
        while ((bytesRead = input.read(buffer, 0, buffer.length)) != -1) {
            output.write(buffer, 0, bytesRead);
        }
        output.flush();
        output.close();
        input.close();
    }
    */
    
    private File outDir;
    private String joinMessage;

    public static int longestNick = 0; 
    private String rightJustifyNick(String nick) {
        if (nick.length() > longestNick)
            longestNick = nick.length();
        return padLeft(nick,longestNick);
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
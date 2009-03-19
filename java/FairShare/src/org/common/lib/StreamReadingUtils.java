package org.common.lib;

import java.io.IOException;
import java.io.InputStream;

import org.fairshare.Logger;


public class StreamReadingUtils {

    public static String readTo(InputStream in, String hit) throws IOException {
        StringBuffer got = new StringBuffer();
        StringBuffer check = new StringBuffer();
        while (true) {
            int i = in.read();
            if (i == -1)
                break;
            check.append(""+(char)i);
            if (check.length() > hit.length()) {
                char c = check.charAt(0);
                check.deleteCharAt(0);
                got.append(""+c);
            }
            if (check.toString().equals(hit.toString()))
                break;
        }
        return got.toString();
    }

    public static boolean readExpect(InputStream in, String expectStr) throws IOException {
        String before = readTo(in,expectStr);
        if (before.trim().length() > 0) {
            Logger.error("Was expecting >"+expectStr+"< but got >"+before+"<");
            return false;
        }
        return true;
    }

    /*
    public void readWhitespace(InputStream in) {
        while
    }
    */
    
    public static String readToken(InputStream in) throws IOException {
        StringBuffer token = new StringBuffer();
        boolean inWord = false;
        while (true) {
            int i = in.read();
            if (i == -1)
                break;
            char c = (char)i;
            if (c==' ' || c=='\t' || c=='\n' || c=='\r') {
                if (inWord) {
                    break;
                    // NOTE: We have read one token of the whitespace :P
                }
            } else {
                inWord = true;
                token.append(""+c);
            }
        }
        return token.toString();
    }

}

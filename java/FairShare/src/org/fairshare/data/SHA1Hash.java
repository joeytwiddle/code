package org.fairshare.data;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.UnsupportedEncodingException;
import java.security.DigestInputStream;
import java.security.DigestOutputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.Security;


public class SHA1Hash implements Hash {
    
    String hexValue;
    
    public SHA1Hash(String hashHexString) {
        this.hexValue = hashHexString;
    }
    
    public String toHex() {
        return hexValue;
    }
    
    public static String calculateSHA1(String inString) throws Exception {
        return calculateSHA1_s(inString);
    }
    
    public static String calculateSHA1_bad(String inString) throws Exception {
        // Security.addProvider(new BouncyCastleProvider());        
        byte[] input = inString.getBytes();
        // System.out.println("input     : " + new String(input));    
        MessageDigest hash = MessageDigest.getInstance("SHA1");
    
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(input);
        DigestInputStream digestInputStream = new DigestInputStream(byteArrayInputStream, hash);
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
    
        int ch;
        while ((ch = digestInputStream.read()) >= 0) {
          byteArrayOutputStream.write(ch);
        }
    
        byte[] newInput = byteArrayOutputStream.toByteArray();
        // System.out.println("in digest : " + new String(digestInputStream.getMessageDigest().digest()));
    
        byteArrayOutputStream = new ByteArrayOutputStream();
        DigestOutputStream digestOutputStream = new DigestOutputStream(byteArrayOutputStream, hash);
        digestOutputStream.write(newInput);
        digestOutputStream.close();
    
        // System.out.println("out digest: " + new String(digestOutputStream.getMessageDigest().digest()));
        return new String(digestOutputStream.getMessageDigest().digest());
      }
    
    public static String calculateSHA1_s(String text) 
    throws NoSuchAlgorithmException, UnsupportedEncodingException  {
        MessageDigest md;
        md = MessageDigest.getInstance("SHA-1");
        byte[] sha1hash = new byte[40];
        md.update(text.getBytes("iso-8859-1"), 0, text.length());
        sha1hash = md.digest();
        return convertToHex(sha1hash);
    }

    private static String convertToHex(byte[] data) {
        StringBuffer buf = new StringBuffer();
        for (int i = 0; i < data.length; i++) {
                int halfbyte = (data[i] >>> 4) & 0x0F;
                int two_halfs = 0;
                do {
                    if ((0 <= halfbyte) && (halfbyte <= 9))
                        buf.append((char) ('0' + halfbyte));
                    else
                        buf.append((char) ('a' + (halfbyte - 10)));
                    halfbyte = data[i] & 0x0F;
                } while(two_halfs++ < 1);
        }
        return buf.toString();
    }
 
}

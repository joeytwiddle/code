package org.neuralyte.superproxy.plugins.examples;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Vector;

import org.neuralyte.Logger;
import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.simpleserver.httpadapter.HttpRequestHandler;

/*
 *
 *
 * http://www.oxxus.net/help/string-bytebuffer-putting-string-bytebuffer/index.php
 *
 * hello guys, is there a way to get a String out of a ByteBuffer?
 *
 * sure.
 * there's a couple ways.
 *
 * Does toString not do the right thing?
 * (Genuine question. I don't know. It does for charBuffer)
 *
 * no
 *
 * dmlloyd, toString() method return the status, do I need to pass it to a CharBuffer and then user CharBuffer's toString method?
 *
 * ok
 *
 * no way to specify encoding
 *
 * Ah
 *
 * you can wrap it in a charbuffer, or you can get the bytes and use the string byte[] constructor
 * are you trying to get the whole buffer as a complete string? it won't be a partial buffer or anything like that?
 *
 * dmlloyd, I think i like more the second
 * dmlloyd, yes, the complete buffer, since then I need to parse it, and String methods are nice for what i need
 *
 * Surely the second is potentially very inefficient?
 *
 * it's inefficient either way
 * you have to copy
 * well, probably one extra copy for getting a byte array
 *
 * so it's better to use the charbuffer?
 *
 * 1,093 INFO [Server] JBoss (MX MicroKernel) [4.2.0.GA (build: SVNTag=JBoss_4_2_0_GA date=200705111440)] Started in
 *
 * boolean bResult = Boolean.parseBoolean(sBool); but it return me: The method parseBoolean (String) is undefined for the type Boolean.
 *
 * BEWARE, I LIVE!
 *
 * ~tell P4C0 about javadoc CharsetDecoder
 *
 * P4C0, please see java.nio.charset.CharsetDecoder: http://java.sun.com/javase/6/docs/api/java/nio/charset/CharsetDecoder.html
 *
 * that's the "right" way
 *
 * That's what I meant (about the extra copy)
 *
 * I must first declare Boolean as null next insert method parseBoolean ?
 *
 * Actually, more serious problem. You can't always get an array out of a buffer.
 *
 * the CharsetDecoder can also handle partial characters
 * indeed
 *
 * dmlloyd, thanks
 *
 * you don't want to use "asCharBuffer" because it just converts pairs of bytes to chars
 *
 * thanks
 *
 * which is no good unless you know for sure that your buffer is the same type of UTF-16 that the jvm uses
 * including byte order, etc
 *
 * Gah. Stupid fucking computers. Nothing works.
 *
 * they're just good at pointing out our human weaknesses
 *
* I'm fine when it's my weaknesses they're pointing out. I take criticism reasonably well.
* But when I have to deal with the weaknesses in the n million lines of buggy third party code and arcane APIs I depend on, it really really gets to me.
*
* I have java.lang.ClassCastException in this code:
* String str = (String) o;
* if (str.equals("true"))
*
* so o isn't a String
 *
 *
 */

// TODO: Could be useful to gunzip the .content file for the user.

public class SavingProxy extends HttpRequestHandler {

    String sessionName = "" + new SimpleDateFormat("yyyyMMdd-kkmmss").format(new Date());
    
    int requestNumber = 0;
    
    @Override
    public void handleRequest(InputStream clientRequestStream,
            OutputStream clientResponseStream) throws IOException {
        HttpRequest httpRequest = parseRequestFromStream(clientRequestStream);
    
        requestNumber++;
        
        File sessionDir = new File("websessions",sessionName);
        if (!sessionDir.mkdirs()) {
            throw new IOException("Could not create directory: "+sessionDir);
        }
        String fileHead = sessionDir + "/" + requestNumber;
        FileOutputStream reqOut = new FileOutputStream(fileHead+".request");
        FileOutputStream resOut = new FileOutputStream(fileHead+".response");
        FileOutputStream contOut = new FileOutputStream(fileHead+".content");
        
        Logger.log("Will save to "+requestNumber+".request .response and .content");
        
        // TODO consider: not super()?
        // Would super do processDocument before we can get the original response stream?
        
        Logger.log(">> " + httpRequest.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        
        writeHeaders(httpRequest.getRequestAsList(), reqOut);
        reqOut.flush(); reqOut.close();
        
        HttpResponse httpResponse = handleHttpRequest(httpRequest);

        Logger.log("<< " + httpResponse.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        
        writeHeaders(httpResponse.getResponseAsList(), resOut);
        resOut.flush(); resOut.close();
        
        writeHeaders(httpResponse.getResponseAsList(), clientResponseStream);
        
        // streamContent(httpResponse.getContentAsStream(), clientResponseStream);
        List<OutputStream> outStreams = new Vector<OutputStream>();
        outStreams.add(contOut);
        outStreams.add(clientResponseStream);
        multiStream(httpResponse.getContentAsStream(), outStreams);
        contOut.flush();
        contOut.close();
    }

    public HttpResponse handleHttpRequest(HttpRequest httpRequest) {

        Logger.log(">> |    " + httpRequest.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        Logger.log("   | >> " + httpRequest.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");

        HttpResponse httpResponse = passRequestToServer(httpRequest);

        Logger.log("   | << " + httpResponse.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        // httpResponse.removeHeader("Content-length"); // In case the content has been changed by one of the filters
        Logger.log("<< |  " + httpResponse.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");

        return httpResponse;
    }
}

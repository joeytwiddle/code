package org.neuralyte.superproxy;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

import org.apache.xerces.parsers.DOMParser;
import org.cyberneko.html.HTMLConfiguration;
import org.neuralyte.Logger;
import org.neuralyte.common.FileUtils;
import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.simpleserver.SocketServer;
import org.neuralyte.simpleserver.httpadapter.HTTPStreamingTools;
import org.neuralyte.simpleserver.httpadapter.HttpRequestHandler;
import org.neuralyte.superproxy.plugins.examples.GrimeApe;
import org.neuralyte.superproxy.plugins.examples.OpenLinksInIframe;
import org.neuralyte.superproxy.plugins.examples.SessionTrackerBar;
import org.neuralyte.util.dom.DOMUtils;
import org.neuralyte.webserver.WebRequest;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;

/** Extend SuperProxy and override processDocument(org.w3c.dom.Document document). **/ 

// for x in document.xpath://object" do replace(x, niceobject(x))
// ...

// TODO: rename to DocumentProcessor?

public class SuperProxy extends HttpRequestHandler {

    //#ifdef DEBUGGING
    public static boolean writeDebugFiles = true; // What about SavingProxy? :P
    public static int reqNum = 0;
    //#endif

    // We may get StackOverflow on instantiation without that static.  ;)
    static DocumentProcessor[] processors = {
            new SessionTrackerBar(),
            new OpenLinksInIframe(),
            new GrimeApe()
    };

    public static void main(String[] args) {
        new SocketServer(7152,new SuperProxy()).run();
        // new SocketServer(7152,new OpenLinksInIframe()).run();
        // new SocketServer(7152,new SavingProxy()).run();
    }

    public HttpResponse handleHttpRequest(HttpRequest httpRequest) {

        // Logger.log(">> |    " + httpRequest.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        // Logger.log("   | >> " + httpRequest.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");

        WebRequest webRequest = new WebRequest(httpRequest);

        // Normal proxied requests never go in here.
        // This is only for requests made directly to the proxy as if it was a webserver.
        if (webRequest.getURL().startsWith("/")) {
            // Logger.warn("URL starts with / so either webrequest or proxy request to host "+httpRequest.getHeader("Host"));
            String url = webRequest.getURL();
            if (url.charAt(0)!='/') {
                Logger.error("Expected path to start with '/' but it = "+url);
            }
            url = url.substring(1);
            if (url.indexOf('/')>=0) {
                String host = url.substring(0,url.indexOf('/')); // Get first pathBit
                if (!host.contains(".")) {
                    Logger.warn("Dirty situation - request made but no host given!");
                    host = "www.google.co.uk"; // TODO: should really be fixed during processing, but as emergency fallback we can use lastHost or maybe Referer!  This is still dirty though - what if the first directory(pathBit) did contain some "."s?
                    url = '/' + url;
                } else {
                    Logger.info("Accepting as web request to host "+host);
                    url = url.substring(url.indexOf('/')); // Get rest of path
                }
                httpRequest.setHeader("Host", host);
                url = "http://"+host+url;
                httpRequest.setTopLine(httpRequest.getType()+" "+url+" HTTP/1.1");
                webRequest = new WebRequest(httpRequest); // To get .path to refresh.
            } else {
                Logger.info("Failed to proxy web-request");
                // Logger.error("TODO: We should return a response saying \"You forgot the hostname!\".");
                // throw new Error("TODO");
                HttpResponse res = new HttpResponse();
                res.setTopLine("HTTP/1.1 200 OK");
                res.setHeader("Content-type", "text/html");
                res.setContent("<HTML><BODY>You need to give me a hostname and a '/'!</BODY></HTML>");
                return res;
            }
        }

        // TODO: We are dumb ATM - this should change.
        if (httpRequest.getHeader("Connection").equalsIgnoreCase("Keep-Alive")) {
            httpRequest.setHeader("Connection", "");
        }
        // However without it, currently the initial streaming takes ages, because the remote does not close the stream.
        HttpResponse httpResponse = passRequestToServer(httpRequest);
        // Really during keep-alive, we should read only content-length, then release the socket to the pool.

        try {
            // Logger.log("Starting de-chunking...");
            HTTPStreamingTools.unencodeResponse(httpResponse);
            // Logger.log("Finished de-chunking.");
            //#ifdef DEBUG
            // File file = new File("/tmp/proxydocs/"+webRequest.getPath());
            File file = new File("/tmp/proxydocs/"+reqNum+"-"+webRequest.getPath().replaceAll("[/\\\\]","_"));
            file.getParentFile().mkdirs();
            reqNum++; 
            //#endif
            if (httpResponse.getHeader("Content-Encoding").toLowerCase().equals("gzip")) {
                if (writeDebugFiles) {
                    //// We used to stall here at the first bit of streaming, if Connection: Keep-Alive was set.
                    // Logger.log("Starting streaming..."); 
                    String originalZipped = httpResponse.getContentAsString();
                    // Logger.log("Finished streaming.");
                    // Logger.log("Starting save...");
                    FileUtils.writeStringToFile(originalZipped, new File(file+".zipin"));
                    httpResponse.setContent(originalZipped);
                    // Logger.log("Finished save.");
                }
                // Logger.log("Starting unzipping...");
                GZIPInputStream in = new GZIPInputStream(httpResponse.getContentAsStream());
                httpResponse.setContentStream(in);
                // Logger.log("Finished unzipping.");
            }
            // TODO: This is just testing our streaming implementation doesn't break the stream.
            // if (writeDebugFiles) {
            // Logger.log("Starting streaming...");
            String originalDocumentString = httpResponse.getContentAsString();
            // Logger.log("Finished streaming.");
                FileUtils.writeStringToFile(originalDocumentString, new File(file+".original"));
                // otherwise we leave empty stream :f  TODO: fix elsewhere!
                httpResponse.setContent(originalDocumentString);
           // }
            Logger.log("Starting parsing...");
            Document document = parseDocument(httpResponse.getContentAsStream());
            Logger.log("Finished parsing.");
            if (writeDebugFiles) {
                String parsedDocumentString = DOMUtils.getHtmlFromDOM(document);
                FileUtils.writeStringToFile(parsedDocumentString, new File(file+".parsed"));
            }
            document.setDocumentURI(webRequest.getCGIString());
            document = processDocument(document);
            String newDocumentString = DOMUtils.getHtmlFromDOM(document);
            if (writeDebugFiles) { 
                FileUtils.writeStringToFile(newDocumentString, new File(file+".processed"));
            }
            // TODO: optionally clear the head and don't re-zip, to provide faster experience, albeit slightly less "real".
            if (httpResponse.getHeader("Content-Encoding").toLowerCase().contains("gzip")) {
                byte[] bytes = gzip(newDocumentString);
//                StringBuffer sb = new StringBuffer();
//                sb.append(bytes);
//                // new StringBufferInputStream(sb);
//                newDocumentString = sb.toString();
//                // newDocumentString = StreamUtils.streamStringFrom(in);
//                httpResponse.setContent(newDocumentString);
//                // GZIPOutputStream out = new GZIPOutputStream(outStr);
                InputStream in = new ByteArrayInputStream(bytes);
                httpResponse.setContentStream(in);
                newDocumentString = httpResponse.getContentAsString();
                if (writeDebugFiles) {
                    String outZipped = httpResponse.getContentAsString();
                    FileUtils.writeStringToFile(outZipped, new File(file+".zipout"));
                    httpResponse.setContent(outZipped);
                }
            }
            // newDocumentString = originalDocumentString;
            httpResponse.setHeader("Content-length", ""+newDocumentString.length());
            httpResponse.setContent(newDocumentString);
        } catch (Exception e) {
            Logger.warn(e);
        }

        // Logger.log("   | << " + httpResponse.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        // httpResponse.removeHeader("Content-length"); // In case the content has been changed by one of the filters
        // Logger.log("<< |  " + httpResponse.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");

        return httpResponse;
    }

    private byte[] gzip(String input) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        GZIPOutputStream gzip = new GZIPOutputStream( baos );
        OutputStreamWriter out = new OutputStreamWriter( gzip );
        out.write(input);
        out.flush();
        out.close();
        return baos.toByteArray();
    }

    public static Document parseDocument(InputStream is) {

        // NekoHtml:
        try {

            /** Problem with this default parser: it always turns tag names to uppercase! **/
            // org.cyberneko.html.parsers.DOMParser parser = new org.cyberneko.html.parsers.DOMParser();
            /** This is a workaround from the nekoHtml FAQ: **/
            DOMParser parser = new DOMParser( new HTMLConfiguration() );
            parser.setProperty("http://cyberneko.org/html/properties/names/elems", "lower");
            parser.setProperty("http://cyberneko.org/html/properties/names/attrs", "lower");

            parser.parse(new InputSource(is));

            return parser.getDocument();

        } catch (Exception e) {
            e.printStackTrace(System.err);
        }

        return null;

        // JTidy:
        /*
        Tidy tidy = new Tidy();
        StringWriter errorsWriter = new java.io.StringWriter();
        java.io.PrintWriter pw = new java.io.PrintWriter(errorsWriter, true);
        tidy.setErrout(pw);
        tidy.setQuiet(true);
        tidy.setShowWarnings(false);
        tidy.setRawOut(true);
        tidy.setLiteralAttribs(true);
        tidy.setMakeClean(false);
        tidy.setWrapAttVals(false);
        Document document = tidy.parseDOM(is, null);
        return document;
        */

    }

    public Document processDocument(Document document) {
        for (DocumentProcessor p : processors) {
            try {
                p.processDocument(document);
            } catch (Exception e) {
                Logger.error("Problem processing through "+p);
                Logger.error(e);
            }
        }
        return document;
    }

}

package org.neuralyte.superproxy;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.util.zip.GZIPOutputStream;

import org.apache.xerces.parsers.DOMParser;
import org.cyberneko.html.HTMLConfiguration;
import org.neuralyte.Logger;
import org.neuralyte.common.FileUtils;
import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.simpleserver.SocketServer;
import org.neuralyte.simpleserver.httpadapter.HTTPStreamingTools;
import org.neuralyte.superproxy.grimeape.GrimeApe;
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

/* TODO Consider: Waiting to parse the whole page is inefficient, if we could have just streamed the page instead.
 * This is the case when simply adding some Javascript before the final </BODY>.
 * Although it may be inconvenient, streaming support is much faster.
 * In other words, we should allow support for processing the Stream OR the parsed document.
 * We should provide support methods for inserting at certain places (e.g. after just before first "<DIV>", just after first "<BODY>").
 * 
 * Proxies should be able to easily modify the request, the response, or the response document.
 * 
 * If a proxy modifies the request, and wishes to send it off, they should call some
 * common directed performRequest(), which will pass the request to the next proxy
 * in the chain.
 * 
 * We will just have to scrap Content-length entirely!  (Calculating it means we can't stream!)
*/                                  

public class SuperProxy extends PluggableHttpRequestHandler {

    //#ifdef DEBUGGING
    public static boolean writeDebugFiles = true; // What about SavingProxy? :P
    public static int reqNum = 0;
    //#endif

    static PluggableHttpRequestHandler[] handlers = {
        new GrimeApe()
    };
    
    // We may get StackOverflow on instantiation without that static.  ;)
    static DocumentProcessor[] processors = {
            new SessionTrackerBar(),
            new OpenLinksInIframe(),
            // new GrimeApe()
    };

    public static void main(String[] args) {
        new SocketServer(7152,new SuperProxy()).run();
        // new SocketServer(7152,new OpenLinksInIframe()).run();
        // new SocketServer(7152,new SavingProxy()).run();
    }

    public HttpResponse handleHttpRequest(HttpRequest httpRequest) throws IOException {

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
                Logger.info("Should we change referer? "+httpRequest.getHeader("Referer"));
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
        // if (httpRequest.getHeader("Connection").equalsIgnoreCase("Keep-Alive")) {
        // }
        httpRequest.setHeader("Connection", "close");
        if (httpRequest.getHeader("Proxy-Connection").length()>0) {
            // We are definitely being used as a proxy in the normal way.
            // But we want to hide this from the target webserver!
            httpRequest.removeHeader("Proxy-Connection");
        }
        // However without it, currently the initial streaming takes ages, because the remote does not close the stream.
        HttpResponse httpResponse = passRequestToServer(httpRequest);
        // Really during keep-alive, we should read only content-length, then release the socket to the pool.

		// @todo This section is kinda inefficient, for testing, but it works now.
        String contentType = httpResponse.getHeader("Content-type");
        Logger.log("Content-type of response: "+contentType);
        contentType = contentType.replaceFirst(";.*","");
        if (!contentType.equalsIgnoreCase("text/html")) {
            Logger.warn("Skipping! Content-type = \"" + contentType + "\" for: " + httpRequest.getTopLine());
        } else {
            try {
                // Logger.log("Starting de-chunking...");
                HTTPStreamingTools.unencodeResponse(httpResponse);
                // Logger.log("Finished de-chunking.");
                //#ifdef DEBUG
                // File file = new File("/tmp/proxydocs/"+webRequest.getPath());
                File file = new File("/tmp/proxydocs/"+reqNum+"-"+webRequest.getPath().replaceAll("[/\\\\]","_"));
                if (!file.getParentFile().isDirectory() && !file.getParentFile().mkdirs()) {
                    throw new IOException("Could not create directory: "+file.getParentFile());
                }
                reqNum++; 
                //#endif
                /*
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
                    httpResponse.setHeader("Content-Encoding","none"); // TODO "none" was just a guess, could just remove it. :P
                    // Logger.log("Finished unzipping.");
                }
                 */
                HTTPStreamingTools.unzipResponse(httpResponse);
                // TODO: This is just testing our streaming implementation doesn't break the stream.
                // if (writeDebugFiles) {
                // Logger.log("Starting streaming...");
                String originalDocumentString = httpResponse.getContentAsString();
                // Logger.log("Finished streaming.");
                FileUtils.writeStringToFile(originalDocumentString, new File(file+".original"));
                // otherwise we leave empty stream :f  TODO: fix elsewhere!
                httpResponse.setContent(originalDocumentString);
                // }
                // Logger.log("Starting parsing...");
                Document document = parseDocument(httpResponse.getContentAsStream());
                // Logger.log("Finished parsing.");
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
                // TODO:
                // Loop through all <A> tags changing any absolute href values to point through me.
                // Also loop through all other tags with src attribute.
            } catch (Exception e) {
                Logger.warn(e);
            }
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

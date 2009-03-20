package org.neuralyte.superproxy;

import java.io.InputStream;

import org.apache.xerces.parsers.DOMParser;
import org.cyberneko.html.HTMLConfiguration;
import org.neuralyte.Logger;
import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.simpleserver.SocketServer;
import org.neuralyte.simpleserver.httpadapter.HTTPStreamingTools;
import org.neuralyte.simpleserver.httpadapter.HttpRequestHandler;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;

// for x in document.xpath://object" do replace(x, niceobject(x))
// ...

public class SuperProxy extends HttpRequestHandler {

    public static void main(String[] args) {
        new SocketServer(7152,new SuperProxy()).run();
        // new SocketServer(7152,new OpenLinksInIframe()).run();
        // new SocketServer(7152,new SavingProxy()).run();
    }

    public HttpResponse handleHttpRequest(HttpRequest httpRequest) {

        Logger.log(">> |    " + httpRequest.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        Logger.log("   | >> " + httpRequest.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");

        HttpResponse httpResponse = passRequestToServer(httpRequest);

        try {            
            HTTPStreamingTools.unencodeResponse(httpResponse);
            // One of these breaks the gzipped stream we get back from Google.
            String temp = httpResponse.getContentAsString();
            httpResponse.setContent(temp);
            // Document document = parseDocument(httpResponse.getContentAsStream());
            // document = processDocument(document);
        } catch (Exception e) {
            Logger.warn(e);
        }

        Logger.log("   | << " + httpResponse.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");
        // httpResponse.removeHeader("Content-length"); // In case the content has been changed by one of the filters
        Logger.log("<< |  " + httpResponse.getTopLine() + " ["+httpRequest.getHeadersAsList().size()+"]");

        return httpResponse;
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
        return document;
    }
    
}

package org.neuralyte.superproxy.grimeape;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.URLDecoder;
import java.util.Date;
import java.util.List;

import org.apache.xerces.dom.CoreDocumentImpl;
import org.apache.xerces.dom.TextImpl;
import org.neuralyte.Logger;
import org.neuralyte.common.FileUtils;
import org.neuralyte.common.io.StreamUtils;
import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.simpleserver.SocketServer;
import org.neuralyte.superproxy.HTMLDOMUtils;
import org.neuralyte.superproxy.PluggableHttpRequestHandler;
import org.neuralyte.webserver.WebRequest;
import org.neuralyte.webserver.WebRequestHandler;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

/**
 * GrimeApe aims to be a GreaseMonkey emulator which works as a web-proxy.
 */

/*
// TODO: If possible, rather than insert the script inline into the page it
// would be better
// to insert an external link to the script, so it can be loaded more
// intelligently (cached)
// by the browser.
// I guess these should be directed at the proxy, and the proxy should intercept
// them and handle them like a webserver would.
// Hmm in fact I would like to add one SCRIPT at the end of the page, which
// waits maybe a second for the page to really finish loading, then goes ahead
// and appends all our userscripts.

// @todo We should be checking each script's include/exclude patterns.

// @todo We should probably cache the scripts in memory, for speed, but update
// them if the file changes. include/exclude metas should be cached again (in
// fact we should have the regexps pre-compiled :).

// @todo We may be able to include some of the Firefox GM libraries, and get
// more userscripts working. Some functions might be portable if they don't work
// immediately.

// Hmm GM has browser privileges, and sandbox. We are running inside the page.
// This does mean a little "insecurity" - we are making our source available to
// the page, in theory it could be read and sent back to the site!

// Anyway we are much more limited than GM, but we could pull some tricks.

// We could implement GM_getValue and GM_setValue by making XMLHttpRequests
// (AJAX) to the proxy. GM will need to be high enough in the plugin chain to
// intercept these special http requests.

// Ah nice, we have opened up more insecurities. Now in theory the remote server
// could start fiddling with our proxy params!

// Since it's unlikely we will be able to really fix these, I recommend a
// "paranoid-mode" setting for users. They could choose to 'Always Allow'
// read-write to certain GM_setValue vars, choose 'Always Ask' for others. :)
*/

public class GrimeApe extends PluggableHttpRequestHandler {

    // We don't only want to processDocuments.
    // Sometimes we may want to intercept HTTP requests,
    // e.g. the browser is requesting one of the scripts which we told it to
    // load on a previously response page.
    // OK so we don't need to do that.
    // But we do need the URL of the document we
    // are "processing". We should only add/run the relevant scripts with
    // matching includes/excludes.

    /*//// Intercepts ////
     * Requests made to /gRiMeApE/ are handled in special ways by the proxy.
     * /gRiMeApE/javascript/<script_name> to get a core script
     * /gRiMeApE/userscripts/<script_name> to get a userscript
     * e.g. /gRiMeApE/userscripts/config.js
     */
    
    public static void main(String[] args) {
        new SocketServer(7152,new GrimeApe()).run();
    }
        
    static String topDir = ".";
    static String coreScriptsDir = "./javascript/";
    static String userscriptsDir = "./userscripts/";
    
    public HttpResponse handleHttpRequest(HttpRequest request) throws IOException {

        //// Check for special requests directed at GrimeApe, not the web.
        WebRequest wreq = new WebRequest(request);
        // Logger.warn("path = " + wreq.getPath());
        if (wreq.getPath().startsWith("/_gRiMeApE_/")) {
            Logger.log("Handling special GA request: "+wreq.getPath());
            String[] args = wreq.getPath().split("/");
            // Logger.warn("args[1] = " + args[1]);
            if (args[2].equals("javascript") || args[2].equals("userscripts") || args[2].equals("images")) {
                // Return script as webserver
                // Before we factor this out to another method:
                // What headers should we build?
                // And do they need to know the request headers in order to be generated?
                String fileBeyond = wreq.getPath().replaceAll("^/[^/]*/[^/]*/*", ""); // aka args[3] onward, joined again.
                String scriptDir = topDir + "/" + args[2];
                return makeFileHttpResponse(new File(scriptDir,fileBeyond),"text/javascript",request);
               /** @todo danger of "../../../../etc/passwd" in args2 ! */
                
            } else if (args[2].equals("log")) {
                // Should be tool for this pff.
                String cgi = wreq.getCGIString();
                // argh String logData = cgi.replaceAll("^.*data=([^&]*).*","\\1");
                String logData = cgi.replaceAll("^.*data=", "");
                logData = URLDecoder.decode(logData);
                Logger.info("GM_LOG: "+logData);
                // return ""; // "\/\*thanksforlogging\*\/"
                // throw new Error("Just deal with it.");
                return failedHttpResponse("too lazy to respond empty"); // TODO
                
            } else {
                Logger.error("Bad request: "+wreq.getPath());
                throw new Error("Bad request: "+wreq.getPath());
            }
        }
        
        //// OK we have a normal web request.
        //// Handle it, then inject scripts if it is a web page.
        
        // HttpResponse response = HTTPStreamingTools.passRequestToServer(request);
        HttpResponse response = super.handleHttpRequest(request);
        
        /** @todo What does GM trigger on?  I've seen it run (fail) on .txt and .js pages also. **/
        if (response.getHeader("Content-type").toLowerCase().startsWith("text/html")) {
            StringBuffer responseString = StreamUtils.streamStringBufferFrom(response.getContentAsStream());
            // Logger.log(""+responseString);
            int i = responseString.lastIndexOf("</BODY>");
            if (i == -1)
                i = responseString.lastIndexOf("</body>");
            if (i == -1) {
                i = responseString.length();
                // yeah nice Google doesn't bother with </BODY> lol
            }
            if (i == -1) {
                Logger.warn("Failed to inject script tag.");
            } else {
                String[] scriptsToInject = {
                        "javascript/test.js",
                        "javascript/grimeape_greasemonkey_compat.js",
                        "javascript/grimeape_config.js",
                        "userscripts/faviconizegoogle.user.js",
                        "userscripts/track_history.user.js",
                        "userscripts/alert_watcher/alert_watcher.user.js",
                        "userscripts/reclaim_cpu/reclaim_cpu.user.js",
                };
                for (String script : scriptsToInject) {
                    // String srcURL = "/_gRiMeApE_/javascript/test.js";
                    String srcURL = "/_gRiMeApE_/"+script;
                    String injectHTML = "<SCRIPT type='text/javascript' src='" + srcURL + "'/>\n";
                    responseString.insert(i, injectHTML);
                    i += injectHTML.length();
                }
            }
            // We must reset it after streaming it, even if we didn't change it.
            response.setContent(responseString.toString());
        }
        
        return response;
    }

    public HttpResponse makeFileHttpResponse(File file, String contentType, HttpRequest request) {
        HttpResponse httpResponse = new HttpResponse();
        
        try {
            if (file.exists()) {

                // Duplicated from WebRequestHandler:

                httpResponse.setTopLine("HTTP/1.0 200 OK"); // wget barfed when we were returning HTTP/1.x
                // httpResponse.setHeader("Connection","close");
                httpResponse.setHeader("Date", WebRequestHandler.getFormattedDate(new Date(file.lastModified())));
                httpResponse.setContentStream(new FileInputStream(file));
                httpResponse.setHeader("Content-Length",""+file.length());

                return httpResponse;

            } else {
                Logger.warn("File does not exist: "+file);
            }
        } catch (Exception e) {
            Logger.warn(""+e);
        }
        return failedHttpResponse("Sorry no file for you.");
    }
    
    public static HttpResponse failedHttpResponse(String str) {
        HttpResponse response = new HttpResponse();
        response.setTopLine("HTTP/1.0 539 OHDEAR");
        response.setHeader("Date",WebRequestHandler.getFormattedDate());
        response.setHeader("Connection","close");
        response.setContent(""); // If we don't do this, we actually don't get NullPointerException, but a "Stream closed" IOException.
        return response;
    }
    
    /** @deprecated GrimeApe converted from DocumentProcessor to HttpRequestHandler  **/
    public Document injectScripts(Document document) {
        Logger.info("DocumentURI = " + document.getBaseURI());
        List<Node> tags = HTMLDOMUtils.getElementsByTagName(document, "A");
        Logger.info("  has " + tags.size() + " links.");

        CoreDocumentImpl core = (CoreDocumentImpl) document;
        /*
         * Node body = DOMUtils.getChildNodesMeetingCondition(document, new
         * DOMUtils.NodeCondition() { public boolean metBy(Node n) { return
         * (n.getNodeName().equalsIgnoreCase("BODY")); } } )[0];
         */
        // Node body = HTMLDOMUtils.getElementsByTagName(document,
        // "BODY").get(0);
        Node body = HTMLDOMUtils.getFirstElementWithTagName(document, "BODY");
        if (body == null) {
            Logger.warn("Cannot add script to " + document
                    + " - it has no body!");
        } else {

            Logger.info("=> Adding scripts!");

            /*
            // String toAdd =
            // "<script language=\"JavaScript\" src=\"http://hwi.ath.cx/powerbar/add_powerbar.js\"></script>\n";
            String toAdd = "<script language=\"JavaScript\"  type=\"text/javascript\" src=\"http://hwi.ath.cx/powerbar/add_powerbar.js\"></script>\n";
            TextImpl textNode = new org.apache.xerces.dom.TextImpl(core, toAdd);
            // textNode.setData(toAdd);
            body.appendChild(textNode);
            */

            File scriptsDir = new File("userscripts");
            for (File scriptFile : scriptsDir.listFiles()) {
                if (scriptFile.getName().endsWith(".user.js")) {
                    try {
                        String script = FileUtils
                                .readStringFromFile(scriptFile);
                        
                        /*
                        if (script.contains("XPathResult")) {
                            Logger
                                    .warn("Script "
                                            + scriptFile.getName()
                                            + " will probably fail in Konqueror/GrimeApe - it uses 'XPathResult'.");
                        }

                        TextImpl textNode = TextImpl(core,
                                "<SCRIPT type=\"text/javascript\">"
                                + '\n' + script + '\n'
                                + "</SCRIPT>\n"
                        );
                        // textNode.setData("<SCRIPT language=\"javascript\">" +
                        // script + "</SCRIPT>\n");
                        body.appendChild(textNode);
                        */
                        
                        String srcURL = "/_gRiMeApE_/userscript/" + scriptFile.getName(); // +"?sessionid="+sessionID;
                        TextImpl textNode = new TextImpl(core,"<SCRIPT type='text/javascript' src='" + srcURL + "'/>\n");
                        body.appendChild(textNode);

                    } catch (Exception e) {
                        Logger.error(e);
                    }
                }
            }

        }

        return document;
    }

}

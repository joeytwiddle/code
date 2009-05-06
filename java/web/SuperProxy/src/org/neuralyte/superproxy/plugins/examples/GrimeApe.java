package org.neuralyte.superproxy.plugins.examples;

import java.io.File;
import java.util.List;

import org.apache.xerces.dom.CoreDocumentImpl;
import org.apache.xerces.dom.TextImpl;
import org.neuralyte.Logger;
import org.neuralyte.common.FileUtils;
import org.neuralyte.superproxy.DocumentProcessor;
import org.neuralyte.superproxy.HTMLDOMUtils;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

/**
 * GrimeApe serves a similar function to Mozilla's GreaseMonkey - it adds
 * Javascript to webpages specified by the user, to change their behaviour or
 * looks.
 * 
 * @author joey
 * 
 */

// TODO: If possible, rather than insert the script inline into the page it
// would be better
// to insert an external link to the script, so it can be loaded more
// intelligently (cached)
// by the browser.

// @todo We should be checking each script's include/exclude patterns.

// @todo We should probably cache the scripts in memory, for speed, but update
// them if the file changes.  include/exclude metas should be cached again.

// @todo We may be able to include some of the Firefox GM libraries, and get
// more userscripts working. Some functions might be portable if they don't work
// immediately.

public class GrimeApe implements DocumentProcessor {

    // We don't only want to processDocuments.
    // Sometimes we may want to intercept HTTP requests,
    // e.g. the browser is requesting one of the scripts which we told it to
    // load on a previously response page.
    // OK so we don't need to do that.
    // But we do need the URL of the document we
    // are "processing". We should only add/run the relevant scripts with
    // matching includes/excludes.

    public Document processDocument(Document document) {
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

            // String toAdd =
            // "<script language=\"JavaScript\" src=\"http://hwi.ath.cx/powerbar/add_powerbar.js\"></script>\n";
            String toAdd = "<script language=\"JavaScript\"  type=\"text/javascript\" src=\"http://hwi.ath.cx/powerbar/add_powerbar.js\"></script>\n";
            TextImpl textNode = new org.apache.xerces.dom.TextImpl(core, toAdd);
            // textNode.setData(toAdd);
            body.appendChild(textNode);

            File scriptsDir = new File("userscripts");
            for (File scriptFile : scriptsDir.listFiles()) {
                if (scriptFile.getName().endsWith(".user.js")) {
                    try {
                        String script = FileUtils
                                .readStringFromFile(scriptFile);
                        if (script.contains("XPathResult")) {
                            Logger
                                    .warn("Script "
                                            + scriptFile.getName()
                                            + " will probably fail in Konqueror/GrimeApe - it uses 'XPathResult'.");
                        }

                        textNode = new org.apache.xerces.dom.TextImpl(core,
                                "<SCRIPT language=\"javascript\">" + script
                                        + "</SCRIPT>\n");
                        // textNode.setData("<SCRIPT language=\"javascript\">" +
                        // script + "</SCRIPT>\n");
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

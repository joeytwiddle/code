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


public class GrimeApe implements DocumentProcessor {

    public Document processDocument(Document document) {
        Logger.info("DocumentURI = "+document.getBaseURI());
        List<Node> tags = HTMLDOMUtils.getElementsByTagName(document, "A");
        Logger.info("  has "+ tags.size() +" links.");

        CoreDocumentImpl core = (CoreDocumentImpl)document;
        /* Node body = DOMUtils.getChildNodesMeetingCondition(document,
                new DOMUtils.NodeCondition() {
            public boolean metBy(Node n) {
                return (n.getNodeName().equalsIgnoreCase("BODY"));
            }
        }
        )[0]; */
        // Node body = HTMLDOMUtils.getElementsByTagName(document, "BODY").get(0);
        Node body = HTMLDOMUtils.getFirstElementWithTagName(document, "BODY");
        if (body == null) {
            Logger.warn("Cannot add script to "+document+" - it has no body!");
        } else {

            /*
            String toAdd = "<script language=\"JavaScript\" src=\"http://hwi.ath.cx/powerbar/add_powerbar.js\"></script>\n";
            TextImpl textNode = new org.apache.xerces.dom.TextImpl(core,toAdd);
            // textNode.setData(toAdd);
            body.appendChild(textNode);
            */

            File scriptsDir = new File("userscripts");
            for (File scriptFile : scriptsDir.listFiles()) {
                if (scriptFile.getName().endsWith(".user.js")) {
                    try {
                        String script = FileUtils.readStringFromFile(scriptFile);
                        if (script.contains("XPathResult")) {
                            Logger.warn("Script "+scriptFile.getName()+" will probably fail in Konqueror/GrimeApe - it uses 'XPathResult'.");
                        }

                        /*
                    // DOMUtils.printHierarchyOfNode(document);
                    new DOMTraverser() {
                        public void doBefore(Node node, int depth) {
                            super.doAfter(node, depth);
                            String indent = StringUtils.repeat("|--",depth);
                            String name = node.getNodeName();
                            String classType = node.getClass().toString();
                            String text = node.toString().replaceAll("\n","\\n");
                            // Logger.log("[DOM] " + indent + " " + node.getClass() + " " + node.toString());
                            Logger.log("[DOM] " + indent + name + " \""+text+"\" ("+classType+")");
                        }
                    }.traverse(document);
                         */

                       Node  textNode = new org.apache.xerces.dom.TextImpl(core,"<SCRIPT language=\"javascript\">" + script + "</SCRIPT>\n");
                        // textNode.setData("<SCRIPT language=\"javascript\">" + script + "</SCRIPT>\n");
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

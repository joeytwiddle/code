package org.neuralyte.superproxy.plugins.examples;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.neuralyte.Logger;
import org.neuralyte.superproxy.DocumentProcessor;
import org.neuralyte.superproxy.HTMLDOMUtils;
import org.neuralyte.superproxy.SuperProxy;
import org.w3c.dom.Document;
import org.w3c.dom.Node;

public class SessionTrackerBar implements DocumentProcessor {

    // Or maybe it's more optional.
    // When on any page, we can say: Turn this page into a sidebar, and open the links I click in the main panel.
    
    // Rather than a HashMap, we are using a Queue for cleanup, sometimes changing the positions.
    // Do we want a two-ended queue?
    // Queue<SearchPage> recentSearchPages = new SynchronousQueue<SearchPage>();
    Map<String,SearchPage> recentSearchPages = new HashMap<String,SearchPage>();
    
    // But also track the chains between them.
    // Use Referer!
    
    static class SearchPage {
        String url;
        String[] resolvedLinks;
    }

    public Document processDocument(Document document) {
        Logger.info("DocumentURI = "+document.getBaseURI());
        // FAILS: NodeList tags = document.getElementsByTagName("A");
        List<Node> tags = HTMLDOMUtils.getElementsByTagName(document, "A");
        Logger.info("  has "+ tags.size() +" links.");
        SearchPage sp = new SearchPage();
        sp.url = document.getBaseURI();
        // sp.resolvedLinks = (String[])document.getElementsByTagName("a");
        // recentSearchPages.add(sp);
        recentSearchPages.put(sp.url,sp);
        return document;
    }
    
}

package org.neuralyte.webmuncher;

import java.io.IOException;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.neuralyte.Logger;
import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.superproxy.SuperProxy;
import org.w3c.dom.Document;
import org.w3c.dom.Node;


public class WebMuncher extends SuperProxy {

    public HttpResponse handleHttpRequest(HttpRequest httpRequest) throws IOException {
        return super.handleHttpRequest(httpRequest);
    }

    public Document processDocument(Document document) {
        Map<String,Integer> tagCounts = new HashMap<String, Integer>();
        processNode(document,tagCounts);
        
        for (String k : tagCounts.keySet()) {
            int count = tagCounts.get(k);
            Logger.log("<"+k+"> : "+count);
        }
        return document;
    }

    private void processNode(Node document, Map<String, Integer> tagCounts) {
        String tag = document.getNodeName();
        int count = tagCounts.get(tag);
        count++;
        for (int i = 0; i < document.getChildNodes().getLength(); i++) {
            processNode(document.getChildNodes().item(i), tagCounts);
        }
    }

}

package org.neuralyte.superproxy.plugins.examples;

import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.superproxy.DocumentProcessor;
import org.neuralyte.superproxy.SuperProxy;
import org.w3c.dom.Document;


public class OpenLinksInIframe /*extends SuperProxy */ implements DocumentProcessor {

    /*
    // TODO BUG: handHttpRequest() is not called! :P
    public HttpResponse handleHttpRequest(HttpRequest httpRequest) {
        return super.handleHttpRequest(httpRequest);
    }
    */

    public Document processDocument(Document document) {
        return document;
    }
    
}

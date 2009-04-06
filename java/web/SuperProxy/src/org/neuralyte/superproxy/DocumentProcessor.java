package org.neuralyte.superproxy;

import org.w3c.dom.Document;

/**
 * A DocumentProcessor can assume it has some state.
 * It's up to the SessionManager to ensure it has the right state.  :P
 * 
 * @author joey
 *
 */

public interface DocumentProcessor {

    public Document processDocument(Document document);
    
}

package org.neuralyte.superproxy;

import java.util.List;
import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Node;



public class HTMLDOMUtils {

    public static Node getFirstElementWithTagName(Node node, String tagName) {
        if (node.getNodeName().equalsIgnoreCase(tagName)) {
            return node;
        }
        for (int i=0;i<node.getChildNodes().getLength();i++) {
            Node found = getFirstElementWithTagName(node.getChildNodes().item(i),tagName);
            if (found != null)
                return found;
        }
        return null;
    }
    
    public static List<Node> getElementsByTagName(Node node, String tagName) {
        List<Node> list = new Vector<Node>();
        addElementsWithTag(node, tagName, list);
        return list;
    }
    
    public static void addElementsWithTag(Node node, String tagName, List<Node> list) {
        if (node.getNodeName().equalsIgnoreCase(tagName)) {
            list.add(node);
        }
        for (int i=0;i<node.getChildNodes().getLength();i++) {
            addElementsWithTag(node.getChildNodes().item(i),tagName,list);
        }
    }
    
}

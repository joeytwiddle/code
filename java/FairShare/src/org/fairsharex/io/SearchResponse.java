package org.fairsharex.io;

import java.util.HashMap;
import java.util.Map;

import org.neuralyte.hashing.Hash;


public class SearchResponse implements Response {

    // The String is actually a Hash.
    Map<String,Double> results = new HashMap<String,Double>(); 

}

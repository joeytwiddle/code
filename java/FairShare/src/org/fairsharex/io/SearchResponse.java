package org.fairsharex.io;

import java.util.HashMap;
import java.util.Map;

import org.fairshare.data.Hash;


public class SearchResponse implements Response {

    Map<Hash,Double> results = new HashMap<Hash,Double>(); 

}

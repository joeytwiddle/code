package org.common.lib;

import java.util.Hashtable;
import java.util.List;
import java.util.Vector;


public class ListMap<K,V> extends Hashtable<K, List<V>> {
    
    public List<V> getList(K key) {
        List<V> list = super.get(key);
        if (list == null) {
            list = new Vector<V>();
            super.put(key,list);
        }
        return list;
    }
    
}

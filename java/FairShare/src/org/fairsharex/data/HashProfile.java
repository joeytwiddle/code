package org.fairsharex.data;

import java.util.Set;


public class HashProfile {

    public boolean mask[];
    
    public HashProfile(Set<String> hashes) {
        mask = new boolean[64*64];
        for (String hash : hashes) {
            int i = Math.abs(hash.hashCode()) % (64*64);
            mask[i] = true;
        }
    }
    
    public void outputInfo() {
        System.out.println("Your hash mask is:");
        for (int i=0;i<64*64;i++) {
            if (i>0 && i%64 == 0) {
                System.out.println();
            }
            System.out.print(" " + (mask[i]?"X":"_"));
        }
        System.out.println();
    }
    
}

package org.fairshare.data;

import org.neuralyte.hashing.Hash;


public class KnownFile {

    Hash hash;
    
    /*
     * TODO
     * During transfer of a large file, we don't want peers to be able to get away with
     * sending false data.  So we will need some way of detecting early that they are
     * not sending the true file.
     * e.g. sub-hashes every 2meg block.  Once we get the full block, if the sum
     * doesn't match, then one of the clients was lying!
     */
    
    /*
     * Other extensions:
     * To make it easier to keep hashes of user files up-to-date, we should
     * also store the file's lastKnownSize and lastKnownModifiedDate, and
     * watch them for changes.
     */
    
}

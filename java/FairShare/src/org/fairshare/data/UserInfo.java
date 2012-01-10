package org.fairshare.data;


public class UserInfo {

    String name;

    // Proof of being who you say you are:
    String puzzle;
    String key; // On owner has this.
    
    String contact;
    
    long compatibilityMask;
    // Held to refer recommended users to this user.
    // Can at least block completely incompatible users.
    
}

// I can do something difficult.
// I can unhash my puzzle to get my key.
// Nobody else can do that.
// But how can I prove that I can do it?!

// Ahhh make it one-time.

// One thing I can do, is quite quickly find the hash of
// my key+B where B is a byte, because I can brute-force
// the 256 possible byte values.


/*
 * 
 * We could let the other users decide what tricks we do.
 * e.g. B tells C, "A's key converts X into Y, just try it!"
 * When talking to A, C might check A can do U->V, but he might also ask
 * A's response to W.  If C gets back X, he can offer the W->X or U->V
 * challenge-response to other users.
 * 
 * Of course it means a lot of other users will know working CRs for recent clients.
 * But clients can offer out new CRs often.
 * 
 * One CR per user pair?
 * No user A shall hold two CRs for user B, one for frequent use, one for identity
 * recover in emergencies when first has become insecure.
 * At generation, both know the secret date of change-over.  Other clients will not!  ;) 
 * 
 * But clients can generate new CRs.  Or use old ones to check.  ;)
 * If there are a huge number of them around, it might be impossible to collect
 * enough to fake another client!
 * 
 * It could also be a way of letting B prove that he can talk to C right now.
 * 
 */


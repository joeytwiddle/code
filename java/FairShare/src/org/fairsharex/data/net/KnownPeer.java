package org.fairsharex.data.net;

/**
 * FairShare is reputation-based. If you do good things for other peers on the
 * network, they will award you a good reputation, and communicate it to others
 * also.
 * 
 * There is also an element of "owing" another User. Users who have given a lot
 * can cash in their debts, e.g. by doing some superfast streaming.
 * 
 * You can increase your reputation by storing and streaming data currently
 * needed by the network but not available enough.
 * 
 * (This could involve caching relayed data in case someone else wants it.
 * Getting rare parts of files first.
 * Getting rare parts of any files when seeds start to die.
 * ...
 * )
 */

public class KnownPeer {

    String ipAddress;

    float  scoreCompatibilityWithMe;

    float  scoreAlignment;          // good/bad

    String userHash;

}

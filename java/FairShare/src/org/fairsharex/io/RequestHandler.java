package org.fairsharex.io;

import org.fairshare.FairShare;


public class RequestHandler {

    public Response handleRequest(Request req, Context context) {
        // TODO: 
        // Context context = new Context();
        // context.database = FairShare.da
        return req.process(context);
    }
    
}

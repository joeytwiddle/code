package org.neuralyte.superproxy;

import java.io.IOException;

import org.neuralyte.httpdata.HttpRequest;
import org.neuralyte.httpdata.HttpResponse;
import org.neuralyte.simpleserver.httpadapter.AdvancedHttpRequestHandler;
import org.neuralyte.simpleserver.httpadapter.HTTPStreamingTools;

// joey Apr 12, 2006 9:53:55 PM

/**
 * PluggableHttpRequestHandlers are better than HttpRequestHandlers, because they
 * can be formed into a chain of processors.
 * <P/>
 * (And they have a convenient function for passing their request to the remote server.)
 * <P/>
 *  They have this ability because they meet the following obligation:
 *  Classes extending PluggableHttpRequestHandler should not contact the remote
 *  server directly, to retrieve the resource that was requested.
 *  Instead they should call super.handleHttpRequest() or (TODO DECIDE) maybe
 *  parentPluginManager.passRequest().
 *  That call will handle the chaining of later plugins, and give back a response.
 *  When we return our response, it will be passed back to the plugins before us.
 *   <P/>
 *   (What if this plugin wants to make an extra http request, as part of its efforts.
 *   Well it should either make a direct request, or maybe better to call
 *   parentPluginManager.handleNewRequestFromTheTop() to handle the request
 *   by passing it through all the plugins the same as our current (intial) request.)  
 *   <P/>
 *  NOTE TODO: This model of passing altered requests down to other plugins, and then
 *  dealing with the response they give, is done by nested method calls which build up
 *  on the stack.
 *  This means we effectively lose control of the order in which events are processed
 *  on the way back.
 *  I can imagine a situation where a plugin late in the chain might encounter an
 *  unusual error, and want to respond directly to the client with a helpful message,
 *  so the user can fix it, rather than responding with a failed document, which may
 *  cause the earlier plugins to barf on the return path, and provide the user with no help.
 *  <P/>
 *  If only Java had some kind of ON ERROR <do_something> catch/throw mechanism like BASIC... ;) 
 *  <P/>
 *  On the other hand, would it be wrong for a plugin to be able to override earlier
 *  plugins?
 *  Possibly we could use a separate log stream and have that presented to the user
 *  either in the proxy logs, or as HTML in their browser (injected as popup or floating div
 *  or available as visitable URL).  Yuk.
 *  <P/>
 *  NOTE TODO: Also this system is sucking because we are creating only one kind
 *  of chaining algorithm, the one implemented in this class.
 *  Well if we pass off to parentPluginManager, maybe he can be a bit more varied?
 *  How do we get him?
 * <P/>
 *  Damn this class is so sad.  And yet so short.
 */

// We extend HTTPStreamingTools just for library functions.

public class PluggableHttpRequestHandler extends AdvancedHttpRequestHandler {

    public HttpResponse handleHttpRequest(HttpRequest request) throws IOException {
        // TODO WRONG!  Should pass to next plugin, if exists.
        
        // if (nextHandler!=null) { nextHandler.handle() } else {
        
        HttpResponse response = HTTPStreamingTools.passRequestToServer(request);
        
        // So this is actual response from remote server.
        // De-chunking has been performed
        // It's unlikely the later plugins will want to work with a gzipped stream.
        // Let's gunzip it now if we need to, leaving no need to check again.

        // @todo I suggest removing this, or somehow making it optional
        // Plenty of proxies will ignore 90% of data passing through, so unzipping
        // stream is BAD!
        
        HTTPStreamingTools.unzipResponse(response);
        
        // We could easily put a warning in getResponseStreamAsStringBuffer ;)
        
        return response;
        
    }
    
}

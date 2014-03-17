/*
 * This is a simple routing utility for designing RESTful APIs in Connect-powered applications
 * 
 * It's also a huge pain in the ass to use with anything moderately complex.
 * 
 * MIT license, copyright Isaac Wagner <isaacbwagner@gmail.com> 2012
 * 
 */
 
var url = require('url'),
    http = require('http');
 
module.exports = function(tree) {
    return function(req, res, next) {
	var parse = url.parse(req.url);
	var path = parse.pathname.
		replace(/^\/+/, '').
	    replace(/\/+$/, '').
	    split('/');
 
	req.params = {};
 
	var node = tree;
 
	for(var depth = 0; depth <= path.length; depth += 1) {
	    if(node.$pre) {
		if(!node.$pre(req, res))
		    return null;
	    }
 
	    if(depth == path.length) {
		if(node.$any) {
		    if(!node.$any(req, res))
			return null;
		} else {
		    var methods = {
			'GET': '$get',
			'PUT': '$put',
			'POST': '$post',
			'DELETE': '$del'
		    };
 
		    var func = methods[req.method];
		    if(func && node[func]) {
			if(!node[func](req, res)) {
			    return null;
			}
		    } else {
			res.statusCode = 405;
			res.end('405 Method Not Allowed\n');
			return null;
		    }
		}
 
		break; 
	    } else {
		var param = null;
		var match = false;
 
		for(var key in node) {
		    if(key.indexOf('$') != 0) {
			if(key.indexOf(':') == 0 && !param) {
			    param = key;
			} else {
			    if(path[depth] == key) {
				node = node[key];
				match = true;
				break;
			    }
			}
		    }
		}
 
		if(!match && !param) {
		    return next();
		} else if(param) {
		    req.params[param.slice(1)] = decodeURIComponent(path[depth]);
		    node = node[param];
		}
	    }
	}
 
	return next();
    };
};
 
http.ServerResponse.prototype.json = function json(object) {
    this.statusCode = 200;
    this.setHeader('Content-Type', 'application/json');
    this.end(JSON.stringify(object));
};

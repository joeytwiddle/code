
function doLater(fn) {
	// Simulated async
	setTimeout(fn,0);
}

function add_callback(x,y,callback) {
	doLater(function(){ callback(x+y); });
}

function add_promise(x,y) {
	var onSuccess = null;
	doLater( function(){ onSuccess(x+y); });
	return {
		then: function(_callback) {
			onSuccess = _callback;
		}
	};
}

function logit(r) {
	console.log(r);
}

add_callback(4,5,logit);

add_promise(4,5).
	then(logit);


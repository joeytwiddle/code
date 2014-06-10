

function getOffset(elem) {
	if (!elem) {
		return {top: 0, left: 0};
	}
	if (elem === elem.ownerDocument.body) {
		return o.offset.bodyOffset(elem);
	}
	var G = elem.getBoundingClientRect(),
		J = elem.ownerDocument,
		F = J.body,
		E = J.documentElement,
		L = E.clientTop || F.clientTop || 0,
		K = E.clientLeft || F.clientLeft || 0,
		I = G.top + (self.pageYOffset || o.boxModel && E.scrollTop || F.scrollTop) - L,
		H = G.left + (self.pageXOffset || o.boxModel && E.scrollLeft || F.scrollLeft) - K;
	return {top: I, left: H};
}

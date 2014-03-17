function isAncestor(child, maybeAncestor) {
	return maybeAncestor.contains(child);
}

// useful for event delegation
if (isAncestor(event.target, list)) {
  // do something
}

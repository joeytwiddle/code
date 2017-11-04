// wrapper for manipulating a set more easily (add .toggle)
// possibly written by qswz
// of course we don't have to use a proxy here, we could subclass Set instead
export const SetEnhancer = set => new Proxy(set, {
	get(target, name) {
		if (name === 'toggle') {
			return (value, force = target.has(value)) => {
				if (force) {
					target.add(value);
				} else {
					target.delete(value);
				}
			};
		} else {
			return target[name];
		}
	}
});

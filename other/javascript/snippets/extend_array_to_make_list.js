// By earendel
class List extends Array {
    constructor() {
        super();
    }
    static from(iterable) {
        var li = new List();
        [...iterable].forEach((e, i) => li[i] = e);
        return li;
    }
    first() {
        return this[0];
    }
    last() {
        return this[this.length - 1];
    }
};

var list = List.from([1, 2, 3]).map(x => x * x);

list.last();

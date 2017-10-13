// Run this in non-strict mode

const proxy = new Proxy({}, {
  get(target, key) {
    if(key === Symbol.unscopables) return {}
    return `${key.toUpperCase()} `
  },

  has(target, key) {
    return true
  }
})

const main = () => {
  with (proxy) {
    return This + is + Black + Magic
  }
}

console.log(main())

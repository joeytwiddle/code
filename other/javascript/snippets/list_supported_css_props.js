// From https://twitter.com/LeaVerou/status/998714954386755590
export const listSupportedCSSProps = () => Object.keys(document.body.style).map(p => p.replace(/[A-Z]/g, "-$&").toLowerCase())

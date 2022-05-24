const ecdsautil = require('./build/Release/ecdsautil.node')

module.exports = {
  sign: (key, msg) => {
    return ecdsautil.sign(Buffer.from(key, 'hex'), Buffer.from(msg)).toString('hex')
  }
}

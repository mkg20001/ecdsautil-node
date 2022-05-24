'use strict'

const ecdsautil = require('..')
const assert = require('assert').strict

describe('ecdsautil', () => {
  describe('sign', () => {
    it('can sign message', async () => {
      const sig = ecdsautil.sign('807a18531559c4408a80b3bea114549f1ae124c5c69119cc6d83735fa7ffe763', 'test')
      assert(sig === '6e8d7f7d86ec5c859f5b79093eb2f03d594c455e5901cf0234a6d053094a950f9e2d56c52172b66cba5ef6864e9a8f3d35dfad66e3a930cf4677bc7ef452e906')
    })
  })
})

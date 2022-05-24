#include <ios>
#include "binding.h"

using namespace Napi;

unsigned char * ecdsa_sign(const unsigned char * privkey, const unsigned char * msg, size_t msg_length) {
  unsigned char * out = static_cast<unsigned char *>(malloc(65));

  ecc_int256_t key_packed;
  memcpy(key_packed.p, privkey, 32);

  ecc_int256_t hash;

  ecdsa_sha256_context_t hash_ctx;
  ecdsa_sha256_init(&hash_ctx);
  ecdsa_sha256_update(&hash_ctx, msg, msg_length);

  ecdsa_sha256_final(&hash_ctx, hash.p);

  ecdsa_signature_t signature;

  ecdsa_sign_legacy(&signature, &hash, &key_packed);

  memcpy(out, signature.r.p, 32);
  memcpy(out + 32, signature.s.p, 32);

  return out;

  fail:
    free(out);
    return nullptr;
}

Napi::Value Sign(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments. Must be 2.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsBuffer() || !info[1].IsBuffer()) {
    Napi::TypeError::New(env, "Wrong arguments. Must be buffers.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (info[0].As<Napi::Buffer<unsigned char>>().Length() != 32) {
    Napi::TypeError::New(env, "Invalid privatekey length. Must be 32.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  unsigned char * res;

  res = ecdsa_sign(info[0].As<Napi::Buffer<unsigned char>>().Data(), info[1].As<Napi::Buffer<unsigned char>>().Data(), info[1].As<Napi::Buffer<char>>().Length());

  if (!res) {
    Napi::Error::New(env, "Signing failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Buffer<unsigned char>::Copy(env, res, 64);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "sign"),
              Napi::Function::New(env, Sign));

  return exports;
}

NODE_API_MODULE(ecdsautil, Init)

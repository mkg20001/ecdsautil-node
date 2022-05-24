#include <ios>
#include "binding.h"

using namespace Napi;

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

  auto key = info[0].As<Napi::Buffer<unsigned char>>();
  auto msg = info[1].As<Napi::Buffer<unsigned char>>();

  if (key.Length() != 32) {
    Napi::TypeError::New(env, "Invalid privatekey length. Must be 32.")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  unsigned char * out = static_cast<unsigned char *>(malloc(64));

  ecc_int256_t key_packed;
  memcpy(key_packed.p, key.Data(), key.Length());

  ecc_int256_t hash;

  ecdsa_sha256_context_t hash_ctx;
  ecdsa_sha256_init(&hash_ctx);
  ecdsa_sha256_update(&hash_ctx, msg.Data(), msg.Length());

  ecdsa_sha256_final(&hash_ctx, hash.p);

  ecdsa_signature_t signature;

  ecdsa_sign_legacy(&signature, &hash, &key_packed);

  memcpy(out, signature.r.p, 32);
  memcpy(out + 32, signature.s.p, 32);

  return Napi::Buffer<unsigned char>::Copy(env, out, 64);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "sign"),
              Napi::Function::New(env, Sign));

  return exports;
}

NODE_API_MODULE(ecdsautil, Init)

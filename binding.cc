#include <ios>
#include <sstream>
#include "binding.h"

using namespace Napi;

char * ecdsa_sign(const char * privkey, const char * msg) {
  ecc_int256_t keydata;
  std::istringstream(privkey) >> std::hex >> keydata.p;

  ecc_25519_work_t key;

  ecc_25519_load_packed_legacy(&key, &keydata);
  if (!ecdsa_is_valid_pubkey(&key))
    goto fail;

  ecc_int256_t hash;

  ecdsa_sha256_context_t hash_ctx;
  ecdsa_sha256_init(&hash_ctx);
  ecdsa_sha256_update(&hash_ctx, msg, strlen(msg));

  ecdsa_sha256_final(&hash_ctx, hash.p);

  ecdsa_signature_t signature;

  ecdsa_sign_legacy(&signature, &hash, &keydata);

  char * sig;
  std::ostringstream(sig) << std::hex << signature.r.p << signature.s.p;

  return sig;

  fail:
    return nullptr;
}

Napi::Value Sign(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  char * res;
  res = ecdsa_sign(info[0].As<Napi::String>().Utf8Value().c_str(), info[1].As<Napi::String>().Utf8Value().c_str());

  if (!res) {
    Napi::Error::New(env, "Signing failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::String::New(env, res);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "sign"),
              Napi::Function::New(env, Sign));
}

NODE_API_MODULE(ecdsautil, Init)

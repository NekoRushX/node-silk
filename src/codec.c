#include <node_api.h>
#include <common.h>
#include <codec.h>

typedef int (*codec_func)(unsigned char* silkData, int dataLen, int sampleRate, cb_codec callback, void* userdata);

typedef struct codec_ctx {
  napi_env env;
  napi_value js_cb;
  napi_value this;
} codec_ctx_t;

void codec_cb(void* userdata, unsigned char* data, int len) {
  codec_ctx_t ctx = *(codec_ctx_t*)userdata;
  napi_value buf;
  napi_create_buffer_copy(ctx.env, len, data, NULL, &buf);
  napi_call_function(ctx.env, ctx.this, ctx.js_cb, 1, &buf, napi_undefined);
}

napi_value codec(napi_env env, napi_callback_info info, codec_func func) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  // get buffer
  size_t len;
  uint8_t* buf;
  napi_get_buffer_info(env, argv[0], (void**)&buf, &len);

  // get callback
  napi_value js_cb = argv[1];

  napi_value this;
  napi_get_global(env, &this);
  codec_ctx_t ctx = { env, js_cb, this };

  // encode
  int result = (*func)(buf, len, 24000, codec_cb, &ctx);
  if (!result)
    napi_throw_type_error(env, "node-silk", "Failed to encode/decode silk.");

  return napi_undefined;
}

napi_value silk_encode(napi_env env, napi_callback_info info) {
  return codec(env, info, silkEncode);
}

napi_value silk_decode(napi_env env, napi_callback_info info) {
  return codec(env, info, silkDecode);
}

static napi_value ModuleInit(napi_env env, napi_value exports)
{
  napi_property_descriptor _napi_prop[] = {
    {
        .utf8name = "silkEncode",
        .method = silk_encode,
        .attributes = napi_default,
    },
    {
        .utf8name = "silkDecode",
        .method = silk_decode,
        .attributes = napi_default,
    }
  };

  napi_define_properties(env, exports, 2, _napi_prop);
  return exports;
}

NAPI_MODULE(nodesilk, ModuleInit)

#include <node_api.h>
#include <assert.h>
#include <common.h>
#include <codec.h>

typedef struct codec_ctx {
  napi_env env;
  napi_value func;
} codec_ctx_t;

void _silkCodec(void* userdata, unsigned char* data, int len) {
  napi_status status;
  codec_ctx_t ctx = *(codec_ctx_t*)userdata;
  napi_value buf = NULL;
  napi_create_buffer(ctx.env, len, &data, &buf);
  napi_call_function(ctx.env, napi_null, ctx.func, 1, &buf, napi_undefined);
  assert(status == napi_ok);
}

napi_value _silkEncode(napi_env env, napi_callback_info info) {

  napi_status status;
  size_t argc = 2;
  napi_value argv[2];

  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  assert(status == napi_ok);

  // get buffer
  size_t len;
  uint8_t* buf;
  status = napi_get_buffer_info(env, argv[0], (void **)&buf, &len);
  assert(status == napi_ok);

  // get callback
  napi_value func = argv[1];

  codec_ctx_t ctx = { env, func };

  // encode
  int result = silkEncode(buf, len, 24000, _silkCodec, &ctx);
  if (!result)
    napi_throw_type_error(env, "node-silk", "Failed to encode silk.");

  return napi_undefined;
}

napi_value _silkDecode(napi_env env, napi_callback_info info) {
  return 0;
}

static napi_value ModuleInit(napi_env env, napi_value exports)
{
  napi_property_descriptor _napi_prop[] = {
    {
        .utf8name = "silkEncode",
        .method = _silkEncode,
        .attributes = napi_default,
    },
    {
        .utf8name = "silkDecode",
        .method = _silkDecode,
        .attributes = napi_default,
    }
  };

  napi_define_properties(env, exports, 2, _napi_prop);
  return exports;
}

NAPI_MODULE(nodesilk, ModuleInit)

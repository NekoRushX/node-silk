#include <node_api.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <common.h>
#include <codec.h>

#define NAPI(x) assert(x == napi_ok);

typedef struct codec_data {
  struct codec_data* next;
  uint8_t* block;
  size_t size;
} codec_data_t;

typedef struct codec_ctx {
  size_t total;
  codec_data_t *entry;
  codec_data_t *end;
} codec_ctx_t;

void _silkCodec(void* userdata, unsigned char* p, int len) {
  codec_ctx_t* _context = (codec_ctx_t*)userdata;
  {
    // create a block
    codec_data_t *_block = malloc(sizeof(codec_data_t));
    memset(_block, 0x00, sizeof(codec_data_t));

    // copy data to block
    _block->block = malloc(len);
    memcpy(_block->block, p, len);

    // set length
    _block->size = len;

    // update the context
    if(!_context->entry)
      _context->entry = _block;

    if(!_context->end)
      _context->end = _block;
    else {
      _context->end->next = _block;
      _context->end = _block;
    }
     
    _context->total += len;
  }
}

napi_value _silkEncode(napi_env env, napi_callback_info info) {

  size_t _argc = 1;
  napi_value _args[1];

  // get arguments passed in
  NAPI(napi_get_cb_info(env, info, &_argc, _args, NULL, NULL));
  {
    if (_argc != 1)
    {
      napi_throw_type_error(env, "node-silk", "Wrong number of arguments.");
      return napi_undefined;
    }
  }

  // get the buffer
  size_t _length;
  void *_buffer = NULL;
  NAPI(napi_get_buffer_info(env, _args[0], &_buffer, &_length));

  // make userdata
  codec_ctx_t* _data = malloc(sizeof(codec_ctx_t));
  memset(_data, 0x00, sizeof(codec_ctx_t));

  // encode
  int result = silkEncode((unsigned char *)_buffer, _length, 24000, _silkCodec, _data);
  if (!result) {
    napi_throw_type_error(env, "node-silk", "Failed to encode.");
    return napi_undefined;
  } // make sure clean the chains

  // collect and destroy blocks
  codec_data_t* _next = _data->entry;
  void* _buffer_pos = malloc(_data->total);

  _buffer = _buffer_pos;
  
  FILE * _fd = fopen("out.slk", "a+");

  while(_next) {

    // copy the data
    memcpy(_buffer_pos, _next->block, _next->size);
    _buffer_pos += _next->size;

    // the next
    codec_data_t *_nxt = _next->next;

    // destroy the block
    free(_next->block);
    free(_next);

    _next = _nxt;
  }

  fwrite(_buffer, _data->total, 1, _fd);
  fclose(_fd);

  // create buffer object
  napi_value _n_buffer = NULL;
  NAPI(napi_create_buffer_copy(env, _data->total, _buffer, NULL, &_n_buffer));

  return _n_buffer;
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

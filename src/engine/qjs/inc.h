extern "C" {
  #define JS_STRICT_NAN_BOXING
  #define CONFIG_BIGNUM
  #include "quickjs/quickjs-libc.h"
  #include "quickjs/quickjs.h"

  #define JS_CPP_FUNC_DEF(_name, length, func1) {\
    .name = _name,\
    .prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE,\
    .def_type = JS_DEF_CFUNC,\
    .magic= 0,\
    .u = {\
      .func = {\
        length,\
        JS_CFUNC_generic,\
        { .generic = func1 }\
      }\
     }\
    }
}

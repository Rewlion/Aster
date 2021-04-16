#pragma once

#define __REFLECTION_TAG "__has_reflection;"

#define CLASS(...) class [[clang::annotate(__REFLECTION_TAG # __VA_ARGS__)]]
#define PROPERTY(...) [[clang::annotate(__REFLECTION_TAG # __VA_ARGS__)]]

#define GENERATED_BODY() \
    friend class Meta::TypeAccessor;\
  public:\
    static const struct Meta::Class* StaticClass;\
    void ___test_func(){};
#ifndef MATH_HLSL
#define MATH_HLSL

#define FLOAT_MAX 3.402823466e+38

template<typename T>
void swap(inout T l, inout T r)
{
  T m = l;
  l = r;
  r = m;
}

#endif

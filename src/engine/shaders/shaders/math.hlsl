#ifndef MATH_HLSL
#define MATH_HLSL

#define FLOAT_MAX 3.402823466e+38

#ifndef PI
#define PI (float)3.14159265359
#endif

template<typename T>
void swap(inout T l, inout T r)
{
  T m = l;
  l = r;
  r = m;
}

#endif

#ifndef NOISE_HLSL
#define NOISE_HLSL

// Hash Functions for GPU Rendering, Jarzynski et al.
// http://www.jcgt.org/published/0009/03/02/
float3 random_pcg3d(uint3 v)
{
  v = v * 1664525u + 1013904223u;
  v.x += v.y*v.z; v.y += v.z*v.x; v.z += v.x*v.y;
  v ^= v >> 16u;
  v.x += v.y*v.z; v.y += v.z*v.x; v.z += v.x*v.y;
  return uint3(v) * (1.0/float(0xffffffffu));
}

//https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
float random_pcg(uint input)
{
  uint state = input * 747796405u + 2891336453u;
  uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
  uint res = (word >> 22u) ^ word;

  return res * (1.0/float(0xffffffffu));
}

#endif
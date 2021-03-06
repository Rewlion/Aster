#version 450

layout(location = 0 ) in vec3 texPosition;

layout(location = 0) out vec4 outBaseColor;

layout(set = 0, binding = 1) uniform samplerCube SkyboxTexture;

void main()
{
  outBaseColor = texture(SkyboxTexture, texPosition);
}
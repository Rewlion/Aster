#pragma once

#include <engine/types.h>

namespace gapi
{
  class CmdEncoder;
}

enum class ModelPass
{
  Depth,
  Main
};

void render_models(gapi::CmdEncoder&, const float4x4& view_proj, const ModelPass);

#include <engine/engine.h>
#include <engine/render/gapi.h>

int main()
{
  Engine::Init();

  gapi::init();
  gapi::CommandList cmdList;
  cmdList.push_back(gapi::BeginRenderPassCmd{});
  gapi::submitCommands(std::move(cmdList));

  Engine::StartTick();
}

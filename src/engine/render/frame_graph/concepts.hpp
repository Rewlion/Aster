#pragma once

#include <type_traits>
#include <functional>

namespace gapi
{
  class CmdEncoder;
}

namespace fg
{
  class RenderPassBuilder;
  class RenderPassResources;

  template<class T, class Data>
  concept SetupCbConcept = std::is_convertible<T, std::function<void(RenderPassBuilder&, Data&)>>::value;
  template<class T, class Data>
  concept ExecCbConcept = std::is_convertible<T, std::function<void(const Data&, const RenderPassResources&, gapi::CmdEncoder&)>>::value;

  template<class T>
  concept NoDataSetupCbConcept = std::is_convertible<T, std::function<void(RenderPassBuilder&)>>::value;
  template<class T>
  concept NoDataExecCbConcept = std::is_convertible<T, std::function<void(const RenderPassResources&, gapi::CmdEncoder&)>>::value;
}

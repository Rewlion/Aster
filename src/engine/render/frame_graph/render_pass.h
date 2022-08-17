#pragma once

#include "handles.h"

namespace gapi
{
  class CmdEncoder;
}

namespace fg
{
  class RenderPassResources;

  struct NoDataTag {};

  class Node
  {
    public:
      Node(const NodeHandle id);
      virtual ~Node() {};

      inline NodeHandle getId() const { return m_Id; }
      virtual void exec(const RenderPassResources& resources, gapi::CmdEncoder& encoder) = 0;

    private:
      NodeHandle m_Id;
  };

  template <class Data, class ExecCallback>
  class RenderPass: public Node
  {
    friend class RenderPassBuilder;
    friend class FrameGraph;
    public:
      RenderPass(const NodeHandle id, ExecCallback&& cb);

      virtual void exec(const RenderPassResources& resources, gapi::CmdEncoder& encoder) override;

    private:
      ExecCallback m_ExecCallback;
      Data m_Data;
  };

  template<class ExecCallback>
  class RenderPass<NoDataTag, ExecCallback>: public Node
  {
    friend class RenderPassBuilder;
    friend class FrameGraph;
    public:
      RenderPass(const NodeHandle id, ExecCallback&& cb);

      virtual void exec(const RenderPassResources& resources, gapi::CmdEncoder& encoder) override;

    private:
      ExecCallback m_ExecCallback;
  };
}

#include "render_pass.inc"

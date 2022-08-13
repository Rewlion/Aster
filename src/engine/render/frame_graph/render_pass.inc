namespace fg
{
  template<class Data, class ExecCallback>
  RenderPass<Data, ExecCallback>::RenderPass(const NodeHandle id, ExecCallback&& cb)
        : Node(id)
        , m_ExecCallback(cb)
  {
  }

  template<class Data, class ExecCallback>
  void RenderPass<Data, ExecCallback>::exec(const RenderPassResources& resources, gapi::CmdEncoder& encoder)
  {
    m_ExecCallback(m_Data, resources, encoder);
  }
}

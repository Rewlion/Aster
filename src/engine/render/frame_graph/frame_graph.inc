namespace fg
{
  template<class Data, class SetupCallback, class ExecCallback>
  requires SetupCbConcept<SetupCallback, Data> &&
           ExecCbConcept<ExecCallback, Data>
  Data& FrameGraph::addCallbackPass(const std::string_view name, SetupCallback&& setup, ExecCallback&& exec)
  {
    const NodeHandle rpId = (NodeHandle)m_RenderPasses.size();
    RenderPass<Data, ExecCallback>* rp = new RenderPass<Data, ExecCallback>{rpId, std::forward<ExecCallback>(exec)};
    m_RenderPasses.push_back(std::unique_ptr<Node>(rp));
    m_RenderPassPins.push_back(RenderPassPins{});

    RenderPassBuilder builder(*this, m_RenderPassPins.back(), *m_RenderPasses.back());
    setup(builder, rp->m_Data);

    return rp->m_Data;
  }
}

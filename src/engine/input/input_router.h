#pragma once

#include <engine/input/drivers/drivers.h>

#include <queue>
#include <EASTL/vector.h>

namespace Engine::Input
{
  enum class InputRouterProcessStatus
  {
    ProcessFurther,
    StopProcessing
  };

  class IInputRouterListener
  {
    public:
      virtual int getInputRouterPriority() = 0;

      virtual
      auto onMouseButtonStateChanged(const IPointer& device,
                                     const int key, 
                                     const bool pressed) -> InputRouterProcessStatus { return InputRouterProcessStatus::ProcessFurther; }

      virtual
      auto onMouseMove(const IPointer& device,
                       const int2 new_pos,
                       const int2 delta) -> InputRouterProcessStatus { return InputRouterProcessStatus::ProcessFurther; }

      virtual
      auto onKeyStateChanged(const IKeyboard& device,
                             const int key,
                             const bool pressed) -> InputRouterProcessStatus { return InputRouterProcessStatus::ProcessFurther; }
  };

  struct InputRouterPrioritySort
  {
    bool operator()(IInputRouterListener* l, IInputRouterListener* r)
    {
      return l->getInputRouterPriority() < r->getInputRouterPriority();
    };
  };

  using InputPriorityQueueBase = std::priority_queue<IInputRouterListener*,
                                   eastl::vector<IInputRouterListener*>,
                                   InputRouterPrioritySort>;
  class InputPriorityQueue: public InputPriorityQueueBase
  {
    public:
      using container_type = eastl::vector<IInputRouterListener*>;

      auto begin() -> container_type::iterator { return InputPriorityQueueBase::c.begin(); }
      auto end() -> container_type::iterator { return InputPriorityQueueBase::c.end(); }
  };

  class InputRouter : public IPointerListener, IKeyboardListener
  {
    public:
      InputRouter();
      ~InputRouter();

      virtual
      void onMouseButtonStateChanged(const IPointer& device,
                                     const int key,
                                     const bool pressed) override;

      virtual
      void onMouseMove(const IPointer& device,
                       const int2 new_pos,
                       const int2 delta) override;

      virtual
      void onKeyStateChanged(const IKeyboard& device,
                             const int key,
                             const bool pressed) override;

      static
      void registerListener(IInputRouterListener*);
      
      static
      void unregisterListener(IInputRouterListener*);
    
      static void init();
      static void destroy();

    private:
      static InputRouter* m_Router;

      InputPriorityQueue m_Listeners;
  };
}

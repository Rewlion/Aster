#pragma once

#include <engine/input/drivers/drivers.h>

#include <EASTL/vector_multiset.h>

#include <limits>

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
      constexpr static int PRIORITY_DONT_CARE = std::numeric_limits<int>::max();

      virtual
      auto getInputRouterPriority() -> int = 0;

      virtual
      auto onMouseButtonStateChanged(const IPointer& device,
                                     const int key, 
                                     const bool pressed) -> InputRouterProcessStatus { return InputRouterProcessStatus::ProcessFurther; }

      virtual
      auto onMouseMove(const IPointer& device,
                       const int2 new_pos,
                       const int2 delta) -> InputRouterProcessStatus { return InputRouterProcessStatus::ProcessFurther; }

      virtual
      auto onMouseWheelScroll(const IPointer& device, const float delta)
        -> InputRouterProcessStatus { return InputRouterProcessStatus::ProcessFurther; }

      virtual
      auto onKeyStateChanged(const IKeyboard& device,
                             const int key,
                             const bool pressed) -> InputRouterProcessStatus { return InputRouterProcessStatus::ProcessFurther; }
  };

  class Router : public IPointerListener, IKeyboardListener
  {
    public:
      Router();
      ~Router();

      void onMouseButtonStateChanged(const IPointer& device,
                                     const int key,
                                     const bool pressed) override;

      void onMouseMove(const IPointer& device,
                       const int2 new_pos,
                       const int2 delta) override;

      void onMouseWheelScroll(const IPointer& device, const float delta) override;

      void onKeyStateChanged(const IKeyboard& device,
                             const int key,
                             const bool pressed) override;

      static
      void registerListener(IInputRouterListener*);

      static
      void unregisterListener(IInputRouterListener*);
    
      static
      void init();

      static
      void destroy();

    private:
      static Router* m_Router;

      struct RoutingComparator
      {
        auto operator()(IInputRouterListener* l, IInputRouterListener* r) const -> bool
        {
          return l->getInputRouterPriority() < r->getInputRouterPriority();
        }
      };
      eastl::vector_multiset<IInputRouterListener*, RoutingComparator> m_Listeners;
  };
}

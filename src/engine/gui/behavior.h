#pragma once

#include "constants.h"

#include <engine/qjs/forward_decl.h>

#include <EASTL/vector_map.h>

#include <memory>

namespace Engine::gui
{
  enum class BhvStateChange
  {
    OnMouseHoverBegin,
    OnMouseHoverEnd,
  };

  class Element;

  class IBehavior
  {
    public:
      virtual ~IBehavior(){};
      virtual BehaviorType getType() const = 0;
      virtual void onStateChange(Element&, const BhvStateChange) = 0;
  };

  class BehaviorsStorage
  {
    public:
      void init();
      IBehavior* getBehavior(const BehaviorType);
    private:
      eastl::vector_map<BehaviorType, IBehavior*> m_Behaviors;
  };

  class BehaviorRegistration
  {
    public:
      BehaviorRegistration(IBehavior*);

      template<class T>
      static void forEach(T cb)
      {
        const BehaviorRegistration* p = m_List;
        while(p)
        {
          cb(p->m_Bhv.get());
          p = p->m_Next;
        }
      }

    private:
      static BehaviorRegistration* m_List;
      BehaviorRegistration* m_Next;

      std::unique_ptr<IBehavior> m_Bhv;
  };

  #define REGISTER_BEHAVIOR(bhv) static BehaviorRegistration behavior_reg ## __LINE__ {new bhv}
}

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
    OnMouseClickBegin,
    OnMouseClickEnd
  };

  enum BhvResult: int
  {
    BHV_RES_NONE = 0,
    BHV_RES_PROCESSED = 1
  };

  enum BhvInputSupport: int
  {
    BHV_INPUT_NONE = 0,
    BHV_INPUT_MOUSE = 1,
  };

  class Element;

  class IBehavior
  {
    public:
      virtual ~IBehavior(){};
      virtual BehaviorType getType() const = 0;
      virtual BhvResult onMouseStateChange(Element&, const BhvStateChange, const float2 pos, const BhvResult) = 0;
      virtual BhvInputSupport getInputSupport() const { return BHV_INPUT_NONE; }
      virtual void onInit(Element&) {};
      virtual void onDestroy() {};
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

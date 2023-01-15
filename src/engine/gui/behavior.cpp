#include "behavior.h"

#include <engine/assert.h>

namespace Engine::gui
{
  BehaviorRegistration* BehaviorRegistration::m_List = nullptr;
  BehaviorRegistration::BehaviorRegistration(IBehavior* bhv)
  {
    m_Next = m_List;
    m_List = this;

    m_Bhv.reset(bhv);
  }

  void BehaviorsStorage::init()
  {
    m_Behaviors.clear();
    BehaviorRegistration::forEach([this](IBehavior* bhv) {
      ASSERT_FMT(m_Behaviors.find(bhv->getType()) == m_Behaviors.end(),
        "can't register Behavior: {}, it's already registered", bhv->getType());
      m_Behaviors.emplace(eastl::make_pair(bhv->getType(), bhv));
    });
  }

  IBehavior* BehaviorsStorage::getBehavior(const BehaviorType type)
  {
    const auto it = m_Behaviors.find(type);
    ASSERT_FMT(it != m_Behaviors.end(), "unsupported behavior {}", type);
    return it->second;
  }
}

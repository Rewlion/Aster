#pragma once

#include "input_handler.h"

namespace Engine::Input
{
  class Mouse: public InputHandler
  {
    public:
      virtual bool getAnalogStatus(const string_hash action, float2& delta) const override;

    private:
      virtual void processAnalogInput() override;

    private:
      float2 m_CurrentPosition;
      float2 m_Delta;
  };
}
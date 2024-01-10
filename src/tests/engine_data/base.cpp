#include "gtest/gtest.h"

#include <engine/log.h>

bool expect_ed_assert = false;

#define ED_ASSERT_FMT_RETURN(cond, ret_val, f, ...) if (!expect_ed_assert && !(cond)) FAIL() << fmt::format(f, __VA_ARGS__); return ret_val;
#include <engine/data/data.h>

class VC {};

TEST(EngineData, AddAttribute)
{
  expect_ed_assert = false;
  
  std::shared_ptr<TypesRegistry> registry{new TypesRegistry};
  registry->add<int>("i", ExpectedTypeValue::Int);

  TEngineData<VC> ed{registry};
  ed.addAttribute("int_val", "annotation", TEngineData<VC>::TrivialValue<int>{3}, "i");
}

TEST(EngineData, AddBadAttribute)
{
  expect_ed_assert = true;
  
  std::shared_ptr<TypesRegistry> registry{new TypesRegistry};
  registry->add<int>("i", ExpectedTypeValue::Int);

  TEngineData<VC> ed{registry};
  ed.addAttribute("int_val", "annotation", TEngineData<VC>::TrivialValue<float>{3.0}, "i");
}

TEST(EngineData, AddUnknownAttribute)
{
  expect_ed_assert = true;
  
  std::shared_ptr<TypesRegistry> registry{new TypesRegistry};
  registry->add<int>("i", ExpectedTypeValue::Int);

  TEngineData<VC> ed{registry};
  ed.addAttribute("int_val", "annotation", TEngineData<VC>::TrivialValue<int>{3}, "i2");
}

#include "gtest/gtest.h"

#include <engine/log.h>

#include <engine/data/data.h>


TEST(EngineData, Variable)
{
  EngineData ed;
  EXPECT_TRUE(ed.addVariable("intvar", "annotation", 3));
  EXPECT_TRUE(ed.addVariable("textvar", "annotation", "valuee"));

  static_assert(std::is_same_v<int, decltype(ed.getVariable<int>("intvar"))>);
  
  EXPECT_EQ(ed.getVariable<int>("intvar"), 3);
}

TEST(EngineData, VariableDefinition)
{
  EngineData ed;
  EXPECT_TRUE(ed.addVariable("intvar", "kiki", int2(2,5)));

  const EngineData::Variable* var = ed.getVariableDefinition<int2>("intvar");
  EXPECT_NE(var, nullptr);
  EXPECT_EQ(var->getValueType(), EngineData::ValueType::Int2);
  EXPECT_EQ(var->name, "intvar");
  EXPECT_EQ(var->annotation, "kiki");
  EXPECT_TRUE(std::holds_alternative<int2>(var->value));
  EXPECT_EQ(std::get<int2>(var->value), int2(2,5));
}


TEST(EngineData, ObjectVariable)
{
  std::shared_ptr<CustomTypeRegistry> registry{new CustomTypeRegistry};

  struct TestObj
  {
    TestObj(){};
    TestObj(const EngineData* data)
    {
      EXPECT_NE(data, nullptr);
      i = data->getVariable<int>("intvar");
      j = data->getVariable<string>("unknowntextvar");
      k = data->getVariable<string>("textvar");
    }

    int i = 0;
    string j = "would be cleared";
    string k;
  };
  registry->add<TestObj>("TestObj");

  struct TestObjNoData
  {
    TestObjNoData(){};
    TestObjNoData(const EngineData* data)
    {
      EXPECT_EQ(data, nullptr);
    }
  };
  registry->add<TestObjNoData>("TestObjNoData");

  std::unique_ptr<EngineData> ctor{new EngineData};
  EXPECT_TRUE(ctor->addVariable("intvar", "annotation", 3));
  EXPECT_TRUE(ctor->addVariable("textvar", "", "kiki"));

  EngineData ed{registry};
  EXPECT_TRUE(ed.addVariable("objvar", "annotation", "TestObj", std::move(ctor)));
  EXPECT_TRUE(ed.addVariable("objvarnodata", "annotation", "TestObjNoData", nullptr));
  EXPECT_FALSE(ed.addVariable("unknown obj var", "annotation", "Foo2", nullptr));
  
  struct UnknownType {};
  EXPECT_FALSE(ed.addVariable<UnknownType>("somevar", "123", nullptr));
  EXPECT_TRUE(ed.addVariable<TestObjNoData>("a", "asd", nullptr));

  static_assert(std::is_same_v<TestObj, decltype(ed.getVariable<TestObj>("objvar"))>);
  TestObj obj = ed.getVariable<TestObj>("objvar");
  EXPECT_EQ(obj.i, 3);
  EXPECT_EQ(obj.j, "");
  EXPECT_EQ(obj.k, "kiki");

  TestObjNoData objnodata = ed.getVariable<TestObjNoData>("objvarnodata");
}
#include "gtest/gtest.h"

#include <engine/log.h>

#include <engine/data/data.h>


TEST(EngineData, Variable)
{
  TEngineData ed;
  EXPECT_TRUE(ed.addVariable("intvar", "annotation", 3));
  EXPECT_TRUE(ed.addVariable("textvar", "annotation", "valuee"));

  static_assert(std::is_same_v<int, decltype(ed.getVariable<int>("intvar"))>);
  
  EXPECT_EQ(ed.getVariable<int>("intvar"), 3);
}

TEST(EngineData, ObjectVariable)
{
  std::shared_ptr<CustomTypeRegistry> registry{new CustomTypeRegistry};

  struct TestObj
  {
    TestObj(){};
    TestObj(const TEngineData* data)
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
    TestObjNoData(const TEngineData* data)
    {
      EXPECT_EQ(data, nullptr);
    }
  };
  registry->add<TestObjNoData>("TestObjNoData");

  std::unique_ptr<TEngineData> ctor{new TEngineData};
  EXPECT_TRUE(ctor->addVariable("intvar", "annotation", 3));
  EXPECT_TRUE(ctor->addVariable("textvar", "", "kiki"));

  TEngineData ed{registry};
  EXPECT_TRUE(ed.addVariable("objvar", "annotation", TEngineData::TypeConstructor{"TestObj",std::move(ctor)}));
  EXPECT_TRUE(ed.addVariable("objvarnodata", "annotation", TEngineData::TypeConstructor{"TestObjNoData",nullptr}));

  EXPECT_FALSE(ed.addVariable("unknown obj var", "annotation", TEngineData::TypeConstructor{"Foo2"}));

  static_assert(std::is_same_v<TestObj, decltype(ed.getVariable<TestObj>("objvar"))>);
  TestObj obj = ed.getVariable<TestObj>("objvar");
  EXPECT_EQ(obj.i, 3);
  EXPECT_EQ(obj.j, "");
  EXPECT_EQ(obj.k, "kiki");

  TestObjNoData objnodata = ed.getVariable<TestObjNoData>("objvarnodata");
}
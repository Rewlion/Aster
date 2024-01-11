#include "gtest/gtest.h"

#include <engine/log.h>

#include <engine/data/ed.h>


TEST(EngineData, Variable)
{
  ed::Scope ed;
  EXPECT_TRUE(ed.addVariable("i",  "a",  3));
  EXPECT_TRUE(ed.addVariable("i2", "a2", int2(3,3)));
  EXPECT_TRUE(ed.addVariable("i3", "a3", int3(3,3,3)));
  EXPECT_TRUE(ed.addVariable("i4", "a4", int4(3,3,3,3)));
  EXPECT_TRUE(ed.addVariable("f",  "a5",  3.0f));
  EXPECT_TRUE(ed.addVariable("f2", "a6", float2(3,3)));
  EXPECT_TRUE(ed.addVariable("f3", "a7", float3(3,3,3)));
  EXPECT_TRUE(ed.addVariable("f4", "a8", float4(3,3,3,3)));
  EXPECT_TRUE(ed.addVariable("t", "a9", "text"));

  static_assert(std::is_same_v<int, decltype(ed.getVariable<int>("intvar"))>);
  
  EXPECT_EQ(ed.getVariable<int>("i"),  3);
  EXPECT_EQ(ed.getVariable<int2>("i2"), int2(3,3));
  EXPECT_EQ(ed.getVariable<int3>("i3"), int3(3,3,3));
  EXPECT_EQ(ed.getVariable<int4>("i4"), int4(3,3,3,3));
  EXPECT_EQ(ed.getVariable<float>("f"),  3.0f);
  EXPECT_EQ(ed.getVariable<float2>("f2"), float2(3,3));
  EXPECT_EQ(ed.getVariable<float3>("f3"), float3(3,3,3));
  EXPECT_EQ(ed.getVariable<float4>("f4"), float4(3,3,3,3));
  EXPECT_EQ(ed.getVariable<string>("t"), string{"text"});
}

TEST(EngineData, VariableDefinition)
{
  ed::Scope ed;
  EXPECT_TRUE(ed.addVariable("intvar", "kiki", int2(2,5)));

  const ed::Variable* var = ed.getVariableDefinition<int2>("intvar");
  EXPECT_NE(var, nullptr);
  EXPECT_EQ(var->getValueType(), ed::ValueType::Int2);
  EXPECT_EQ(var->name, "intvar");
  EXPECT_EQ(var->annotation, "kiki");
  EXPECT_TRUE(std::holds_alternative<int2>(var->value));
  EXPECT_EQ(std::get<int2>(var->value), int2(2,5));
}


TEST(EngineData, ObjectVariable)
{
  std::shared_ptr<ed::CustomTypeRegistry> registry{new ed::CustomTypeRegistry};

  struct TestObj
  {
    TestObj(){};
    TestObj(const ed::Scope* data)
    {
      EXPECT_NE(data, nullptr);
      if (data)
      {
        i = data->getVariable<int>("intvar");
        j = data->getVariable<string>("unknowntextvar");
        k = data->getVariable<string>("textvar");
      }
    }

    int i = 0;
    string j = "would be cleared";
    string k;
  };
  registry->add<TestObj>("TestObj");

  struct TestObjNoData
  {
    TestObjNoData(){};
    TestObjNoData(const ed::Scope* data)
    {
      EXPECT_EQ(data, nullptr);
    }
  };
  registry->add<TestObjNoData>("TestObjNoData");

  std::unique_ptr<ed::Scope> ctor{new ed::Scope};
  EXPECT_TRUE(ctor->addVariable("intvar", "annotation", 3));
  EXPECT_TRUE(ctor->addVariable("textvar", "", "kiki"));

  ed::Scope ed{registry};
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


TEST(EngineData, NestedScopes)
{
  ed::Scope s1{"a", "a1"};
  EXPECT_TRUE(s1.addVariable<int>("v1", "", 1));

  ed::Scope s2{"bdc", "a2"};
  EXPECT_TRUE(s2.addVariable<int>("v2", "", 2));
  EXPECT_TRUE(s2.addVariable<int>("v22", "", 22));
  EXPECT_TRUE(s2.addVariable<int>("v222", "", 222));
  EXPECT_TRUE(s2.addVariable<int>("v2222", "", 2222));

  ed::Scope s3{"ker", "a3"};
  EXPECT_TRUE(s3.addVariable<int>("v3", "", 3));
  EXPECT_TRUE(s3.addVariable<int>("v33", "", 36));

  s2.addScope(std::move(s3));
  s1.addScope(std::move(s2));

  ed::Scope& fS3 = s1.getScope("bdc/ker");
  EXPECT_TRUE(!fS3.isEmpty());
  EXPECT_EQ(fS3.getVariable<int>("v3"), 3);

  ed::Scope& fS2 = s1.getScope("bdc");
  EXPECT_TRUE(!fS2.isEmpty());
  EXPECT_EQ(fS2.getVariable<int>("v2"), 2);
  EXPECT_EQ(fS2.getAnnotation(), "a2");

  ed::Scope& fS23 = fS2.getScope("ker");
  EXPECT_TRUE(!fS23.isEmpty());
  EXPECT_EQ(fS23.getVariable<int>("v3"), 3);
  EXPECT_EQ(fS23.getAnnotation(), "a3");

  EXPECT_EQ(s1.getScopes().size(), 1);
  EXPECT_EQ(s1.getVariables().size(), 1);
  EXPECT_EQ(fS2.getScopes().size(), 1);
  EXPECT_EQ(fS2.getVariables().size(), 4);
  EXPECT_EQ(fS23.getScopes().size(), 0);
  EXPECT_EQ(fS23.getVariables().size(), 2);
}
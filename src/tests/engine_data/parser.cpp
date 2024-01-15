#include "gtest/gtest.h"

#include <engine/log.h>

#include <engine/data/ed.h>
#include <engine/data/parser/parser.h>

TEST(EngineDataParser, Variable)
{
  ed::Parser parser{};
  ed::Scope ed = parser.parseFile("cases/engine_data/variables.ed", true);

  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  EXPECT_EQ(ed.getVariable<bool>("b1var"), true);
  EXPECT_EQ(ed.getVariable<bool>("b2var"), false);
  EXPECT_EQ(ed.getVariable<bool>("b3var"), true);
  EXPECT_EQ(ed.getVariable<bool>("b4var"), false);
  EXPECT_EQ(ed.getVariable<bool>("b5var"), true);
  EXPECT_EQ(ed.getVariable<bool>("b6var"), false);

  EXPECT_EQ(ed.getVariable<int> ("intvar"), 4);
  EXPECT_EQ(ed.getVariable<int2>("int2var"), int2(2,5));
  EXPECT_EQ(ed.getVariable<int3>("int3var"), int3(6,6,6));
  EXPECT_EQ(ed.getVariable<int4>("int4var"), int4(1,2,3,4));

  EXPECT_EQ(ed.getVariable<float> ("floatvar"), 4.0f);
  EXPECT_EQ(ed.getVariable<float2>("float2var"), float2(2.0f,5.0f));
  EXPECT_EQ(ed.getVariable<float3>("float3var"), float3(6.0f,6.0f,6.0f));
  EXPECT_EQ(ed.getVariable<float4>("float4var"), float4(1.0f,2.0f,3.0f,4.0f));

  EXPECT_EQ(ed.getVariable<float3x3>("m3var"), float3x3{2.0});
  EXPECT_EQ(ed.getVariable<float4x4>("m4var"), float4x4{2.0});

  EXPECT_EQ(ed.getVariable<string>("textvar"), string("kek"));
  EXPECT_EQ(ed.getVariable<ed::IntArray>("iarr"), (ed::IntArray{1,2,3}));
  EXPECT_EQ(ed.getVariable<ed::FloatArray>("farr"), (ed::FloatArray{1.0f, 2.0f, 3.0f}));
  EXPECT_EQ(ed.getVariable<ed::TextArray>("tarr"), (ed::TextArray{"1", "2"}));
}

TEST(EngineDataParser, VariableAnnotation)
{
  ed::Parser parser{};
  ed::Scope ed = parser.parseFile("cases/engine_data/annotated_var.ed", true);
  
  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  const ed::Variable* var = ed.getVariableDefinition<int>("var");
  EXPECT_NE(var, nullptr);
  if (var)
  {
    EXPECT_EQ(var->name, string{"var"});
    EXPECT_EQ(var->annotation, string("annotation"));
  }
}

TEST(EngineDataParser, Scope)
{
  ed::Parser parser{};
  ed::Scope ed = parser.parseFile("cases/engine_data/scope.ed", true);
  
  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  const ed::Scope& s1 = ed.getScope("scope1");
  EXPECT_TRUE(!s1.isEmpty());

  const ed::Scope& s2 = ed.getScope("scope1/scope2");
  EXPECT_TRUE(!s2.isEmpty());

  const ed::Scope& s12 = s1.getScope("scope2");
  EXPECT_TRUE(!s12.isEmpty());

  EXPECT_EQ(s1.getVariable<int>("v1"), 200);
  EXPECT_EQ(s2.getVariable<string>("v2"), string{"asd"});
}

TEST(EngineDataParser, ScopeAnnotation)
{
  ed::Parser parser{};
  ed::Scope ed = parser.parseFile("cases/engine_data/annotated_scope.ed", true);

  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  EXPECT_EQ(ed.getScope("s").getAnnotation(), string{"annotation?"});
}

TEST(EngineDataParser, CustomType)
{
  auto reg = std::make_shared<ed::CustomTypeRegistry>();

  struct FooType
  {
    FooType(){};
    FooType(const ed::Scope* data)
    {
      if (data)
      {
        health = data->getVariable<int>("health");
        mana = data->getVariable<int>("mana");
      }
    }

    int health = 0;
    int mana = 0;
  };
  reg->add<FooType>("FooType");

  ed::Parser parser{reg};
  ed::Scope ed = parser.parseFile("cases/engine_data/custom_type.ed", true);

  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  FooType var = ed.getVariable<FooType>("ctvar");
  EXPECT_EQ(var.health, 200);
  EXPECT_EQ(var.mana, 10);
}

TEST(EngineDataParser, CustomTypeNested)
{
  auto reg = std::make_shared<ed::CustomTypeRegistry>();

  struct SpeedComponent
  {
    SpeedComponent(){};
    SpeedComponent(const ed::Scope* data)
    {
      if (data)
      {
        velocity = data->getVariable<float>("velocity");
      }
    }

    float velocity = 0;
  };
  reg->add<SpeedComponent>("SpeedComponent");

  struct FooType
  {
    FooType(){};
    FooType(const ed::Scope* data)
    {
      if (data)
      {
        health = data->getVariable<int>("health");
        mana = data->getVariable<int>("mana");
        speed = data->getVariable<SpeedComponent>("speed");
      }
    }

    int health = 0;
    int mana = 0;
    SpeedComponent speed;
  };
  reg->add<FooType>("FooType");

  ed::Parser parser{reg};
  ed::Scope ed = parser.parseFile("cases/engine_data/custom_type_nested.ed", true);

  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  FooType var = ed.getVariable<FooType>("ctvar");
  EXPECT_EQ(var.health, 200);
  EXPECT_EQ(var.mana, 10);
  EXPECT_EQ(var.speed.velocity, 200.0f);
}

TEST(EngineDataParser, Include)
{
  ed::Parser parser{};
  ed::Scope ed = parser.parseFile("cases/engine_data/include.ed", true);
  
  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  EXPECT_EQ(ed.getVariable<int>("v"), 1);
  EXPECT_EQ(ed.getVariable<int>("test"), 250);
  EXPECT_EQ(ed.getVariable<int>("test2"), 252);
}

TEST(EngineDataParser, ParamsOrder)
{
  ed::Parser parser{};
  ed::Scope ed = parser.parseFile("cases/engine_data/param_order.ed", true);
  
  if (!parser.isParsingOk())
  {
    FAIL() << parser.getErrors();
    return;
  }

  const eastl::vector<ed::Variable>& vars = ed.getVariables();
  const eastl::vector<ed::Scope>& scopes = ed.getScopes();

  EXPECT_EQ(vars.size(), 2);
  EXPECT_EQ(scopes.size(), 2);

  if (vars.size() != 2 || scopes.size() != 2)
    return;


  EXPECT_EQ(vars[0].name, "v1");
  EXPECT_EQ(vars[1].name, "v2");
  EXPECT_EQ(scopes[0].getName(), "s1");
  EXPECT_EQ(scopes[1].getName(), "s2");
}
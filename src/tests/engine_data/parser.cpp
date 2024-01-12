#include "gtest/gtest.h"

#include <engine/log.h>

#include <engine/data/ed.h>
#include <engine/data/parser/parser.h>

TEST(EngineDataParser, Variable)
{
  ed::CustomTypeRegistry reg;
  ed::Parser parser{reg};
  ed::Scope ed = parser.parseFile("cases/engine_data/variables.ed", true);

  if (!parser.isParsingOk())
    FAIL() << parser.getErrors();

  EXPECT_EQ(ed.getVariable<int> ("intvar"), 4);
  EXPECT_EQ(ed.getVariable<int2>("int2var"), int2(2,5));
  EXPECT_EQ(ed.getVariable<int3>("int3var"), int3(6,6,6));
  EXPECT_EQ(ed.getVariable<int4>("int4var"), int4(1,2,3,4));

  EXPECT_EQ(ed.getVariable<float> ("floatvar"), 4.0f);
  EXPECT_EQ(ed.getVariable<float2>("float2var"), float2(2.0f,5.0f));
  EXPECT_EQ(ed.getVariable<float3>("float3var"), float3(6.0f,6.0f,6.0f));
  EXPECT_EQ(ed.getVariable<float4>("float4var"), float4(1.0f,2.0f,3.0f,4.0f));

  EXPECT_EQ(ed.getVariable<string>("textvar"), string("kek"));
}

TEST(EngineDataParser, VariableAnnotation)
{
  ed::CustomTypeRegistry reg;
  ed::Parser parser{reg};
  ed::Scope ed = parser.parseFile("cases/engine_data/annotated_var.ed", true);
  
  if (!parser.isParsingOk())
    FAIL() << parser.getErrors();

  const ed::Variable* var = ed.getVariableDefinition<int>("var");
  EXPECT_NE(var, nullptr);
  if (var)
  {
    EXPECT_EQ(var->annotation, string("annotation"));
  }
}

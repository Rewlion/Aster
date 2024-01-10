#include "gtest/gtest.h"

#include <engine/log.h>
#include <engine/datablock/utils.h>

#define GET_CHILD_BK_AND_EXPECT_NOT_EMPTY(from, var, name)\
  auto* var = from.getChildBlock(name);                   \ 
  if (var->isEmpty()) FAIL() << "datablock is empty"

#define EXPECT_BK_EXIST(from, name)\
  if (from.getChildBlock(name)->isEmpty()) FAIL() << "datablock " << name << "doesn't exist"

#define EXPECT_BK_ATTR(attr, expected_type, getter_type, expected_value)                                \
  if (attr.type == expected_type)                                                                       \
  {                                                                                                     \
    const getter_type attrValue = std::get<getter_type>(attr.as);                                       \
    EXPECT_EQ(attrValue, expected_value)                                                                \
      << "invalid bk attribute value. Expected:" << expected_value << " Got:" << attrValue;             \
  }                                                                                                     \
  else                                                                                                  \
    FAIL() << "invalid bk attribute type. Expected:" << #expected_type << " got:" << attr.getTypeStr()

TEST(Engine, DataBlock)
{
  DataBlock bk;
  EXPECT_EQ(load_blk_from_file(&bk, "cases/blk_syntax_test.bk"), true);

  EXPECT_EQ(bk.getInt("imyvar"), 1);
  EXPECT_EQ(bk.getInt3("imyvar"), (int3{3,1,1}));
  EXPECT_EQ(bk.getBool("b1"), true);
  EXPECT_EQ(bk.getBool("b2"), true);
  EXPECT_EQ(bk.getBool("b3"), false);
  EXPECT_EQ(bk.getBool("b4"), false);
  EXPECT_EQ(bk.getText("txt"), (string{"text"}));

  EXPECT_EQ(bk.getInt("doesnt_exist"), 0);
  EXPECT_BK_EXIST(bk, "block1");
  EXPECT_BK_EXIST(bk, "block2");

  GET_CHILD_BK_AND_EXPECT_NOT_EMPTY(bk, annBk, "annotated_block1");
  EXPECT_EQ(annBk->getAnnotation(), (string{"annotation"}));
  
  auto annAttr = annBk->getAttribute("avar", DataBlock::Attribute::Type::Int);
  EXPECT_NE(annAttr, nullptr);
  EXPECT_EQ(annAttr->annotation, (string{"val"}));
  if (const int* v = std::get_if<int>(&annAttr->as))
    EXPECT_EQ(*v, 3);
  else
    FAIL() << "invalid annotated int attribute value";

  GET_CHILD_BK_AND_EXPECT_NOT_EMPTY(bk, nestedBk, "nested");
  GET_CHILD_BK_AND_EXPECT_NOT_EMPTY((*nestedBk), nested2Bk, "nested2");
  EXPECT_EQ(nested2Bk->getText("nested_text"), (string{"nested"}));
  EXPECT_BK_EXIST(bk, "nested/nested2");

  GET_CHILD_BK_AND_EXPECT_NOT_EMPTY(bk, orderBk, "order");
  const auto& orderAttrs = orderBk->getAttributes();
  EXPECT_EQ(orderAttrs.size(), 3);
  EXPECT_BK_ATTR(orderAttrs[0], DataBlock::Attribute::Type::Int, int, 1);
  EXPECT_BK_ATTR(orderAttrs[1], DataBlock::Attribute::Type::Int, int, 2);
  EXPECT_BK_ATTR(orderAttrs[2], DataBlock::Attribute::Type::Int, int, 3);
  
  const auto& autoOrderBks = orderBk->getChildBlocks();
  EXPECT_EQ(autoOrderBks.size(), 2);
  EXPECT_EQ(autoOrderBks[0].getName(), (string{"bk0"}));
  EXPECT_EQ(autoOrderBks[1].getName(), (string{"bk1"}));
}

int main(int argc, char **argv)
{
  Engine::InitLog();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

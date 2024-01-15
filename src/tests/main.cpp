#include "gtest/gtest.h"

#include <engine/log.h>

int main(int argc, char **argv)
{
  Engine::InitLog();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

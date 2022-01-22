#include "utils.h"

#include <engine/datablock/parser/parser.h>
#include <engine/datablock/datablock.h>

bool LoadBlkFromFile(DataBlock* blk, const char* path)
{
  BlkParser parser;
  *blk = parser.ParseFile(path);
  return true;
}
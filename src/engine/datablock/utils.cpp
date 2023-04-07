#include "utils.h"

#include <engine/datablock/parser/parser.h>
#include <engine/datablock/datablock.h>

bool load_blk_from_file(DataBlock* blk, const char* path)
{
  BlkParser parser;
  *blk = parser.ParseFile(path);
  return parser.isParsingOk();
}
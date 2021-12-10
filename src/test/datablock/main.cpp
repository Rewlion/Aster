#include <engine/datablock/parser/parser.h>
#include <engine/datablock/datablock.h>
#include <engine/log.h>

int main()
{
  ::init_log();

  BlkParser parser;
  DataBlock blk = parser.parse_file("templates.blk");
  DataBlock blk2 = parser.parse_file("templates.blk");

}
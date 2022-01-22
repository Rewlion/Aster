#include <engine/datablock/parser/parser.h>
#include <engine/datablock/datablock.h>
#include <engine/log.h>

int main()
{
  ::InitLog();

  BlkParser parser;
  DataBlock blk = parser.ParseFile("templates.blk");
  DataBlock blk2 = parser.ParseFile("templates.blk");

}
#include "gtest/gtest.h"
#include <runic_imp/Imp_Lexer.h>
#include <imp_summoning/Summoner.h>
#include <runic/File_Text_Source.h>
#include <overworld/schema/Dungeon.h>
#include <imp_mirror/Mirror.h>

using namespace std;
using namespace runic_imp;
using namespace imp_mirror;

TEST(Mirror_Test, mirroring) {

  Imp_Lexer lexer(new runic::File_Text_Source<>(string(RESOURCE_PATH) + "pizza.imp"));
  underworld::Dungeon root("", nullptr);
  underworld::Source_File source_file(string(RESOURCE_PATH) + "pizza.imp");
  imp_summoning::Stream stream(lexer, source_file);
  imp_summoning::Lookup lookup;
  imp_summoning::Summoner summoner(stream, lookup);
  summoner.summon(root);

  overworld::Dungeon output_root("");
  overworld::Profession_Library profession_library2;
  imp_mirror::Element_Map element_map;
  overworld::Graph graph;
//  Mirror mirror(profession_library2, element_map, graph);
//  mirror.reflect_dungeon(root, output_root);
//  EXPECT_EQ(1, output_root.get_dungeons().size());

}

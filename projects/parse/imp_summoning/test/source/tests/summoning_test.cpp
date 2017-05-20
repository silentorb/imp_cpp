#include "gtest/gtest.h"
#include <runic_imp/Imp_Lexer.h>
#include <imp_summoning/Summoner.h>
#include <runic/File_Text_Source.h>

using namespace std;
using namespace runic_imp;

TEST(Summoning_Test, summoning) {

  Imp_Lexer lexer(new runic::File_Text_Source<>(string(RESOURCE_PATH) + "pizza.imp"));
  underworld::Dungeon root("");
  underworld::Profession_Library profession_library;
  underworld::Source_File source_file(string(RESOURCE_PATH) + "pizza.imp");
  imp_summoning::Stream stream(lexer, source_file);
  imp_summoning::Lookup lookup(profession_library);
  imp_summoning::Summoner summoner(stream, lookup);
  summoner.summon(root);
  EXPECT_EQ(1, root.get_dungeons().size());

  auto &dungeon = root.get_dungeon("Pizza");
  EXPECT_EQ(1, dungeon.get_members().size());

}

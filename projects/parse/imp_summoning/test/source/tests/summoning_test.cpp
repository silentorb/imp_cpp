#include "gtest/gtest.h"
#include <runic_imp/Imp_Lexer.h>
#include <summoning/Summoner.h>
#include <runic/File_Text_Source.h>

using namespace std;
using namespace runic_imp;

TEST(Summoning_Test, summoning) {

  Imp_Lexer lexer(new runic::File_Text_Source<>(string(RESOURCE_PATH) + "pizza.imp"));
  underworld::Dungeon root("");
  underworld::Profession_Library profession_library;
  summoning::Stream stream(lexer);
  summoning::Summoner summoner(stream, profession_library);
  summoner.summon(root);
  EXPECT_EQ(1, root.get_dungeons().size());

  auto &dungeon = root.get_dungeon("Pizza");
  EXPECT_EQ(1, dungeon.get_members().size());

}

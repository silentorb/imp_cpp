#include "gtest/gtest.h"
#include <runic_imp/Imp_Lexer.h>
#include <summoning/Summoner.h>
#include <runic/File_Text_Source.h>
#include <overworld/schema/Dungeon.h>
#include <imp_mirror/Mirror.h>

using namespace std;
using namespace runic_imp;
using namespace imp_mirror;

TEST(Mirror_Test, mirroring) {

  Imp_Lexer lexer(new runic::File_Text_Source<>(string(RESOURCE_PATH) + "pizza.imp"));
  underworld::Dungeon root("");
  underworld::Profession_Library profession_library;
  summoning::Stream stream(lexer);
  summoning::Summoner summoner(stream, profession_library);
  summoner.summon(root);

  overworld::Dungeon output_root(root);
  Mirror mirror;
  mirror.reflect(root, output_root);
  EXPECT_EQ(1, output_root.get_dungeons().size());

}

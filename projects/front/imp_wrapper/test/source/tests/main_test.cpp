#include "gtest/gtest.h"
#include <runic_imp/Imp_Lexer.h>
#include <imp_summoning/Summoner.h>
#include <runic/File_Text_Source.h>
#include <overworld/schema/Dungeon.h>
#include <imp_mirror/Mirror.h>
#include <imp_wrapper/Wrapper.h>

using namespace std;
using namespace runic_imp;
using namespace imp_mirror;

TEST(Main_Test, full_process) {
  imp_wrapper::Wrapper wrapper;
  wrapper.load_file(string(RESOURCE_PATH) + "pizza.imp");
}

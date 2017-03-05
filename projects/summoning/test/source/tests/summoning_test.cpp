#include "gtest/gtest.h"
#include <runic_imp/Imp_Lexer.h>
#include <summoning/Summoner.h>
#include <runic/File_Text_Source.h>

using namespace std;
using namespace runic_imp;

TEST(Summoning_Test, summoning) {


  Imp_Lexer lexer(new runic::File_Text_Source<>(string(RESOURCE_PATH) + "pizza.imp"));
  underworld::Dungeon dungeon("");
  underworld::Profession_Library profession_library;
  summoning:: Stream stream(lexer);
  summoning::Summoner summoner(stream, profession_library);
  summoner.summon(dungeon);

////  Token token;
////  lexer.next_token(token);
//  vector<Token> tokens;
//  lexer.get_all_tokens(tokens);
//  auto &lexicon = Lexicon::get_instance();
//  EXPECT_EQ(&lexicon.patterns.If, tokens[14].get_match().get_type());
}

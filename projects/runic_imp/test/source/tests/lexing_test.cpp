#include <runic_imp/Imp_Lexer.h>
#include <runic/File_Text_Source.h>
#include "gtest/gtest.h"

using namespace std;
using namespace runic_imp;

TEST(Lexing_Test, test_test) {

  Imp_Lexer lexer(new runic::File_Text_Source<>(string(RESOURCE_PATH) + "pizza.imp"));
//  Token token;
//  lexer.next_token(token);
  vector<Token> tokens;
  lexer.get_all_tokens(tokens);
  auto &lexicon = Lexicon::get_instance();
  EXPECT_EQ(&lexicon.patterns.If, tokens[19].get_match().get_type());
}
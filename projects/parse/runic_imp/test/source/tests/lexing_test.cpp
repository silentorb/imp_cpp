#include <runic_imp/Imp_Lexer.h>
#include <runic/File_Text_Source.h>
#include "gtest/gtest.h"

using namespace std;
using namespace runic_imp;

TEST(Lexing_Test, test_test) {

  Imp_Lexer lexer(new runic::File_Text_Source<>(string(RESOURCE_PATH) + "pizza.imp"));
//  Token token;
//  parse.next_token(token);
  runic::Simple_Source source("pizza.imp");
  vector<Token> tokens;
  lexer.get_all_tokens(tokens, source);
  auto &lexicon = Lexicon::get_instance();
  EXPECT_EQ(&lexicon.patterns.If, tokens[14].get_match().get_type());
}

//class First {
//public:
//    virtual int get_value() {
//      return 0;
//    };
//};
//
//class Second : public First {
//public:
//    int get_value() override {
//      return 2;
//    }
//};
//
//TEST(Inheritance_Test, inheritance) {
//  Second second;
//  auto &first = *(First *) &second;
//  EXPECT_EQ(2, first.get_value());
//
//  First first2 = second;
//  First *first3 = &first2;
//  EXPECT_EQ(0, first2.get_value());
//  EXPECT_EQ(2, first3->get_value());
//}
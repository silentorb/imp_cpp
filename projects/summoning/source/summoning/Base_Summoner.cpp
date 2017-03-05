#include "Base_Summoner.h"
#include "exceptions.h"

namespace summoning {
  Base_Summoner::Base_Summoner(Stream &input, underworld::Profession_Library &profession_library) :
    input(input),
    lexicon(runic_imp::Lexicon::get_instance().patterns),
    profession_library(profession_library) {}

//  void Base_Summoner::require_next(Token &token) {
//    if (!lexer.next_token(token))
//      throw End_Of_File_Exception(token);
//  }
//
//  bool Base_Summoner::require_until(Token &token, const runic_imp::Whisper &whisper) {
//    if (!lexer.next_token(token))
//      throw Expected_Whisper_Exception(whisper, token);
//
//    return !token.is(whisper);
//  }
//
//  void Base_Summoner::expect(Token &token, const runic_imp::Whisper &whisper) {
//    if (!lexer.next_token(token) || !token.is(whisper))
//      throw Expected_Whisper_Exception(whisper, token);
//  }

}
#pragma once

#include <map>
#include <runic/common/Whisper.h>
#include <underworld/schema/Profession.h>
#include <underworld/schema/primitives.h>
#include <underworld/schema/Profession_Library.h>
#include <underworld/expressions/Operator.h>
#include <runic_imp/Imp_Lexicon.h>

namespace imp_summoning {

  using Operator_Map = std::map<const runic::Whisper *, underworld::Operator_Type>;

  class Paser_Lookup {
//      underworld::Profession_Library &profession_library;
      const runic_imp::Symbols &lexicon;
      std::map<const runic::Whisper *, underworld::Primitive_Type> primitive_map;
      Operator_Map expression_operator_map;
      Operator_Map assignment_operator_map;

  public:
      Paser_Lookup(const runic_imp::Symbols &lexicon);

      underworld::Primitive_Type get_primitive(const runic::Whisper *whisper);
      bool get_expression_operator(const runic::Whisper *whisper, underworld::Operator_Type &result);
      bool get_assignment_operator(const runic::Whisper *whisper, underworld::Operator_Type &result);

      const runic_imp::Symbols &get_lexicon() const {
        return lexicon;
      }
  };
}
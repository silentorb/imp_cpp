#pragma once

#include <map>
#include <runic_imp/Whisper.h>
#include <underworld/schema/Profession.h>
#include <underworld/schema/primitives.h>
#include <underworld/schema/Profession_Library.h>
#include <underworld/expressions/Operator.h>

namespace runic_imp {
  struct Symbols;
}

namespace imp_summoning {

  using Operator_Map = std::map<const runic_imp::Whisper *, underworld::Operator_Type>;

  class Lookup {
//      underworld::Profession_Library &profession_library;
      const runic_imp::Symbols &lexicon;
      std::map<const runic_imp::Whisper *, underworld::Primitive_Type> primitive_map;
      Operator_Map expression_operator_map;
      Operator_Map assignment_operator_map;

  public:
      Lookup();

      underworld::Primitive_Type get_primitive(const runic_imp::Whisper *whisper);
      bool get_expression_operator(const runic_imp::Whisper *whisper, underworld::Operator_Type &result);
      bool get_assignment_operator(const runic_imp::Whisper *whisper, underworld::Operator_Type &result);
  };
}
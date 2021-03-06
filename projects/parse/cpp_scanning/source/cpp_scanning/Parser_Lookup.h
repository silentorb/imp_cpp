#pragma once

#include <map>
#include <runic/common/Whisper.h>
#include <underworld/schema/Profession.h>
#include <underworld/schema/primitives.h>
#include <underworld/expressions/Operator.h>
#include <runic_cpp/Cpp_Lexicon.h>

namespace cpp_scanning {

  using Operator_Map = std::map<const runic::Whisper *, underworld::Operator_Type>;

  class Parser_Lookup {
      const runic_cpp::Symbols &lexicon;
      std::map<const runic::Whisper *, underworld::Primitive_Type> primitive_map;
      Operator_Map expression_operator_map;
      Operator_Map assignment_operator_map;

  public:
      Parser_Lookup(const runic_cpp::Symbols &lexicon);

      underworld::Primitive_Type get_primitive(const runic::Whisper *whisper);
      bool get_expression_operator(const runic::Whisper *whisper, underworld::Operator_Type &result);
      bool get_assignment_operator(const runic::Whisper *whisper, underworld::Operator_Type &result);

      const runic_cpp::Symbols &get_lexicon() const {
        return lexicon;
      }
  };
}
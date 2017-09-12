#include "Parser_Lookup.h"

using namespace underworld;

namespace imp_summoning {

  Parser_Lookup::Parser_Lookup(const runic_imp::Symbols &lexicon) :
    lexicon(lexicon),
    primitive_map(
      {
        {&lexicon.Bool, Primitive_Type::Bool},
        {&lexicon.Double, Primitive_Type::Double},
        {&lexicon.Float, Primitive_Type::Float},
        {&lexicon.Int, Primitive_Type::Int},
        {&lexicon.String, Primitive_Type::String},
      }),
    expression_operator_map(
      {

      }),
    assignment_operator_map(
      {
        {&lexicon.assignment, Operator_Type::assignment},
      }) {
  }

  underworld::Primitive_Type Parser_Lookup::get_primitive(const runic::Whisper *whisper) {
    auto search = primitive_map.find(whisper);
    if (search != primitive_map.end()) {
      return (*search).second;
    }

    return Primitive_Type::Unknown;
  }

  bool find(const Operator_Map &operator_map, const runic::Whisper *whisper, underworld::Operator_Type &result) {
    auto search = operator_map.find(whisper);
    if (search != operator_map.end()) {
      result = (*search).second;
      return true;
    }

    return false;
  }

  bool Parser_Lookup::get_expression_operator(const runic::Whisper *whisper, underworld::Operator_Type &result) {
    return find(expression_operator_map, whisper, result);
  }

  bool Parser_Lookup::get_assignment_operator(const runic::Whisper *whisper, underworld::Operator_Type &result) {
    return find(assignment_operator_map, whisper, result);
  }
}
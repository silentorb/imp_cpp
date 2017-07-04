#include <runic_imp/Lexicon.h>
#include "Lookup.h"

using namespace underworld;

namespace imp_summoning {

  Lookup::Lookup() :
    lexicon(runic_imp::Lexicon::get_instance().patterns),
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

  underworld::Primitive_Type Lookup::get_primitive(const runic_imp::Whisper *whisper) {
    auto search = primitive_map.find(whisper);
    if (search != primitive_map.end()) {
      return (*search).second;
    }

    return Primitive_Type::Unknown;
  }

  bool find(const Operator_Map &operator_map, const runic_imp::Whisper *whisper, underworld::Operator_Type &result) {
    auto search = operator_map.find(whisper);
    if (search != operator_map.end()) {
      result = (*search).second;
      return true;
    }

    return false;
  }

  bool Lookup::get_expression_operator(const runic_imp::Whisper *whisper, underworld::Operator_Type &result) {
    return find(expression_operator_map, whisper, result);
  }

  bool Lookup::get_assignment_operator(const runic_imp::Whisper *whisper, underworld::Operator_Type &result) {
    return find(assignment_operator_map, whisper, result);
  }
}
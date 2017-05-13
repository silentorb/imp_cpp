#include <runic_imp/Lexicon.h>
#include "Lookup.h"

using namespace underworld;

namespace imp_summoning {

  Lookup::Lookup(underworld::Profession_Library &profession_library) :
    lexicon(runic_imp::Lexicon::get_instance().patterns),
    profession_library(profession_library),
    primitive_map(
      {
        {&lexicon.Bool, &profession_library.get_primitive(Primitive_Type::Bool)},
        {&lexicon.Double, &profession_library.get_primitive(Primitive_Type::Double)},
        {&lexicon.Float, &profession_library.get_primitive(Primitive_Type::Float)},
        {&lexicon.Int, &profession_library.get_primitive(Primitive_Type::Int)},
        {&lexicon.String, &profession_library.get_primitive(Primitive_Type::String)},
      }),
    expression_operator_map(
      {

      }),
    assignment_operator_map(
      {
        {&lexicon.assignment, Operator_Type::equals},
      }) {
  }

  const underworld::Profession &Lookup::get_primitive(const runic_imp::Whisper *whisper) {
    auto search = primitive_map.find(whisper);
    if (search != primitive_map.end()) {
      return *(*search).second;
    }

    return profession_library.get_unknown();
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
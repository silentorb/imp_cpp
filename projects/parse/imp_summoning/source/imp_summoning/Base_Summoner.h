#pragma once

#include <underworld/schema/Profession_Library.h>
#include <underworld/schema/Member.h>
#include "Stream.h"
#include "Lookup.h"
#include "Context.h"

namespace imp_summoning {

  class Base_Summoner {
  protected:
      Stream &input;
      const runic_imp::Symbols &lexicon;
      Lookup &lookup;
//      underworld::Profession_Library &profession_library;

//      underworld::Member &find_member(const Token &rune, Context &context);
      underworld::Profession_Owner process_profession_token(Context &context);
      underworld::Profession_Owner process_profession(Context &context);
      underworld::Profession_Owner parse_profession_path(std::vector<std::string> &path, Context &context);

      source_mapping::Source_Point get_source_point() {
        return input.get_source_point();
      }

      template<typename T>
      inline T &cast(underworld::Expression &expression, underworld::Expression::Type type,
                     const std::string &message) {
        if (expression.get_type() != type)
          throw imp_summoning::Invalid_Token_Exception(input.current(), message);

        return *dynamic_cast<T *>(&expression);
      }

      template<typename Output, typename Input>
      inline Output &cast(Input &expression) {
        return *dynamic_cast<Output *>(&expression);
      }

      template<typename Output, typename Input>
      inline const Output &cast(const Input &expression) {
        return *dynamic_cast<const Output *>(&expression);
      }

//      template<typename T>
//      inline T &cast(underworld::Expression &expression) {
//        return *dynamic_cast<T *>(&expression);
//      }

  public:
      Base_Summoner(Stream &input, Lookup &lookup);

  };

}

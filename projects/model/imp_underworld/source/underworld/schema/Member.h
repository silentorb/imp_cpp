#pragma once
#include <memory>
#include "underworld/Source_Point.h"
#include "Profession.h"

namespace underworld {

  class Member {
      Source_Point source_point;

  public:
      Member(const Source_Point &source_point) : source_point(source_point) {}

      enum class Type {
          function,
          minion,
          profession,
      };

      virtual Type get_type() const = 0;
      virtual const std::string get_name() const = 0;
      virtual const Profession &get_profession() const = 0;

      const Source_Point &get_source_point() const {
        return source_point;
      }

  };

  using Member_Pointer = std::unique_ptr<Member>;
}
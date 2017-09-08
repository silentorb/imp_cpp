#pragma once
#include <memory>
#include "source_mapping/Source_Point.h"
#include "Profession.h"

namespace underworld {

  class Member {
      source_mapping::Source_Point source_point;

  public:
      Member(const source_mapping::Source_Point &source_point) : source_point(source_point) {}

      enum class Type {
          function,
          minion,
          profession,
      };

      virtual Type get_type() const = 0;
      virtual const std::string get_name() const = 0;
      virtual const Profession * get_profession() const = 0;

      const source_mapping::Source_Point &get_source_point() const {
        return source_point;
      }

  };

  using Member_Owner = std::unique_ptr<Member>;
}
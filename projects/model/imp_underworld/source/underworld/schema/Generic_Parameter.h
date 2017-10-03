#pragma once

#include <string>
#include <source_mapping/Source_Point.h>

namespace underworld {

  class Generic_Parameter {
      std::string name;
      source_mapping::Source_Range source_range;

  public:
      Generic_Parameter(const std::string &name, const source_mapping::Source_Range &source_range) :
        name(name), source_range(source_range) {}

      const std::string &get_name() const {
        return name;
      }

      const source_mapping::Source_Range &get_source_range() const {
        return source_range;
      }
  };
}
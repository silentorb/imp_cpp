#pragma once
#include <memory>
#include "source_mapping/Source_Point.h"
#include "Profession.h"
#include "Enchantment.h"

namespace underworld {

  class Member {
      source_mapping::Source_Range source_point;
      Enchantment_Array enchantments;

  public:
      Member(const source_mapping::Source_Range &source_point) : source_point(source_point) {}

      virtual ~Member() {

      }

      enum class Type {
          dungeon,
          function,
          minion,
//          profession,
      };

      virtual Type get_type() const = 0;
      virtual const std::string get_name() const = 0;
//      virtual const Profession * get_profession() const = 0;

      const source_mapping::Source_Range &get_source_point() const {
        return source_point;
      }

      const Enchantment_Array &get_enchantments() const {
        return enchantments;
      }

      void add_enchantment(Enchantment_Owner enchantment) {
        enchantments.push_back(std::move(enchantment));
      }
  };

  using Member_Owner = std::unique_ptr<Member>;
}
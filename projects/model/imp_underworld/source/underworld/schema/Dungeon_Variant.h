#pragma once

#include <vector>
#include "Profession.h"

namespace underworld {

  class Dungeon_Variant : public Profession {
      Profession_Owner original;
      std::vector<Profession_Owner> arguments;
      source_mapping::Source_Range source_point;

  public:
      Dungeon_Variant(Profession_Owner original) : original(std::move(original)) {

      }

      const Profession &get_original() const {
        return *original;
      }

      void add_argument(Profession_Owner argument) {
        arguments.push_back(std::move(argument));
      }

      const std::vector<Profession_Owner> &get_arguments() const {
        return arguments;
      }

      Profession_Type get_type() const override {
        return Profession_Type::variant;
      }

      const std::string get_name() const override {
        return original->get_name();
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }
  };
}
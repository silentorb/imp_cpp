#pragma once

#include <memory>
#include <source_mapping/Source_Point.h>
#include "Member.h"
#include "primitives.h"

namespace underworld {

  class Minion : public Member {
      const std::string name;
      Profession_Owner profession;
      Expression_Owner expression;

  public:
      Minion(const std::string &name, Profession_Owner profession, const source_mapping::Source_Range &source_point) :
        name(name), profession(std::move(profession)), Member(source_point) {}

      Minion(const std::string &name, const source_mapping::Source_Range &source_point) :
        name(name), Member(source_point) {}

      virtual ~Minion() {

      }

      Type get_type() const override {
        return Type::minion;
      }

      const std::string get_name() const override {
        return name;
      }

      const Profession *get_profession() const {
        return profession.get();
      }

      virtual bool is_parameter() const {
        return false;
      }

      void set_profession(Profession_Owner value) {
        profession = std::move(value);
      }
  };

  using Minion_Owner = std::unique_ptr<Minion>;
}
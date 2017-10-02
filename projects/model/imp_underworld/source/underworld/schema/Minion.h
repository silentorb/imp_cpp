#pragma once

#include <memory>
#include <source_mapping/Source_Point.h>
#include "Member.h"
#include "primitives.h"

namespace underworld {

  class Minion : public Member {
      const std::string name;
      Profession_Owner profession;
      bool _is_static = false;

  public:
      Minion(const std::string &name, Profession_Owner profession, const source_mapping::Source_Point &source_point) :
        name(name), profession(std::move(profession)), Member(source_point) {}

      Minion(const std::string &name, const source_mapping::Source_Point &source_point) :
        name(name), Member(source_point) {}

      virtual ~Minion() {

      }

      Type get_type() const override {
        return Type::minion;
      }

      const std::string get_name() const override {
        return name;
      }

      const Profession *get_profession() const override {
        return profession.get();
      }

      bool is_static() const {
        return _is_static;
      }

      void set_is_static(bool value) {
        _is_static = value;
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
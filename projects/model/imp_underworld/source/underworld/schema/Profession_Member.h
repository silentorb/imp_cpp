#pragma once

#include <string>
#include "Member.h"
#include "Profession.h"

namespace underworld {

  class Profession_Member : public Member {
      const Profession &profession;

  public:
      Profession_Member(const Profession &profession, const Source_Point &source_point) :
        profession(profession), Member(source_point) {}

      const Profession &get_profession() const {
        return profession;
      }

      Type get_type() const override {
        return Member::Type::profession;
      }

      const std::string get_name() const override {
        return profession.get_name();
      }
  };
}
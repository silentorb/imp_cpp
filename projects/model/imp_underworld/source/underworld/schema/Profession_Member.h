#pragma once

#include <string>
#include "Member.h"
#include "Profession.h"
#include <vector>

namespace underworld {

  class Profession_Member : public Member {
      Profession_Owner profession;

  public:
      Profession_Member(std::unique_ptr<Profession> &profession, const source_mapping::Source_Point &source_point) :
        Member(source_point), profession(std::move(profession)) {
      }

      const Profession * get_profession() const override {
        return profession.get();
      }

      Profession &get_profession() {
        return *profession;
      }

      Type get_type() const override {
        return Member::Type::profession;
      }

      const std::string get_name() const override {
        return profession->get_name();
      }
  };
}
#pragma once

#include <string>
#include "Member.h"
#include "Profession.h"
#include <vector>

namespace underworld {

  class Profession_Member : public Member {
      std::vector<std::unique_ptr<Profession>> professions;

  public:
      Profession_Member(std::unique_ptr<Profession> &profession, const Source_Point &source_point) :
        Member(source_point) {
        add_profession(profession);
      }

      void add_profession(std::unique_ptr<Profession> &profession) {
        professions.push_back(std::move(profession));
      }

      const Profession &get_profession() const override {
        return *professions[0];
      }

      Profession &get_profession() {
        return *professions[0];
      }

      Type get_type() const override {
        return Member::Type::profession;
      }

      const std::string get_name() const override {
        return professions[0]->get_name();
      }
  };
}
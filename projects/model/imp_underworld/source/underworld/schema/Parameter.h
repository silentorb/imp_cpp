#pragma once

#include "Minion.h"

namespace underworld {

  class Parameter : public Minion {
  public:
      Parameter(const std::string &name, Profession_Owner profession, const source_mapping::Source_Range &source_point)
        : Minion(name, std::move(profession), source_point) {}

      virtual ~Parameter() {
        int k = 0;
      }

      bool is_parameter() const override {
        return true;
      }
  };
  using Parameter_Owner = std::unique_ptr<Parameter>;

}
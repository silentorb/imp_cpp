#pragma once
#include "Dungeon.h"

namespace overworld {

  class Parameter : public Minion {
      std::unique_ptr<Dungeon> temporary_interface;

  public:
      Parameter(const std::string &name, Profession &profession) : Minion(name, profession) {}

      virtual ~Parameter();

      bool is_parameter() const override {
        return true;;
      }

      virtual bool transfers_ownership() const {
        return false;
      }

      const std::unique_ptr<Dungeon> &get_temporary_interface() const {
        return temporary_interface;
      }

      void set_temporary_interface(std::unique_ptr<Dungeon> &value) {
        temporary_interface = std::move(value);
      }

      void set_profession(Profession &value) override;
  };

  class Owning_Parameter : public Parameter {
  public:
      Owning_Parameter(const std::string &name, Profession &profession) :
        Parameter(name, profession) {}

      bool transfers_ownership() const override {
        return true;
      }
  };

}
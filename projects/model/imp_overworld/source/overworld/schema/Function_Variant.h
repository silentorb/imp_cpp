#pragma once

#include "Function_Interface.h"

namespace overworld {

  class Virtual_Function;

  class Function_Variant : public Variant, public virtual Function_Interface {
      Virtual_Function &original;
      Dungeon_Interface *dungeon = nullptr;

  public:
      Function_Variant(Virtual_Function &original, Dungeon_Interface &dungeon, const std::vector<Profession *> &professions);

      const std::vector<Generic_Parameter *> &get_generic_parameters() const override {
        return Variant::get_generic_parameters();
      }

      Virtual_Function &get_original() override {
        return original;
      }

      Dungeon_Interface &get_dungeon() {
        return *dungeon;
      }
  };

  using Function_Variant_Owner = std::unique_ptr<Function_Variant>;
}
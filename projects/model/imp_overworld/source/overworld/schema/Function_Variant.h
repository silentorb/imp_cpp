#pragma once

#include "Function_Interface.h"

namespace overworld {

  class Function;

  class Function_Variant : public Variant, public virtual Function_Interface {
      Function &original;
      Dungeon_Interface *dungeon = nullptr;

  public:
      Function_Variant(Function &original, Dungeon_Interface &dungeon, std::vector<Profession_Reference> &professions);

      const std::vector<Generic_Parameter *> &get_generic_parameters() const override {
        return Variant::get_generic_parameters();
      }

      Function &get_original() override {
        return original;
      }

      Dungeon_Interface &get_dungeon() {
        return *dungeon;
      }
  };

  using Function_Variant_Owner = std::unique_ptr<Function_Variant>;
}
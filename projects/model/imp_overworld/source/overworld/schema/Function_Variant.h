#pragma once

#include "Function_Interface.h"

namespace overworld {

  class Function;

  class Function_Variant : public Function_Interface {
      Variant variant;
      Function &original;
      Dungeon_Interface *dungeon = nullptr;

  public:
      Function_Variant(Function &original, Dungeon_Interface &dungeon, std::vector<Profession_Reference> &professions);

      ~Function_Variant() = default;

      const std::vector<Generic_Parameter *> &get_generic_parameters() const override {
        return variant.get_generic_parameters();
      }

      Function &get_original() override {
        return original;
      }

      Dungeon_Interface &get_dungeon() {
        return *dungeon;
      }

      Generic_Argument_Array &get_arguments() {
        return variant.get_arguments();
      }

      const Generic_Argument_Array &get_arguments() const {
        return variant.get_arguments();
      }
  };

  using Function_Variant_Owner = std::unique_ptr<Function_Variant>;
}
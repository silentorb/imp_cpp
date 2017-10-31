#pragma once

#include "Function_Interface.h"

namespace overworld {

  class Function;

  class Function_Variant : public Function_Interface {
      Variant variant;
      Function &original;
      Basic_Dungeon *dungeon = nullptr;

  public:
      Function_Variant(Function &original, Basic_Dungeon &dungeon, std::vector<Profession_Reference> &professions);

      virtual ~Function_Variant() = default;

      const std::vector<Generic_Parameter *> &get_generic_parameters() const override {
        return variant.get_generic_parameters();
      }

      Function &get_original() override {
        return original;
      }

      Basic_Dungeon &get_dungeon() {
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
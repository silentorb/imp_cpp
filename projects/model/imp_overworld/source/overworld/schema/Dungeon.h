#pragma once

#include "Scope.h"
#include "File.h"
#include "Generic_Parameter.h"
#include "Dungeon.h"
#include "Enchantment.h"
#include "Enchantment_Container.h"
#include "Generic_Argument.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon {
      std::string name;
      Scope scope;
      Generic_Parameter_Array generic_parameters;
      Generic_Argument_Array arguments;
      Dungeon *original = nullptr;
      File *header_file = nullptr;
//      Ownership default_ownership = Ownership::;
      Storage_Type default_storage = Storage_Type::pointer;
      Dungeon *base_dungeon = nullptr;
      std::vector<Profession_Reference> contracts;
      std::vector<std::unique_ptr<Generic_Parameter>> owned_generic_parameters;
      Enchantment_Container enchantments;
      Profession_Reference self;

      const std::string format_debug_name() const;

  public:
      Dungeon(const std::string &name, Scope &parent, const source_mapping::Source_Range source_range);

      Dungeon(const std::string &name, Scope &parent) :
        Dungeon(name, parent, source_mapping::Source_Range()) {}

      Dungeon(Dungeon &original) :
        Dungeon(original.get_name(), *original.get_scope().get_parent_scope(), source_mapping::Source_Range()) {
        if (original.get_original())
          throw std::runtime_error("Not yet supported.");

        this->original = &original;
        generic_parameters = original.get_generic_parameters();
//        for (auto i = 0; i < generic_parameters.size(); ++i) {
//          arguments.push_back(Generic_Argument_Owner(new Generic_Argument(*generic_parameters[i], professions[i])));
//        }
      }

      explicit Dungeon(const std::string &name);

      Dungeon(const Dungeon &) = delete;

      virtual ~Dungeon() {}

      Generic_Argument &add_generic_argument(const Profession_Reference &profession, const Ownership_Storage &info) {
        auto index = arguments.size();
        auto argument = new Generic_Argument(*generic_parameters[index], profession);
        arguments.push_back(Generic_Argument_Owner(argument));
        argument->get_node().set_ownership(info.ownership);
        argument->get_node().set_storage(info.storage);
        return *argument;
      }

      const std::string get_name() const {
        return name;
      }

      Profession_Reference &get_reference() {
        return self;
      }

      const Profession_Reference &get_profession() const {
        return self;
      }

      Dungeon *get_original() {
        return original;
      }

      const Dungeon *get_original() const {
        return original;
      }

      const Dungeon &get_base_original() const {
        return original
               ? original->get_base_original()
               : *this;
      }

      bool is_class() const;

      void set_file(File *value) {
        header_file = value;
      }

      void set_file(File &value) {
        header_file = &value;
      }

      Storage_Type get_default_storage() const {
        return default_storage;
      }

      void set_default_storage(Storage_Type default_storage) {
        Dungeon::default_storage = default_storage;
      }

//      Ownership get_ownership() const {
//        return default_ownership;
//      }
//
//      void set_default_ownership(Ownership value) {
//        this->default_ownership = value;
//      }

      Scope &get_scope() {
        return scope;
      }

      const std::vector<Minion_Owner> &get_minions() const {
        return scope.get_minions();
      }

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return scope.get_functions();
      }

      const Scope &get_scope() const {
        return scope;
      }

      Generic_Argument_Array &get_arguments() {
        return arguments;
      }

      const Generic_Argument_Array &get_arguments() const {
        return arguments;
      }

      File *get_file() const {
        return header_file;
      }

      Function &get_or_create_constructor();

      bool is_generic() const {
        return generic_parameters.size() > 0;
      }

      Enchantment_Container &get_enchantments() {
        return enchantments;
      }

      const Enchantment_Container &get_enchantments() const {
        return enchantments;
      }

      bool has_enchantment(const Dungeon &enchantment) const {
        if (enchantments.has_enchantment(enchantment))
          return true;

        if (original)
          return original->has_enchantment(enchantment);

        return false;
      }

      bool is_external() const;

      Dungeon &add_dungeon(std::unique_ptr<Dungeon> dungeon);

      Dungeon *get_base_dungeon() {
        return base_dungeon;
      }

      const Dungeon *get_base_dungeon() const {
        return base_dungeon;
      }

      bool is_interface() const {
        return false;
      }

      void add_contract(Dungeon &dungeon, Profession_Reference &profession) {
        if (!dungeon.is_interface()) {
          if (base_dungeon)
            throw std::runtime_error(get_name() + " already has a base dungeon.");

          base_dungeon = &dungeon;
        }

        contracts.push_back(profession);
      }

      Member *get_member_or_null(const std::string &name);

      const std::vector<Profession_Reference> *get_contracts() const {
        return &contracts;
      }

      std::vector<Profession_Reference> *get_contracts() {
        return &contracts;
      }

      Function &create_function(const std::string &name, Profession &profession,
                                const source_mapping::Source_Range &source_point = source_mapping::Source_Range());

      Minion &get_minion(const std::string &name);
      Member &get_member(const std::string &name);

      void add_generic_parameter(Generic_Parameter_Owner parameter) {
        generic_parameters.push_back(parameter.get());
        if (parameter->get_name() == "")
          parameter->set_name(get_generic_parameter_name(generic_parameters.size() - 1));

        scope.add_member(parameter->get_name(), Member(*parameter));
        owned_generic_parameters.push_back(std::move(parameter));
//        rename_generic_parameters(owned_generic_parameters);

      }

      const std::string get_debug_name() const {
        return format_debug_name();
      }

      std::vector<Generic_Parameter *> &get_generic_parameters() {
        return generic_parameters;
      }

      const std::vector<Generic_Parameter *> &get_generic_parameters() const {
        return generic_parameters;
      }

      bool has_generic_parameter(Generic_Parameter &parameter) {
        for (auto &item : generic_parameters) {
          if (item == &parameter)
            return true;
        }
        return false;
      }

      int get_generic_parameter_index(Generic_Parameter &parameter) {
        int index = 0;
        for (auto &item : generic_parameters) {
          if (item == &parameter)
            return index;

          ++index;
        }

        return -1;
      }

      Generic_Argument *get_generic_argument(Generic_Parameter &generic_parameter) {
        auto index = get_generic_parameter_index(generic_parameter);
        return get_arguments()[index].get();
      }
  };

  Function &create_variant_function(Dungeon &owning_dungeon, Function &original_function);

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
}
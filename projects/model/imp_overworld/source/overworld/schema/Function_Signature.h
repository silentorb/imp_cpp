#pragma once

#include <overworld/imp_graph/Node.h>
#include "professions/Profession.h"
#include "Element.h"
#include "Parameter.h"

namespace overworld {

  class Function_Signature : public Profession {
      std::vector<Parameter_Owner> elements;

  public:
//      Function_Signature(Dungeon_Interface *dungeon, Function *function,
//                         const source_mapping::Source_Range &source_range) :
//        element(Element_Type::other, "Function Signature", *this, source_range),
//        node(element, dungeon, function) {}

      ~Function_Signature() override = default;

      Profession_Type get_type() const override {
        return Profession_Type::function;
      }

//      Scope *get_scope() override {
//        return nullptr;
//      }
//
//      const Scope *get_scope() const override {
//        return nullptr;
//      }

      File *get_file() const override {
        return nullptr;
      }

      const std::string get_name() const override {
        return "Function Signature";
      }

      const std::string get_debug_name() const override;
      const std::string get_debug_name_with_names() const;

      Profession_Reference &get_base(Profession_Reference &self) override {
        return self;
      }

      const Profession &get_base() const override {
        return *this;
      }

      void add_element(Parameter_Owner element) {
        elements.push_back(std::move(element));
      }

      const std::vector<Parameter_Owner> &get_elements() const {
        return elements;
      }

      const std::vector<Parameter *> get_parameters() const {
        std::vector<Parameter *> result;
        for (auto i = 0; i < elements.size() - 1; ++i) {
          result.push_back(elements[i].get());
        }
        return result;
      }

      Parameter &get_last() {
        if (elements.size() == 0)
          throw std::runtime_error("Cannot get last profession from empty function signature.");

        return *elements[elements.size() - 1];
      }

      const Parameter &get_last() const {
        if (elements.size() == 0)
          throw std::runtime_error("Cannot get last profession from empty function signature.");

        return *elements[elements.size() - 1];
      }

      void set_last_profession(Profession_Reference &profession, Profession_Setter &setter) {
        if (elements.size() == 0)
          throw std::runtime_error("Cannot get last profession from empty function signature.");

        elements[elements.size() - 1]->set_profession(profession, setter);
      }

      Parameter *get_element(const std::string &name) {
        for (auto &element : elements) {
          if (element->get_name() == name)
            return element.get();
        }
        return nullptr;
      }

      bool matches(const std::vector<Profession_Reference>&professions) {
        if (professions.size() != elements.size())
          return false;

        auto it = elements.begin();
        for (auto &profession : professions) {
          auto &other = (*it++);
          auto other_profession = other->get_profession().get();
          if (profession.get() != other_profession)
            return false;
        }

        return true;
      }
  };
}
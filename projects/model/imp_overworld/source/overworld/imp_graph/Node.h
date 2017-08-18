#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/professions/Profession_Reference.h>
#include <overworld/schema/Dungeon_Interface.h>
#include <overworld/schema/Function_Interface.h>

namespace overworld {

  class Node : public graphing::Node<Node, Connection> {
      bool resolved = false;
      bool changed = false;
      Dungeon_Interface *dungeon = nullptr;
      Function_Interface *function = nullptr;

  public:
      Node(Dungeon_Interface &dungeon, Function_Interface *function) :
        dungeon(&dungeon), function(function) {}

      virtual ~Node() {}

      virtual Profession_Reference &get_profession_reference() = 0;
      virtual const Profession_Reference &get_profession_reference() const = 0;

      virtual void set_profession(Profession &value) {
        get_profession_reference().set_profession(value);
      }

      Profession &get_profession() {
        return get_profession_reference().get_profession();
      }

      virtual bool is_resolved() const {
        return resolved;
      }

      bool is_changed() const {
        return changed;
      }

      void set_resolved(bool resolved) {
        Node::resolved = resolved;
      }

      void set_changed(bool changed) {
        Node::changed = changed;
      }

      std::string get_debug_string() const;

      Dungeon_Interface &get_dungeon() const {
        return *dungeon;
      }
//      virtual const std::string get_name() const = 0;
  };

  template<typename T>
  class Profession_Node : public Node {
      T &element;
      Profession *original_profession;

  public:
      Profession_Node(T &element, Profession &original_profession, Dungeon_Interface &dungeon,
                      Function_Interface *function) :
        Node(dungeon, function), element(element), original_profession(&original_profession) {}

      Profession_Reference &get_profession_reference() override {
        return element;
      }

      const Profession_Reference &get_profession_reference() const override {
        return element;
      }

      bool is_resolved() const override {
        auto &profession = element.get_profession();
        return profession.get_base().get_type() != overworld::Profession_Type::unknown;
      }

      void set_original_profession(Profession &value) {
        original_profession = &value;
      }

      Profession &get_original_profession() {
        return *original_profession;
      }
//      const std::string get_name() const override {
//        return element.get_name();
//      }
  };

  class Node_Copy : public Node, public virtual Profession_Reference {
      Node &original;
      Profession *profession;

  public:
      Node_Copy(Node &original, Profession &profession, Dungeon_Interface &dungeon, Function_Interface *function) :
        Node(dungeon, function), original(original), profession(&profession) {}

      Profession_Reference &get_profession_reference() override {
        return *this;
      }

      const Profession_Reference &get_profession_reference() const override {
        return *this;
      }

      bool is_resolved() const override {
        return profession->get_base().get_type() != overworld::Profession_Type::unknown;
      }

      Profession &get_profession() override {
        return *profession;
      }

      const Profession &get_profession() const override {
        return *profession;
      }

      void set_profession(Profession &value) override {
        profession = &value;
      }

      const underworld::Source_Point &get_source_point() const override {
        return original.get_profession_reference().get_source_point();
      }

      const std::string get_name() const override {
        return original.get_profession_reference().get_name();
      }

      Node &get_original() const {
        return original;
      }
  };

  using Node_Owner = std::unique_ptr<Node>;
}
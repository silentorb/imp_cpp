#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/professions/Profession_Reference.h>

namespace overworld {

  class Node : public graphing::Node<Node, Connection> {
      bool resolved = false;
      bool changed = false;

  public:
      Node() {

      }

      virtual ~Node() {}

      virtual Profession_Reference &get_profession_reference() = 0;
      virtual const Profession_Reference &get_profession_reference() const = 0;

      virtual void set_profession(Profession &value) {
        get_profession_reference().set_profession(value);
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
//      virtual const std::string get_name() const = 0;
  };

  template<typename T>
  class Profession_Node : public Node {
      T &element;
      Profession *original_profession;

  public:
      Profession_Node(T &element, Profession &original_profession) :
        element(element), original_profession(&original_profession) {}

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

  using Node_Pointer = std::unique_ptr<Node>;
}
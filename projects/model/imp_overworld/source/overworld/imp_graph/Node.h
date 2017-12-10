#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/Element.h>

namespace overworld {

  class Function;

  class Dungeon;

  enum class Node_Status {
      unresolved,
      resolved,
      partial,
      optional,
  };

  class Node;

  Node_Status get_status_using_profession(const Profession_Reference &profession);

//  using Node_Change_Delegate = std::function<void(Node &)>;

  class Profession_Setter {
  public:
      virtual void set_profession(Node &node, Profession_Reference &profession) = 0;
//      virtual void set_changed(Node &node) = 0;
  };

  class Node : public graphing::Node<Node, Connection> {
      Node_Status status = Node_Status::unresolved;
      bool changed = false;
      Profession_Reference original_profession;
      Profession_Reference profession;
      Element &element;
      Ownership ownership = Ownership::unknown;
      Storage_Type storage = Storage_Type::unknown;
//      Node_Change_Delegate on_changed;

  public:
      Node(Profession_Reference original_profession, Element &element) :
        element(element),
        original_profession(original_profession),
        profession(original_profession) {
        status = _get_status();
      }

      virtual Node_Status _get_status() const {
        auto &p = const_cast<Profession_Reference &>(profession);
        auto &base_profession = p.get_base(p);
        return get_status_using_profession(base_profession);
      }

      Node(const Node &) = delete;

      virtual ~Node() {
		  int k = 0;
	  }

      Element &get_element() {
        return element;
      }

      const Element &get_element() const {
        return element;
      }

      Node_Status get_status() const {
        return status;
      }

      Parent &get_parent() {
        return element.get_parent();
      }

      bool is_changed() const {
        return changed;
      }

      void set_changed(bool changed) {
        Node::changed = changed;
      }

//      const Node_Change_Delegate &get_on_changed() const {
//        return on_changed;
//      }

//      void set_on_changed(const Node_Change_Delegate &value) {
//        on_changed = value;
//      }

      virtual std::string get_debug_string() const;

      Profession_Reference &get_profession() {
        return profession;
      }

      const Profession_Reference &get_profession() const {
        return profession;
      }

      const Profession &get_simple_profession() const {
        return *profession;
      }

      void set_profession(Profession_Reference &value, Profession_Setter &setter) {
        profession = value;
        status = _get_status();
//        if (on_changed)
//          on_changed(*this, value);
      }

      static const std::string get_advanced_debug_string(const Node &node);

      void set_ownership(Ownership value) {
        ownership = value;
      }

      Ownership get_ownership() const {
        return ownership;
      }

      Storage_Type get_storage() const {
        return storage;
      }

      void set_storage(Storage_Type value) {
        storage = value;
      }

      const Ownership_Storage get_attributes() const {
        return {ownership, storage};
      }

      void set_attributes(const Ownership_Storage &info) {
        ownership = info.ownership;
        storage = info.storage;
      }
  };

  class Empty_Profession_Setter : public overworld::Profession_Setter {
  public:
      static Empty_Profession_Setter &get_instance();
      void set_profession(overworld::Node &node, overworld::Profession_Reference &profession) override;
  };

  class Node_Copy : public Node {
      Node &original;
      Profession_Reference profession;

  protected:

  public:
      Node_Copy(Node &original, Profession_Reference &profession, Dungeon *dungeon,
                Function *function) :
        Node(profession, original.get_element()), original(original), profession(profession) {}

      Node_Status _get_status() const override {
        return profession->get_base().get_type() != overworld::Profession_Type::unknown
               ? Node_Status::resolved
               : Node_Status::unresolved;
      }

      Node &get_original() const {
        return original;
      }
  };

  using Node_Owner = std::unique_ptr<Node>;
}
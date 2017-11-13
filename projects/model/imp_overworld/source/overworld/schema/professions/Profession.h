#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "Optional_Shared_Pointer.h"

namespace overworld {

  class Dungeon;

  class Basic_Dungeon;

  class File;

  class Scope;

  class Node;

  enum class Ownership {
      unknown,

      move,
      owner,
      pointer,
      reference,
      value,
  };

  enum class Profession_Type {
      unknown,
      dungeon,
      function,
      generic_parameter,
      primitive,
      reference,
      temporary_interface,
      Union,
      Void,
  };

  class Profession_Reference;

  class Profession {
  public:
      virtual ~Profession() = default;

      virtual Profession_Type get_type() const = 0;

      virtual File *get_file() const {
        return nullptr;
      }

      virtual const std::string get_name() const = 0;

      virtual const std::string get_debug_name() const {
        return get_name();
      };

//      virtual Node &get_node() = 0;

      virtual Ownership get_ownership() const {
        return Ownership::unknown;
      }

      virtual Profession_Reference &get_base(Profession_Reference &self) {
        return self;
      }

      virtual const Profession &get_base() const {
        return *this;
      }

      virtual std::vector<Profession_Reference> *get_contracts() {
        return nullptr;
      }

      virtual const std::vector<Profession_Reference> *get_contracts() const {
        return nullptr;
      }

      Basic_Dungeon &get_dungeon_interface();
      const Basic_Dungeon &get_dungeon_interface() const;
  };

  using Profession_Owner = std::unique_ptr<Profession>;

  class Profession_Reference : public Optional_Shared_Pointer<Profession> {
      Ownership ownership;

  public:
      explicit Profession_Reference(Profession_Reference &reference, Ownership ownership) :
        Optional_Shared_Pointer(reference.shared_pointer, reference.pointer), ownership(ownership) {}

      explicit Profession_Reference(Profession &pointer, Ownership ownership = Ownership::unknown) :
        Optional_Shared_Pointer(pointer), ownership(ownership) {}

      explicit Profession_Reference(std::shared_ptr<Profession> &shared_pointer,
                                    Ownership ownership = Ownership::unknown) :
        Optional_Shared_Pointer(shared_pointer), ownership(ownership) {}

      explicit Profession_Reference(Profession *shared_pointer, Ownership ownership = Ownership::unknown) :
        Optional_Shared_Pointer(std::shared_ptr<Profession>(shared_pointer)), ownership(ownership) {}

      Profession_Type get_type() const {
        return get()->get_type();
      }

      void set_ownership(Ownership ownership) {
        Profession_Reference::ownership = ownership;
      }

      File *get_file() const {
        return get()->get_file();
      }

      const std::string get_name() const {
        return get()->get_name();
      }

//      Node &get_node() {
//        return get()->get_node();
//      }

      Ownership get_ownership() const {
        return ownership;
      }

      Profession_Reference &get_base(Profession_Reference &self) {
        return get()->get_base(self);
      }

      const Profession &get_base() const {
        return get()->get_base();
      }
  };

  template<typename T>
  class Owned_Profession_Reference {
      std::unique_ptr<T> owner;
      Profession_Reference reference;

  public:
      Owned_Profession_Reference(std::unique_ptr<T> owner) :
        owner(std::move(owner)),
        reference(*owner) {}

      T &get_owner() {
        return *owner;
      }

      Profession_Reference &get_reference() {
        return reference;
      }
  };

  bool can_cast(Profession &target, Profession &source);

}
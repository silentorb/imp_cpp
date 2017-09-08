#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace overworld {

  class Dungeon;

  class File;

  class Scope;

  class Node;

  enum class Ownership {
      unknown,
      value,
      owner,
      reference,
  };

  enum class Profession_Type {
      unknown,
      dungeon,
      function,
      generic_parameter,
      primitive,
      reference,
      Union,
      variant,
      Void,
  };

  class Profession {
  public:

      virtual ~Profession() {}

      virtual Profession_Type get_type() const = 0;
      virtual Scope *get_scope() = 0;
      virtual File *get_file() const = 0;
      virtual const std::string get_name() const = 0;

      virtual const std::string get_debug_name() const {
        return get_name();
      };

      virtual Node &get_node() {
        throw std::runtime_error("Not implemented.");
      }

      virtual Ownership get_ownership() const = 0;
      virtual Profession &get_base()  = 0;
      virtual const Profession &get_base() const = 0;

      virtual std::vector<Profession *> *get_contracts() {
        return nullptr;
      }

      virtual const std::vector<Profession *> *get_contracts() const {
        return nullptr;
      }
  };

  bool can_cast(Profession &target, Profession &source);
}
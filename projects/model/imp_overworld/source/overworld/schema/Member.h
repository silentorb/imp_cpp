#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Function;

  class Dungeon;

  class Minion;

  enum class Member_Type {
      none,
      dungeon,
      function,
      minion,
      profession,
      profession_reference,
  };

  class Member {
      Member_Type type;
      union {
          Dungeon *dungeon;
          Function *function;
          Minion *minion;
          const Profession *profession;
          Profession_Reference *profession_reference;
      };

  public:
      Member() : type(Member_Type::none), dungeon(nullptr) {}

      Member(Dungeon &dungeon) :
        type(Member_Type::dungeon), dungeon(&dungeon) {}

      Member(const Dungeon &dungeon) :
        type(Member_Type::dungeon), dungeon(const_cast<Dungeon *>(&dungeon)) {}

//      // Hack to differentiate from the Dungeon constructor.
//      Member(Profession_Reference &profession, bool) :
//        type(Member_Type::profession), profession(&profession) {
//        throw std::runtime_error("Not supported.");
//      }

      Member(const Profession &profession) :
        type(Member_Type::profession), profession(&profession) {}

      Member(Profession_Reference &profession) :
        type(Member_Type::profession_reference), profession_reference(&profession) {}

      Member(Function &function) :
        type(Member_Type::function), function(&function) {}

      Member(Minion &minion) :
        type(Member_Type::minion), minion(&minion) {}

      Member_Type get_type() const {
        return type;
      }

      const Function &get_function() const {
        return *function;
      }

      const Dungeon &get_dungeon() const {
        return *dungeon;
      }

      const Minion &get_minion() const {
        return *minion;
      }

      Function &get_function() {
        return *function;
      }

      Dungeon &get_dungeon() {
        return *dungeon;
      }

      Profession_Reference &get_profession_reference() {
        return *profession_reference;
      }

      const Profession_Reference &get_profession_reference() const {
        return *profession_reference;
      }

      const Profession &get_profession() const {
        return *profession;
      }

      Minion &get_minion() {
        return *minion;
      }

      const void *get_value() const {
        return dungeon;
      }
  };
  Node &get_member_node(Member &member);

  Profession_Reference &get_member_profession_reference(Member &member);
  const Profession &get_member_profession(const Member &member);

  const std::string get_member_name(const Member &member);

  const std::string get_member_name(Member &member);

}
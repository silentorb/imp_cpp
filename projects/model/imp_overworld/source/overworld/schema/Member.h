#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Function;

  class Dungeon;

  class Minion;

  class Parameter;

  class Temporary_Member;

  enum class Member_Type {
      none,
      dungeon,
      function,
      minion,
      parameter,
      profession,
      profession_reference,
      temporary,
  };

  class Member {
      Member_Type type;
      union {
          Dungeon *dungeon;
          Function *function;
          Minion *minion;
          Parameter *parameter;
          const Profession *profession;
          Temporary_Member *temporary;
      };

  public:
      Member() : type(Member_Type::none), dungeon(nullptr) {}

      Member(Dungeon &dungeon) :
        type(Member_Type::dungeon), dungeon(&dungeon) {}

      Member(const Dungeon &dungeon) :
        type(Member_Type::dungeon), dungeon(const_cast<Dungeon *>(&dungeon)) {}

      Member(Parameter &parameter);

      Member(const Profession &profession) :
        type(Member_Type::profession), profession(&profession) {}

      Member(Function &function) :
        type(Member_Type::function), function(&function) {}

      Member(Minion &minion) :
        type(Member_Type::minion), minion(&minion) {}

      Member(Temporary_Member &temporary) :
        type(Member_Type::temporary), temporary(&temporary) {}

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

      Parameter &get_parameter() {
        return *parameter;
      }

      const Parameter &get_parameter() const {
        return *parameter;
      }

      Function &get_function() {
        return *function;
      }

      Dungeon &get_dungeon() {
        return *dungeon;
      }

      Temporary_Member &get_temporary_member() {
        return *temporary;
      }

      const Temporary_Member &get_temporary_member() const {
        return *temporary;
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

  inline const Node &get_member_node(const Member &member) {
    return get_member_node(const_cast<Member &>(member));
  };

  Profession_Reference &get_member_profession_reference(Member &member);
  Profession_Reference get_member_profession_reference2(Member &member);

  const Profession_Reference &get_member_profession(const Member &member);

  const std::string get_member_name(const Member &member);

  const std::string get_member_name(Member &member);

}
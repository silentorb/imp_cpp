#pragma once

#include <overworld/schema/Scope.h>

namespace imp_mirror {

  class Scope {
      overworld::Scope &overworld_scope;
      std::vector<overworld::Scope *> imported_scopes;
      std::unordered_map<std::string, overworld::Member> dungeons;
      Scope *parent = nullptr;

  public:
      explicit Scope(overworld::Scope &overworld_scope) : overworld_scope(overworld_scope) {}

      Scope(overworld::Scope &overworld_scope, Scope &parent) :
        overworld_scope(overworld_scope), parent(&parent) {}

      overworld::Scope &get_overworld_scope() const {
        return overworld_scope;
      }

      overworld::Member *find_member(const std::string &name) {
        auto result = overworld_scope.get_member_or_null(name);

        if (result)
          return result;

        for (auto scope : imported_scopes) {
          auto result = scope->get_member_or_null(name);
          if (result)
            return result;
        }

        if (dungeons.count(name) > 0)
          return &dungeons.at(name);

        if (parent)
          return parent->find_member(name);

        return nullptr;
      }

      Scope *get_parent() {
        return parent;
      }

      void add_dungeon(overworld::Dungeon &dungeon) {
        dungeons[dungeon.get_name()] = overworld::Member(dungeon);
      }

      void import(overworld::Scope &scope) {
        imported_scopes.push_back(&scope);
      }
  };
}
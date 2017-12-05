#pragma once

#include <overworld/schema/Dungeon.h>

namespace imp_wrapper {

  class Project_Bundle {
      overworld::Dungeon &overworld_root;
      underworld::Dungeon underworld_root;
      imp_mirror::Scope mirror_scope;
      overworld::Graph graph;

  public:
      explicit Project_Bundle(overworld::Dungeon &overworld_root) :
        overworld_root(overworld_root),
        underworld_root("", nullptr),
        mirror_scope(overworld_root.get_scope()) {}

      overworld::Dungeon &get_overworld_root() {
        return overworld_root;
      }

      underworld::Dungeon &get_underworld_root() {
        return underworld_root;
      }

      imp_mirror::Scope &get_mirror_scope()  {
        return mirror_scope;
      }

      overworld::Graph &get_graph() {
        return graph;
      }

  };
}
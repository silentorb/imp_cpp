#pragma once
#include <map>
#include <underworld/schema/Dungeon.h>
#include <memory>

namespace cpp_scanning {

  using Dungeon_Map = std::map<const std::string, std::unique_ptr<underworld::Dungeon>>;

}
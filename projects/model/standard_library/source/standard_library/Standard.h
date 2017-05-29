#pragma once

#include <overworld/schema/Dungeon.h>

namespace standard_library {

  class Standard : public underworld::Dungeon {
      underworld::Source_File file;

  public:
      Standard();

  };
}
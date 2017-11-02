#pragma once

#include <spatial/Position.h>

namespace game {

  class Camera {
      spatial::Position position;

      spatial::Position &get_position() {
        return position;
      }

  public:
  };
}
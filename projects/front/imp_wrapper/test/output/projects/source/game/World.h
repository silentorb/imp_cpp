#pragma once

#include "Camera.h"

namespace game {
  class World {
        game::Camera camera;

      game::Camera get_camera() {
        return camera;
      }

  public:
  };
}
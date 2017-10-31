#pragma once

#include "Camera.h"

namespace game {

  class Camera;

  class World {
      Camera camera;

      Camera &get_camera() {
        return camera;
      }

  public:
  };
}
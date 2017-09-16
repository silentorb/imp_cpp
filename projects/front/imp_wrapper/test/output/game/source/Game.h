#pragma once

#include <memory>

class World;

class Game {
    std::unique_ptr<World> world;

public:
    std::unique_ptr<Game> _new();
};
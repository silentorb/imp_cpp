#pragma once
#include <memory>

class World;

class Game {
public:
    std::unique_ptr<World> world;
    Game(std::unique_ptr<World> world);
    static Game *_new();
};
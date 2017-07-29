#pragma once

#include <memory>

class ?;
class World;

class Game {
    unknown ;
    std::unique_ptr<World> world;

public:
    std::unique_ptr<Game> new();
};
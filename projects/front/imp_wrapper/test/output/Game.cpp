#include <World.h>
#include <Game.h>

std::unique_ptr<Game> Game::_new() {
  auto game = std::unique_ptr<Game>(new Game());
  game->world = std::unique_ptr<World>(new World());
  auto hero = std::unique_ptr<Hero>(new Hero());
  game->world->add_creature(hero);
  game->world->add_creature(std::unique_ptr<Monster>(new Monster()));
  return game;
}
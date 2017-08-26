#include "Hero.h"
#include "World.h"
#include "Game.h"
#include "Monster.h"
#include "Creature.h"

std::unique_ptr<Game> Game::_new() {
  auto game = std::unique_ptr<Game>(new Game());
  game->world = std::unique_ptr<World>(new World());
  auto hero = std::unique_ptr<Hero>(new Hero());
  game->world->add_creature(std::move(hero));
  game->world->add_creature(std::move(std::unique_ptr<Monster>(new Monster())));
  return game;
}
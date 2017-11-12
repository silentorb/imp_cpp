#include "gtest/gtest.h"
#include <imp_wrapper/Global_Bundle.h>
#include <utility/utility.h>
#include <imp_wrapper/project_builder.h>

using namespace std;

TEST(Main_Test, pizza) {
  compile("pizza");
  Comparison test("pizza");
  test.compare("Pizza.cpp");
  test.compare("Pizza.h");
}

TEST(Main_Test, game) {
  compile("game");
  Comparison test("game");
  test.compare("Game.cpp");
  test.compare("Game.h");
  test.compare("World.h");
  test.compare("World.cpp");
  test.compare("Creature.h");
  test.compare("Hero.h");
  test.compare("Monster.h");
}

TEST(Main_Test, generic_dungeon) {
  compile("generic_dungeon");
  Comparison test("generic_dungeon");
  test.compare("House.h");
  test.compare("General.h");
  test.compare("General.cpp");
}

TEST(Main_Test, generic_function) {
  compile("generic_function");
  Comparison test("generic_function");
  test.compare("General.h");
  test.compare("General.cpp");
}

TEST(Main_Test, fail_generic_dungeon) {
  compile("fail_generic_dungeon");
}

TEST(Main_Test, iteration) {
  compile("iteration");
  Comparison test("iteration");
//  test.compare("Goblikin.h");
//  test.compare("Goblikin.cpp");
//  test.compare("Ingredient.h");
}

TEST(Main_Test, projects) {
  imp_wrapper::build_workspace(std::string(RESOURCE_PATH) + "projects");
  Comparison test("projects");
  test.compare("game/Camera.h");
  test.compare("game/World.h");
}

#include "gtest/gtest.h"
#include <imp_wrapper/Wrapper.h>
#include <utility/utility.h>

using namespace std;

TEST(Main_Test, pizza) {
//  compile("pizza");
//
//  compare(string(RESOURCE_PATH) + "pizza/Pizza.h", string(OUTPUT_PATH) + "Pizza.h");
//  compare(string(RESOURCE_PATH) + "pizza/Pizza.cpp", string(OUTPUT_PATH) + "Pizza.cpp");
}

TEST(Main_Test, game) {
//  compile("game");
//
//  compare(string(RESOURCE_PATH) + "game/World.cpp", string(OUTPUT_PATH) + "World.cpp");
//  compare(string(RESOURCE_PATH) + "game/World.h", string(OUTPUT_PATH) + "World.h");
//
//  compare(string(RESOURCE_PATH) + "game/Game.cpp", string(OUTPUT_PATH) + "Game.cpp");
//  compare(string(RESOURCE_PATH) + "game/Game.h", string(OUTPUT_PATH) + "Game.h");
//
//  compare(string(RESOURCE_PATH) + "game/Creature.h", string(OUTPUT_PATH) + "Creature.h");
}

TEST(Main_Test, fail_generic_dungeon) {
//  compile("fail_generic_dungeon");
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
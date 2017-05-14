#include "gtest/gtest.h"
#include <imp_wrapper/Wrapper.h>
#include <boost/filesystem/operations.hpp>
#include <utility/utility.h>

using namespace std;



TEST(Main_Test, pizza) {
  compile("pizza/pizza.imp");

  compare(string(RESOURCE_PATH) + "pizza/Pizza.h", string(OUTPUT_PATH) + "Pizza.h");
//  compare(string(RESOURCE_PATH) + "pizza/Pizza.cpp", string(OUTPUT_PATH) + "Pizza.cpp");
}

TEST(Main_Test, game) {
  compile("game/game.imp");

//  compare(string(RESOURCE_PATH) + "Pizza.h", string(OUTPUT_PATH) + "Pizza.h");
//  compare(string(RESOURCE_PATH) + "Pizza.cpp", string(OUTPUT_PATH) + "Pizza.cpp");
}

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

#include "gfx/Font.h"

#include <iostream>

TEST_CASE("grouping of numbers formatting") {
  
  REQUIRE(Fonts::groupDigits(1) == "1");
  
}

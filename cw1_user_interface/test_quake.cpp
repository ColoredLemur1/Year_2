// COMP2811 Coursework 1: tests for Quake class

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "quake.hpp"

TEST_CASE("Latitude initialization") {
  SUBCASE("Value stored correctly") {
    Quake q("2024-10-07T00:00:01Z", 35.0, 0.0, 10.0, 5.0);
    CHECK(q.getLatitude() == doctest::Approx(35.0));
  }
  SUBCASE("Exception if too low") {
    CHECK_THROWS(Quake("2024-10-07T00:00:01Z", -90.1, 0.0, 10.0, 5.0));
  }
  SUBCASE("Exception if too high") {
    CHECK_THROWS(Quake("2024-10-07T00:00:01Z", 90.1, 0.0, 10.0, 5.0));
  }
}

TEST_CASE("Longitude initialization") {
  SUBCASE("Value stored correctly") {
    Quake q("2024-10-07T00:00:01Z", 0.0, 115.0, 10.0, 5.0);
    CHECK(q.getLongitude() == doctest::Approx(115.0));
  }
  SUBCASE("Exception if too low") {
    CHECK_THROWS(Quake("2024-10-07T00:00:01Z", 0.0, -180.1, 10.0, 5.0));
  }
  SUBCASE("Exception if too high") {
    CHECK_THROWS(Quake("2024-10-07T00:00:01Z", 0.0, 180.1, 10.0, 5.0));
  }
}

TEST_CASE("Depth initialization") {
  SUBCASE("Value stored correctly") {
    Quake q("2024-10-07T00:00:01Z", 0.0, 0.0, 42.0, 5.0);
    CHECK(q.getDepth() == doctest::Approx(42.0));
  }
  SUBCASE("Exception if negative") {
    CHECK_THROWS(Quake("2024-10-07T00:00:01Z", 0.0, 0.0, -0.1, 5.0));
  }
}

TEST_CASE("Magnitude initialization") {
  SUBCASE("Value stored correctly") {
    Quake q("2024-10-07T00:00:01Z", 0.0, 0.0, 10.0, 4.7);
    CHECK(q.getMagnitude() == doctest::Approx(4.7));
  }
  SUBCASE("Exception if negative") {
    CHECK_THROWS(Quake("2024-10-07T00:00:01Z", 0.0, 0.0, 10.0, -0.1));
  }
}

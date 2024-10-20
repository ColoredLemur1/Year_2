// COMP2811 Coursework 1: tests for QuakeDataset class

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "dataset.hpp"

TEST_CASE("Empty dataset") {
  QuakeDataset data;

  SUBCASE("size() is 0") {
    CHECK(data.size() == 0);
  }

  SUBCASE("Exception when accessing quake at index 0") {
    CHECK_THROWS(data[0]);
  }

  SUBCASE("Exception when requesting strongest quake") {
    CHECK_THROWS(data.strongest());
  }

  SUBCASE("Exception when requesting shallowest quake") {
    CHECK_THROWS(data.shallowest());
  }

  SUBCASE("Exception when requesting mean magnitude") {
    CHECK_THROWS(data.meanMagnitude());
  }

  SUBCASE("Exception when requesting mean depth") {
    CHECK_THROWS(data.meanDepth());
  }
}

TEST_CASE("test0.csv") {
  QuakeDataset data("../data/test0.csv");

  SUBCASE("size() is 0") {
    CHECK(data.size() == 0);
  }
}

TEST_CASE("test10.csv") {
  QuakeDataset data("../data/test10.csv");

  SUBCASE("size() is 10") {
    CHECK(data.size() == 10);
  }

  SUBCASE("Correct details for first quake") {
    Quake q = data[0];
    CHECK(q.getTime() == "2022-08-02T21:11:41.381Z");
    CHECK(q.getLatitude() == doctest::Approx(-18.1158));
    CHECK(q.getLongitude() == doctest::Approx(-178.202));
    CHECK(q.getDepth() == doctest::Approx(521.281));
    CHECK(q.getMagnitude() == doctest::Approx(5.6));
  }

  SUBCASE("Correct details for last quake") {
    Quake q = data[9];
    CHECK(q.getTime() == "2022-08-02T00:42:56.247Z");
    CHECK(q.getLatitude() == doctest::Approx(-3.1388));
    CHECK(q.getLongitude() == doctest::Approx(148.457));
    CHECK(q.getDepth() == doctest::Approx(10));
    CHECK(q.getMagnitude() == doctest::Approx(4.7));
  }

  SUBCASE("Strongest quake identified correctly") {
    CHECK(data.strongest().getTime() == "2022-08-02T21:11:41.381Z");
  }

  SUBCASE("Shallowest quake identified correctly") {
    CHECK(data.shallowest().getTime() == "2022-08-02T15:17:01.461Z");
  }

  SUBCASE("meanDepth() is close to 127.058") {
    CHECK(data.meanDepth() == doctest::Approx(127.058));
  }

  SUBCASE("meanMagnitude() is close to 5.08") {
    CHECK(data.meanMagnitude() == doctest::Approx(5.08));
  }
}

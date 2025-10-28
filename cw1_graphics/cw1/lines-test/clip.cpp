#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Partially offscreen", "[clip]"){
    Surface surface( 640, 480 );
    surface.clear();

    //Only one test case for now
    SECTION( "go right" ){
        draw_line_solid( surface,
            { 1.f, 240.f },
            { 1920.f, 240.f },
            { 255, 255, 255 }
        );

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
    }
}
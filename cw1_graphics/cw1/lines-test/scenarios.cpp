#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
// scenario 1 :clipped lines partially offscreen

TEST_CASE( "Partially offscreen", "[scenarios]" ){
	Surface surface( 100, 100 );
	surface.clear();

	// Test all inside/outside combinations 
	
	SECTION( "inside to right" )
	{
		draw_line_solid( surface,
			{ 10.f, 50.f },
			{ 200.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "left to inside" )
	{
		draw_line_solid( surface,
			{ -50.f, 50.f },
			{ 10.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "inside to left" )
	{
		draw_line_solid( surface,
			{ 10.f, 50.f },
			{ -50.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "right to inside" )
	{
		draw_line_solid( surface,
			{ 200.f, 50.f },
			{ 10.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "inside to top" )
	{
		draw_line_solid( surface,
			{ 50.f, 10.f },
			{ 50.f, -50.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "bottom to inside" )
	{
		draw_line_solid( surface,
			{ 50.f, 150.f },
			{ 50.f, 10.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "inside to bottom" )
	{
		draw_line_solid( surface,
			{ 50.f, 10.f },
			{ 50.f, 150.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "top to inside" )
	{
		draw_line_solid( surface,
			{ 50.f, -50.f },
			{ 50.f, 10.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "diagonal inside to outside" )
	{
		draw_line_solid( surface,
			{ 10.f, 10.f },
			{ 150.f, 150.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "diagonal outside to inside" )
	{
		draw_line_solid( surface,
			{ -50.f, -50.f },
			{ 10.f, 10.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "crossing horizontally" )
	{
		draw_line_solid( surface,
			{ -50.f, 50.f },
			{ 150.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "crossing vertically" )
	{
		draw_line_solid( surface,
			{ 50.f, -50.f },
			{ 50.f, 150.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
}
// scenario 2 :thin lines (no thickness greater than 1 pixel in any direction)
TEST_CASE( "Thin lines", "[scenarios]" ){
    Surface surface( 100, 100 );
    surface.clear();

    //test all the thin line cases
    SECTION( "horizontal" ){
        draw_line_solid( surface,
            { 10.f, 50.f },
            { 90.f, 50.f },
            { 255, 255, 255 }
        );

        REQUIRE( max_row_pixel_count( surface ) > 1 );
        REQUIRE( 1 == max_col_pixel_count( surface ) );
    }

    SECTION( "vertical" ){
        draw_line_solid( surface,
            { 50.f, 10.f },
            { 50.f, 90.f },
            { 255, 255, 255 }
        );

        REQUIRE( 1 == max_row_pixel_count( surface ) );
        REQUIRE( max_col_pixel_count( surface ) > 1 );
    }
    //diagonal line that chnages more in the x direction than the y direction
    SECTION( "slanted low" ){
        draw_line_solid( surface,
            { 10.f, 20.f },
            { 90.f, 40.f },
            { 255, 255, 255 }
        );

        REQUIRE( max_row_pixel_count( surface ) > 1 );
        REQUIRE( 1 == max_col_pixel_count( surface ) );
    }
    //diagonal line that chnages more in the y direction than the x direction
    SECTION( "slanted high" ){
        draw_line_solid( surface,
            { 50.f, 10.f },
            { 40.f, 90.f },
            { 255, 255, 255 }
        );

        REQUIRE( 1 == max_row_pixel_count( surface ) );
        REQUIRE( max_col_pixel_count( surface ) > 1 );
    }
    //diagonal line that changes equally in the x and y directions
    SECTION( "diagonal" ){
        draw_line_solid( surface,
            { 10.f, 10.f },
            { 90.f, 90.f },
            { 255, 255, 255 }
        );

        REQUIRE( 1 == max_row_pixel_count( surface ) );
        REQUIRE( 1 == max_col_pixel_count( surface ) );
    }
}
// scenario 3 :no gaps between two connected lines that are drawn one after the other
TEST_CASE( "No gaps", "[scenarios]" ){
    Surface surface( 100, 100 );
    surface.clear();

    //test all the no gaps cases
    SECTION( "horizontal" ){
        draw_line_solid( surface,
            { 1.f, 50.f },
            { 50.f, 50.f },
            { 255, 255, 255 }
        );
		draw_line_solid( surface,
			{ 50.f, 50.f },
			{ 99.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const counts = count_pixel_neighbours( surface );
		REQUIRE( 2 == counts[1] );
		REQUIRE( counts[2] > 0 );
		REQUIRE( 0 == counts[0] );
		for( std::size_t i = 3; i < counts.size(); ++i ){
			REQUIRE( 0 == counts[i]  );
		}
    }
    SECTION( "vertical" ){
        draw_line_solid( surface,
            { 50.f, 1.f },
            { 50.f, 50.f },
            { 255, 255, 255 }
        );
		draw_line_solid( surface,
			{ 50.f, 50.f },
			{ 50.f, 99.f },
			{ 255, 255, 255 }
		);
        auto const counts = count_pixel_neighbours( surface );
        REQUIRE( 2 == counts[1] );
        REQUIRE( counts[2] > 0 );
        REQUIRE( 0 == counts[0] );
        for( std::size_t i = 3; i < counts.size(); ++i ){
            REQUIRE( 0 == counts[i]  );
        }
    }
    SECTION( "diagonal" ){
        draw_line_solid( surface,
            { 1.f, 1.f },
            { 50.f, 50.f },
            { 255, 255, 255 }
        );
		draw_line_solid( surface,
			{ 50.f, 50.f },
			{ 99.f, 99.f },
			{ 255, 255, 255 }
		);
        auto const counts = count_pixel_neighbours( surface );
        REQUIRE( 2 == counts[1] );
        REQUIRE( counts[2] > 0 );
        REQUIRE( 0 == counts[0] );
        for( std::size_t i = 3; i < counts.size(); ++i ){
            REQUIRE( 0 == counts[i]  );
        }
    }
    SECTION( "slanted high" ){
        draw_line_solid( surface,
            { 1.f, 1.f },
            { 4.f, 50.f },
            { 255, 255, 255 }
        );
		draw_line_solid( surface,
			{ 4.f, 50.f },
			{ 7.f, 99.f },
			{ 255, 255, 255 }
		);
        auto const counts = count_pixel_neighbours( surface );
        REQUIRE( 2 == counts[1] );
        REQUIRE( counts[2] > 0 );
        REQUIRE( 0 == counts[0] );
        for( std::size_t i = 3; i < counts.size(); ++i )
            REQUIRE( 0 == counts[i]  );
    }
    SECTION( "slanted low" ){
        draw_line_solid( surface,
            { 1.f, 1.f },
            { 50.f, 4.f },
            { 255, 255, 255 }
        );
		draw_line_solid( surface,
			{ 50.f, 4.f },
			{ 99.f, 7.f },
			{ 255, 255, 255 }
		);

        auto const counts = count_pixel_neighbours( surface );
        REQUIRE( 2 == counts[1] );
        REQUIRE( counts[2] > 0 );
        REQUIRE( 0 == counts[0] );
        for( std::size_t i = 3; i < counts.size(); ++i )
            REQUIRE( 0 == counts[i]  );
    }
}
// scenario 4 :comparing identical lines drawn in reverse order
TEST_CASE( "Reversed lines", "[scenarios]" ){
	Surface surface1( 100, 100 );
	surface1.clear();

	Surface surface2( 100, 100 );
	surface2.clear();

	SECTION( "horizontal p1 inside->outside p2 outside->inside" ) {
		draw_line_solid( surface1,
			{ -10.f, 50.f },
			{ 90.f, 50.f },
			{ 255, 255, 255 }
		);

		draw_line_solid( surface2,
			{ 90.f, 50.f },
			{ -10.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const pixels1 = max_col_pixel_count( surface1 );
		auto const pixels2 = max_col_pixel_count( surface2 );
		REQUIRE( pixels1 == pixels2 );
	}
	SECTION("horizontal p1 outside->inside p2 inside->outside"){
		draw_line_solid( surface1,
			{ 10.f, 50.f },
			{ 110.f, 50.f },
			{ 255, 255, 255 }
		);

		draw_line_solid( surface2,
			{ 110.f, 50.f },
			{ 10.f, 50.f },
			{ 255, 255, 255 }
		);

		auto const pixels1 = max_col_pixel_count( surface1 );
		auto const pixels2 = max_col_pixel_count( surface2 );
		REQUIRE( pixels1 == pixels2 );
	}
	SECTION("vertical p1 inside->outside p2 outside->inside"){
		draw_line_solid( surface1,
			{ 50.f, -10.f },
			{ 50.f, 90.f },
			{ 255, 255, 255 }
		);

		draw_line_solid( surface2,
			{ 50.f, 90.f },
			{ 50.f, -10.f },
			{ 255, 255, 255 }
		);

		auto const pixels1 = max_row_pixel_count( surface1 );
		auto const pixels2 = max_row_pixel_count( surface2 );
		REQUIRE( pixels1 == pixels2 );
	}
	SECTION("vertical p1 outside->inside p2 inside->outside"){
		draw_line_solid( surface1,
			{ 50.f, 10.f },
			{ 50.f, 110.f },
			{ 255, 255, 255 }
		);
		draw_line_solid( surface2,
			{ 50.f, 110.f },
			{ 50.f, 10.f },
			{ 255, 255, 255 }
		);
		auto const pixels1 = max_row_pixel_count( surface1 );
		auto const pixels2 = max_row_pixel_count( surface2 );
		REQUIRE( pixels1 == pixels2 );
	}
	SECTION("diagonal p1 inside->outside p2 outside->inside"){
		draw_line_solid( surface1,
			{ -10.f, -10.f },
			{ 90.f, 90.f },
			{ 255, 255, 255 }
		);
		draw_line_solid( surface2,
			{ 90.f, 90.f },
			{ -10.f, -10.f },
			{ 255, 255, 255 }
		);
		auto const pixels1 = max_row_pixel_count( surface1 );
		auto const pixels2 = max_row_pixel_count( surface2 );
		REQUIRE( pixels1 == pixels2 );
	}
	SECTION("diagonal p1 outside->inside p2 inside->outside"){
		draw_line_solid( surface1,
			{ 10.f, 10.f },
			{ 110.f, 110.f },
			{ 255, 255, 255 }
		);
		draw_line_solid( surface2,
			{ 110.f, 110.f },
			{ 10.f, 10.f },
			{ 255, 255, 255 }
		);
		auto const pixels1 = max_row_pixel_count( surface1 );
		auto const pixels2 = max_row_pixel_count( surface2 );
		REQUIRE( pixels1 == pixels2 );
	}
}
#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
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
    SECTION( "x-major" ){
        draw_line_solid( surface,
            { 10.f, 20.f },
            { 90.f, 40.f },
            { 255, 255, 255 }
        );

        REQUIRE( max_row_pixel_count( surface ) > 1 );
        REQUIRE( 1 == max_col_pixel_count( surface ) );
    }
    //diagonal line that chnages more in the y direction than the x direction
    SECTION( "y-major" ){
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
TEST_CASE( "No gaps", "[scenarios]" ){
    Surface surface( 100, 100 );
    surface.clear();

    //test all the no gaps cases
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

        auto const counts = count_pixel_neighbours( surface );
        REQUIRE( 2 == counts[1] );
        REQUIRE( counts[2] > 0 );
        REQUIRE( 0 == counts[0] );
        for( std::size_t i = 3; i < counts.size(); ++i )
            REQUIRE( 0 == counts[i]  );
    }
    SECTION( "diagonal" ){
        draw_line_solid( surface,
            { 10.f, 10.f },
            { 90.f, 90.f },
            { 255, 255, 255 }
        );

        auto const counts = count_pixel_neighbours( surface );
        REQUIRE( 2 == counts[1] );
        REQUIRE( counts[2] > 0 );
        REQUIRE( 0 == counts[0] );
        for( std::size_t i = 3; i < counts.size(); ++i )
            REQUIRE( 0 == counts[i]  );
    }
    SECTION( "slanted high" ){
        draw_line_solid( surface,
            { 10.f, 10.f },
            { 10.f, 90.f },
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
            { 10.f, 90.f },
            { 10.f, 10.f },
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

TEST_CASE( "Special cases", "[scenarios]" ){
    Surface surface( 100, 100 );
    surface.clear();

    //test all the special cases
    SECTION( "zero length" ){
        draw_line_solid( surface,
            { 10.f, 10.f },
            { 10.f, 10.f },
            { 255, 255, 255 }
        );

        auto const counts = count_pixel_neighbours( surface );
        REQUIRE( 0 == counts[0] );
        for( std::size_t i = 1; i < counts.size(); ++i )
            REQUIRE( 0 == counts[i]  );
    }
}
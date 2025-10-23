#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Partially offscreen", "[clip]" )
{
	Surface surface( 640, 480 );
	surface.clear();

	// Test all inside/outside combinations 
	
	SECTION( "inside to right" )
	{
		draw_line_solid( surface,
			{ 100.f, 240.f },
			{ 1920.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "left to inside" )
	{
		draw_line_solid( surface,
			{ -100.f, 240.f },
			{ 100.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "inside to left" )
	{
		draw_line_solid( surface,
			{ 100.f, 240.f },
			{ -100.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "right to inside" )
	{
		draw_line_solid( surface,
			{ 1920.f, 240.f },
			{ 100.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "inside to top" )
	{
		draw_line_solid( surface,
			{ 320.f, 100.f },
			{ 320.f, -100.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "bottom to inside" )
	{
		draw_line_solid( surface,
			{ 320.f, 600.f },
			{ 320.f, 100.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "inside to bottom" )
	{
		draw_line_solid( surface,
			{ 320.f, 100.f },
			{ 320.f, 600.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "top to inside" )
	{
		draw_line_solid( surface,
			{ 320.f, -100.f },
			{ 320.f, 100.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "diagonal inside to outside" )
	{
		draw_line_solid( surface,
			{ 100.f, 100.f },
			{ 800.f, 600.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "diagonal outside to inside" )
	{
		draw_line_solid( surface,
			{ -100.f, -100.f },
			{ 100.f, 100.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "crossing horizontally" )
	{
		draw_line_solid( surface,
			{ -100.f, 240.f },
			{ 800.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
	
	SECTION( "crossing vertically" )
	{
		draw_line_solid( surface,
			{ 320.f, -100.f },
			{ 320.f, 600.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 1 == pixels );
		REQUIRE( pixels > 0 ); 
	}
}

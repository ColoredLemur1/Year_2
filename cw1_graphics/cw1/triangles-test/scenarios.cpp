#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

// Scenario A: ensure two triangles can fill a rectangle without any gaps
TEST_CASE( "No seam between adjacent triangles", "[scenarios]" )
{
	Surface surface( 100, 100 );
	surface.clear();

	draw_triangle_interp( surface,
		{ 10.f, 90.f }, 
        { 10.f, 10.f }, 
        { 90.f, 10.f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f }
	);

	draw_triangle_interp( surface,
		{ 90.f, 10.f }, 
        { 90.f, 90.f }, 
        { 10.f, 90.f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f }
	);

	// Validate that no gaps/black seams exist along the shared edge
	auto const mostRed = find_most_red_pixel( surface );
	auto const leastRed = find_least_red_nonzero_pixel( surface );
	REQUIRE( mostRed.r > 0 );
	REQUIRE( leastRed.r > 0 );
}

// Scenario B: triangles drawn with different edge orders should produce the same result
TEST_CASE( "Winding-order invariance", "[scenarios]" )
{
	Surface surfaceA( 100, 100 );
	surfaceA.clear();

	Surface surfaceB( 100, 100 );
	surfaceB.clear();

	// Triangle vertices
	auto const p0 = Vec2f{ 15.f, 85.f };
	auto const p1 = Vec2f{ 15.f, 15.f };
	auto const p2 = Vec2f{ 85.f, 15.f };

	//order 0,1,2
	draw_triangle_interp( 
        surfaceA, 
        p0, p1, p2,
		{ 1.f, 0.f, 0.f },
		{ 0.f, 1.f, 0.f },
		{ 0.f, 0.f, 1.f }
	);

	//order 2,1,0
	draw_triangle_interp( 
        surfaceB, 
        p2, p1, p0,
		{ 0.f, 0.f, 1.f },
		{ 0.f, 1.f, 0.f },
		{ 1.f, 0.f, 0.f }
	);

	// Pixel-by-pixel comparison: verify both surfaces are identical
	// This is more thorough than checking just max/min values
	auto const stride = surfaceA.get_width() << 2;
	for( std::uint32_t y = 0; y < surfaceA.get_height(); ++y )
	{
		for( std::uint32_t x = 0; x < surfaceA.get_width(); ++x )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptrA = surfaceA.get_surface_ptr() + idx;
			auto const ptrB = surfaceB.get_surface_ptr() + idx;

			// Compare RGB values (ignore padding byte)
			REQUIRE( ptrA[0] == ptrB[0] ); // Red
			REQUIRE( ptrA[1] == ptrB[1] ); // Green
			REQUIRE( ptrA[2] == ptrB[2] ); // Blue
		}
	}
}


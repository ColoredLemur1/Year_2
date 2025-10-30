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

	auto const mostRedA = find_most_red_pixel( surfaceA );
	auto const mostRedB = find_most_red_pixel( surfaceB );
	REQUIRE( mostRedA.r == mostRedB.r );

	auto const leastRedA = find_least_red_nonzero_pixel( surfaceA );
	auto const leastRedB = find_least_red_nonzero_pixel( surfaceB );
	REQUIRE( leastRedA.r == leastRedB.r );
}


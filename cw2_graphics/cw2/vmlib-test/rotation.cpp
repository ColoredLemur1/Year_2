#include <catch2/catch_amalgamated.hpp>

#include <numbers>

#include "../vmlib/mat44.hpp"
#include "../vmlib/vec4.hpp"

TEST_CASE( "Rotation matrix tests", "[mat44]" )
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	// Simple check: rotating zero degrees should yield an idenity matrix
	SECTION( "Identity" )
	{
		auto const identity = make_rotation_z( 0.f );

		REQUIRE_THAT( (identity[0,0]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[1,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,1]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[2,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,2]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians.
	SECTION( "90 degrees" )
	{
		auto const right = make_rotation_z( std::numbers::pi_v<float>/2.f );

		REQUIRE_THAT( (right[0,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[0,1]), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( (right[0,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (right[1,0]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (right[1,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[1,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (right[2,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[2,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[2,2]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (right[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (right[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 11.7 degrees (this is a somewhat arbitrary pick, to make
	// sure it isn't a special case; it might sense to pick these purely
	// randomly)
	SECTION( "11.7 degrees" )
	{
		auto const right = make_rotation_z( 11.7f*std::numbers::pi_v<float>/180.f );

		REQUIRE_THAT( (right[0,0]), WithinAbs(  0.9792228f, kEps_ ) );
		REQUIRE_THAT( (right[0,1]), WithinAbs( -0.2027873f, kEps_ ) );
		REQUIRE_THAT( (right[0,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (right[1,0]), WithinAbs(  0.2027873f, kEps_ ) );
		REQUIRE_THAT( (right[1,1]), WithinAbs(  0.9792228f, kEps_ ) );
		REQUIRE_THAT( (right[1,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (right[2,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[2,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[2,2]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (right[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (right[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (right[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Test identity for X-axis rotation (0 degrees)
	SECTION( "Rotation X - Identity" )
	{
		auto const identity = make_rotation_x( 0.f );

		REQUIRE_THAT( (identity[0,0]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[1,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,1]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[2,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,2]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians. X axis
	SECTION( "Rotation X - 90 degrees" )
	{
		auto const rotX = make_rotation_x( std::numbers::pi_v<float>/2.f );

		REQUIRE_THAT( (rotX[0,0]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (rotX[0,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[0,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotX[1,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[1,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[1,2]), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( (rotX[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotX[2,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[2,1]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (rotX[2,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotX[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 34.6 degrees around X-axis
	SECTION( "Rotation X - 34.6 degrees" )
	{
		auto const rotX = make_rotation_x( 34.6f*std::numbers::pi_v<float>/180.f );

		REQUIRE_THAT( (rotX[0,0]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (rotX[0,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[0,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotX[1,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[1,1]), WithinAbs( 0.8241261f, kEps_ ) );
		REQUIRE_THAT( (rotX[1,2]), WithinAbs( -0.5664062f, kEps_ ) );
		REQUIRE_THAT( (rotX[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotX[2,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[2,1]), WithinAbs( 0.5664062f, kEps_ ) );
		REQUIRE_THAT( (rotX[2,2]), WithinAbs( 0.8241261f, kEps_ ) );
		REQUIRE_THAT( (rotX[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotX[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotX[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Test identity for Y-axis rotation (0 degrees)
	SECTION( "Rotation Y - Identity" )
	{
		auto const identity = make_rotation_y( 0.f );

		REQUIRE_THAT( (identity[0,0]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[1,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,1]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[2,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,2]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (identity[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (identity[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (identity[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 90 degrees = pi/2 radians. Y axis
	SECTION( "Rotation Y - 90 degrees" )
	{
		auto const rotY = make_rotation_y( std::numbers::pi_v<float>/2.f );

		REQUIRE_THAT( (rotY[0,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[0,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[0,2]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (rotY[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotY[1,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[1,1]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (rotY[1,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotY[2,0]), WithinAbs( -1.f, kEps_ ) );
		REQUIRE_THAT( (rotY[2,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[2,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotY[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[3,3]), WithinAbs( 1.f, kEps_ ) );
	}

	// Rotating 67.7 degrees around Y-axis
	SECTION( "Rotation Y - 67.7 degrees" )
	{
		auto const rotY = make_rotation_y( 67.7f*std::numbers::pi_v<float>/180.f );

		REQUIRE_THAT( (rotY[0,0]), WithinAbs( 0.15512891606f , kEps_ ) );
		REQUIRE_THAT( (rotY[0,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[0,2]), WithinAbs( -0.98789423492f, kEps_ ) );
		REQUIRE_THAT( (rotY[0,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotY[1,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[1,1]), WithinAbs( 1.f, kEps_ ) );
		REQUIRE_THAT( (rotY[1,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[1,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotY[2,0]), WithinAbs( 0.98789423492f, kEps_ ) );
		REQUIRE_THAT( (rotY[2,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[2,2]), WithinAbs( 0.15512891606f, kEps_ ) );
		REQUIRE_THAT( (rotY[2,3]), WithinAbs( 0.f, kEps_ ) );

		REQUIRE_THAT( (rotY[3,0]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[3,1]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[3,2]), WithinAbs( 0.f, kEps_ ) );
		REQUIRE_THAT( (rotY[3,3]), WithinAbs( 1.f, kEps_ ) );
	}
}


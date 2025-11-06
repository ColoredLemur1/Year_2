#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	// This is the function that will be used to benchmark the line drawing operation
	//replace this with your own code.

	// Benchmark to test if color affects line drawing performance
	void draw_line_benchmark_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0)); // width of the surface
		auto const height = std::uint32_t(aState.range(1)); // height of the surface
		auto const lineLength = std::uint32_t(aState.range(2)); // length of the line
		
		// index colors (0-7) the input is the index and the output is the color
		static constexpr ColorU8_sRGB colors[] = {
			{255, 0, 0},    // Red
			{0, 255, 0},    // Green
			{0, 0, 255},    // Blue
			{255, 255, 255}, // White
			{0, 0, 0},      // Black
			{255, 255, 0},  // Yellow
			{0, 255, 255},  // Cyan
			{255, 0, 255}   // Magenta
		};
		auto const colorIndex = std::uint32_t(aState.range(3)) % (sizeof(colors) / sizeof(colors[0]));
		auto const color = colors[colorIndex];

		SurfaceEx surface( width, height );
		surface.clear();

		// Draw a horizontal line starting from the center-left of the surface
		Vec2f begin{ width / 4.0f, height / 2.0f };
		Vec2f end{ begin.x + lineLength, begin.y };

		for( auto _ : aState )
		{
			draw_line_solid( surface, begin, end, color );

			// ClobberMemory() ensures that the compiler won't optimize away
			// our line drawing operation. (Unlikely, but technically possible.)
			benchmark::ClobberMemory(); 
		}
	}
}




// Test cases testing the 3 variables:
// Variable 1: Surface Area (width x height)
// Variable 2: Line Length
// Variable 3: Line Color

// Small surface area (640x360)
BENCHMARK( draw_line_benchmark_ )
	->Args( { 640, 360, 10, 0 } )   // short line, red
	->Args( { 640, 360, 100, 0 } )  // medium line, red
	->Args( { 640, 360, 500, 0 } )  // long line, red
	->Args( { 640, 360, 100, 1 } )  // medium line, green
	->Args( { 640, 360, 100, 2 } )  // medium line, blue
	->Args( { 640, 360, 100, 3 } )  // medium line, white
	->Args( { 640, 360, 100, 4 } )  // medium line, black
;

// Medium surface area (1920x1080)
BENCHMARK( draw_line_benchmark_ )
	->Args( { 1920, 1080, 10, 0 } )   // short line, red
	->Args( { 1920, 1080, 100, 0 } )  // medium line, red
	->Args( { 1920, 1080, 500, 0 } )  // long line, red
	->Args( { 1920, 1080, 1000, 0 } ) // very long line, red
	->Args( { 1920, 1080, 5000, 0 } ) // extremely long line, red
	->Args( { 1920, 1080, 100, 1 } )  // medium line, green
	->Args( { 1920, 1080, 100, 2 } )  // medium line, blue
	->Args( { 1920, 1080, 100, 3 } )  // medium line, white
	->Args( { 1920, 1080, 100, 4 } )  // medium line, black
	->Args( { 1920, 1080, 1000, 1 } ) // long line, green
	->Args( { 1920, 1080, 1000, 3 } ) // long line, white
;

// Large surface area (3840x2160)
BENCHMARK( draw_line_benchmark_ )
	->Args( { 3840, 2160, 10, 0 } )   // short line, red
	->Args( { 3840, 2160, 100, 0 } )  // medium line, red
	->Args( { 3840, 2160, 500, 0 } )  // long line, red
	->Args( { 3840, 2160, 1000, 0 } ) // very long line, red
	->Args( { 3840, 2160, 5000, 0 } ) // extremely long line, red
	->Args( { 3840, 2160, 100, 1 } )  // medium line, green
	->Args( { 3840, 2160, 100, 2 } )  // medium line, blue
	->Args( { 3840, 2160, 100, 3 } )  // medium line, white
	->Args( { 3840, 2160, 100, 4 } )  // medium line, black
;

// Very large surface area (7680x4320)
BENCHMARK( draw_line_benchmark_ )
	->Args( { 7680, 4320, 10, 0 } )   // short line, red
	->Args( { 7680, 4320, 100, 0 } )  // medium line, red
	->Args( { 7680, 4320, 500, 0 } )  // short line, red
	->Args( { 7680, 4320, 1000, 0 } ) // long line, red
	->Args( { 7680, 4320, 5000, 0 } ) // extremely long line, red
	->Args( { 7680, 4320, 100, 1 } )  // medium line, green
	->Args( { 7680, 4320, 100, 2 } )  // medium line, blue
	->Args( { 7680, 4320, 100, 3 } )  // medium line, white
	->Args( { 7680, 4320, 100, 4 } )  // medium line, black
;


BENCHMARK_MAIN();

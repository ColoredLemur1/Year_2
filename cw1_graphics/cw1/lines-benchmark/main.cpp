#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	// Benchmark: draw N lines of a given length and color on a 2D surface.
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: line length (pixels)
	//   3: color index (0-7)
	//   4: number of lines
	void draw_line_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = std::uint32_t(aState.range(2));
		auto const numLines   = std::uint32_t(aState.range(4));

		// Indexed colors (0-7)
		static constexpr ColorU8_sRGB colors[] = {
			{255, 0, 0},     // 0 Red
			{0, 255, 0},     // 1 Green
			{0, 0, 255},     // 2 Blue
			{255, 255, 255}, // 3 White
			{0, 0, 0},       // 4 Black
			{255, 255, 0},   // 5 Yellow
			{0, 255, 255},   // 6 Cyan
			{255, 0, 255}    // 7 Magenta
		};
		auto const colorIndex = std::uint32_t(aState.range(3)) % (sizeof(colors) / sizeof(colors[0]));
		auto const color      = colors[colorIndex];

		SurfaceEx surface( width, height );
		surface.clear();

		// Base X so lines start somewhere near the left-middle area
		float const baseX = std::max(0.0f, (width - std::min(lineLength, width)) * 0.25f);

		for( auto _ : aState )
		{
			// Draw numLines horizontal lines at different Y positions; wrap if needed
			for( std::uint32_t i = 0; i < numLines; ++i )
			{
				float const y = float(i % std::max<std::uint32_t>(1, height));
				Vec2f begin{ baseX, y };
				Vec2f end{ begin.x + float(lineLength), y };
				draw_line_solid( surface, begin, end, color );
			}

			// Prevent over-optimization of the draw calls
			benchmark::ClobberMemory();
		}
	}
}

// Benchmark cases at fixed resolution 1920x1080 varying:
//  - line length
//  - line color
//  - number of lines
// Args are: { width, height, length, colorIndex, numLines }
BENCHMARK( draw_line_benchmark_ )
	// Vary line length with modest number of lines
	->Args( { 1920, 1080, 10,   0, 100 } )
	->Args( { 1920, 1080, 100,  0, 100 } )
	->Args( { 1920, 1080, 500,  0, 100 } )
	->Args( { 1920, 1080, 1000, 0, 100 } )
	// Vary color (same length, same count)
	->Args( { 1920, 1080, 500,  0, 100 } )
	->Args( { 1920, 1080, 500,  1, 100 } )
	->Args( { 1920, 1080, 500,  2, 100 } )
	->Args( { 1920, 1080, 500,  3, 100 } )
	->Args( { 1920, 1080, 500,  4, 100 } )
	->Args( { 1920, 1080, 500,  5, 100 } )
	->Args( { 1920, 1080, 500,  6, 100 } )
	->Args( { 1920, 1080, 500,  7, 100 } )
	// Vary number of lines (work scaling)
	->Args( { 1920, 1080, 500,  0,    1 } )
	->Args( { 1920, 1080, 500,  0,   10 } )
	->Args( { 1920, 1080, 500,  0,  100 } )
	->Args( { 1920, 1080, 500,  0, 1000 } )
;


BENCHMARK_MAIN();

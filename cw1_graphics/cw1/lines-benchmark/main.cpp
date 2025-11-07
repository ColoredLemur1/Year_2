#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	//function to draw horizontal lines with DDA
	void draw_lines_helper( SurfaceEx& surface, std::uint32_t width, std::uint32_t height, 
	                        std::uint32_t lineLength, std::uint32_t numLines, ColorU8_sRGB color )
	{
		// Base X so lines start somewhere near the left-middle area
		float const baseX = std::max(0.0f, (width - std::min(lineLength, width)) * 0.25f);

		// Draw numLines horizontal lines at different Y positions
		for( std::uint32_t i = 0; i < numLines; ++i )
		{
			float const y = float(i % std::max<std::uint32_t>(1, height));
			Vec2f begin{ baseX, y };
			Vec2f end{ begin.x + float(lineLength), y };
			draw_line_solid( surface, begin, end, color );
		}
	}

	//function to draw horizontal lines with Bresenham
	void draw_ex_lines_helper( SurfaceEx& surface, std::uint32_t width, std::uint32_t height, 
	                           std::uint32_t lineLength, std::uint32_t numLines, ColorU8_sRGB color )
	{
		// Base X so lines start somewhere near the left-middle area
		float const baseX = std::max(0.0f, (width - std::min(lineLength, width)) * 0.25f);

		// Draw numLines horizontal lines at different Y positions
		for( std::uint32_t i = 0; i < numLines; ++i )
		{
			float const y = float(i % std::max<std::uint32_t>(1, height));
			Vec2f begin{ baseX, y };
			Vec2f end{ begin.x + float(lineLength), y };
			draw_ex_line_solid( surface, begin, end, color );
		}
	}

	//function to draw diagonal lines with DDA
	void draw_diagonal_lines_dda_helper( SurfaceEx& surface, std::uint32_t width, std::uint32_t height, 
	                                     std::uint32_t lineLength, std::uint32_t numLines, ColorU8_sRGB color )
	{
		// Draw numLines diagonal lines starting at different positions
		for( std::uint32_t i = 0; i < numLines; ++i )
		{
			// Start position - spread lines across the surface
			float const startX = float(i * 50 % std::max<std::uint32_t>(1, width - lineLength));
			float const startY = float(i * 30 % std::max<std::uint32_t>(1, height - lineLength));
			
			// Diagonal line
			Vec2f begin{ startX, startY };
			Vec2f end{ startX + float(lineLength), startY + float(lineLength) };
			draw_line_solid( surface, begin, end, color );
		}
	}

	//function to draw diagonal lines with Bresenham
	void draw_diagonal_lines_bresenham_helper( SurfaceEx& surface, std::uint32_t width, std::uint32_t height, 
	                                           std::uint32_t lineLength, std::uint32_t numLines, ColorU8_sRGB color )
	{
		// Draw numLines diagonal lines starting at different positions
		for( std::uint32_t i = 0; i < numLines; ++i )
		{
			// Start position - spread lines across the surface
			float const startX = float(i * 50 % std::max<std::uint32_t>(1, width - lineLength));
			float const startY = float(i * 30 % std::max<std::uint32_t>(1, height - lineLength));
			
			// Diagonal line
			Vec2f begin{ startX, startY };
			Vec2f end{ startX + float(lineLength), startY + float(lineLength) };
			draw_ex_line_solid( surface, begin, end, color );
		}
	}

	//function to draw diagonal lines with draw_ex_diagonal
	void draw_diagonal_lines_baseline_helper( SurfaceEx& surface, std::uint32_t width, std::uint32_t height, 
	                                          std::uint32_t lineLength, std::uint32_t numLines, ColorU8_sRGB color )
	{
		// Draw numLines diagonal lines starting at different positions
		for( std::uint32_t i = 0; i < numLines; ++i )
		{
			// Start position - spread lines across the surface
			float const startX = float(i * 50 % std::max<std::uint32_t>(1, width - lineLength));
			float const startY = float(i * 30 % std::max<std::uint32_t>(1, height - lineLength));
			
			// Diagonal line
			draw_ex_diagonal( surface, { startX, startY }, float(lineLength), color );
		}
	}

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

	// Benchmark: Vary line length
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: line length (pixels)
	void draw_line_length_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = std::uint32_t(aState.range(2));
		auto const numLines   = 100u;  // Fixed: 100 lines
		auto const color      = colors[0];  // Fixed: Red

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_lines_helper( surface, width, height, lineLength, numLines, color );
			benchmark::ClobberMemory();
		}
	}

	// Benchmark: Vary line color
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: color index (0-7)
	void draw_line_color_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = 500u;  // Fixed: 500 pixels
		auto const numLines   = 100u;  // Fixed: 100 lines
		auto const colorIndex = std::uint32_t(aState.range(2)) % (sizeof(colors) / sizeof(colors[0]));
		auto const color      = colors[colorIndex];

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_lines_helper( surface, width, height, lineLength, numLines, color );
			benchmark::ClobberMemory();
		}
	}

	// Benchmark: Vary number of lines
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: number of lines
	void draw_line_count_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = 500u;  // Fixed: 500 pixels
		auto const numLines   = std::uint32_t(aState.range(2));
		auto const color      = colors[0];  // Fixed: Red

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_lines_helper( surface, width, height, lineLength, numLines, color );
			benchmark::ClobberMemory();
		}
	}

	// Benchmark: Vary line length with Bresenham (draw_ex_line_solid) - horizontal lines
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: line length (pixels)
	void draw_ex_line_length_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = std::uint32_t(aState.range(2));
		auto const numLines   = 100u;  // Fixed: 100 lines
		auto const color      = colors[0];  // Fixed: Red

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_ex_lines_helper( surface, width, height, lineLength, numLines, color );
			benchmark::ClobberMemory();
		}
	}

	// Benchmark: Diagonal lines with DDA (draw_line_solid)
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: line length (pixels)
	void draw_diagonal_dda_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = std::uint32_t(aState.range(2));
		auto const numLines   = 100u;  // Fixed: 100 lines
		auto const color      = colors[0];  // Fixed: Red

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_diagonal_lines_dda_helper( surface, width, height, lineLength, numLines, color );
			benchmark::ClobberMemory();
		}
	}

	// Benchmark: Diagonal lines with Bresenham (draw_ex_line_solid)
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: line length (pixels)
	void draw_diagonal_bresenham_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = std::uint32_t(aState.range(2));
		auto const numLines   = 100u;  // Fixed: 100 lines
		auto const color      = colors[0];  // Fixed: Red

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_diagonal_lines_bresenham_helper( surface, width, height, lineLength, numLines, color );
			benchmark::ClobberMemory();
		}
	}

	// Benchmark: Diagonal lines with draw_ex_diagonal (baseline - fastest)
	// Parameters (via aState.range):
	//   0: width  (keep 1920)
	//   1: height (keep 1080)
	//   2: line length (pixels)
	void draw_diagonal_baseline_benchmark_( benchmark::State& aState )
	{
		auto const width      = std::uint32_t(aState.range(0));
		auto const height     = std::uint32_t(aState.range(1));
		auto const lineLength = std::uint32_t(aState.range(2));
		auto const numLines   = 100u;  // Fixed: 100 lines
		auto const color      = colors[0];  // Fixed: Red

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_diagonal_lines_baseline_helper( surface, width, height, lineLength, numLines, color );
			benchmark::ClobberMemory();
		}
	}
}

// Benchmark: Vary line length at fixed resolution 1920x1080
// Args are: { width, height, lineLength }

BENCHMARK( draw_line_length_benchmark_ )
	->Args( { 1920, 1080, 10 } )
	->Args( { 1920, 1080, 100 } )
	->Args( { 1920, 1080, 500 } )
	->Args( { 1920, 1080, 1000 } )
	->Args( { 1920, 1080, 5000 } )
;
// Benchmark: Vary line color at fixed resolution 1920x1080
// Args are: { width, height, colorIndex }
BENCHMARK( draw_line_color_benchmark_ )
	->Args( { 1920, 1080, 0 } )  // Red
	->Args( { 1920, 1080, 1 } )  // Green
	->Args( { 1920, 1080, 2 } )  // Blue
	->Args( { 1920, 1080, 3 } )  // White
	->Args( { 1920, 1080, 4 } )  // Black
	->Args( { 1920, 1080, 5 } )  // Yellow
	->Args( { 1920, 1080, 6 } )  // Cyan
	->Args( { 1920, 1080, 7 } )  // Magenta
;

// Benchmark: Vary number of lines at fixed resolution 1920x1080
// Args are: { width, height, numLines }
BENCHMARK( draw_line_count_benchmark_ )
	->Args( { 1920, 1080, 1 } )
	->Args( { 1920, 1080, 10 } )
	->Args( { 1920, 1080, 100 } )
	->Args( { 1920, 1080, 1000 } )
;

// Benchmark: Vary line length with Bresenham (draw_ex_line_solid) - horizontal lines
// Args are: { width, height, lineLength }
BENCHMARK( draw_ex_line_length_benchmark_ )
	->Args( { 1920, 1080, 10 } )
	->Args( { 1920, 1080, 100 } )
	->Args( { 1920, 1080, 500 } )
	->Args( { 1920, 1080, 1000 } )
	->Args( { 1920, 1080, 5000 } )
;

// Benchmark: Diagonal lines with DDA (draw_line_solid)
// Args are: { width, height, lineLength }
BENCHMARK( draw_diagonal_dda_benchmark_ )
	->Args( { 1920, 1080, 10 } )
	->Args( { 1920, 1080, 100 } )
	->Args( { 1920, 1080, 500 } )
	->Args( { 1920, 1080, 1000 } )
	->Args( { 1920, 1080, 5000 } )
;

// Benchmark: Diagonal lines with Bresenham (draw_ex_line_solid)
// Args are: { width, height, lineLength }
BENCHMARK( draw_diagonal_bresenham_benchmark_ )
	->Args( { 1920, 1080, 10 } )
	->Args( { 1920, 1080, 100 } )
	->Args( { 1920, 1080, 500 } )
	->Args( { 1920, 1080, 1000 } )
	->Args( { 1920, 1080, 5000 } )
;

// Benchmark: Diagonal lines with draw_ex_diagonal (baseline - fastest)
// Args are: { width, height, lineLength }
BENCHMARK( draw_diagonal_baseline_benchmark_ )
	->Args( { 1920, 1080, 10 } )
	->Args( { 1920, 1080, 100 } )
	->Args( { 1920, 1080, 500 } )
	->Args( { 1920, 1080, 1000 } )
	->Args( { 1920, 1080, 5000 } )
;


BENCHMARK_MAIN();

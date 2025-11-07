#include <benchmark/benchmark.h>

#include <algorithm>

#include <cassert>

#include "../draw2d/image.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	// Helper function to calculate position and effective dimensions
	void calculate_blit_params( 
		std::uint32_t width, std::uint32_t height,
		std::uint32_t clipScenario, std::uint32_t imageSizeFactor,
		std::uint32_t sourceWidth, std::uint32_t sourceHeight,
		float& outPosX, float& outPosY,
		std::uint32_t& outEffectiveWidth, std::uint32_t& outEffectiveHeight
	)
	{
		// Determine effective image dimensions based on size factor
		switch( imageSizeFactor )
		{
			case 0: outEffectiveWidth = 256; outEffectiveHeight = 256; break;
			case 1: outEffectiveWidth = 384; outEffectiveHeight = 384; break;
			case 2: outEffectiveWidth = 512; outEffectiveHeight = 512; break;
			case 3: outEffectiveWidth = 768; outEffectiveHeight = 768; break;
			case 4: outEffectiveWidth = 1024; outEffectiveHeight = 1024; break;
			default: 
				outEffectiveWidth = sourceWidth;
				outEffectiveHeight = sourceHeight;
				break;
		}

		// Clamp to actual source image size
		outEffectiveWidth = std::min( outEffectiveWidth, sourceWidth );
		outEffectiveHeight = std::min( outEffectiveHeight, sourceHeight );

		// Calculate position based on clipping scenario
		switch( clipScenario )
		{
			case 0: // Center - full visible (minimal clipping, ~100% visible)
				outPosX = float(width / 2) - float(outEffectiveWidth / 2);
				outPosY = float(height / 2) - float(outEffectiveHeight / 2);
				break;
			case 1: // Light clipping (about 75% visible)
				outPosX = float(width) - float(outEffectiveWidth) * 0.75f;
				outPosY = float(height) - float(outEffectiveHeight) * 0.75f;
				break;
			case 2: // Partial clipping (about 50% visible)
				outPosX = float(width) - float(outEffectiveWidth) * 0.5f;
				outPosY = float(height) - float(outEffectiveHeight) * 0.5f;
				break;
			case 3: // Heavy clipping (about 25% visible)
				outPosX = float(width) - float(outEffectiveWidth) * 0.25f;
				outPosY = float(height) - float(outEffectiveHeight) * 0.25f;
				break;
			case 4: // Very heavy clipping (about 10% visible)
				outPosX = float(width) - float(outEffectiveWidth) * 0.1f;
				outPosY = float(height) - float(outEffectiveHeight) * 0.1f;
				break;
			default:
				outPosX = 0.f;
				outPosY = 0.f;
				break;
		}
	}

	// Helper function to calculate bytes processed
	std::size_t calculate_bytes_processed(
		std::uint32_t width, std::uint32_t height,
		float posX, float posY,
		std::uint32_t effectiveWidth, std::uint32_t effectiveHeight,
		std::uint32_t sourceWidth, std::uint32_t sourceHeight
	)
	{
		int const posXi = static_cast<int>(posX);
		int const posYi = static_cast<int>(posY);
		
		int const clipStartX = std::max(0, posXi);
		int const clipStartY = std::max(0, posYi);
		int const clipEndX = std::min(static_cast<int>(width), posXi + static_cast<int>(effectiveWidth));
		int const clipEndY = std::min(static_cast<int>(height), posYi + static_cast<int>(effectiveHeight));
		
		int const finalClipEndX = std::min(clipEndX, static_cast<int>(sourceWidth));
		int const finalClipEndY = std::min(clipEndY, static_cast<int>(sourceHeight));
		
		int const clippedWidth = std::max(0, finalClipEndX - clipStartX);
		int const clippedHeight = std::max(0, finalClipEndY - clipStartY);

		return 2ULL * clippedWidth * clippedHeight * 4;
	}

	// Benchmark: blit_masked
	// Parameters (via aState.range):
	//   0: surface width
	//   1: surface height
	//   2: clipping scenario (0=center/full, 1=corner/partial, 2=edge/heavy)
	//   3: effective image size factor (0=256x256, 1=512x512, 2=1024x1024)
	void blit_masked_benchmark_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));
		auto const clipScenario = std::uint32_t(aState.range(2));
		auto const imageSizeFactor = std::uint32_t(aState.range(3));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		float posX, posY;
		std::uint32_t effectiveWidth, effectiveHeight;
		calculate_blit_params( width, height, clipScenario, imageSizeFactor,
		                       source->get_width(), source->get_height(),
		                       posX, posY, effectiveWidth, effectiveHeight );

		for( auto _ : aState )
		{
			blit_masked( surface, *source, {posX, posY} );
			benchmark::ClobberMemory(); 
		}

		auto const bytesPerIter = calculate_bytes_processed(
			width, height, posX, posY, effectiveWidth, effectiveHeight,
			source->get_width(), source->get_height()
		);
		aState.SetBytesProcessed( bytesPerIter * aState.iterations() );
	}

	// Benchmark: blit_ex_solid
	// Parameters (via aState.range):
	//   0: surface width
	//   1: surface height
	//   2: clipping scenario (0=center/full, 1=corner/partial, 2=edge/heavy)
	//   3: effective image size factor (0=256x256, 1=512x512, 2=1024x1024)
	void blit_ex_solid_benchmark_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));
		auto const clipScenario = std::uint32_t(aState.range(2));
		auto const imageSizeFactor = std::uint32_t(aState.range(3));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		float posX, posY;
		std::uint32_t effectiveWidth, effectiveHeight;
		calculate_blit_params( width, height, clipScenario, imageSizeFactor,
		                       source->get_width(), source->get_height(),
		                       posX, posY, effectiveWidth, effectiveHeight );

		for( auto _ : aState )
		{
			blit_ex_solid( surface, *source, {posX, posY} );
			benchmark::ClobberMemory(); 
		}

		auto const bytesPerIter = calculate_bytes_processed(
			width, height, posX, posY, effectiveWidth, effectiveHeight,
			source->get_width(), source->get_height()
		);
		aState.SetBytesProcessed( bytesPerIter * aState.iterations() );
	}

	// Benchmark: blit_ex_memcpy
	// Parameters (via aState.range):
	//   0: surface width
	//   1: surface height
	//   2: clipping scenario (0=center/full, 1=corner/partial, 2=edge/heavy)
	//   3: effective image size factor (0=256x256, 1=512x512, 2=1024x1024)
	void blit_ex_memcpy_benchmark_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));
		auto const clipScenario = std::uint32_t(aState.range(2));
		auto const imageSizeFactor = std::uint32_t(aState.range(3));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		float posX, posY;
		std::uint32_t effectiveWidth, effectiveHeight;
		calculate_blit_params( width, height, clipScenario, imageSizeFactor,
		                       source->get_width(), source->get_height(),
		                       posX, posY, effectiveWidth, effectiveHeight );

		for( auto _ : aState )
		{
			blit_ex_memcpy( surface, *source, {posX, posY} );
			benchmark::ClobberMemory(); 
		}

		auto const bytesPerIter = calculate_bytes_processed(
			width, height, posX, posY, effectiveWidth, effectiveHeight,
			source->get_width(), source->get_height()
		);
		aState.SetBytesProcessed( bytesPerIter * aState.iterations() );
	}
}

// Benchmark: blit_masked
// Resolutions: 1920x1080, 2560x1440, 3840x2160, 5120x2880, 7680x4320
// Clipping: 0=center/full, 1=light/75%, 2=partial/50%, 3=heavy/25%, 4=very_heavy/10%
// Image sizes: 0=256x256, 1=384x384, 2=512x512, 3=768x768, 4=1024x1024
BENCHMARK( blit_masked_benchmark_ )
	// Test resolution: 1920x1080, center position, all image sizes
	->Args( { 1920, 1080, 0, 0 } )  // center, 256x256
	->Args( { 1920, 1080, 0, 1 } )  // center, 384x384
	->Args( { 1920, 1080, 0, 2 } )  // center, 512x512
	->Args( { 1920, 1080, 0, 3 } )  // center, 768x768
	->Args( { 1920, 1080, 0, 4 } )  // center, 1024x1024
	// Test resolution: 2560x1440, center position, all image sizes
	->Args( { 2560, 1440, 0, 0 } )  // center, 256x256
	->Args( { 2560, 1440, 0, 1 } )  // center, 384x384
	->Args( { 2560, 1440, 0, 2 } )  // center, 512x512
	->Args( { 2560, 1440, 0, 3 } )  // center, 768x768
	->Args( { 2560, 1440, 0, 4 } )  // center, 1024x1024
	// Test resolution: 3840x2160, center position, all image sizes
	->Args( { 3840, 2160, 0, 0 } )  // center, 256x256
	->Args( { 3840, 2160, 0, 1 } )  // center, 384x384
	->Args( { 3840, 2160, 0, 2 } )  // center, 512x512
	->Args( { 3840, 2160, 0, 3 } )  // center, 768x768
	->Args( { 3840, 2160, 0, 4 } )  // center, 1024x1024
	// Test resolution: 5120x2880, center position, all image sizes
	->Args( { 5120, 2880, 0, 0 } )  // center, 256x256
	->Args( { 5120, 2880, 0, 1 } )  // center, 384x384
	->Args( { 5120, 2880, 0, 2 } )  // center, 512x512
	->Args( { 5120, 2880, 0, 3 } )  // center, 768x768
	->Args( { 5120, 2880, 0, 4 } )  // center, 1024x1024
	// Test resolution: 7680x4320, center position, all image sizes
	->Args( { 7680, 4320, 0, 0 } )  // center, 256x256
	->Args( { 7680, 4320, 0, 1 } )  // center, 384x384
	->Args( { 7680, 4320, 0, 2 } )  // center, 512x512
	->Args( { 7680, 4320, 0, 3 } )  // center, 768x768
	->Args( { 7680, 4320, 0, 4 } )  // center, 1024x1024
	// Test clipping scenarios: 1920x1080, 512x512 image, all clip scenarios
	->Args( { 1920, 1080, 0, 2 } )  // center, 512x512
	->Args( { 1920, 1080, 1, 2 } )  // light clip, 512x512
	->Args( { 1920, 1080, 2, 2 } )  // partial clip, 512x512
	->Args( { 1920, 1080, 3, 2 } )  // heavy clip, 512x512
	->Args( { 1920, 1080, 4, 2 } )  // very heavy clip, 512x512
;

// Benchmark: blit_ex_solid
// Resolutions: 1920x1080, 2560x1440, 3840x2160, 5120x2880, 7680x4320
// Clipping: 0=center/full, 1=light/75%, 2=partial/50%, 3=heavy/25%, 4=very_heavy/10%
// Image sizes: 0=256x256, 1=384x384, 2=512x512, 3=768x768, 4=1024x1024
BENCHMARK( blit_ex_solid_benchmark_ )
	// Test resolution: 1920x1080, center position, all image sizes
	->Args( { 1920, 1080, 0, 0 } )  // center, 256x256
	->Args( { 1920, 1080, 0, 1 } )  // center, 384x384
	->Args( { 1920, 1080, 0, 2 } )  // center, 512x512
	->Args( { 1920, 1080, 0, 3 } )  // center, 768x768
	->Args( { 1920, 1080, 0, 4 } )  // center, 1024x1024
	// Test resolution: 2560x1440, center position, all image sizes
	->Args( { 2560, 1440, 0, 0 } )  // center, 256x256
	->Args( { 2560, 1440, 0, 1 } )  // center, 384x384
	->Args( { 2560, 1440, 0, 2 } )  // center, 512x512
	->Args( { 2560, 1440, 0, 3 } )  // center, 768x768
	->Args( { 2560, 1440, 0, 4 } )  // center, 1024x1024
	// Test resolution: 3840x2160, center position, all image sizes
	->Args( { 3840, 2160, 0, 0 } )  // center, 256x256
	->Args( { 3840, 2160, 0, 1 } )  // center, 384x384
	->Args( { 3840, 2160, 0, 2 } )  // center, 512x512
	->Args( { 3840, 2160, 0, 3 } )  // center, 768x768
	->Args( { 3840, 2160, 0, 4 } )  // center, 1024x1024
	// Test resolution: 5120x2880, center position, all image sizes
	->Args( { 5120, 2880, 0, 0 } )  // center, 256x256
	->Args( { 5120, 2880, 0, 1 } )  // center, 384x384
	->Args( { 5120, 2880, 0, 2 } )  // center, 512x512
	->Args( { 5120, 2880, 0, 3 } )  // center, 768x768
	->Args( { 5120, 2880, 0, 4 } )  // center, 1024x1024
	// Test resolution: 7680x4320, center position, all image sizes
	->Args( { 7680, 4320, 0, 0 } )  // center, 256x256
	->Args( { 7680, 4320, 0, 1 } )  // center, 384x384
	->Args( { 7680, 4320, 0, 2 } )  // center, 512x512
	->Args( { 7680, 4320, 0, 3 } )  // center, 768x768
	->Args( { 7680, 4320, 0, 4 } )  // center, 1024x1024
	// Test clipping scenarios: 1920x1080, 512x512 image, all clip scenarios
	->Args( { 1920, 1080, 0, 2 } )  // center, 512x512
	->Args( { 1920, 1080, 1, 2 } )  // light clip, 512x512
	->Args( { 1920, 1080, 2, 2 } )  // partial clip, 512x512
	->Args( { 1920, 1080, 3, 2 } )  // heavy clip, 512x512
	->Args( { 1920, 1080, 4, 2 } )  // very heavy clip, 512x512
;

// Benchmark: blit_ex_memcpy
// Resolutions: 1920x1080, 2560x1440, 3840x2160, 5120x2880, 7680x4320
// Clipping: 0=center/full, 1=light/75%, 2=partial/50%, 3=heavy/25%, 4=very_heavy/10%
// Image sizes: 0=256x256, 1=384x384, 2=512x512, 3=768x768, 4=1024x1024
BENCHMARK( blit_ex_memcpy_benchmark_ )
	// Test resolution: 1920x1080, center position, all image sizes
	->Args( { 1920, 1080, 0, 0 } )  // center, 256x256
	->Args( { 1920, 1080, 0, 1 } )  // center, 384x384
	->Args( { 1920, 1080, 0, 2 } )  // center, 512x512
	->Args( { 1920, 1080, 0, 3 } )  // center, 768x768
	->Args( { 1920, 1080, 0, 4 } )  // center, 1024x1024
	// Test resolution: 2560x1440, center position, all image sizes
	->Args( { 2560, 1440, 0, 0 } )  // center, 256x256
	->Args( { 2560, 1440, 0, 1 } )  // center, 384x384
	->Args( { 2560, 1440, 0, 2 } )  // center, 512x512
	->Args( { 2560, 1440, 0, 3 } )  // center, 768x768
	->Args( { 2560, 1440, 0, 4 } )  // center, 1024x1024
	// Test resolution: 3840x2160, center position, all image sizes
	->Args( { 3840, 2160, 0, 0 } )  // center, 256x256
	->Args( { 3840, 2160, 0, 1 } )  // center, 384x384
	->Args( { 3840, 2160, 0, 2 } )  // center, 512x512
	->Args( { 3840, 2160, 0, 3 } )  // center, 768x768
	->Args( { 3840, 2160, 0, 4 } )  // center, 1024x1024
	// Test resolution: 5120x2880, center position, all image sizes
	->Args( { 5120, 2880, 0, 0 } )  // center, 256x256
	->Args( { 5120, 2880, 0, 1 } )  // center, 384x384
	->Args( { 5120, 2880, 0, 2 } )  // center, 512x512
	->Args( { 5120, 2880, 0, 3 } )  // center, 768x768
	->Args( { 5120, 2880, 0, 4 } )  // center, 1024x1024
	// Test resolution: 7680x4320, center position, all image sizes
	->Args( { 7680, 4320, 0, 0 } )  // center, 256x256
	->Args( { 7680, 4320, 0, 1 } )  // center, 384x384
	->Args( { 7680, 4320, 0, 2 } )  // center, 512x512
	->Args( { 7680, 4320, 0, 3 } )  // center, 768x768
	->Args( { 7680, 4320, 0, 4 } )  // center, 1024x1024
	// Test clipping scenarios: 1920x1080, 512x512 image, all clip scenarios
	->Args( { 1920, 1080, 0, 2 } )  // center, 512x512
	->Args( { 1920, 1080, 1, 2 } )  // light clip, 512x512
	->Args( { 1920, 1080, 2, 2 } )  // partial clip, 512x512
	->Args( { 1920, 1080, 3, 2 } )  // heavy clip, 512x512
	->Args( { 1920, 1080, 4, 2 } )  // very heavy clip, 512x512
;

BENCHMARK_MAIN();

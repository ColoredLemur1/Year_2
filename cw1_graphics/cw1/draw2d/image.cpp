#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"{}\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	// Get dimensions of the image and the surface
	int width = aImage.get_width();
	int height = aImage.get_height();
	int surfaceWidth = aSurface.get_width();
	int surfaceHeight = aSurface.get_height();

	// rectangular clipping is used to get the visible region of the image on the surface
	int imgStartX = static_cast<int>(aPosition.x);
	int imgStartY = static_cast<int>(aPosition.y);
	int imgEndX = imgStartX + width;
	int imgEndY = imgStartY + height;

	// clip the image to the surface bounds
	int clipStartX = std::max(0, imgStartX);
	int clipStartY = std::max(0, imgStartY);
	int clipEndX = std::min(surfaceWidth, imgEndX);
	int clipEndY = std::min(surfaceHeight, imgEndY);

	// if the image is off the surface exit the function
	if (clipStartX >= clipEndX || clipStartY >= clipEndY) {
		return; 
	}

	// calculate the source image offsets (where to start reading from image)
	int srcOffsetX = clipStartX - imgStartX;
	int srcOffsetY = clipStartY - imgStartY;
	int clipWidth = clipEndX - clipStartX;
	int clipHeight = clipEndY - clipStartY;

	// iterate only over the visible region no per-pixel bounds checks needed for performance
	for (int y = 0; y < clipHeight; ++y) {
		for (int x = 0; x < clipWidth; ++x) {
			// coordinates of the pixel in the source image
			int srcX = srcOffsetX + x;
			int srcY = srcOffsetY + y;

			// get the color of the pixel from the source image
			ColorU8_sRGB_Alpha pixel = aImage.get_pixel(srcX, srcY);
			
			// if the pixel is transparent skip it
			if (pixel.a == 0) {
				continue;
			}
			
			// Alpha blending with black background (0, 0, 0)
			float alpha = pixel.a / 255.0f;
			
			// Perform alpha blending
			std::uint8_t blendedR = static_cast<std::uint8_t>(pixel.r * alpha);
			std::uint8_t blendedG = static_cast<std::uint8_t>(pixel.g * alpha);
			std::uint8_t blendedB = static_cast<std::uint8_t>(pixel.b * alpha);
			
			ColorU8_sRGB blendedColor = { blendedR, blendedG, blendedB };
			
			// Draw the pixel at the clipped position
			aSurface.set_pixel_srgb(clipStartX + x, clipStartY + y, blendedColor);
		}
	}
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}

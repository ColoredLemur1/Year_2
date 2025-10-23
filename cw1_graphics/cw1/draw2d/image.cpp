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
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	int width = aImage.get_width();
	int height = aImage.get_height();
	int surfaceWidth = aSurface.get_width();
	int surfaceHeight = aSurface.get_height();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int posX = aPosition.x + x;
			int posY = aPosition.y + y;

			
			if (posX >= 0 && posX < surfaceWidth && posY >= 0 && posY < surfaceHeight) {
				// Check if source coordinates are within image bounds
				if (x >= 0 && x < width && y >= 0 && y < height) {
					ColorU8_sRGB_Alpha pixel = aImage.get_pixel(x, y);
					if (!(pixel.a == 0)) { 
						ColorU8_sRGB color = { pixel.r, pixel.g, pixel.b };
						aSurface.set_pixel_srgb(posX, posY, color);
					}
				}
			}
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

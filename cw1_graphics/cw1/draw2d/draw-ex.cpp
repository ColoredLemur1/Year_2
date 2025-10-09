#include "draw-ex.hpp"

#include <algorithm>

#include <cstring> // for std::memcpy()

#include "draw.hpp"
#include "image.hpp"
#include "surface-ex.hpp"

void draw_ex_line_solid( SurfaceEx& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aBegin;   // is properly implemented.
	(void)aEnd;
	(void)aColor;
}

void blit_ex_solid( SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	(void)aSurface;  // Avoid warnings about unused arguments until the
	(void)aImage;    // function is properly implemented.
	(void)aPosition;
}

void blit_ex_memcpy( SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	(void)aSurface;  // Avoid warnings about unused arguments until the
	(void)aImage;    // function is properly implemented.
	(void)aPosition;
}

void draw_ex_diagonal( SurfaceEx& aSurface, Vec2f aBegin, float aSteps, ColorU8_sRGB aColor )
{
	std::size_t const steps = std::size_t(aSteps);
	std::size_t const stride = 4*aSurface.get_width();

	std::uint8_t* sptr = aSurface.get_surface_ptr();
	sptr += std::size_t(aBegin.y) * stride + 4*std::size_t(aBegin.x);

	for( std::size_t i = 0; i < steps; ++i )
	{
		sptr[0] = aColor.r;
		sptr[1] = aColor.g;
		sptr[2] = aColor.b;
		sptr[3] = 0;
		sptr += stride + 4;
	}
}

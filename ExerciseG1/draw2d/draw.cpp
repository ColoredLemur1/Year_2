#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "color.hpp"
#include "surface.hpp"

void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	float minX = std::min(aMinCorner.x, aMaxCorner.x);
    float maxX = std::max(aMinCorner.x, aMaxCorner.x);
    float minY = std::min(aMinCorner.y, aMaxCorner.y);
    float maxY = std::max(aMinCorner.y, aMaxCorner.y);

	int startX = static_cast<int>(std::floor(minX));
    int endX = static_cast<int>(std::ceil(maxX));
    int startY = static_cast<int>(std::floor(minY));
    int endY = static_cast<int>(std::ceil(maxY));
	

	//TODO: remove the following when you start your implementation
	for (int y = startY; y < endY; ++y)
    {
        for (int x = startX; x < endX; ++x)
        {
            aSurface.set_pixel_srgb(x, y, aColor);
        }
    }
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}

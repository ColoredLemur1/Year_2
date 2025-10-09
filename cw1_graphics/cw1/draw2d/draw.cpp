#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"


bool clip_line( Rect2F const& aTargetArea, Vec2f& aBegin, Vec2f& aEnd )
{
	// Note: If the line is clipped, update aBegin and aEnd correspondingly.
	// They are non-const references (&), meaning we can modify the values
	// passed in through them.
	//
	// Return true to indicate that the line should be drawn and false that
	// it should be discarded.
	if (aBegin.x < aTargetArea.xmin && aEnd.x < aTargetArea.xmin) return false;
	if (aBegin.x > aTargetArea.width && aEnd.x > aTargetArea.width) return false;
	if (aBegin.y < aTargetArea.ymin && aEnd.y < aTargetArea.ymin) return false;
	if (aBegin.y > aTargetArea.height && aEnd.y > aTargetArea.height) return false;

	// Perform clipping
	if (aBegin.x < aTargetArea.xmin) aBegin.x = aTargetArea.xmin;
	if (aEnd.x > aTargetArea.width) aEnd.x = aTargetArea.width;
	if (aBegin.y < aTargetArea.ymin) aBegin.y = aTargetArea.ymin;
	if (aEnd.y > aTargetArea.height) aEnd.y = aTargetArea.height;

	//TODO: remove the following when you start your implementation
	(void)aTargetArea; // Avoid warnings about unused arguments until the function
	(void)aBegin;      // is properly implemented.
	(void)aEnd;

	return true;
}

void draw_clip_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	float dx = aEnd.x - aBegin.x;
	float dy = aEnd.y - aBegin.y;

	float steps = std::max(std::abs(dx), std::abs(dy));

	// Handle the case of a single point
	if (steps == 0)
	{
		int px = static_cast<int>(std::round(aBegin.x));
		int py = static_cast<int>(std::round(aBegin.y));
		if (px >= 0 && px < aSurface.get_width() && py >= 0 && py < aSurface.get_height())
		{
			aSurface.set_pixel_srgb(px, py, aColor);
		}
		return;
	}

	float x_inc = dx / steps;
	float y_inc = dy / steps;

	float x = aBegin.x;
	float y = aBegin.y;

	for (int i = 0; i <= static_cast<int>(steps); ++i)
	{
		int px = static_cast<int>(std::round(x));
		int py = static_cast<int>(std::round(y));
		
		// Check bounds before drawing to prevent assertion failure
		if (px >= 0 && px < aSurface.get_width() && py >= 0 && py < aSurface.get_height())
		{
			aSurface.set_pixel_srgb(px, py, aColor);
		}

		x += x_inc;
		y += y_inc;
	}
}


void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	if( clip_line( aSurface.clip_area(), aBegin, aEnd ) )
		draw_clip_line_solid( aSurface, aBegin, aEnd, aColor );
}
void draw_line_solid( Surface& aSurface, Rect2F const& aClipArea, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	if( clip_line( aClipArea, aBegin, aEnd ) )
		draw_clip_line_solid( aSurface, aBegin, aEnd, aColor );
}


void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;      // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aC0;
	(void)aC1;
	(void)aC2;
}

// You are not required to implement the following, but they can be useful for
// debugging.
void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
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

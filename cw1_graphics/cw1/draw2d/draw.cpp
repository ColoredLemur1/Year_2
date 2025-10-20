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
	// This function is meant to draw a filled, color-interpolated triangle.
	// For now, we are just drawing the wireframe outline using the vertex colors.
	float area = 0.5f * std::abs((aP1.x - aP0.x) * (aP2.y - aP0.y) - (aP2.x - aP0.x) * (aP1.y - aP0.y));
    if (area < 0.0001f) {
        return; 
    }

	draw_triangle_solid(aSurface, aP0, aP1, aP2, linear_to_srgb(aC0));

	// Draw the three lines that form the triangle's edges.
	// Use linear_to_srgb() to convert ColorF to ColorU8_sRGB
}

// You are not required to implement the following, but they can be useful for
// debugging.
void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	draw_line_solid( aSurface, aP0, aP1, aColor );
	draw_line_solid( aSurface, aP1, aP2, aColor );
	draw_line_solid( aSurface, aP2, aP0, aColor );
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	if (aP1.y < aP0.y) std::swap(aP0, aP1);
	if (aP2.y < aP0.y) std::swap(aP0, aP2);
	if (aP2.y < aP1.y) std::swap(aP1, aP2);

	auto inv_slope = [](const Vec2f& p1, const Vec2f& p2) -> float {
		float dy = p2.y - p1.y;
		if (dy == 0) return 0;
		return (p2.x - p1.x) / dy;
	};

	float inv_slope_0 = inv_slope(aP0, aP1);
	float inv_slope_1 = inv_slope(aP0, aP2);

	float currentx1 = aP0.x;
	float currentx2 = aP0.x;

	for (int y = static_cast<int>(aP0.y); y <= static_cast<int>(aP1.y); ++y) {
		draw_line_solid(aSurface, Vec2f(currentx1, y), Vec2f(currentx2, y), aColor);

		currentx1 += inv_slope_0;
		currentx2 += inv_slope_1;
	}

	inv_slope_0 = inv_slope(aP1, aP2);
	inv_slope_1 = inv_slope(aP0, aP2);

	currentx1 = aP1.x;
	currentx2 = aP0.x + inv_slope_1 * (aP1.y - aP0.y);

	for (int y = static_cast<int>(aP1.y); y <= static_cast<int>(aP2.y); ++y) {
		draw_line_solid(aSurface, Vec2f(currentx1, y), Vec2f(currentx2, y), aColor);

		currentx1 += inv_slope_0;
		currentx2 += inv_slope_1;
	}

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

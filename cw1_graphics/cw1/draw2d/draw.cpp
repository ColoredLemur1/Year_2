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
	auto edge = []( Vec2f const& a, Vec2f const& b, float x, float y ) -> float {
        return (b.x - a.x) * (y - a.y) - (b.y - a.y) * (x - a.x);
    };
    float area2 = edge( aP0, aP1, aP2.x, aP2.y ); // 2x area (signed)
    if( std::abs( area2 ) < 1e-6f )
        return; // degenerate
    float orient = (area2 >= 0.f) ? 1.f : -1.f;
    float invArea2 = orient / area2; // equals 1 / |area2|
    int minX = (int)std::floor( std::min( std::min( aP0.x, aP1.x ), aP2.x ) );
    int maxX = (int)std::ceil ( std::max( std::max( aP0.x, aP1.x ), aP2.x ) );
    int minY = (int)std::floor( std::min( std::min( aP0.y, aP1.y ), aP2.y ) );
    int maxY = (int)std::ceil ( std::max( std::max( aP0.y, aP1.y ), aP2.y ) );
    minX = std::max( 0, minX );
    minY = std::max( 0, minY );
    maxX = std::min( (int)aSurface.get_width(),  maxX );
    maxY = std::min( (int)aSurface.get_height(), maxY );
    for( int y = minY; y < maxY; ++y )
    {
        for( int x = minX; x < maxX; ++x )
        {
            // Pixel center sampling
            float px = x + 0.5f;
            float py = y + 0.5f;
            float w0 = orient * edge( aP1, aP2, px, py );
            float w1 = orient * edge( aP2, aP0, px, py );
            float w2 = orient * edge( aP0, aP1, px, py );
            // Inside if all are non-negative (top-left style omitted for simplicity)
            if( w0 >= 0.f && w1 >= 0.f && w2 >= 0.f )
            {
                // Normalize to barycentric in [0,1]
                float b0 = w0 * invArea2;
                float b1 = w1 * invArea2;
                float b2 = w2 * invArea2;
                ColorF cLin{
                    b0 * aC0.r + b1 * aC1.r + b2 * aC2.r,
                    b0 * aC0.g + b1 * aC1.g + b2 * aC2.g,
                    b0 * aC0.b + b1 * aC1.b + b2 * aC2.b
                };
                aSurface.set_pixel_srgb( x, y, linear_to_srgb( cLin ) );
            }
        }
    }
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

void draw_triangle_solid( Surface& aSurface,Vec2f aP0, Vec2f aP1, Vec2f aP2,ColorU8_sRGB aColor )
{
	//because draw_triangle solid only computed one color only it was hard for me to implement barycentric color interolation
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

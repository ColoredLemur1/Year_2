#include "draw-ex.hpp"

#include <algorithm>
#include <cmath> // this is necessary the round function

#include <cstring> // for std::memcpy()

#include "draw.hpp"
#include "image.hpp"
#include "surface-ex.hpp"

void draw_ex_line_solid( SurfaceEx& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	// Convert floating point coordinates to integers
	int x0 = static_cast<int>(std::round(aBegin.x));
	int y0 = static_cast<int>(std::round(aBegin.y));
	int x1 = static_cast<int>(std::round(aEnd.x));
	int y1 = static_cast<int>(std::round(aEnd.y));

	// Get surface dimensions and pointer
	std::uint32_t const width = aSurface.get_width();
	std::uint32_t const height = aSurface.get_height();
	std::uint8_t* surface_ptr = aSurface.get_surface_ptr();
	std::size_t const stride = 4 * width; // 4 bytes per pixel (RGBx)

	// Helper function to set a pixel at (x, y) if within bounds
	auto set_pixel = [&](int x, int y) {
		if (x >= 0 && x < static_cast<int>(width) && y >= 0 && y < static_cast<int>(height)) {
			std::uint8_t* pixel_ptr = surface_ptr + y * stride + 4 * x;
			pixel_ptr[0] = aColor.r;
			pixel_ptr[1] = aColor.g;
			pixel_ptr[2] = aColor.b;
			pixel_ptr[3] = 0;
		}
	};

	
	if (x0 == x1 && y0 == y1) {
		// Single point
		set_pixel(x0, y0);
		return;
	}

	
	int dx = x1 - x0;
	int dy = y1 - y0;

	// Handle horizontal line
	if (dy == 0) {
		int x_start = (x0 < x1) ? x0 : x1;
		int x_end = (x0 < x1) ? x1 : x0;
		for (int x = x_start; x <= x_end; ++x) {
			set_pixel(x, y0);
		}
		return;
	}

	// Handle vertical line
	if (dx == 0) {
		int y_start = (y0 < y1) ? y0 : y1;
		int y_end = (y0 < y1) ? y1 : y0;
		for (int y = y_start; y <= y_end; ++y) {
			set_pixel(x0, y);
		}
		return;
	}

	// Bresenham's line algorithm to determine if line is steep or shallow
	bool steep = std::abs(dy) > std::abs(dx);

	// If steep, swap x and y to make it shallow
	if (steep) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		std::swap(dx, dy);
	}

	// Ensure we always draw from left to right
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		dx = -dx;
		dy = -dy;
	}

	// Calculate step direction for y
	int y_step = (dy > 0) ? 1 : -1;
	dy = std::abs(dy);

	// Bresenham's decision variable
	int error = 2 * dy - dx;
	int y = y0;

	// Draw the line
	for (int x = x0; x <= x1; ++x) {
		if (steep) {
			// Swap back for steep lines
			set_pixel(y, x);
		} else {
			set_pixel(x, y);
		}

		// Update error and y
		if (error > 0) {
			y += y_step;
			error -= 2 * dx;
		}
		error += 2 * dy;
	}
}

void blit_ex_solid(SurfaceEx& destination_surface,ImageRGBA const& source_image,Vec2f target_position) 
{
    // Define pixel format (assuming 32-bit RGBA or RGBx)
    constexpr int bytes_per_pixel = 4;

    // get dimensions
    std::uint32_t const source_width = source_image.get_width();
    std::uint32_t const source_height = source_image.get_height();
    std::uint32_t const dest_width = destination_surface.get_width();
    std::uint32_t const dest_height = destination_surface.get_height();

    // calculate target area
    // Area we want to draw to
    int const target_x_start = static_cast<int>(target_position.x);
    int const target_y_start = static_cast<int>(target_position.y);
    int const target_x_end = target_x_start + static_cast<int>(source_width);
    int const target_y_end = target_y_start + static_cast<int>(source_height);

    // clip target area to destination bounds
    // the actual area we can draw to
    int const clipped_x_start = std::max(0, target_x_start);
    int const clipped_y_start = std::max(0, target_y_start);
    int const clipped_x_end = std::min(static_cast<int>(dest_width), target_x_end);
    int const clipped_y_end = std::min(static_cast<int>(dest_height), target_y_end);

    // calculate clipped dimensions
    int const clipped_width = clipped_x_end - clipped_x_start;
    int const clipped_height = clipped_y_end - clipped_y_start;

    // exit if the clipped area is empty
    if (clipped_width <= 0 || clipped_height <= 0) {
        return;
    }

    // calculate source read-offset
    int const source_x_offset = clipped_x_start - target_x_start;
    int const source_y_offset = clipped_y_start - target_y_start;

    // get buffers and strides
    std::uint8_t* dest_buffer = destination_surface.get_surface_ptr();
    std::uint8_t const* source_buffer = source_image.get_image_ptr();

    // stride is the total number of bytes in one full row (width) of the image/surface.
    std::size_t const dest_stride_bytes = dest_width * bytes_per_pixel;
    std::size_t const source_stride_bytes = source_width * bytes_per_pixel;

    // blit the clipped area
    for (int y = 0; y < clipped_height; ++y) {
        // calculate the row to read from and write to
        int const source_row_index = source_y_offset + y;
        int const dest_row_index = clipped_y_start + y;

        // get pointers to the *start* of the relevant rows
        std::uint8_t const* source_row_ptr = source_buffer + (source_row_index * source_stride_bytes);
        std::uint8_t* dest_row_ptr = dest_buffer + (dest_row_index * dest_stride_bytes);

        // get pointers to the specific *pixel* to start copying from/to
        std::uint8_t const* source_pixel_ptr = source_row_ptr + (source_x_offset * bytes_per_pixel);
        std::uint8_t* dest_pixel_ptr = dest_row_ptr + (clipped_x_start * bytes_per_pixel);

        // copy all pixels in this row
        for (int x = 0; x < clipped_width; ++x) {
            int const pixel_byte_index = x * bytes_per_pixel;

            // copy rgb values
            dest_pixel_ptr[pixel_byte_index + 0] = source_pixel_ptr[pixel_byte_index + 0]; // R
            dest_pixel_ptr[pixel_byte_index + 1] = source_pixel_ptr[pixel_byte_index + 1]; // G
            dest_pixel_ptr[pixel_byte_index + 2] = source_pixel_ptr[pixel_byte_index + 2]; // B
            
            // set destination padding/alpha to 0
            dest_pixel_ptr[pixel_byte_index + 3] = 0;
        }
    }
}

void blit_ex_memcpy(SurfaceEx& destination_surface,ImageRGBA const& source_image,Vec2f target_position) 
{
    // define pixel format
    constexpr int bytes_per_pixel = 4;

    // get dimensions
    std::uint32_t const source_width = source_image.get_width();
    std::uint32_t const source_height = source_image.get_height();
    std::uint32_t const dest_width = destination_surface.get_width();
    std::uint32_t const dest_height = destination_surface.get_height();

    // calculate target area
    int const target_x_start = static_cast<int>(target_position.x);
    int const target_y_start = static_cast<int>(target_position.y);
    int const target_x_end = target_x_start + static_cast<int>(source_width);
    int const target_y_end = target_y_start + static_cast<int>(source_height);

    // clip target area to destination bounds
    int const clipped_x_start = std::max(0, target_x_start);
    int const clipped_y_start = std::max(0, target_y_start);
    int const clipped_x_end = std::min(static_cast<int>(dest_width), target_x_end);
    int const clipped_y_end = std::min(static_cast<int>(dest_height), target_y_end);

    // calculate clipped dimensions
    int const clipped_width = clipped_x_end - clipped_x_start;
    int const clipped_height = clipped_y_end - clipped_y_start;

    // exit if the clipped area is empty
    if (clipped_width <= 0 || clipped_height <= 0) {
        return;
    }

    // calculate source read-offset
    int const source_x_offset = clipped_x_start - target_x_start;
    int const source_y_offset = clipped_y_start - target_y_start;

    // get buffers and strides
    std::uint8_t* dest_buffer = destination_surface.get_surface_ptr();
    std::uint8_t const* source_buffer = source_image.get_image_ptr();

    // stride is the total number of bytes in one full row (width) of the image/surface.
    std::size_t const dest_stride_bytes = dest_width * bytes_per_pixel;
    std::size_t const source_stride_bytes = source_width * bytes_per_pixel;

    // blit the clipped area
    std::size_t const bytes_per_row_to_copy = clipped_width * bytes_per_pixel;

    for (int y = 0; y < clipped_height; ++y) {
        // calculate the row to read from and write to
        int const source_row_index = source_y_offset + y;
        int const dest_row_index = clipped_y_start + y;

        // get pointers to the *start* of the relevant rows
        std::uint8_t const* source_row_ptr = source_buffer + (source_row_index * source_stride_bytes);
        std::uint8_t* dest_row_ptr = dest_buffer + (dest_row_index * dest_stride_bytes);

        // get pointers to the specific *pixel* to start copying from/to
        std::uint8_t const* source_pixel_ptr = source_row_ptr + (source_x_offset * bytes_per_pixel);
        std::uint8_t* dest_pixel_ptr = dest_row_ptr + (clipped_x_start * bytes_per_pixel);

        // copy the entire row of pixel data at once
        std::memcpy(dest_pixel_ptr, source_pixel_ptr, bytes_per_row_to_copy);

        // zero out the padding/alpha channel in the destination
        // do this *after* the fast copy.
        for (int x = 0; x < clipped_width; ++x) {
            // the alpha/padding byte is at offset 3
            dest_pixel_ptr[x * bytes_per_pixel + 3] = 0;
        }
    }
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

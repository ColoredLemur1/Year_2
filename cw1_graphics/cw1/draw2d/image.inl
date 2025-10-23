inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
	assert( aX < mWidth && aY < mHeight ); // Leave this at the top of the function.

	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	if (aX < 0 || aY < 0) {
		return ColorU8_sRGB_Alpha{0, 0, 0, 0}; // Return transparent
	}
	Index index = (aY * mWidth + aX) * 4;

	uint8_t r = mData[index];
	uint8_t g = mData[index + 1];
	uint8_t b = mData[index + 2];
	uint8_t a = mData[index + 3];

	return ColorU8_sRGB_Alpha{ r, g, b, a };
}

inline
auto ImageRGBA::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto ImageRGBA::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	return aY * mWidth + aX;
}

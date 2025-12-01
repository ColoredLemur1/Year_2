#include "cone.hpp"

#include <numbers>

SimpleMeshData make_cone( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
	std::vector<Vec3f> positions;

	// Generate cone sides (tapers from circle at x=0 to point at x=1)
	float prevY = std::cos(0.f);
	float prevZ = std::sin(0.f);

	for (std::size_t i = 0; i < aSubdivs; ++i)
	{
		float const angle = (i+1) / float(aSubdivs) * 2.f * std::numbers::pi_v<float>;
		float y = std::cos(angle);
		float z = std::sin(angle);

		// Triangle from base circle to apex point
		positions.emplace_back( Vec3f{0.f, prevY, prevZ} ); // Base circle previous
		positions.emplace_back( Vec3f{0.f, y, z} );         // Base circle current
		positions.emplace_back( Vec3f{1.f, 0.f, 0.f} );     // Apex point
		
		prevY = y;
		prevZ = z;
	}

	// Generate base cap if requested
	if (aCapped)
	{
		// Base cap (at x = 0)
		// Center point is at (0, 0, 0)
		prevY = std::cos(0.f);
		prevZ = std::sin(0.f);
		
		for (std::size_t i = 0; i < aSubdivs; ++i)
		{
			float const angle = (i+1) / float(aSubdivs) * 2.f * std::numbers::pi_v<float>;
			float y = std::cos(angle);
			float z = std::sin(angle);

			// Triangle facing LEFT (negative x direction)
			positions.emplace_back( Vec3f{0.f, 0.f, 0.f} );   // Center
			positions.emplace_back( Vec3f{0.f, y, z} );        // Current edge
			positions.emplace_back( Vec3f{0.f, prevY, prevZ} ); // Previous edge
			
			prevY = y;
			prevZ = z;
		}
	}

	// Apply pre-transformation to all positions
	for (auto& p: positions)
	{
		Vec4f p4{p.x, p.y, p.z, 1.f};
		Vec4f t = aPreTransform * p4;
		t /= t.w;

		p = Vec3f{t.x, t.y, t.z};
	}

	std::vector col(positions.size(), aColor);
	return SimpleMeshData{std::move(positions), std::move(col)};
}


#include "cylinder.hpp"

#include <numbers>

SimpleMeshData make_cylinder( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
	//TODO: implement me
	std::vector<Vec3f> positions;

	float prevY = std::cos(0.f);
	float prevZ = std::sin(0.f);

	for (std::size_t i = 0; i < aSubdivs; ++i)
	{
		float const angle = (i+1) / float(aSubdivs) * 2.f * std::numbers::pi_v<float>;
		float y = std::cos(angle);
		float z = std::sin(angle);

		positions.emplace_back( Vec3f{0.f, prevY, prevZ} );
		positions.emplace_back( Vec3f{0.f, y, z} );
		positions.emplace_back( Vec3f{1.f, prevY, prevZ} );

		positions.emplace_back( Vec3f{0.f, y, z} );
		positions.emplace_back( Vec3f{1.f, y, z} );
		positions.emplace_back( Vec3f{1.f, prevY, prevZ} );
		prevY = y;
		prevZ = z;
	}

	if (aCapped)
	{
		// Bottom cap (at x = 0)
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
            positions.emplace_back( Vec3f{0.f, y, z} );        // Current edge (swapped)
            positions.emplace_back( Vec3f{0.f, prevY, prevZ} ); // Previous edge (swapped)
            
            prevY = y;
            prevZ = z;
        }

        // Top cap (at x = 1)
        // Center point is at (1, 0, 0)
        prevY = std::cos(0.f);
        prevZ = std::sin(0.f);
        
        for (std::size_t i = 0; i < aSubdivs; ++i)
        {
            float const angle = (i+1) / float(aSubdivs) * 2.f * std::numbers::pi_v<float>;
            float y = std::cos(angle);
            float z = std::sin(angle);

            // Triangle facing RIGHT (positive x direction)
            positions.emplace_back( Vec3f{1.f, 0.f, 0.f} );   // Center
            positions.emplace_back( Vec3f{1.f, prevY, prevZ} ); // Previous edge (swapped)
            positions.emplace_back( Vec3f{1.f, y, z} );        // Current edge (swapped)
            
            prevY = y;
            prevZ = z;
        }
	}

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

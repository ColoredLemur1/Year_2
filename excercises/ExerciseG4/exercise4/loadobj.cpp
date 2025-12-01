#include "loadobj.hpp"

#include <rapidobj/rapidobj.hpp>

#include "../support/error.hpp"

SimpleMeshData load_wavefront_obj( char const* aPath )
{
	auto res = rapidobj::ParseFile( aPath );
	if (res.error)
	{
		throw Error( "Error loading OBJ file '{}': {} (line {}: '{}')",
			aPath,
			res.error.code.message(),
			res.error.line_num,
			res.error.line
		);
	}
	rapidobj::Triangulate(res);

	SimpleMeshData ret;

	for (auto const& shape: res.shapes)
	{
		for (std::size_t i =0; i < shape.mesh.indices.size(); ++i)
		{
			auto idx = shape.mesh.indices[i];

			ret.positions.emplace_back( Vec3f{
				res.attributes.positions[3 * idx.position_index + 0],
				res.attributes.positions[3 * idx.position_index + 1],
				res.attributes.positions[3 * idx.position_index + 2]
			} );

			auto const& mat = res.materials[ shape.mesh.material_ids[i / 3] ];
			ret.colors.emplace_back( Vec3f{
				mat.ambient[0],
				mat.ambient[1],
				mat.ambient[2]
			} );
		}
	}
	return ret;
}
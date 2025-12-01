#include "simple_mesh.hpp"

SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const& aN )
{
	aM.positions.insert( aM.positions.end(), aN.positions.begin(), aN.positions.end() );
	aM.colors.insert( aM.colors.end(), aN.colors.begin(), aN.colors.end() );
	aM.normals.insert( aM.normals.end(), aN.normals.begin(), aN.normals.end() );
	return aM;
}


GLuint create_vao( SimpleMeshData const& aMeshData )
{
	//TODO: implement me
	return 0;
}


#include "simple_mesh.hpp"

SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const& aN )
{
	aM.positions.insert( aM.positions.end(), aN.positions.begin(), aN.positions.end() );
	aM.texcoords.insert( aM.texcoords.end(), aN.texcoords.begin(), aN.texcoords.end() );
	return aM;
}


GLuint create_vao( SimpleMeshData const& aMeshData )
{
	GLuint vao = 0;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	GLuint positionVBO = 0;
	glGenBuffers( 1, &positionVBO );
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glBufferData( GL_ARRAY_BUFFER, 
	              aMeshData.positions.size() * sizeof(Vec3f),
	              aMeshData.positions.data(),
	              GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
	glEnableVertexAttribArray( 0 );

	GLuint textCoordVBO = 0;
	glGenBuffers( 1, &textCoordVBO );
	glBindBuffer( GL_ARRAY_BUFFER, textCoordVBO );
	glBufferData( GL_ARRAY_BUFFER, 
	              aMeshData.texcoords.size() * sizeof(Vec2f),
	              aMeshData.texcoords.data(),
	              GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, nullptr );
	glEnableVertexAttribArray( 1 );

	return vao;
}


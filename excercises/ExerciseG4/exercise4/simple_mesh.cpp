#include "simple_mesh.hpp"

SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const& aN )
{
	aM.positions.insert( aM.positions.end(), aN.positions.begin(), aN.positions.end() );
	aM.colors.insert( aM.colors.end(), aN.colors.begin(), aN.colors.end() );
	return aM;
}


GLuint create_vao( SimpleMeshData const& aMeshData )
{
	// Create VBO for positions
	GLuint positionVBO = 0;
	glGenBuffers( 1, &positionVBO );
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW );

	// Create VBO for colors
	GLuint colorVBO = 0;
	glGenBuffers( 1, &colorVBO );
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
	glBufferData( GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW );

	// Create VAO and configure vertex attributes
	GLuint vao = 0;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Bind position VBO and set up attribute 0
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 0 );

	// Bind color VBO and set up attribute 1
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 1 );

	// Unbind VAO and buffers
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	// Delete VBOs (VAO keeps references)
	glDeleteBuffers( 1, &colorVBO );
	glDeleteBuffers( 1, &positionVBO );

	return vao;
}


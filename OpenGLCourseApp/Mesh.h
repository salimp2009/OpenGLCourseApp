// #pragma once
#ifndef MESH_H_
#define MESH_H_
#include <GL/glew.h>
// #include <cstddef>		// needed for std::size_t if needed

class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices); 
	void RenderMesh();		// Draw the mesh; 
	void ClearMesh();		// Delete from the buffer
	~Mesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;			// Indexcount is the number indices we pass when we create an object
								// IndexCount will used glDrawElements() function; it is the second argument
								// GLsizei is functionally equivalent to GLuint. 
								// GLsizei is used specifically for sizes of things
};

#endif // MESH_H_

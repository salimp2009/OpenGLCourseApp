#include "Mesh.h"

Mesh::Mesh() : VAO{ 0 }, VBO{ 0 }, IBO{ 0 }, indexCount{ 0 } { }

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	// trying to sync git again
	// create VAO variable to store the vertex info
	// 1 stands for the count of variables we are creating; it can be more than one
	glGenVertexArrays(1, &VAO);			// creating a vertex array buffer and generate ID for it; that ID will be stored
										// in variable VAO ID ; value 1 indicates we are creating 1 array the number of buffers
	glBindVertexArray(VAO);				// Any Opengl functions or vertex buffer objects will be binded to that ID in VAO  until it is unbinded

	glGenBuffers(1, &IBO);						// Creeating a buffer element indices to draw a complex object using index number of coordinates
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Bind the elemnt array buffer to the given index and store in IBO variable  
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices[0])*numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);				// Create 1 buffer object and generate ID and store in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the VBO to a target; it has multiple target that can be binded
										// we are telling VBO to bind to an Array Buffer which we created above 
										// and pass ID of that buffer inside VBO 

	// Connect buffer object to the vertex array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*numOfVertices, vertices, GL_STATIC_DRAW);	// we are telling we are using an Array Buffer
																				// then pass the size of data; we pass the size of the array named matrices 
																				// or we could have used sizeof(float)*number elements 
																				// next we pass the the data as a pointer which we use the array name
																				// next we identify whether we will change any of those data or not; 
																				// GL_STATIC_DRAW means no change later; GL_DYNAMIC draw means there will be change  
	
	// creating Vertext Attribute Pointer for vertices that will be used in the shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (vertices[0])*5, 0);	// first value is the location to start store the data in shader
																				// second value is number of each data; x,y,z so it is 3 and second value is the type of data(Float here)
																				// third value ; normalize the values or not;
																				// fourth value is stride; if we position and color info and want to pass position only then we specify how many data to pass
																				// fifth is offset where we want the data to start typically 0 but if you want to start from 2nd raw then we could pass 1			
	glEnableVertexAttribArray(0);												// the value is the same value above first value
	
	// creating Vertext Attribute Pointer for textures that will be used in the shader
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0])*3)); // this one is for the texture coordinates in mesh array defined in vertices
	glEnableVertexAttribArray(1); // this can be in RenderMesh() function

	// Unbind the Buffer from VBO and VAO so we can pass others if needed
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Mesh::RenderMesh() 
{	// needs a check if(idexCount==0) return; // to avoid drawing an empty Mesh 
	// Assign the current VAO to be used in the shader
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	// Drawing the object; ,
	// first argument specifies the type of object; triangle, rectangle..
	// that way OpenGL know how it is going to use vertices
	// second argument is the start of array position and
	// third agument is the number data 3; x,y,z
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind the VAO once we are done to bind another
	glBindVertexArray(0);

}
void Mesh::ClearMesh() 
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);		// clear the GPU buffer/memory once we are done
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);		// clear the GPU buffer/memory once we are done
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);		// clear the GPU buffer/memory once we are done
		VBO = 0;
	}
	indexCount = 0;
}


Mesh::~Mesh() { ClearMesh();  }

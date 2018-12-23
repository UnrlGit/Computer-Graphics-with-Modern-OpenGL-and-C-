#include "Mesh.h"


Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::CreateMesh(GLfloat * vertices, unsigned int * indicies, unsigned int numOfVertices, unsigned int numOfIndicies)
{
	indexCount = numOfIndicies;

	// binding
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//INDEXED DRAW STUFF
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies[0]) * numOfIndicies, indicies, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//connect buffer data - vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*numOfVertices, vertices, GL_STATIC_DRAW);

	// goes to (location = 0) in shader?
	// adding stride for texture (5th value)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0])*5, 0);

	glEnableVertexAttribArray(0);
	//for texture? for shader: 3 is offset
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	// unbinding?
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//UNBINDING INDEXED DRAW
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbinding
	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);

	//INDEXED DRAW STUFF
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbinding array
	glBindVertexArray(0);
}

void Mesh::ClearMesh() {

	// deleting the buffer off of the graphics card memory
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

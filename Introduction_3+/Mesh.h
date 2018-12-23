#pragma once

#include <GL\glew.h>
class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indicies, unsigned int numOfVertices, unsigned int numOfIndicies);
	void RenderMesh();
	void ClearMesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};


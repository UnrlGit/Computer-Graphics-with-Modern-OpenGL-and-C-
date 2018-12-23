#pragma once

#include <GL\glew.h>
#include <GLM\glm.hpp>
class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);
	~Light();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation);


private:
	glm::vec3 colour;
	GLfloat ambientIntensity;
};


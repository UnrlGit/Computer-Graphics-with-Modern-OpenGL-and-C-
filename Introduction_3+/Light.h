#pragma once

#include <GL\glew.h>
#include <GLM\glm.hpp>
class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);
	~Light();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);


private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};


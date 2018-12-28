#pragma once
#include "PointLight.h"
class SpotLight : public PointLight
{
public:
	SpotLight();
	~SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat constnt, GLfloat lin, GLfloat expnt,
		GLfloat edge);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	// takes pos and direction and sets spot light to it
	void SetFlash(glm::vec3 pos, glm::vec3 dir);

private:
	glm::vec3 direction;


	// angle where light stops (edge only)?
	GLfloat _edge, _processedEdge;

};


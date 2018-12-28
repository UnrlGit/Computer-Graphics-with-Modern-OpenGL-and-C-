#include "PointLight.h"



PointLight::PointLight()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;



}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
						GLfloat aIntensity, GLfloat dIntensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat constnt, GLfloat lin, GLfloat expnt) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = constnt;
	linear = lin;
	exponent = expnt;
}


PointLight::~PointLight()
{
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
							GLfloat diffuseIntensityLocation, GLfloat positionLocation,
							GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

#include "Light.h"


Light::Light():colour{glm::vec3{1.0f, 1.0f, 1.0f}}, ambientIntensity{1.0f} {}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity) : colour{glm::vec3{red, green, blue}}, ambientIntensity{aIntensity} {}  // if used in a project, add std::clamp for red, green, blue to limit value btwn 1-0

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);				// ambientColourLocation is the ID passed to shader; colour.x, colour.y, colour.z will be passed as value to shader
	glUniform1f(ambientIntensityLocation, ambientIntensity);						// ambientIntensityLocation is the ID passed to shader; ambientintensity  will be passed as value to shader
	
}
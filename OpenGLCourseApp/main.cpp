//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>					// not needed here either disable or remove !!!!!

#include "MyWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"


constexpr float toRadians = 3.14159265f / 180.0f;

MyWindow mainWindow;

//std::vector<Mesh*>meshList;					// Not used; Original course method; used unique_ptr
std::vector<std::unique_ptr<Mesh>>meshList2;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;							// typicall in mesh class in the Render function; will be cleanedup later
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime{0.0f};						// deltaTime is in seconds
GLfloat lastTime{0.0f};

// Vertex Shader;
/* version 330 is the version of GLSL shader language; different from OpenGl version
// location=0 identifies where the glVertexAttribPointer data starts
// vec4 is the output data to be used in fragment shader
// values are x,y,z of position and an extra value 1 is passed to align with matrice format
// fragment shader where the data can be modified to create visual effects such as lava..etc
// 0.4 * pos.x is just to demonstrate that we can modify the position of the object in vertex shader as well
// we modified only x & y ; z is how the object from the camera we only want to object to modified on x-y plane for this demo
*/

static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
/* Fragment Shader has only one color output of the pixels
// out	vec4 colour; out specifier is not required since there is only one output
// colour=vec4(1.0f, 0.0, 0.0, 1.0);  // RGB values; creating a red triangle 
*/
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	 GLfloat vertices[] = {
		 //x	 y		z	  u		v			// u,v are the cordinate of the 2D Texture ; x,y,z are the coordiates of vertices
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,			// u,v coord bottom left
		 0.0f, -1.0f, 1.0f, 0.5f, 0.0f,			// u,v coord bottom half on x-axis this is one in z-dimension
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,		    // u,v coord bottom right
		 0.0f, 1.0f, 0.0f,  0.5f, 1.0f			// u,v coord top middle since it is a triangle facing front
	};

	// Original class method; not used ; used unique_ptr instead
	//Mesh *obj1 = new Mesh();
	//obj1->CreateMesh(vertices, indices, 12, 12);
	//meshList.push_back(obj1);
	
	meshList2.emplace_back(std::make_unique<Mesh>());
	meshList2[0]->CreateMesh(vertices, indices, 20, 12);

	meshList2.emplace_back(std::make_unique<Mesh>());
	meshList2[1]->CreateMesh(vertices, indices, 20, 12);
}

void CreateShaders()
{
	//Shader* shader1 = new Shader();
	//shader1->CreateFromFiles(vShader, fShader);
	//shaderList.push_back(*shader1);
	
	shaderList.emplace_back();
	shaderList[0].CreateFromFiles(vShader, fShader);

	//std::shared_ptr<Shader> shader1(new Shader());
	//std::shared_ptr<Shader> shader1 = std::make_shared<Shader>();
	//shader1->CreateFromFiles(vShader, fShader);
	//shaderList.push_back(*shader1);
}

int main() {

	mainWindow = MyWindow(800, 600);
	mainWindow.Initialise();

	// Create the triangle and shaders
	CreateObjects();
	CreateShaders();

	// Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	camera = Camera(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 5.0f, 0.15f);

	// "Texture/brick.png";
	brickTexture = Texture((char*)"Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture((char*)"Textures/dirt.png");
	dirtTexture.LoadTexture();

	mainLight = Light(1.0f, 1.0f, 1.0f, 0.8f);


	GLuint uniformView{0};
	GLuint uniformProjection{0};
	GLuint uniformModel{0};
	GLuint uniformAmbientIntensity{ 0 };
	GLuint uniformAmbientColour{ 0 };

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// loop until windows closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = static_cast<float>(glfwGetTime());	// SDL version: SDL_GetPerformanceCounter; but not used here	
		deltaTime = now - lastTime;		// calculate time difference between when the loop (rendering a frame ended) & current time; SLD Version; (now-lastTime)*1000/SDL_GetPerformanceFrequency()
		lastTime = now;					// update lastTime to now ; deltaTime is in seconds

	// get + handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		// Clear window for a fresh one; RGB color and transparency, 1=opaque
		// changed the color from red to blue because we are creating a red triangle and background needs to be different
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		// Clear all the color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get the shader working
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour);

		glm::mat4 model(1.0f);
		
		model = glm::translate(model, glm::vec3(0.0f, -0.0f, -2.5f));
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()) );
		brickTexture.UseTexture();
		
		// uses raw pointer; in the original class
		//meshList[0]->RenderMesh();
		// using std::unique_ptr
		meshList2[0]->RenderMesh();


		// creating a second object; reset the model to apply different translate and scale to position
		// these will be later handled by an object class
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList2[1]->RenderMesh();

		// close the shader
		glUseProgram(0);
		
		mainWindow.SwapBuffers();
	}

	return 0;
}
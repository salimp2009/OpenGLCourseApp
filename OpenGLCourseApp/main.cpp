
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

//#include <string>



//Window Dimensions
const GLint WIDTH{ 800 }, HEIGHT{ 600 };
constexpr float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

// setting the values to move the triangle left and right along x-axis
// by applying uniform value and transform matrix
bool direction{true};  // moving right (positive x axis) if true, moving left if false
float triOffset{0.0f};
float triMaxOffset{0.7f};
float triIncrement{0.0015f};
float currentAngle{ 0.0f };

bool sizeDirection{true};
float curSize{0.4f};
float maxSize{0.8f};
float minSize{0.1f};

// creating a vertex and fragment shader; typically done in an external file (this will be removed to another file)
// vertex data will be passed into shader where we can move them around if we want to before passing to fragment shader

// Vertex Shader;
/* version 330 is the version of GLSL shader language; different from OpenGl version
// location=0 identifies where the glVertexAttribPointer data starts
// vec4 is the output data to be used in fragment shader
// values are x,y,z of position and an extra value 1 is passed to align with matrice format
// fragment shader where the data can be modified to create visual effects such as lava..etc
// 0.4 * pos.x is just to demonstrate that we can modify the position of the object in vertex shader as well
// we modified only x & y ; z is how the object from the camera we only want to object to modified on x-y plane for this demo
*/
static const char* vShader = "								\n\
#version 330												\n\
															\n\
layout (location = 0) in vec3 pos;							\n\
															\n\
uniform mat4 model;										    \n\
				 											\n\
void main()													\n\
{															\n\
	gl_Position=model * vec4(pos, 1.0);						\n\
}";	


// Fragment Shader
/* Fragment Shader has only one color output of the pixels
// out	vec4 colour; out specifier is not required since there is only one output
// colour=vec4(1.0f, 0.0, 0.0, 1.0);  // RGB values; creating a red triangle 
*/
static const char* fShader = "								\n\
#version 330												\n\
															\n\
out	vec4 colour;											\n\
															\n\
void main()													\n\
{															\n\
	colour=vec4(1.0f, 0.0f, 0.0f, 1.0);						\n\
}";

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f
	};

	// trying to sync git again
	// create VAO variable to store the vertex info
	// 1 stands for the count of variables we are creating; it can be more than one
	glGenVertexArrays(1, &VAO);			// creating a vertex array buffer and generate ID for it; that ID will be stored
										// in variable VAO ID ; value 1 indicates we are creating 1 array the number of buffers
	glBindVertexArray(VAO);				// Any Opengl functions or vertex buffer objects will be binded to that ID in VAO  until it is unbinded

	glGenBuffers(1, &VBO);				// Create 1 buffer object and generate ID and store in VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind the VBO to a target; it has multiple target that can be binded
										// we are telling VBO to bind to an Array Buffer which we created above 
										// and pass ID of that buffer inside VBO 
	
	// Connect buffer object to the vertex array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);	// we are telling we are using an Array Buffer
																				// then pass the size of data; we pass the size of the array named matrices 
																				// or we could have used sizeof(float)*number elements 
																				// next we pass the the data as a pointer which we use the array name
																				// next we identify whether we will change any of those data or not; 
																				// GL_STATIC_DRAW means no change later; GL_DYNAMIC draw means there will be change  
	// creating Vertext Attribute Pointer that will be used in the shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);						// first value is the location to start store the data in shader
																				// second value is number of each data; x,y,z so it is 3 and second value is the type of data(Float here)
																				// third value ; normalize the values or not;
																				// fourth value is stride; if we position and color info and want to pass position only then we specify how many data to pass
																				// fifth is offset where we want the data to start typically 0 but if you want to start from 2nd raw then we could pass 1			
	glEnableVertexAttribArray(0);												// the value is the same value above first value

	// Unbind the Buffer from VBO and VAO so we can pass others if needed
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	// create an empty shader(fragment or fragment) and pass the ID
	GLuint theShader = glCreateShader(shaderType);				

	// required variable format for OpenGL glShaderSource() function
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = static_cast<GLint>(strlen(shaderCode));		// strlen is C version of std::strlen() ; we included string.h C library

	// Load the shader file into GL
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// Get any erros from shaders
	// hard to debug
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);			// we pass the program and which info we want the which is status and store the status in result variable
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);	// store the error info in the variable elog to debug if there is a problem in vertex and fragment shaders
		printf("Error compiling the %d shader: %s \n", shaderType, elog);
		return;
	}
	glAttachShader(theProgram, theShader);
	return;
}

void CompileShaders()
{
	// create a program and give shader the ID; actual program sits on GPU
	shader = glCreateProgram();			
	if (!shader) {
		printf("Error creating the shader program....\n");
		return;
	}

	// add shaders to program; vShader and fShader that was created above
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	// Get any erros from shaders
	// hard to debug
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	// create all executble on GPU
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);			// we pass the program and which info we want the which is status and store the status in result variable
	
	// check for errors
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);	// store the error info in the variable elog to debug if there is a problem in vertex and fragment shaders
		printf("Error linking program: %s \n", elog);
		return;
	}

	// validate & check for errors
	glValidateProgram(shader);										// check if the shader program is valid for Open GL
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);			// we pass the program and which info we want the which is status and store the status in result variable
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);		// store the error info in the variable elog to debug if there is a problem in vertex and fragment shaders
		printf("Error validating program: %s \n", elog);
		return;
	}
	
	uniformModel = glGetUniformLocation(shader, "model");

}

int main() {

	//initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!!!...");
		glfwTerminate();
		return 1; 
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile means no backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW Window creation failed!!!...");
		//std::cout << "GLFW Window creation failed!!!...\n";
		glfwTerminate();
		return 1;
	}
	// Get Buffer size info
	int bufferWidth{0}, bufferHeight{0};
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set the context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	// initialize Glew
	if (glewInit()!=GLEW_OK)
	{
		printf("GLEW initialisation failed!!!...");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Create & setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Create the triangle and shaders
	CreateTriangle();
	CompileShaders();

	// loop until windows closed
	while (!glfwWindowShouldClose(mainWindow))
	{
	// get + handle user input events
		glfwPollEvents();
		if (direction)
		{
			triOffset += triIncrement;
		} 
		else {
			triOffset -= triIncrement;
		}


		if (std::abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		currentAngle += 0.1f;
		if (currentAngle >= 360.0f) {
			currentAngle -= 360.0f;
		}

		if (sizeDirection) 
		{
			curSize += 0.001f;
		}
		else {
			curSize -= 0.001f;
		}
		if (curSize >= maxSize || curSize <= minSize) 
		{
			sizeDirection = !sizeDirection;
		}
		// Clear window for a fresh one; RGB color and transparency, 1=opaque
		// changed the color from red to blue because we are creating a red triangle and background needs to be different
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		// Clear all the color
		glClear(GL_COLOR_BUFFER_BIT);

		// get the shader working
		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
		model = glm::rotate(model, currentAngle*toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Assign the current VAO to be used in the shader
		glBindVertexArray(VAO);

		// Drawing the object; ,
		// first argument specifies the type of object; triangle, rectangle..
		//that way OpenGL how it is going to use vertices
		// second argument is the start of array position and
		// third agument is the number data 3; x,y,z
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Unbind the VAO once we are done to bind another
		glBindVertexArray(0);

		// close the shader
		glUseProgram(0);
		
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
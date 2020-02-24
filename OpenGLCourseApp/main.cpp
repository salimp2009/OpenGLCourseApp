#include <iostream>
#include <stdio.h>
#include <string>
//#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Window Dimensions
const GLint WIDTH{ 800 }, HEIGHT{ 600 };

GLuint VAO, VBO, shader;

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
	glGenVertexArrays(1, &VAO);			// creating a vertex array generate VAO ID
	glBindVertexArray(VAO);				// Bind VAO with that ID

	glGenBuffers(1, &VBO);				// generate VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// bind VBO with ID; now you are working on the chosen VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer( GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

int main() {

	//initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!!!...");
		//std::cout << "GLFW Initialization failed!!!..\n";
		glfwTerminate();
		return 1; 
	}

	// Setup GLFW window properties
	// OpenGL version

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile means no backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward cmpatibility
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

	// setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// loop until windows closed
	while (!glfwWindowShouldClose(mainWindow))
	{
	// get + handle user input events
		glfwPollEvents();

		// Clear window for a fresh one; RGB color and transparency, 1=opaque
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		// Clear all the color
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
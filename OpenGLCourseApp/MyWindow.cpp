#include "MyWindow.h"

MyWindow::MyWindow() :mainWindow{ nullptr }, width{ 800 }, height{ 600 }, bufferHeight{ 0 }, bufferWidth{ 0 } {}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight) : mainWindow{ nullptr }, width{ windowWidth }, height{ windowHeight }, bufferHeight{ 0 }, bufferWidth{ 0 } {}

int MyWindow::Initialise()
{
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW Window creation failed!!!...");
		//std::cout << "GLFW Window creation failed!!!...\n";
		glfwTerminate();
		return 1;
	}

	// Get Buffer size info
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set the context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	// initialize Glew
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!!!...");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create & setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

}

MyWindow::~MyWindow() 
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

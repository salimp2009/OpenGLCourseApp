#include "MyWindow.h"

MyWindow::MyWindow() :mainWindow{ nullptr }, width{ 800 }, height{ 600 }, bufferHeight{ 0 }, bufferWidth{ 0 }, keys{ 0 } {}


MyWindow::MyWindow(GLint windowWidth, GLint windowHeight) : mainWindow{ nullptr }, width{ windowWidth }, height{ windowHeight }, bufferHeight{ 0 }, bufferWidth{ 0 }, keys{0} {}

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

	// Handle key and mouse input
	createCallbacks();


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

	glfwSetWindowUserPointer(mainWindow, this);			// to pass the current window to GLFW, 
														// first parameter is pointer to mainWindow of active object 
}													    // the second parameter sets the window is specific to this object

void MyWindow::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
}

void MyWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) 
{
	MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));	//glfwGetWindowsUserPointer get the value of pointer
																					// that was set in glfwSetWindowUserPointer() function; which assigns the current window object as the mainWindow  

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)	 // if the user Presses ecape key and the stored values of the enums for escape key & release  action will be returned 
	{												 // and the function will become true and the window will be closed
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("Released: %d\n", key);
		}
	}
}																					

MyWindow::~MyWindow() 
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

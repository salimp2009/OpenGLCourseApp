#ifndef _MY_WINDOW_H
#define _MY_WINDOW_H

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// this classed needs to be refactored; it became a big monolitic class
// better to have mouse and keyboard handles should be components; seperate classes

class MyWindow
{
public:
	MyWindow();
	MyWindow(GLint windowWidth, GLint windowHeight);
	
	int Initialise(); 

	GLfloat getBufferWidth()  { return static_cast<GLfloat>(bufferWidth); }   // add const qualifier if OpenGl is OK
	GLfloat getBufferHeight() { return  static_cast<GLfloat>(bufferHeight); }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys()		  { return keys;}  // will be used to pass the key data to the camera class
	
	GLfloat getXChange();
	GLfloat getYChange();

	void SwapBuffers()	  { return glfwSwapBuffers(mainWindow); }

	~MyWindow();
private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	
	// input from keyboard, mouse to pass it GLFW for the camera movement
	bool keys[1024];			//1024 is to cover all the ASCII code; use max_key
								//when a key is pressed down GLFW will pick it up and
								//that number will refer to the index no in the keys array and value
								//be set to TRUE and when the key is released when we lift off the key
								// this is specific GLFW to do a call back and static is used to make it to specific to a class
	
	// mouse input control values; last coordinates recorded
	// these will be compared the new coordinate which will determine 
	// the change so the camera movement can be calculated
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	// determines the first movement position to initialize 
	// otherwise it will be recorded as movement and the camera/screen might move around violently
	bool mouseFirstMoved;  

	void createCallbacks();

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); 
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};							

#endif // !_MY_WINDOW_H

#ifndef _MY_WINDOW_H
#define _MY_WINDOW_H

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class MyWindow
{
public:
	MyWindow();
	MyWindow(GLint windowWidth, GLint windowHeight);
	
	int Initialise(); 
	GLfloat getBufferWidth()  { return static_cast<GLfloat>(bufferWidth); }   // add const qualifier if OpenGl is OK
	GLfloat getBufferHeight() { return  static_cast<GLfloat>(bufferHeight); }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffers()	  { return glfwSwapBuffers(mainWindow); }

	~MyWindow();
private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	
	// input from keyboard, mouse to pass it GLFW for the camera movement
	bool keys[1024];			//1024 is to cover all the ASCII code; use max_key
								//when a key is pressed down GLFW will pick i tup and
								//that number will refer to the index no in the keys array and value
								//be set to TRUE and when the key is released when we lift off the key
								// this is specific GLFW to do a call back and static is used to make it to specific to a class
								
	void createCallbacks();

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); 						
};							

#endif // !_MY_WINDOW_H
